#include "emu/ldaq/manager/Application.h"
#include "emu/ldaq/manager/version.h"

#include "emu/utils/DOM.h"
#include "emu/utils/String.h"
#include "emu/utils/System.h"
#include "emu/soap/ToolBox.h"
#include "emu/soap/Messenger.h"
#include "emu/base/TypedFact.h"
#include "emu/base/ApplicationStatusFact.h"
#include "emu/ldaq/rui/STEPEventCounter.h"
#include "emu/ldaq/rui/EventStatistics.h"
#include "emu/ldaq/manager/STEPInfo.h"
#include "emu/ldaq/manager/FactTypes.h"

#include "xcept/tools.h"
#include "xoap/Method.h"
#include "xoap/MessageFactory.h"
#include "xoap/SOAPName.h"
#include "xoap/SOAPEnvelope.h"
#include "xoap/SOAPBodyElement.h"
#include "xoap/SOAPSerializer.h"
#include "xdata/InfoSpace.h"
#include "xdata/UnsignedInteger.h"
#include "xdata/UnsignedLong.h"
#include "xdaq/NamespaceURI.h"
#include "toolbox/regex.h"
#include "toolbox/task/TimerFactory.h"
#include "toolbox/TimeInterval.h" 

#include "cgicc/HTTPResponseHeader.h"

#include <limits>
#include <iomanip>

emu::ldaq::manager::Application::Application(xdaq::ApplicationStub *s)
  throw (xdaq::exception::Exception) 
  : xdaq::WebApplication(s),
    emu::base::Supervised(s),
    emu::base::WebReporter(s),
    emu::base::FactFinder( s, emu::base::FactCollection::LOCAL_DAQ, 0 ),
    logger_(Logger::getInstance(generateLoggerName())),
    inFSMTransition_( false ),
    watchdog_( NULL ),
    runInfo_(NULL)
{
  const string aName( "icon" );
  const string aValue( "/emu/ldaq/manager/images/EmuDAQManager64x64.gif" );
  s->getDescriptor()->setAttribute( aName, aValue );
  getAllAppDescriptors();
  // Bind web interface
  xgi::bind(this, &emu::ldaq::manager::Application::defaultWebPage, "Default"   );
  exportParams();
  createAllAppStatuses();
  bindSOAPCommands();
  setUpFSM();
  timeWatchdog();
  LOG4CPLUS_INFO(logger_, "End of constructor");
}

string emu::ldaq::manager::Application::generateLoggerName(){
  ostringstream oss;
  oss << getApplicationDescriptor()->getClassName() << getApplicationDescriptor()->getInstance();
  return oss.str();
}

void emu::ldaq::manager::Application::getAllAppDescriptors()
{
  const xdaq::Zone *zone = getApplicationContext()->getDefaultZone();

    try{
        evmDescriptors_ = getAppDescriptors(zone, "evb::EVM");
    }
    catch(emu::ldaq::manager::exception::Exception e){
        evmDescriptors_.clear();
        LOG4CPLUS_ERROR( logger_, "Failed to get application descriptors for class EVM" << " : " << xcept::stdformat_exception_history(e) );
    }

    try{
        ruDescriptors_ = getAppDescriptors(zone, "evb::RU");
    }
    catch(emu::ldaq::manager::exception::Exception e){
        ruDescriptors_.clear();
        LOG4CPLUS_ERROR( logger_, "Failed to get application descriptors for class RU" << " : " << xcept::stdformat_exception_history(e) );
    }

    try{
        buDescriptors_ = getAppDescriptors(zone, "evb::BU");
    }
    catch(emu::ldaq::manager::exception::Exception e){
        buDescriptors_.clear();
        LOG4CPLUS_ERROR( logger_, "Failed to get application descriptors for class BU" << " : " << xcept::stdformat_exception_history(e) );
    }

    try{
        ruiDescriptors_ = getAppDescriptors(zone, "evb::test::DummyFEROL");
    }
    catch(emu::ldaq::manager::exception::Exception e){
        ruiDescriptors_.clear();
        LOG4CPLUS_ERROR( logger_, "Failed to get application descriptors for class DummyFEROL" << " : " << xcept::stdformat_exception_history(e) );
    }

    try{
      if ( ruiDescriptors_.size() == 0 ){
        ruiDescriptors_ = getAppDescriptors(zone, "emu::ldaq::rui::Application");
      }
    }
    catch(emu::ldaq::manager::exception::Exception e){
        ruiDescriptors_.clear();
        LOG4CPLUS_ERROR( logger_, "Failed to get application descriptors for class emu::ldaq::rui::Application" << " : " << xcept::stdformat_exception_history(e) );
    }

}

vector< const xdaq::ApplicationDescriptor* > emu::ldaq::manager::Application::getAppDescriptors
(
    const xdaq::Zone      *zone,
    const string           appClass
)
throw (emu::ldaq::manager::exception::Exception)
{
    vector< const xdaq::ApplicationDescriptor* > orderedDescriptors;
    set< const xdaq::ApplicationDescriptor* > descriptors;

    try
    {
        descriptors = zone->getApplicationDescriptors(appClass);
    }
    catch(xdaq::exception::ApplicationDescriptorNotFound e)
    {
        string s;

        s = "Failed to get application descriptors for class: " + appClass;

        XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, s, e);
    }

    // Fill application descriptors in instance order allowing non-contiguous numbering
    while( !descriptors.empty() ){
      // Find app with smallest instance number
      uint32_t minInstance = 99999;
      set< const xdaq::ApplicationDescriptor* >::iterator adOfSmallest;
      set< const xdaq::ApplicationDescriptor* >::iterator ad;
      for ( ad=descriptors.begin(); ad!=descriptors.end(); ++ad )
	if ( (*ad)->getInstance() < minInstance ){
	  adOfSmallest = ad;
	  minInstance  = (*ad)->getInstance();
	}
      // Append it to the ordered vector
      orderedDescriptors.push_back( *adOfSmallest );
      // Remove it from the unordered vector
      descriptors.erase( adOfSmallest );
    }

    return orderedDescriptors;
}

void emu::ldaq::manager::Application::exportParams()
{
  xdata::InfoSpace *s = getApplicationInfoSpace();

  runDbBookingCommand_ = "java -jar runnumberbooker.jar";
  runDbWritingCommand_ = "java -jar runinfowriter.jar";
  runDbAddress_        = "dbc:oracle:thin:@oracms.cern.ch:10121:omds";
  runDbUserFile_       = "";
  s->fireItemAvailable( "runDbBookingCommand", &runDbBookingCommand_ );
  s->fireItemAvailable( "runDbWritingCommand", &runDbWritingCommand_ );
  s->fireItemAvailable( "runDbAddress",        &runDbAddress_        );
  s->fireItemAvailable( "runDbUserFile",       &runDbUserFile_       );

  supervisedMode_             = true;
  configuredInSupervisedMode_ = true;
  runNumber_                  = 1;
  maxNumberOfEvents_          = -1;
  runStartTime_               = "YYMMDD_hhmmss_UTC";
  runStopTime_                = "YYMMDD_hhmmss_UTC";
  runType_                    = "Monitor";
  isGlobalInControl_          = false;
  buildEvents_                = false;
  calibRunIndex_              = 0;
  calibNRuns_                 = 0;
  calibStepIndex_             = 0;
  calibNSteps_                = 0;

  s->fireItemAvailable( "supervisedMode"            , &supervisedMode_              );
  s->fireItemAvailable( "configuredInSupervisedMode", &configuredInSupervisedMode_  );
  s->fireItemAvailable( "runNumber"                 , &runNumber_                   );
  s->fireItemAvailable( "maxNumberOfEvents"         , &maxNumberOfEvents_           );
  s->fireItemAvailable( "runStartTime"              , &runStartTime_                );
  s->fireItemAvailable( "runStopTime"               , &runStopTime_                 );
  s->fireItemAvailable( "runType"                   , &runType_                     );
  s->fireItemAvailable( "runTypes"                  , &runTypes_                    );
  s->fireItemAvailable( "isGlobalInControl"         , &isGlobalInControl_           );
  s->fireItemAvailable( "buildEvents"               , &buildEvents_                 );
  s->fireItemAvailable( "calibRunIndex"             , &calibRunIndex_               );
  s->fireItemAvailable( "calibNRuns"                , &calibNRuns_                  );
  s->fireItemAvailable( "calibStepIndex"            , &calibStepIndex_              );
  s->fireItemAvailable( "calibNSteps"               , &calibNSteps_                 );

  s->fireItemAvailable( "dataFileNames"             , &dataFileNames_               );
  s->fireItemAvailable( "dataDirNames"              , &dataDirNames_                );
    
  s->addItemChangedListener("buildEvents",this);
  s->addItemChangedListener("supervisedMode",this);

  s->addItemRetrieveListener("runStartTime" ,this);
  s->addItemRetrieveListener("runStopTime"  ,this);
  s->addItemRetrieveListener("dataFileNames",this);
  s->addItemRetrieveListener("dataDirNames" ,this);

  writeBadEventsOnly_ = true;
  s->fireItemAvailable("writeBadEventsOnly",&writeBadEventsOnly_);
      
  daqState_ = "UNKNOWN";
  s->fireItemAvailable("daqState",&daqState_);
  s->addItemRetrieveListener("daqState",this);  

  STEPCount_ = 0;
  s->fireItemAvailable("STEPCount",&STEPCount_);
  s->addItemRetrieveListener("STEPCount",this);
      
  STEPFinished_ = false;
  s->fireItemAvailable("STEPFinished",&STEPFinished_);
  s->addItemRetrieveListener("STEPFinished",this);
      
  STEPCountsTableDir_ = "/tmp";
  s->fireItemAvailable("STEPCountsTableDir",&STEPCountsTableDir_);
      
  hardwareMapping_ = "/emu/ldaq/xml/RUI-to-chamber_mapping.xml";
  s->fireItemAvailable("hardwareMapping",&hardwareMapping_);
}

void emu::ldaq::manager::Application::createAllAppStatuses(){
  currentAppStatuses_.clear();
  currentAppStatuses_.defineSynonyms( "Configured", "configured,Ready,ready" ).defineSynonyms( "Enabled", "enabled,Started,started,Running,running" );
  if ( (bool)buildEvents_ ){ 
    currentAppStatuses_.insertApps( evmDescriptors_.begin(), evmDescriptors_.end() );
    currentAppStatuses_.insertApps( buDescriptors_ .begin(), buDescriptors_ .end() );
    currentAppStatuses_.insertApps( ruDescriptors_ .begin(), ruDescriptors_ .end() );
  }
  currentAppStatuses_.insertApps( ruiDescriptors_.begin(), ruiDescriptors_.end() );

  // daqContexts_.clear();
  // set<const xdaq::ApplicationDescriptor*> apps( currentAppStatuses_.getApps() );
  // set<const xdaq::ApplicationDescriptor*>::iterator a;
  // for ( a=apps.begin(); a!=apps.end(); ++a ){
  //   // Collect different contexts too
  //   daqContexts_.insert( (*a)->getContextDescriptor()->getURL() );
  // }
}

void emu::ldaq::manager::Application::queryAppStatuses(){
  emu::soap::Messenger m( this );
  // Perform query only if state info at least 1 second old.
  cout << "currentAppStatuses_.getAgeInSeconds() = " << currentAppStatuses_.getAgeInSeconds() << endl;
  if ( currentAppStatuses_.getAgeInSeconds() > 1 ){
    bool isCurrentTestDurationUndefined = ( runType_.toString().find("STEP",0) != string::npos );
    set<const xdaq::ApplicationDescriptor*> apps = currentAppStatuses_.getApps();
    set<const xdaq::ApplicationDescriptor*>::iterator a;
    xdata::String state( "UNKNOWN" );
    xdata::UnsignedLong events( 0 );
    emu::ldaq::rui::EventStatistics eventStatistics;
    for ( a=apps.begin(); a!=apps.end(); ++a ){
      state = "UNKNOWN";
      events = 0;
      eventStatistics.zero();
      STEPInfo step;
      STEPInfo* stepInfo = &step; // just a pointer to step
      try
	{
	  emu::soap::Parameters p;
	  p.add( "stateName", &state );
	  if ( ! isCurrentTestDurationUndefined  ){
	    // It's a pulsing STEP test, a non-pulsing STEP test of predefined duration, or not a STEP test at all.
	    // These don't have "STEP" in their name.
	    if      ( (*a)->getClassName() == "emu::ldaq::rui::Application"                   )
	      p .add( "nEventsRead"    , &events                          )
		.add( "dataRate"       , &eventStatistics.dataRate        )
		.add( "eventRate"      , &eventStatistics.eventRate       )
		.add( "sampledFraction", &eventStatistics.sampledFraction )
		.add( "sizeMean"       , &eventStatistics.sizeMean        )
		.add( "sizeStD"        , &eventStatistics.sizeStD         );
	    else if ( (*a)->getClassName() == "evb::RU" || (*a)->getClassName() == "evb::EVM" )
	      p .add( "eventCount"   , &events );
	    else if ( (*a)->getClassName() == "evb::BU"                                       )
	      p .add( "nbEventsBuilt", &events );
	    stepInfo = NULL;
	  }
	  else{
	    // It's a non-pulsing STEP test, where we want each input to have at least a specified number of events.
	    emu::soap::extractParameters( m.sendCommand( *a, "STEPQuery" ),
					  emu::soap::Parameters()
					  .add( "PersistentDDUError", &step.persistentDDUError )
					  .add( "EventsRead"        , &step.eventsRead         )
					  .add( "TotalCount"        , &step.totalCount         )
					  .add( "LowestCount"       , &step.lowestCount        )
					  .add( "Counts"            , &step.counts             )
					  .add( "ReadCounts"        , &step.readCounts         )
					  .add( "Masks"             , &step.masks              )
					  .add( "LiveInputs"        , &step.liveInputs         ) );
	  }
	  m.getParameters( *a, p );
	}
      catch(xcept::Exception &e)
	{
	  state = "UNKNOWN";
	  events = 0;
	  stepInfo = NULL;
	  ostringstream oss;
	  oss << "Failed to get event count and state of " << (*a)->getClassName() << "." << (*a)->getInstance() << " : " ;
	  LOG4CPLUS_WARN(logger_, oss.str() + xcept::stdformat_exception_history(e));
	}
      currentAppStatuses_.setAppStatus( *a, state, events, &eventStatistics, stepInfo );
    } // for ( a=apps.begin(); a!=apps.end(); ++a )
    //cout << "Previous " << previousAppStatuses_;
    cout << "Current "  << currentAppStatuses_;
    // Update the combined DAQ state
    daqState_ = currentAppStatuses_.getCombinedState();
    // Update STEP monitoring parameters, too
    STEPCount_ = ( isCurrentTestDurationUndefined ? 
		   currentAppStatuses_.getLowestOfLowestSTEPCount()                    : // The lowest count of any DDU input.
		   currentAppStatuses_.getLowestCount( "emu::ldaq::rui::Application" )   // The lowest count of any DDU.
		   );
    STEPFinished_ = ( (int64_t) STEPCount_ >= maxNumberOfEvents_ );
  } // if ( currentAppStatuses_.getAgeInSeconds() > 1 )
}

void emu::ldaq::manager::Application::bindSOAPCommands(){
  xoap::bind(this, &emu::ldaq::manager::Application::onConfigure, "Configure", XDAQ_NS_URI);
  xoap::bind(this, &emu::ldaq::manager::Application::onEnable,    "Enable",    XDAQ_NS_URI);
  xoap::bind(this, &emu::ldaq::manager::Application::onHalt,      "Halt",      XDAQ_NS_URI);
}

void emu::ldaq::manager::Application::setUpFSM(){
    // Try driving FSM by asynchronous SOAP in order for DQM hang-ups not to block us (and, eventually, the global run).
    // Execute FSM transitions in a separate thread.
    workLoop_ = toolbox::task::getWorkLoopFactory()->getWorkLoop("emu::ldaq::manager::Application", "waiting");
    workLoop_->activate();
    configureSignature_ = toolbox::task::bind(this, &emu::ldaq::manager::Application::configureActionInWorkLoop, "configureActionInWorkLoop");
    enableSignature_    = toolbox::task::bind(this, &emu::ldaq::manager::Application::enableActionInWorkLoop,    "enableActionInWorkLoop");
    haltSignature_      = toolbox::task::bind(this, &emu::ldaq::manager::Application::haltActionInWorkLoop,      "haltActionInWorkLoop");

    fsm_.addState('H', "Halted",     this, &emu::ldaq::manager::Application::stateChanged);
    fsm_.addState('C', "Configured", this, &emu::ldaq::manager::Application::stateChanged);
    fsm_.addState('E', "Enabled",    this, &emu::ldaq::manager::Application::stateChanged);
    
    fsm_.addStateTransition('H', 'C', "Configure", this, &emu::ldaq::manager::Application::configureAction);
    fsm_.addStateTransition('C', 'C', "Configure", this, &emu::ldaq::manager::Application::noAction);
    fsm_.addStateTransition('C', 'E', "Enable",    this, &emu::ldaq::manager::Application::enableAction);
    fsm_.addStateTransition('E', 'E', "Configure", this, &emu::ldaq::manager::Application::noAction);
    fsm_.addStateTransition('C', 'H', "Halt",      this, &emu::ldaq::manager::Application::haltAction);
    fsm_.addStateTransition('E', 'H', "Halt",      this, &emu::ldaq::manager::Application::haltAction);
    fsm_.addStateTransition('H', 'H', "Enable",    this, &emu::ldaq::manager::Application::noAction);    
    fsm_.addStateTransition('H', 'H', "Halt",      this, &emu::ldaq::manager::Application::noAction);
    fsm_.addStateTransition('E', 'E', "Enable",    this, &emu::ldaq::manager::Application::noAction);

    fsm_.setFailedStateTransitionChanged(this, &emu::ldaq::manager::Application::stateChanged);

    // Set the inital state according to the combined state of the DAQ (useful if DAQ Manager crashes and is restarted)
    // TODO: get run parameters, too, from RUIs.
    char initialState = 'H';
    try{
      queryAppStatuses();
      string combinedState = currentAppStatuses_.getCombinedState();

      stringstream ss;
      ss << "Current state of the DAQ is " << combinedState;
      cout << ss.str() << endl << flush;
      LOG4CPLUS_INFO( getApplicationLogger(), ss.str() );

      if      ( combinedState == "Configured" ) initialState = 'C';
      else if ( combinedState == "Enabled"    ) initialState = 'E';
    }
    catch(xcept::Exception& e){
      stringstream ss;
      ss << "Failed to query the local DAQ state. The lDAQ Manager's state will be 'Halted' at its construction." << xcept::stdformat_exception_history(e);
      LOG4CPLUS_WARN( getApplicationLogger(), ss.str() );
      XCEPT_DECLARE( xcept::Exception, eObj, ss.str() );
      this->notifyQualified( "warning", eObj );      
    }
    fsm_.setInitialState( initialState );
    fsm_.reset();

    inFSMTransition_ = false;

    state_ = fsm_.getStateName(fsm_.getCurrentState());
}

void emu::ldaq::manager::Application::timeWatchdog(){
  stringstream timerName;
  timerName << "DAQWatchdog." << getApplicationDescriptor()->getClassName() << "." << getApplicationDescriptor()->getInstance();
  try{
    toolbox::task::Timer * timer = toolbox::task::getTimerFactory()->createTimer( timerName.str() );
    toolbox::TimeInterval interval( 30, 0 ); // period in sec
    toolbox::TimeVal start( toolbox::TimeVal::gettimeofday() + toolbox::TimeVal( 5, 0 ) ); // start in 5 seconds from now
    timer->scheduleAtFixedRate( start, this, interval,  0, "" );
  } catch(xcept::Exception& e){
    ostringstream ss;
    ss << "Failed to create " << timerName.str() << " , therefore no scheduled check of state of DAQ applications will be done: " << xcept::stdformat_exception_history(e);
    LOG4CPLUS_WARN( getApplicationLogger(), ss.str() );
    XCEPT_DECLARE( xcept::Exception, eObj, ss.str() );
    this->notifyQualified( "warning", eObj );
  }
}

void emu::ldaq::manager::Application::retireWatchdog(){
  stringstream timerName;
  timerName << "DAQWatchdog." << getApplicationDescriptor()->getClassName() << "." << getApplicationDescriptor()->getInstance();
  try{
    if ( toolbox::task::getTimerFactory()->hasTimer( timerName.str() ) ){
      toolbox::task::getTimerFactory()->removeTimer( timerName.str() );
    }
  } catch(xcept::Exception& e){
    ostringstream ss;
    ss << "Failed to remove " << timerName.str() << ": " << xcept::stdformat_exception_history(e);
    LOG4CPLUS_WARN( getApplicationLogger(), ss.str() );
    XCEPT_DECLARE( xcept::Exception, eObj, ss.str() );
    this->notifyQualified( "warning", eObj );
  }
}

void emu::ldaq::manager::Application::waitForAppsState( vector<const xdaq::ApplicationDescriptor*> apps, const string state, const unsigned int seconds ){
  emu::ldaq::manager::AppStatuses appStatuses;
  appStatuses.insertApps( apps.begin(), apps.end() );
  emu::soap::Messenger m( this );
  xdata::String s( "UNKNOWN" );
  xdata::UnsignedLong events( 0 );
  for ( unsigned int i=0; i<=seconds; ++i ){
    for ( vector<const xdaq::ApplicationDescriptor*>::iterator a=apps.begin(); a!=apps.end(); ++a ){
      s = "UNKNOWN";
      events = 0;
      try{
	emu::soap::Parameters p;
	p.add( "stateName", &s );
	if      ( (*a)->getClassName() == "emu::ldaq::rui::Application"                   ) p.add( "nEventsRead"  , &events );
	else if ( (*a)->getClassName() == "evb::RU" || (*a)->getClassName() == "evb::EVM" ) p.add( "eventCount"   , &events );
	else if ( (*a)->getClassName() == "evb::BU"                                       ) p.add( "nbEventsBuilt", &events );
	m.getParameters( *a, p );
      }
      catch(xcept::Exception &e){
	s = "UNKNOWN";
	events = 0;
	ostringstream oss;
	oss << "Failed to get state (and event count) of " << (*a)->getClassName() << "." << (*a)->getInstance() << " : " ;
	LOG4CPLUS_WARN(logger_, oss.str() + xcept::stdformat_exception_history(e));
      }
      appStatuses.setAppStatus( *a, s, events, NULL, NULL );
    }
    if ( appStatuses.getCombinedState() == state ) return;
    ::sleep( 1 );
  }
  ostringstream oss;
  oss << "Apps failed to reach '" << state << "' state in " << seconds << " seconds.\n" << appStatuses;
  XCEPT_RAISE( emu::ldaq::manager::exception::Exception, oss.str() );
}


void emu::ldaq::manager::Application::stateChanged(toolbox::fsm::FiniteStateMachine &fsm)
        throw (toolbox::fsm::exception::Exception)
{
	emu::base::Supervised::stateChanged(fsm);
	getApplicationInfoSpace()->fireItemValueChanged( "stateName" );
}

void emu::ldaq::manager::Application::configureDAQ()
  throw (emu::ldaq::manager::exception::Exception)
{
  // See evb/test/scripts/TestCase.py for proper sequences in FSM transitions.

  emu::soap::Messenger m( this );
  vector< const xdaq::ApplicationDescriptor* >::const_iterator appdescr;
  
  runStartTime_ = "YYMMDD_hhmmss_UTC";
  runStopTime_ = "YYMMDD_hhmmss_UTC";
  
  queryAppStatuses();
  cout << "Before configuring\n"  << currentAppStatuses_;

  LOG4CPLUS_INFO( logger_, "Configuring..." );
  //////////////
  // Configure RUIs //
  //////////////
  
  for(appdescr = ruiDescriptors_.begin(); appdescr != ruiDescriptors_.end(); appdescr++)
    {
      try
	{
	  m.setParameters( *appdescr, 
			   emu::soap::Parameters()
			   .add( "runType"              , &runType_     )
			   .add( "writeBadEventsOnly"   , &writeBadEventsOnly_ )
			   .add( "maxEvents"            , &maxNumberOfEvents_ )
			   .add( "passDataOnToRUBuilder", &buildEvents_ ) );
	  LOG4CPLUS_INFO(logger_,"Set run type for " + (*appdescr)->getClassName() + " to " + runType_.toString() + ", event building " + buildEvents_.toString());
	}
      catch(xcept::Exception &e)
	{
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception,
			"Failed to set run type for " + (*appdescr)->getClassName() + " to " + runType_.toString() + ", event building " + buildEvents_.toString(), e);
	}

    try
      {
	m.sendCommand( *appdescr, "Configure" );
      }
    catch(xcept::Exception &e)
      {
	ostringstream oss;
	oss << "Failed to configure ";
	oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();
	XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
      }
    }

  LOG4CPLUS_INFO( logger_, "Sent configure command to RUIs." );

  waitForAppsState( ruiDescriptors_, "Ready", 30 );

  ::sleep(1);
  queryAppStatuses();
  cout << "After configuring RUIs\n"  << currentAppStatuses_;

  if ( ! (bool)buildEvents_ ) return;
  
  //////////////
  // Configure EVM //
  //////////////
  
  try
    {
      m.sendCommand( evmDescriptors_[0], "Configure" );
    }
  catch(xcept::Exception &e)
    {
      ostringstream oss;
      oss << "Failed to configure ";
      oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();
      XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
    }
  
  waitForAppsState( evmDescriptors_, "Ready", 30 );

  queryAppStatuses();
  cout << "After configuring EVM\n"  << currentAppStatuses_;
  
  //////////////
  // Configure RUs //
  //////////////
  
  for(appdescr = ruDescriptors_.begin(); appdescr != ruDescriptors_.end(); appdescr++)
    {
      try
        {
	  m.sendCommand( *appdescr, "Configure" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to configure ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
        }
    }

  waitForAppsState( ruDescriptors_, "Ready", 30 );
  queryAppStatuses();
  cout << "After configuring RU\n"  << currentAppStatuses_;

  //////////////
  // Configure BUs //
  //////////////
  
  for(appdescr = buDescriptors_.begin(); appdescr != buDescriptors_.end(); appdescr++)
    {
      try
        {
	  m.sendCommand( *appdescr, "Configure" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to configure ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
        }
    }

  waitForAppsState( buDescriptors_, "Ready", 30 );
  queryAppStatuses();
  cout << "After configuring BU\n"  << currentAppStatuses_;
}


void emu::ldaq::manager::Application::startDAQ()
throw (emu::ldaq::manager::exception::Exception)
{
  LOG4CPLUS_INFO( logger_, "Starting..." );

  xdata::UnsignedInteger runNumber( 1 );
  xdata::String          runStopTime( "YYMMDD_hhmmss_UTC" );
  xdata::String          runType( "Monitor" );

  runStartTime_ = getDateTime();
  
  emu::soap::Messenger m( this );
  vector< const xdaq::ApplicationDescriptor* >::const_iterator appdescr;
  
  if ( (bool)buildEvents_ ){

  //////////////
  // Enable EVM //
  //////////////
  
  try
    {
      m.setParameters( evmDescriptors_[0],
		       emu::soap::Parameters().add( "runNumber", &runNumber  ) );
    }
  catch(xcept::Exception &e)
    {
      ostringstream oss;
      oss << "Failed to set run number in ";
      oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
      // LOG4CPLUS_ERROR( logger_, oss.str() );
      XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
    }

  try
    {
      m.sendCommand( evmDescriptors_[0], "Enable" );
    }
  catch(xcept::Exception &e)
    {
      ostringstream oss;
      oss << "Failed to enable ";
      oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
      // LOG4CPLUS_ERROR( logger_, oss.str() );
      XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
    }
  
  
  //////////////
  // Enable RUs //
  //////////////
  
  for(appdescr = ruDescriptors_.begin(); appdescr != ruDescriptors_.end(); appdescr++)
    {
      try
	{
	  m.setParameters( *appdescr, emu::soap::Parameters().add( "runNumber", &runNumber  ) );
	}
      catch(xcept::Exception &e)
	{
	  ostringstream oss;
	  oss << "Failed to set run number in ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
	}
    }
  
  for(appdescr = ruDescriptors_.begin(); appdescr != ruDescriptors_.end(); appdescr++)
    {
      try
        {
	  m.sendCommand( *appdescr, "Enable" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to enable ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();	  
	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
        }
    }

  //////////////
  // Enable BUs //
  //////////////
  
  for(appdescr = buDescriptors_.begin(); appdescr != buDescriptors_.end(); appdescr++)
    {
      try
	{
	  m.setParameters( *appdescr, emu::soap::Parameters().add( "runNumber", &runNumber  ) );
	}
      catch(xcept::Exception &e)
	{
	  ostringstream oss;
	  oss << "Failed to set run number in ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
	}
    }

  for(appdescr = buDescriptors_.begin(); appdescr != buDescriptors_.end(); appdescr++)
    {
      try
        {
	  m.sendCommand( *appdescr, "Enable" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to enable ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();	  
	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
        }
    }

  }
  
  //////////////
  // Enable RUIs //
  //////////////
  
  for(appdescr = ruiDescriptors_.begin(); appdescr != ruiDescriptors_.end(); appdescr++)
    {
      try
	{
	  m.setParameters( *appdescr,
			   emu::soap::Parameters()
			   .add( "runNumber"   , &runNumber_         )
			   .add( "maxEvents"   , &maxNumberOfEvents_ )
			   .add( "runStartTime", &runStartTime_      )
			   );
	}
      catch(xcept::Exception &e)
	{
	  ostringstream oss;
	  oss << "Failed to set run number in ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
 	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
	}
    }

  for(appdescr = ruiDescriptors_.begin(); appdescr != ruiDescriptors_.end(); appdescr++)
    {
      try
        {
	  m.sendCommand( *appdescr, "Enable" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to enable ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance();	  
	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
        }
    }

  queryAppStatuses();
  cout << "After starting\n"  << currentAppStatuses_;
}

void emu::ldaq::manager::Application::stopDAQ()
throw (emu::ldaq::manager::exception::Exception)
{
  LOG4CPLUS_INFO( logger_, "Stopping lDAQ:" );

  runStopTime_ = getDateTime();

  emu::soap::Messenger m( this );
  vector< const xdaq::ApplicationDescriptor* >::const_iterator appdescr;
  
  //////////////
  // Halt RUIs //
  //////////////
  
  for(appdescr = ruiDescriptors_.begin(); appdescr != ruiDescriptors_.end(); appdescr++)
    {
      try
        {
	  LOG4CPLUS_INFO( logger_, "Stopping " << (*appdescr)->getClassName() << (*appdescr)->getInstance() );
	  m.sendCommand( *appdescr, "Halt" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to halt " << (*appdescr)->getClassName() << "." << (*appdescr)->getInstance()
	      << ": " << xcept::stdformat_exception_history( e );
	  // Don't raise exception here. Go on to try to stop the others.
	  LOG4CPLUS_ERROR( logger_, oss.str() );
        }
      try
	{
	  m.setParameters( *appdescr, emu::soap::Parameters().add( "runStopTime", &runStopTime_ ) );
	}
      catch(xcept::Exception &e)
	{
	  ostringstream oss;
	  oss << "Failed to set run stop time in ";
	  oss << (*appdescr)->getClassName() << (*appdescr)->getInstance() << ": " <<  xcept::stdformat_exception_history(e);	  
 	  // LOG4CPLUS_ERROR( logger_, oss.str() );
	  XCEPT_RETHROW(emu::ldaq::manager::exception::Exception, oss.str(), e);
	}
    }

  if ( ! (bool)buildEvents_ ) return;

  //////////////
  // Halt EVM //
  //////////////
  
  try
    {
      LOG4CPLUS_INFO( logger_, "Stopping " << evmDescriptors_[0]->getClassName() << evmDescriptors_[0]->getInstance() );
      m.sendCommand( evmDescriptors_[0], "Halt" );
    }
  catch(xcept::Exception &e)
    {
      ostringstream oss;
      oss << "Failed to halt " << (*appdescr)->getClassName() << "." << (*appdescr)->getInstance()
	  << ": " << xcept::stdformat_exception_history( e );
      // Don't raise exception here. Go on to try to stop the others.
      LOG4CPLUS_ERROR( logger_, oss.str() );
    }
  
  
  //////////////
  // Halt BUs //
  //////////////
  
  for(appdescr = buDescriptors_.begin(); appdescr != buDescriptors_.end(); appdescr++)
    {
      try
        {
	  LOG4CPLUS_INFO( logger_, "Stopping " << (*appdescr)->getClassName() << (*appdescr)->getInstance() );
	  m.sendCommand( *appdescr, "Halt" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to halt " << (*appdescr)->getClassName() << "." << (*appdescr)->getInstance()
	      << ": " << xcept::stdformat_exception_history( e );
	  // Don't raise exception here. Go on to try to stop the others.
	  LOG4CPLUS_ERROR( logger_, oss.str() );
        }
    }
  
  //////////////
  // Halt RUs //
  //////////////
  
  for(appdescr = ruDescriptors_.begin(); appdescr != ruDescriptors_.end(); appdescr++)
    {
      try
        {
	  LOG4CPLUS_INFO( logger_, "Stopping " << (*appdescr)->getClassName() << (*appdescr)->getInstance() );
	  m.sendCommand( *appdescr, "Halt" );
        }
      catch(xcept::Exception &e)
        {
	  ostringstream oss;
	  oss << "Failed to halt " << (*appdescr)->getClassName() << "." << (*appdescr)->getInstance()
	      << ": " << xcept::stdformat_exception_history( e );
	  // Don't raise exception here. Go on to try to stop the others.
	  LOG4CPLUS_ERROR( logger_, oss.str() );
        }
    }
  LOG4CPLUS_INFO( logger_, "Stopping lDAQ done." );
}


xoap::MessageReference emu::ldaq::manager::Application::onConfigure(xoap::MessageReference message)
		throw (xoap::exception::Exception)
{
  // fireEvent("Configure");
  // Execute it in another thread:
  workLoop_->submit(configureSignature_);

  return createReply(message);
}

xoap::MessageReference emu::ldaq::manager::Application::onEnable(xoap::MessageReference message)
		throw (xoap::exception::Exception)
{
  // fireEvent("Enable");
  // Execute it in another thread:
  workLoop_->submit(enableSignature_);

  return createReply(message);
}

xoap::MessageReference emu::ldaq::manager::Application::onHalt(xoap::MessageReference message)
		throw (xoap::exception::Exception)
{
  // fireEvent("Halt");
  // Execute it in another thread:
  workLoop_->submit(haltSignature_);

  return createReply(message);
}

bool emu::ldaq::manager::Application::configureActionInWorkLoop(toolbox::task::WorkLoop *wl){
  // Let's wait a sec to allow the async FSM SOAP command to safely return
  ::sleep(1);
  fireEvent("Configure");
  return false;
}

bool emu::ldaq::manager::Application::enableActionInWorkLoop(toolbox::task::WorkLoop *wl){
  // Let's wait a sec to allow the async FSM SOAP command to safely return
  ::sleep(1);
  fireEvent("Enable");
  return false;
}

bool emu::ldaq::manager::Application::haltActionInWorkLoop(toolbox::task::WorkLoop *wl){
  // Let's wait a sec to allow the async FSM SOAP command to safely return
  ::sleep(1);
  fireEvent("Halt");
  return false;
}

void emu::ldaq::manager::Application::configureAction(toolbox::Event::Reference e)
        throw (toolbox::fsm::exception::Exception)
{
  inFSMTransition_ = true;

  // Simulate slow transition
  //::sleep(15);
  // Simulate crash
  //exit(1);

  createAllAppStatuses();

  // Set configuredInSupervisedMode_ to true if and when configuration succeeds.
  configuredInSupervisedMode_ = false;

  try{
    configureDAQ();
  }
  catch(xcept::Exception &ex){
    inFSMTransition_ = false;
    XCEPT_RETHROW( toolbox::fsm::exception::Exception, "Failed to configure EmuDAQ", ex );
  }

  // Successfully configured in supervised mode.
  if ( supervisedMode_.value_ ) configuredInSupervisedMode_ = true;

  inFSMTransition_ = false;
}

void emu::ldaq::manager::Application::enableAction(toolbox::Event::Reference e)
		throw (toolbox::fsm::exception::Exception)
{
  inFSMTransition_ = true;

  if ( ! (bool)supervisedMode_ ){
    try{
      bookRunNumber();
    }
    catch(...){
      LOG4CPLUS_ERROR(logger_,
		      "<![CDATA[ Failed to book run number. ==> Falling back to run number " 
		      << runNumber_.value_ << " ]]>" );
    }
  }

  try{
    startDAQ();
  }
  catch(xcept::Exception &ex){
    inFSMTransition_ = false;
    XCEPT_RETHROW(toolbox::fsm::exception::Exception,
		    "Failed to enable EmuDAQ", ex);
  }
  
  LOG4CPLUS_DEBUG(getApplicationLogger(), e->type());
  inFSMTransition_ = false;
}

void emu::ldaq::manager::Application::haltAction(toolbox::Event::Reference e)
		throw (toolbox::fsm::exception::Exception)
{
  inFSMTransition_ = true;

  try{
    stopDAQ();
  }
  catch(xcept::Exception &ex){
    inFSMTransition_ = false;
    stringstream ss;
    ss << "Failed to stop EmuDAQ: " << xcept::stdformat_exception_history(ex);
    XCEPT_RETHROW(toolbox::fsm::exception::Exception, ss.str(), ex);
  }

  if ( runType_.toString().find("STEP",0) != string::npos ) saveSTEPCountsTable();

  inFSMTransition_ = false;
}

void emu::ldaq::manager::Application::reConfigureAction(toolbox::Event::Reference e)
        throw (toolbox::fsm::exception::Exception)
{   
  inFSMTransition_ = true;
  try{
    stopDAQ();
  }
  catch(xcept::Exception &ex){
    inFSMTransition_ = false;
    XCEPT_RETHROW(toolbox::fsm::exception::Exception,
		  "Failed to stop EmuDAQ before reconfiguration", ex);
  }
  
  try{
    configureDAQ();
  }
  catch(xcept::Exception &ex){
    inFSMTransition_ = false;
    XCEPT_RETHROW(toolbox::fsm::exception::Exception,
		  "Failed to reconfigure EmuDAQ", ex);
  }
  inFSMTransition_ = false;
}

void emu::ldaq::manager::Application::noAction(toolbox::Event::Reference e)
		throw (toolbox::fsm::exception::Exception)
{
  // Inaction...
  stringstream ss;
  ss << e->type() 
     << " attempted when in " 
     << fsm_.getStateName(fsm_.getCurrentState())
     << " state.";
  LOG4CPLUS_WARN(getApplicationLogger(), ss.str() );
}

void emu::ldaq::manager::Application::defaultWebPage(xgi::Input *in, xgi::Output *out)
throw (xgi::exception::Exception){
  processCommandForm( in, out );
  
  xdata::InfoSpace *is = getApplicationInfoSpace();

  DOMImplementation *impl = DOMImplementationRegistry::getDOMImplementation(xoap::XStr("LS"));
  DOMDocument* doc = impl->createDocument(xoap::XStr( "http://schemas.xmlsoap.org/soap/envelope/" ), // root element namespace URI.
  					  xoap::XStr("env:Envelope"), // root element name
  					  0);
  doc->insertBefore( doc->createProcessingInstruction( xercesc::XMLString::transcode("xml-stylesheet"),
  						       xercesc::XMLString::transcode("type=\"text/xml\" href=\"/emu/ldaq/manager/html/htmlRenderer_XSLT.xml\"") ),
  		     doc->getDocumentElement() );
  xdata::soap::Serializer().exportAll( is, dynamic_cast<DOMElement*>( doc->getDocumentElement() ), false );

  queryAppStatuses();
  doc->getDocumentElement()->appendChild( currentAppStatuses_.toDOM( doc ) );
  
  *out << emu::utils::serializeDOM( doc );
}
  
uint32_t emu::ldaq::manager::Application::purgeIntNumberString( string* s ){
  // Emu: purge string of all non-numeric characters
  uint32_t nCharactersErased = 0;
  for ( string::size_type i = s->find_first_not_of("-0123456789",0); 
	i != string::npos; 
	i = s->find_first_not_of("-0123456789",i) ){
    s->erase(i,1);
    nCharactersErased++;
  }
  return nCharactersErased;
}

void emu::ldaq::manager::Application::processCommandForm(xgi::Input *in, xgi::Output *out)
throw (xgi::exception::Exception)
{
    cgicc::Cgicc         cgi(in);

    std::vector<cgicc::FormEntry> fev = cgi.getElements();
    std::vector<cgicc::FormEntry>::iterator fe;

    if ( fev.size() ) webRedirect(in, out);

    //const cgicc::CgiEnvironment& env = cgi.getEnvironment();
    //cout << "QueryString: \"" << env.getQueryString() << "\"" << endl << flush;

    // for ( fe=fev.begin(); fe!=fev.end(); ++fe )
    //   cout << "name: " << fe->getName() << " value: \"" << fe->getValue() << "\"" << " value size: " << fe->getValue().size() << endl << flush;
    // cout << "---------------------------------" << endl << flush;


    // If there is a command from the html form
    cgicc::form_iterator cmdElement = cgi.getElement("command");
    if(cmdElement != cgi.getElements().end())
    {
        string cmdName = (*cmdElement).getValue();

	if ( (cmdName == "configure") )
	  {
	    for ( fe=fev.begin(); fe!=fev.end(); ++ fe )
	      if ( fe->getName() == "runtype" ){
		if ( fe->isEmpty() ){
		  LOG4CPLUS_ERROR(logger_, "Failed to get run type from http form");
		  stringstream ss22;
		  ss22 <<  "Failed to get run type from http form";
		  XCEPT_DECLARE( emu::ldaq::manager::exception::Exception, eObj, ss22.str() );
		  this->notifyQualified( "error", eObj );
		}
		else{
		  runType_.fromString( fe->getValue() );
		}
	      }

	    // Apparently the query string does not even include the checkbox element if it's not checked...
	    cgicc::form_iterator buildEventsElement = cgi.getElement("buildevents");
	    buildEvents_ = ( buildEventsElement != cgi.getElements().end() );

	    // Set run number to 1. If booking is successful, it will be replaced by the booked one.
	    runNumber_ = 1;

	    cgicc::form_iterator maxEvtElement = cgi.getElement("maxevents");
	    if( maxEvtElement != cgi.getElements().end() ){
	      string maxNumEvents  = (*maxEvtElement).getValue();
	      purgeIntNumberString( &maxNumEvents );
	      maxNumberOfEvents_.fromString( maxNumEvents );
	    }

	    // Check if only bad events are to be written to file.
	    cgicc::form_iterator writeBadEventsOnlyElement = cgi.getElement("writeBadEventsOnly");
	    writeBadEventsOnly_ = ( writeBadEventsOnlyElement != cgi.getElements().end() );

	    // Obviously, global cannot be in control if command is issued from web page
	    isGlobalInControl_ = false;

	    fireEvent("Configure");
	  }
	else if ( (cmdName == "start") && fsm_.getCurrentState() == 'C' )
	  {
	    fireEvent("Enable");
	  }
        else if( cmdName == "stop" )
	  {
	    // Obviously, global cannot be in control if command is issued from web page
	    isGlobalInControl_ = false;

	    fireEvent("Halt");
	  }
    }

    // If there is a command to take/realease control
    cmdElement = cgi.getElement("governor");
    if(cmdElement != cgi.getElements().end())
    {
        string cmdName = (*cmdElement).getValue();
	if ( cmdName == "take control" ){
	  supervisedMode_ = false;
	}
	else if ( cmdName == "release control" ){
	  supervisedMode_ = true;
	}
    }
           
    // If there is a request to mask from the html form
    cmdElement = cgi.getElement("mask");
    if( cmdElement != cgi.getElements().end() )
      {
        string cmdName = (*cmdElement).getValue();
	
    	if ( cmdName == "count in" )
    	  {
    	    maskDDUInputs( true, fev );
    	  }
    	else if ( cmdName == "count out" )
    	  {
    	    maskDDUInputs( false, fev );
    	  }
      }

    // If there is a request to recycle (i.e., to terminate to be then restarted automatically) an RUI process:
    cmdElement = cgi.getElement("recycle");
    if( cmdElement != cgi.getElements().end() )
      {
        stringstream ss( (*cmdElement).getValue() );
	uint32_t ruiInstance;
	ss >> ruiInstance;
	try{
	  LOG4CPLUS_WARN( logger_, "Recycling emu::ldaq::rui::Application." << ruiInstance );
	  emu::soap::Messenger( this ).sendCommand( "emu::ldaq::rui::Application", ruiInstance, "Terminate" );
	}
	catch(xcept::Exception &e){
	  ss.str() = "";
	  ss << "Failed to recycle emu::ldaq::rui::Application." << (*cmdElement).getValue();
	  XCEPT_RETHROW( xgi::exception::Exception, ss.str(), e );
	}
      }
}

void emu::ldaq::manager::Application::maskDDUInputs( const bool in, const std::vector<cgicc::FormEntry>& fev ){

  //
  // Collect the RUI instances and DDU input indices that are to be masked
  //

  std::map< uint32_t, std::set<uint32_t> > instanceToInputs; // an RUI_instance --> [DDU inputs] map

  std::vector<cgicc::FormEntry>::const_iterator fe;
  for ( fe=fev.begin(); fe!=fev.end(); ++fe ){
    std::vector<std::string> matches; // matches[1] will be the instance, matches[2] the DDU input
    if ( toolbox::regx_match( fe->getName(), "^EmuRUI.[0-9]{1,2}.[0-9]{1,2}$") ){ // Make sure there's a match...
      toolbox::regx_match( fe->getName(), "^EmuRUI.([0-9]{1,2}).([0-9]{1,2})$", matches ); // ...because this crashes if no match.
      stringstream instance( matches[1] );
      stringstream input   ( matches[2] );
      uint32_t ins; instance >> ins;
      uint32_t inp; input    >> inp;
      std::map< uint32_t, std::set<uint32_t> >::iterator i = instanceToInputs.find( ins );
      if ( i == instanceToInputs.end() ){
	// First selected input of this instance
	std::set<uint32_t> inputs;
	inputs.insert( inp );
	instanceToInputs[ins] = inputs;
      }
      else{
	// This isntance has already input(s) selected
	i->second.insert( inp );
      }
    }
  }

  //
  // Loop over RUIs that have inputs selected and send them their list
  //

  std::map< uint32_t, std::set<uint32_t> >::iterator i;
  for ( i = instanceToInputs.begin(); i!= instanceToInputs.end(); ++i ){
    sendDDUInputMask( in, i->first, i->second );
  }

}

void emu::ldaq::manager::Application::sendDDUInputMask( const bool                    in, 
							const uint32_t            ruiInstance, 
							const std::set<uint32_t>& inputs ){
  // Sends a mask to a RUI exclude or include DDU inputs in the STEP event counts

  string commandName = "excludeDDUInputs";
  if ( in ) commandName = "includeDDUInputs";

  // Find the application descriptor for RUI of this instance
  const xdaq::ApplicationDescriptor* ruiDescriptor = NULL;
  vector< const xdaq::ApplicationDescriptor* >::const_iterator rui;
  for ( rui = ruiDescriptors_.begin(); rui!=ruiDescriptors_.end(); ++rui )
    if ( (*rui)->getInstance() == ruiInstance ) ruiDescriptor = (*rui);
  if ( ruiDescriptor == NULL ){
    LOG4CPLUS_ERROR(logger_, "Failed to get descriptor of emu::ldaq::rui::Application instance " << ruiInstance );
    return;
  }

  // Transfer inputs into an xdata vector to be serialized into SOAP
  xdata::Vector<xdata::UnsignedInteger32> inputsToMask;
  std::set<uint32_t>::iterator i;
  for( i = inputs.begin(); i != inputs.end(); ++i ){
    inputsToMask.push_back( *i );
  }

  try{
    emu::soap::Messenger( this ).sendCommand( ruiDescriptor, commandName, emu::soap::Parameters().add( "DDUInputs", &inputsToMask ) );
  } catch( xcept::Exception &e ){
    LOG4CPLUS_ERROR(logger_,
		    "Failed to mask DDU inputs in emu::ldaq::rui::Application instance " << ruiInstance 
		    << ": " << xcept::stdformat_exception_history(e) );
  } catch( ... ){
    LOG4CPLUS_ERROR(logger_,
		    "Failed to mask DDU inputs in emu::ldaq::rui::Application instance " << ruiInstance << ": Unknown exception." );
  }
  
  return;

}

void emu::ldaq::manager::Application::saveSTEPCountsTable(){
  stringstream STEPCountsTable, fileName;
  bool STEPFinished = true;
  try{
    fileName << STEPCountsTableDir_.toString()
	     << "/EventCounts_"                << runType_.toString()
	     << "_" << setfill('0') << setw(8) << runNumber_.value_
	     << "_"                            << runStartTime_.toString()
	     << ".txt";
    STEPFinished = printSTEPCountsTableASCII( STEPCountsTable );
    if ( ! STEPFinished ){
      LOG4CPLUS_WARN( logger_, "Saving table of counts for unfinished STEP run." );
    }
    emu::utils::writeFile( fileName.str(), STEPCountsTable.str() );
    LOG4CPLUS_INFO( logger_, "Saved table of STEP counts in " << fileName.str() );
  }
  catch( xcept::Exception& e ){
    LOG4CPLUS_ERROR( logger_, "Failed to save table of STEP counts. " << xcept::stdformat_exception_history( e ) );
  }
  catch( std::exception& e ){
    LOG4CPLUS_ERROR( logger_, "Failed to save table of STEP counts. " << e.what() );
  }
}

bool emu::ldaq::manager::Application::printSTEPCountsTableASCII( stringstream& out ){
  // Prints STEP counts in ASCII table.
  // Returns true if STEP has finished, false otherwise.
  bool isFinished = true;
  emu::soap::Messenger m( this );

  // Write ASCII table

  // Comment lines
  out << "# For each RUI:\n";
  out << "#     * 1st row: the counts in the accepted events\n";
  out << "#     * 2nd row: the counts in the read events\n";
  out << "# Count for\n";
  out << "#     * live (connected) input:                  number of events containing data from this input\n";
  out << "#     * not live (disconnected or killed) input: -1\n";
  out << "#     * masked (in local DAQ sw) input:          -2\n";
  out << "#     * masked and not live input:               -3\n";
  // First row: titles
  out << "#rui     total";
  for ( uint32_t i = 0; i < emu::ldaq::rui::STEPEventCounter::maxDDUInputs_; ++i )
    out << "   input" << setfill('0') << setw(2) << i;
  out << "\n";

  // Loop over RUIs and query them for STEP info, and write a table row
  vector<const xdaq::ApplicationDescriptor* >::const_iterator rui;
  for(rui = ruiDescriptors_.begin(); rui != ruiDescriptors_.end(); rui++){
    STEPInfo step;
    try{
      emu::soap::extractParameters( m.sendCommand( *rui, "STEPQuery" ),
				    emu::soap::Parameters()
				    .add( "PersistentDDUError", &step.persistentDDUError )
				    .add( "EventsRead"        , &step.eventsRead         )
				    .add( "TotalCount"        , &step.totalCount         )
				    .add( "LowestCount"       , &step.lowestCount        )
				    .add( "Counts"            , &step.counts             )
				    .add( "ReadCounts"        , &step.readCounts         )
				    .add( "Masks"             , &step.masks              )
				    .add( "LiveInputs"        , &step.liveInputs         ) );

      isFinished &= ( (int64_t) step.lowestCount.value_ >= maxNumberOfEvents_.value_ ); 
    } catch( emu::ldaq::manager::exception::Exception e ){
      LOG4CPLUS_WARN( logger_, "Failed to get STEP info from " 
		      << (*rui)->getClassName() << (*rui)->getInstance() 
		      << " : " << xcept::stdformat_exception_history(e));
      stringstream ss53;
      ss53 <<  "Failed to get STEP info from " 
		      << (*rui)->getClassName() << (*rui)->getInstance() 
		      << " : " ;
      XCEPT_DECLARE_NESTED( emu::ldaq::manager::exception::Exception, eObj, ss53.str(), e );
      this->notifyQualified( "warning", eObj );
      isFinished = false;
    } catch (xoap::exception::Exception& e){
      LOG4CPLUS_WARN( logger_, "Failed to parse STEP info from reply from " 
		      << (*rui)->getClassName() << (*rui)->getInstance() 
		      << " : " << xcept::stdformat_exception_history(e));
      stringstream ss54;
      ss54 <<  "Failed to parse STEP info from reply from " 
		      << (*rui)->getClassName() << (*rui)->getInstance() 
		      << " : " ;
      XCEPT_DECLARE_NESTED( emu::ldaq::manager::exception::Exception, eObj, ss54.str(), e );
      this->notifyQualified( "warning", eObj );
      isFinished = false;
    }
    // First line: counts in accepted events.
    // First column: emu::ldaq::rui::Application instance
    out << setfill('0') << setw(4) << (*rui)->getInstance();
    // Second column: total number of events
    out << setfill(' ') << setw(10) << step.totalCount.value_;
    // The remaining columns: event count on each DDU input
    for ( size_t i = 0; i < step.counts.elements(); ++i ){
      int64_t statusCode = 0;
      if ( step.liveInputs.elementAt(i)->toString() != "true" ) statusCode -= 1;
      if ( step.masks     .elementAt(i)->toString() == "true" ) statusCode -= 2;
      if ( statusCode == 0 ) out << setfill(' ') << setw(10) << dynamic_cast<xdata::UnsignedInteger64*>( step.counts.elementAt(i) )->value_;
      else                   out << setfill(' ') << setw(10) << statusCode;
    }
    out << "\n";
    // Second line: counts in accepted events.
    // First column: emu::ldaq::rui::Application instance
    out << setfill('0') << setw(4) << (*rui)->getInstance();
    // Second column: total number of events.
    out << setfill(' ') << setw(10) << step.eventsRead.value_;
    // The remaining columns: event count on each DDU input
    for ( size_t i = 0; i < step.counts.elements(); ++i ){
      int64_t statusCode = 0;
      if ( step.liveInputs.elementAt(i)->toString() != "true" ) statusCode -= 1;
      if ( step.masks     .elementAt(i)->toString() == "true" ) statusCode -= 2;
      if ( statusCode == 0 ) out << setfill(' ') << setw(10) << dynamic_cast<xdata::UnsignedInteger64*>( step.readCounts.elementAt(i) )->value_;
      else                   out << setfill(' ') << setw(10) << statusCode;
    }
    out << "\n";
  }
  return isFinished;
}

void emu::ldaq::manager::Application::getDataPaths(){
  emu::soap::Messenger m( this );
  
  dataFileNames_.clear(); // All RUIs' all data file names
  dataDirNames_.clear(); // All RUIs' data directory names
  for ( vector<const xdaq::ApplicationDescriptor*>::const_iterator rui = ruiDescriptors_.begin(); rui != ruiDescriptors_.end(); ++rui ){
    string ruiHost;
    xdata::Vector<xdata::String> dataFileNames;
    xdata::String dataDirName;
    try{
      m.getParameters( *rui, emu::soap::Parameters().add( "dataFileNames", &dataFileNames ).add( "pathToRUIDataOutFile", &dataDirName ) );
      ruiHost = toolbox::net::URL( (*rui)->getContextDescriptor()->getURL() ).getHost();
    }
    catch(xcept::Exception &e){
      stringstream ss;
      ss << "Failed to get list of data files from " << (*rui)->getClassName() << "." << (*rui)->getInstance() << " : " ;
      LOG4CPLUS_WARN(logger_, ss.str() + xcept::stdformat_exception_history(e));
    }
    // Add this RUI's data file names with the host name prepended:
    for ( size_t i = 0; i < dataFileNames.elements(); ++i ){
      dataFileNames_.push_back( ruiHost + ":" + ( dynamic_cast<xdata::String*> ( dataFileNames.elementAt(i)) )->toString() );
    }
    // Add this RUI's data directory name with the host name prepended:
    dataDirNames_.push_back( ruiHost + ":" + dataDirName.toString() );
  }
}

string emu::ldaq::manager::Application::getDateTime(){
  // Formatted as YYMMDD_hhmmss_UTC
  time_t t;
  struct tm tm;

  time ( &t );
  gmtime_r( &t, &tm ); // Unversal Coordinated Time (reentrant version for thread safety)

  ostringstream oss;
  oss << setfill('0') << setw(2) << tm.tm_year%100
     << setfill('0') << setw(2) << tm.tm_mon+1
     << setfill('0') << setw(2) << tm.tm_mday      << "_"
     << setfill('0') << setw(2) << tm.tm_hour
     << setfill('0') << setw(2) << tm.tm_min
     << setfill('0') << setw(2) << tm.tm_sec       << "_UTC";

  return oss.str();
}

string emu::ldaq::manager::Application::reformatTime( string time ){
  // reformat from YYMMDD_hhmmss_UTC to YYYY-MM-DD hh:mm:ss UTC
  string reformatted("");
  reformatted += "20";
  reformatted += time.substr(0,2);
  reformatted += "-";
  reformatted += time.substr(2,2);
  reformatted += "-";
  reformatted += time.substr(4,2);
  reformatted += " ";
  reformatted += time.substr(7,2);
  reformatted += ":";
  reformatted += time.substr(9,2);
  reformatted += ":";
  reformatted += time.substr(11,2);
  reformatted += " UTC";
  return reformatted;
}

void emu::ldaq::manager::Application::webRedirect(xgi::Input *in, xgi::Output *out)
  throw (xgi::exception::Exception)
{
  string url = in->getenv("PATH_TRANSLATED");
  
  cgicc::HTTPResponseHeader &header = out->getHTTPResponseHeader();
  
  header.getStatusCode(303);
  header.getReasonPhrase("See Other");
  header.addHeader("Location", url.substr(0, url.find("?")));
  // cout << "PATH_TRANSLATED " << url << endl << flush;
  // cout << "PATH_INFO " << in->getenv("PATH_INFO") << endl << flush;
  // cout << "Redirecting to " << url.substr(0, url.find("?")) << endl << flush;
}

void emu::ldaq::manager::Application::bookRunNumber(){
  // Don't book debug or STEP runs:
  if ( runType_.toString() == "Debug" ||  runType_.toString().find("STEP",0) != string::npos ) return;
  
  // Just in case it's left over from the previuos run:
  if ( runInfo_ ) {
    delete runInfo_; 
    runInfo_ = NULL;
  }

  try{
    runInfo_ = emu::supervisor::RunInfo::Instance( runDbBookingCommand_.toString(),
						   runDbWritingCommand_.toString(),
						   runDbUserFile_.toString(),
						   runDbAddress_.toString() );
  }
  catch( string e ){
    LOG4CPLUS_ERROR(logger_, e);
  }

  if ( runInfo_ ){
    
    const string sequence = "CMS.CSC"; // central run database
    
    LOG4CPLUS_INFO(logger_, "Booking run number with " <<
		   runDbBookingCommand_.toString() << " at " <<
		   runDbAddress_.toString()  << " for " << sequence );
    
  
    bool success = runInfo_->bookRunNumber( sequence );
    
    if ( success ){
      runNumber_         = runInfo_->runNumber();
      runSequenceNumber_ = runInfo_->runSequenceNumber();
      LOG4CPLUS_INFO(logger_, "Booked run rumber " << runNumber_.toString() <<
		     " (" << sequence << " " << runSequenceNumber_.toString() << ")");
    }
    else {
      LOG4CPLUS_ERROR(logger_,
		      "<![CDATA[ Failed to book run number: " 
		      <<  runInfo_->errorMessage()
		      << " ==> Falling back to run number " << runNumber_.value_ << " ]]>" );
    }
  } // if ( runInfo_ ){

}

void
emu::ldaq::manager::Application::configureRestartedApps(){
  // Get the halted apps, which were presumably restarted.
  vector<string> halted; // sysnonyms of "Halted"
  halted.push_back( "Halted" );
  set<const xdaq::ApplicationDescriptor*> haltedApps( watchdog_->getAppsInStates( halted ) );
  // Configure the RUIs that have been restarted
  for( vector<const xdaq::ApplicationDescriptor*>::const_iterator rui = ruiDescriptors_.begin(); rui != ruiDescriptors_.end(); ++rui ){
    if ( haltedApps.find( *rui )!=haltedApps.end() ){
      try{
	emu::soap::Messenger( this ).setParameters( *rui, 
						    emu::soap::Parameters()
						    .add( "runType"              , &runType_     )
						    .add( "writeBadEventsOnly"   , &writeBadEventsOnly_ )
						    .add( "maxEvents"            , &maxNumberOfEvents_ )
						    .add( "passDataOnToRUBuilder", &buildEvents_ ) );
	emu::soap::Messenger( this ).sendCommand( *rui, "Configure" );
      }
      catch( xcept::Exception &e ){
	stringstream oss;
	oss << "Watchdog failed to configure " << (*rui)->getClassName() << (*rui)->getInstance() << ": "<< xcept::stdformat_exception_history(e);
	LOG4CPLUS_ERROR( logger_, oss.str() );
      }
      ostringstream ss;
      ss << "Watchdog configured " << (*rui)->getClassName() << "." << (*rui)->getInstance();
      LOG4CPLUS_WARN( logger_, ss.str() );
      XCEPT_DECLARE( emu::ldaq::manager::exception::Exception, eObj, ss.str() );
      this->notifyQualified( "warning", eObj );
    }
  }
}

void
emu::ldaq::manager::Application::enableRestartedApps(){
  // Get the configured apps, which were presumably restarted.
  vector<string> configured; // sysnonyms of "Configured"
  configured.push_back( "Configured" );
  configured.push_back( "Ready"      );
  set<const xdaq::ApplicationDescriptor*> configuredApps( watchdog_->getAppsInStates( configured ) );
  // Enable the RUIs that have been restarted
  for( vector<const xdaq::ApplicationDescriptor*>::const_iterator rui = ruiDescriptors_.begin(); rui != ruiDescriptors_.end(); ++rui ){
    if ( configuredApps.find( *rui )!=configuredApps.end() ){
      try{
	emu::soap::Messenger( this ).setParameters( *rui,
						    emu::soap::Parameters()
						    .add( "runNumber"   , &runNumber_         )
						    .add( "maxEvents"   , &maxNumberOfEvents_ )
						    .add( "runStartTime", &runStartTime_      )
						    );
	emu::soap::Messenger( this ).sendCommand( *rui, "Enable" );
      }
      catch( xcept::Exception &e ){
	stringstream oss;
	oss << "Watchdog failed to enable " << (*rui)->getClassName() << (*rui)->getInstance() << ": "<< xcept::stdformat_exception_history(e);
	LOG4CPLUS_ERROR( logger_, oss.str() );
      }
      ostringstream ss;
      ss << "Watchdog enabled " << (*rui)->getClassName() << "." << (*rui)->getInstance();
      LOG4CPLUS_WARN( logger_, ss.str() );
      XCEPT_DECLARE( emu::ldaq::manager::exception::Exception, eObj, ss.str() );
      this->notifyQualified( "warning", eObj );
    }
  }
}

void
emu::ldaq::manager::Application::timeExpired(toolbox::task::TimerEvent& e){
  // Send out the watchdog to look for restarted applications and herd them back to the proper FSM state.

  // Hold back the watchdog while daq::manager is in a state transition
  if ( inFSMTransition_ ) return;

  // Watchdog cannot be constructed in app's constructor as it needs some exported parameters to have been set already.
  if ( watchdog_ == NULL ) watchdog_ = new emu::ldaq::manager::Watchdog( this );

  vector<string> halted;
  halted.push_back( "Halted" );
  vector<string> configured;
  configured.push_back( "Configured" );
  configured.push_back( "Ready"      );
  LOG4CPLUS_INFO( logger_, "Time expired for event " << e.type() );
  try{
    switch(fsm_.getCurrentState()){
    case 'H': // Halted
      break;
    case 'C': // Configured
      watchdog_->patrol();
      LOG4CPLUS_INFO( logger_, "Watchdog after patrol" << endl << *watchdog_ );
      if ( watchdog_->getAppsInStates( halted ).size() > 0 && !inFSMTransition_  ) configureRestartedApps();
      break;
    case 'E': // Enabled
      watchdog_->patrol();
      LOG4CPLUS_INFO( logger_, "Watchdog after first patrol" << endl << *watchdog_ );
      if ( watchdog_->getAppsInStates( halted ).size() > 0 && !inFSMTransition_ ) configureRestartedApps();
      watchdog_->patrol();
      LOG4CPLUS_INFO( logger_, "Watchdog after second patrol" << endl << *watchdog_ );
      if ( watchdog_->getAppsInStates( configured ).size() > 0 && !inFSMTransition_ ) enableRestartedApps();
      break;
    default:
      break;
    }
  } catch( xcept::Exception& e ){
    LOG4CPLUS_ERROR( logger_, "Watchdog failed to execute scheduled patrol: " << xcept::stdformat_exception_history(e) );
    XCEPT_DECLARE_NESTED( emu::ldaq::manager::exception::Exception, eObj, "Watchdog failed to execute scheduled patrol: ", e );
    this->notifyQualified( "error", eObj );
  }
}

void emu::ldaq::manager::Application::actionPerformed(xdata::Event & received )
{
  // implementation of virtual method of class xdata::ActionListener

  xdata::ItemEvent& e = dynamic_cast<xdata::ItemEvent&>(received);
  
  LOG4CPLUS_INFO(logger_, 
                 "Received an InfoSpace event" <<
                 " Event type: " << e.type() <<
                 " Event name: " << e.itemName() <<
                 " Serializable: " << std::hex << e.item() << std::dec <<
                 " Type of serializable: " << e.item()->type() );

  if ( e.itemName() == "buildEvents" && e.type() == "ItemChangedEvent"  ){
    retireWatchdog();
    timeWatchdog();
  }
  if      ( e.itemName() == "dataFileNames"  && e.type() == "ItemRetrieveEvent" ){ getDataPaths();     }
  else if ( e.itemName() == "dataDirNames"   && e.type() == "ItemRetrieveEvent" ){ getDataPaths();     }
  else if ( e.itemName() == "STEPFinished"   && e.type() == "ItemRetrieveEvent" ){ queryAppStatuses(); }
  else if ( e.itemName() == "STEPCount"      && e.type() == "ItemRetrieveEvent" ){ queryAppStatuses(); }
  else if ( e.itemName() == "daqState"       && e.type() == "ItemRetrieveEvent" ){ queryAppStatuses(); }
  // else if ( e.itemName() == "supervisedMode" && e.type() == "ItemChangedEvent"  ){
  //   sendFact( "emu::ldaq::manager::Application", LocalDAQStatusFact::getTypeName() );
  // }
  // else if ( e.itemName() == "stateName"      && e.type() == "ItemChangedEvent"  ){
  //   if ( state_ == "Halted" || state_ == "Enabled" || state_ == "Failed" )
  //   sendFact( "emu::ldaq::manager::Application", LocalDAQStatusFact::getTypeName() );
  // }
}

vector<emu::base::WebReportItem> 
emu::ldaq::manager::Application::materialToReportOnPage1(){
  vector<emu::base::WebReportItem> items;
  string controlURL( getApplicationDescriptor()->getContextDescriptor()->getURL() + "/" + getApplicationDescriptor()->getURN() );

  queryAppStatuses();
    
  // Title
  items.push_back( emu::base::WebReportItem( "title",
                                             "Local DAQ",
                                             "",
                                             "Click to visit the local DAQ Manager page.",
                                             "",
                                             controlURL ) );

  // State
  string state = currentAppStatuses_.getCombinedState();
  string valueTip;
  if ( ! ( state == "Enabled" || state == "Configured" || state == "Halted" ) ) 
    valueTip = "Local DAQ may need attention. Recreate it, or click to control it manually.";
  items.push_back( emu::base::WebReportItem( "state",
                                             state,
                                             "The overall state of local DAQ.",
                                             valueTip,
                                             controlURL,
                                             controlURL ) );

  // Run number
  items.push_back( emu::base::WebReportItem( "#",
                                             runNumber_.toString(),
                                             "Local run number. In unsupervised mode, this may be different from the global one.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );

  // Run type
  items.push_back( emu::base::WebReportItem( "type",
                                             runType_.toString(),
                                             "Run type.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );

  // Run controller
  items.push_back( emu::base::WebReportItem( "ctrl",
                                             ( bool( isGlobalInControl_ ) ? "global" : "local" ),
                                             "Whether local DAQ is controlled locally or by global DAQ.",
                                             ( bool( isGlobalInControl_ ) ? "Local DAQ was started centrally, in sync with global DAQ." : "Local DAQ was started locally, independently of central DAQ." ),
                                             controlURL,
                                             controlURL ) );

  // Start and end times
  items.push_back( emu::base::WebReportItem( "start",
                                             reformatTime( runStartTime_ ),
                                             "The start time of the local run in UTC.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );

  items.push_back( emu::base::WebReportItem( "stop",
					     ( ( state == "Enabled" || state == "Configured" ) ? "not yet" : reformatTime( runStopTime_ ) ), 
                                             "The stop time of the local run in UTC.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );

  // Progress of calibration runs
  items.push_back( emu::base::WebReportItem( "calib runIndex",
                                             calibRunIndex_.toString(),
                                             "Current run's index in the calibration sequence.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );
  items.push_back( emu::base::WebReportItem( "calib nRuns",
                                             calibNRuns_.toString(),
                                             "Total number of runs in the calibration sequence.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );
  items.push_back( emu::base::WebReportItem( "calib stepIndex",
                                             calibStepIndex_.toString(),
                                             "Index of the current step.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );
  items.push_back( emu::base::WebReportItem( "calib nSteps",
                                             calibNSteps_.toString(),
                                             "Total number of steps in this calibration run.",
                                             "Click to visit the local DAQ Manager page.",
                                             controlURL,
                                             controlURL ) );

  // Min and max RUI counts
  unsigned long maxCount = 0;
  unsigned long minCount = numeric_limits<unsigned long>::max();
  const xdaq::ApplicationDescriptor* minCountingApp = NULL;
  const xdaq::ApplicationDescriptor* maxCountingApp = NULL;
  map<const xdaq::ApplicationDescriptor*, unsigned long> counts = currentAppStatuses_.getAppEventCounts();
  for ( map<const xdaq::ApplicationDescriptor*, unsigned long>::const_iterator c = counts.begin(); c != counts.end(); ++c ){
    if ( c->first->getClassName() == "emu::ldaq::rui::Application" ){
      if ( c->second <= minCount ){ minCount = c->second; minCountingApp = c->first; }
      if ( c->second >= maxCount ){ maxCount = c->second; maxCountingApp = c->first; }
    }
  }
  try{
    ostringstream oss;
    oss << "Events read by RUI " << minCountingApp->getInstance() << ".";
    items.push_back( emu::base::WebReportItem( "min events",
					       emu::utils::stringFrom<unsigned long>( minCount ),
					       "The lowest number of events read by any RUI. In local runs, this should remain close to 'max'.",
					       oss.str(),
					       controlURL, controlURL ) );
    oss.str("");
    oss << "Events read by RUI " << maxCountingApp->getInstance() << ".";
    items.push_back( emu::base::WebReportItem( "max events",
					       emu::utils::stringFrom<unsigned long>( maxCount ),
					       "The highest number of events read by any RUI.",
					       oss.str(),
					       controlURL, controlURL ) );
  }
  catch( const std::exception& e ){
    LOG4CPLUS_WARN(logger_, "Failed to report min and max number of events to Page 1 : " << e.what() );
  }
  
  return items;
}

emu::base::Fact
emu::ldaq::manager::Application::findFact( const emu::base::Component& component, const string& factType ) {
  cout << "*** emu::ldaq::manager::Application::findFact " << component << " : " << factType << endl;

  vector<string> matches;

  if ( factType  == LocalDAQStatusFact::getTypeName() 
       &&
       component == emu::base::Component("emu::ldaq::manager::Application") ){
    cout << currentAppStatuses_;
    queryAppStatuses();
    cout << currentAppStatuses_;
    emu::base::TypedFact<LocalDAQStatusFact> ds;
    ds.setRun( runNumber_.toString() )
      .setComponent( component )
      .setSeverity( emu::base::Fact::INFO )
      .setDescription( "The status of the local DAQ." )
      .setParameter( LocalDAQStatusFact::runType,          runType_.toString()                       )
      .setParameter( LocalDAQStatusFact::state,            fsm_.getStateName(fsm_.getCurrentState()) )
      .setParameter( LocalDAQStatusFact::daqState,         currentAppStatuses_.getCombinedState()    )
      .setParameter( LocalDAQStatusFact::isSupervised,     supervisedMode_.toString()                )
      .setParameter( LocalDAQStatusFact::isBuildingEvents, buildEvents_.toString()                   );
    cout << ds;
    cout << "emu::ldaq::manager::Application::findFact ***" << endl;
    return ds;
  }
  else if ( factType == emu::base::ApplicationStatusFact::getTypeName() ){
    if ( component.isMatchedBy( "^(emu::ldaq::rui::Application|RUI)([0-9]+)$", matches ) ) {
      if ( matches.size() == 3 ){
	stringstream iss( matches[2] );
	uint32_t instance;
	iss >> instance;
	queryAppStatuses();
	for ( vector< const xdaq::ApplicationDescriptor* >::const_iterator ruid=ruiDescriptors_.begin(); ruid!=ruiDescriptors_.end(); ++ruid ){
	  if ( (*ruid)->getInstance() == instance ){
	    map<const xdaq::ApplicationDescriptor*, string> cas = currentAppStatuses_.getAppStates();
	    for ( map<const xdaq::ApplicationDescriptor*, string>::const_iterator s=cas.begin(); s!=cas.end(); ++s ){
	      if ( s->first == (*ruid) ){
		string ruiState = s->second; 
		emu::base::TypedFact<emu::base::ApplicationStatusFact> as;
		as.setRun( runNumber_.toString() )
		  .setComponent( component )
		  .setParameter( emu::base::ApplicationStatusFact::state, ruiState );
		if  ( ruiState == "UNKNOWN" ) 
		  as.setSeverity( emu::base::Fact::FATAL )
		    .setDescription( matches[0] + " is unreachable. It may have crashed, or the network may be down." );
		else if ( ruiState == "Failed"  )
		  as.setSeverity( emu::base::Fact::FATAL )
		    .setDescription( matches[0] + " is in 'Failed' state." );
		else
		  as.setSeverity( emu::base::Fact::INFO  )
		    .setDescription( matches[0] + " is in '" + ruiState + "' state." );
		cout << as;
		cout << "emu::ldaq::manager::Application::findFact ***" << endl;
		return as;
	      }
	    }
	  }
	}
      } // if ( matches.size() == 3 )
    } // if ( component.isMatchedBy( "^(emu::ldaq::rui::Application|RUI)([0-9]+)$", matches ) )
    else if ( component == emu::base::Component("emu::ldaq::manager::Application") ){
      string daqManagerState = fsm_.getStateName(fsm_.getCurrentState());
      emu::base::TypedFact<emu::base::ApplicationStatusFact> as;
      as.setRun( runNumber_.toString() )
	.setComponent( component )
	.setParameter( emu::base::ApplicationStatusFact::state, daqManagerState );
      if ( daqManagerState == "Failed"  )
	as.setSeverity( emu::base::Fact::FATAL )
	  .setDescription( matches[0] + " is in 'Failed' state." );
      else
	as.setSeverity( emu::base::Fact::INFO  )
	  .setDescription( matches[0] + " is in '" + daqManagerState + "' state." );
      cout << as;
      cout << "emu::ldaq::manager::Application::findFact ***" << endl;
      return as;
    }
  }

  stringstream ss;
  ss << "Failed to find fact of type \"" << factType
     << "\" on component \"" << component
     << "\" requested by expert system.";
  LOG4CPLUS_WARN( logger_, ss.str() );
  XCEPT_DECLARE( emu::ldaq::manager::exception::Exception, eObj, ss.str() );
  this->notifyQualified( "warning", eObj );

  // Return an untyped empty fact if no typed fact was found:
  return emu::base::Fact();
}

emu::base::FactCollection
emu::ldaq::manager::Application::findFacts() {
  emu::base::FactCollection fc;
  cout << "*** emu::ldaq::manager::Application::findFacts" << endl;

  // Report DAQ state.
  queryAppStatuses();
  string daqState = currentAppStatuses_.getCombinedState();
  emu::base::TypedFact<LocalDAQStatusFact> ds;
  ds.setComponentId( "emu::ldaq::manager::Application" )
    .setRun( runNumber_.toString() )
    .setSeverity( emu::base::Fact::INFO )
    .setParameter( LocalDAQStatusFact::runType,          runType_.toString()                       )
    .setParameter( LocalDAQStatusFact::state,            fsm_.getStateName(fsm_.getCurrentState()) )
    .setParameter( LocalDAQStatusFact::daqState,         daqState                                  )
    .setParameter( LocalDAQStatusFact::isSupervised,     supervisedMode_.toString()                )
    .setParameter( LocalDAQStatusFact::isBuildingEvents, buildEvents_.toString()                   );
  fc.addFact( ds );

  // Report crashed RUIs, if any.
  if ( daqState == "UNKNOWN" ){
    for ( vector< const xdaq::ApplicationDescriptor* >::const_iterator ruid=ruiDescriptors_.begin(); ruid!=ruiDescriptors_.end(); ++ruid ){
      map<const xdaq::ApplicationDescriptor*, string> cas = currentAppStatuses_.getAppStates();
      for ( map<const xdaq::ApplicationDescriptor*, string>::const_iterator s=cas.begin(); s!=cas.end(); ++s ){
	if ( s->first == (*ruid) && s->second == "UNKNOWN" ){
	  stringstream ruiName;
	  ruiName << (*ruid)->getClassName() << setfill('0') << setw(2) << (*ruid)->getInstance();
// 	  ruiName << "RUI" << setfill('0') << setw(2) << (*ruid)->getInstance();
	  emu::base::TypedFact<emu::base::ApplicationStatusFact> as;
	  as.setComponentId( ruiName.str() )
	    .setRun( runNumber_.toString() )
	    .setSeverity( emu::base::Fact::FATAL )
	    .setDescription( ruiName.str() + " is unreachable. It may have crashed, or the network may be down." )
	    .setParameter( emu::base::ApplicationStatusFact::state, "UNKNOWN" );
	  fc.addFact( as );
	}
      }
    }
  }
  cout << fc << endl;

  cout << "emu::ldaq::manager::Application::findFacts ***" << endl;
  return fc;
}

XDAQ_INSTANTIATOR_IMPL(emu::ldaq::manager::Application)
