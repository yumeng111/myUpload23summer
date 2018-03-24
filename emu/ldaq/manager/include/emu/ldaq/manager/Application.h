#ifndef __emu_ldaq_manager_Application_h__
#define __emu_ldaq_manager_Application_h__

#include "emu/ldaq/manager/exception/Exception.h"
#include "emu/ldaq/manager/Watchdog.h"
#include "emu/ldaq/manager/AppStatuses.h"
#include "emu/supervisor/RunInfo.h"
#include "emu/base/Supervised.h"
#include "emu/base/WebReporter.h"
#include "emu/base/FactFinder.h"

#include "xdaq/WebApplication.h"
#include "xdata/Boolean.h"
#include "xdata/UnsignedInteger32.h"
#include "xdata/UnsignedInteger64.h"
#include "xdata/Integer64.h"
#include "xdata/Vector.h"
#include "toolbox/task/WorkLoop.h"

#include <string>

using namespace std;

namespace emu { namespace ldaq { namespace manager {

      class Watchdog;
class Application 
  : public emu::base::Supervised
    , public emu::base::WebReporter
    , public emu::base::FactFinder
    , public xdata::ActionListener
{

  friend class Watchdog;

public:

    /**
     * Define factory method for the instantion of emu::ldaq::manager::Application applications.
     */
    XDAQ_INSTANTIATOR();

    /**
     * Constructor.
     */
    Application(xdaq::ApplicationStub *s)
    throw (xdaq::exception::Exception);

    /**
     * Invoked when an exception has been received from the sentinel.
     */
    void onException(xcept::Exception &e);

private:

  Logger logger_;
  
  bool inFSMTransition_;
  
  emu::ldaq::manager::Watchdog* watchdog_;
  emu::supervisor::RunInfo *runInfo_; // communicates with run database

  vector< const xdaq::ApplicationDescriptor* > evmDescriptors_;
  vector< const xdaq::ApplicationDescriptor* > buDescriptors_;
  vector< const xdaq::ApplicationDescriptor* > ruDescriptors_;
  vector< const xdaq::ApplicationDescriptor* > ruiDescriptors_;
  vector< const xdaq::ApplicationDescriptor* > utcpDescriptors_;
  
  emu::ldaq::manager::AppStatuses currentAppStatuses_;
  emu::ldaq::manager::AppStatuses previousAppStatuses_;

  // For driving the FSM by asynchronous SOAP.
  toolbox::task::WorkLoop *workLoop_;
  toolbox::task::ActionSignature *configureSignature_;
  toolbox::task::ActionSignature *enableSignature_;
  toolbox::task::ActionSignature *haltSignature_;

  // InfoSpace members
  xdata::UnsignedInteger32 runNumber_;
  xdata::UnsignedInteger32 runSequenceNumber_;
  xdata::Integer64 maxNumberOfEvents_;
  xdata::String runStartTime_;
  xdata::String runStopTime_;
  xdata::Vector<xdata::String> runTypes_; // all possible run types
  xdata::String runType_; // the current run type
  xdata::Boolean isGlobalInControl_;
  xdata::Boolean supervisedMode_; // true if we obey SOAP command only, not the web interface
  xdata::Boolean configuredInSupervisedMode_; // true if this run was configured by SOAP command (presumably by the Supervisor)
  xdata::Boolean writeBadEventsOnly_;
  xdata::String daqState_; // the combinedstate of the apps we manage
  xdata::Boolean buildEvents_;
  xdata::String hardwareMapping_; // file of the hardware mapping (path relative to XDAQ_ROOT

  xdata::UnsignedInteger32 calibRunIndex_;
  xdata::UnsignedInteger32 calibNRuns_;
  xdata::UnsignedInteger32 calibStepIndex_;
  xdata::UnsignedInteger32 calibNSteps_;
  xdata::Vector<xdata::String> dataFileNames_; // all RUIs' all data file names
  xdata::Vector<xdata::String> dataDirNames_; // all RUIs' data directory names
  xdata::UnsignedInteger64 STEPCount_; // The lowest of any RUI's lowest count in a STEP run.
  xdata::Boolean STEPFinished_; // set to TRUE when all DDUs' all live and unmasked inputs have produced the requested number of events
  xdata::String STEPCountsTableDir_; // Directory to save the STEP counts table in as an ascii file. Default: /tmp

  xdata::String runDbBookingCommand_; // e.g. "java -jar runnumberbooker.jar"
  xdata::String runDbWritingCommand_; // e.g. "java -jar runinfowriter.jar"
  xdata::String runDbAddress_;        // e.g. "dbc:oracle:thin:@oracms.cern.ch:10121:omds"
  xdata::String runDbUserFile_;       // file that contains the username:password for run db user
  
  string generateLoggerName();

  void getAllAppDescriptors();
  vector< const xdaq::ApplicationDescriptor* > getAppDescriptors( const xdaq::Zone *zone, const string appClass )
    throw (emu::ldaq::manager::exception::Exception);
  
  void exportParams();
  void createAllAppStatuses();
  void queryAppStatuses();
  void bindSOAPCommands();
  void setUpFSM();
  void timeWatchdog();
  void retireWatchdog();
  void waitForAppsState( vector<const xdaq::ApplicationDescriptor*> apps, const string state, const unsigned int seconds );
  void stateChanged(toolbox::fsm::FiniteStateMachine &fsm) throw (toolbox::fsm::exception::Exception);
  void maskDDUInputs( const bool in, const std::vector<cgicc::FormEntry>& fev );
  void sendDDUInputMask( const bool in, const uint32_t ruiInstance, const std::set<uint32_t>& inputs );
  void saveSTEPCountsTable();
  bool printSTEPCountsTableASCII( stringstream& out );
  void getDataPaths();
  string getDateTime();
  string reformatTime( string time );
  void bookRunNumber();

  void configureDAQ() throw (emu::ldaq::manager::exception::Exception);
  void startDAQ() throw (emu::ldaq::manager::exception::Exception);
  void stopDAQ() throw (emu::ldaq::manager::exception::Exception);

  // Web interface
  void defaultWebPage(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);
  uint32_t purgeIntNumberString( string* s );
  void processCommandForm(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);
  void webRedirect(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);

  // Watchdog's tasks
  void configureRestartedApps();
  void enableRestartedApps();
  
public:
  // SOAP interface
  xoap::MessageReference onConfigure(xoap::MessageReference message)
    throw (xoap::exception::Exception);
  xoap::MessageReference onEnable(xoap::MessageReference message)
    throw (xoap::exception::Exception);
  xoap::MessageReference onHalt(xoap::MessageReference message)
    throw (xoap::exception::Exception);

  // Asynchronous state transitions
  bool configureActionInWorkLoop(toolbox::task::WorkLoop *wl);
  bool enableActionInWorkLoop(toolbox::task::WorkLoop *wl);
  bool haltActionInWorkLoop(toolbox::task::WorkLoop *wl);

  // State transitions
  void configureAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);
  void enableAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);
  void haltAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);
  
  void reConfigureAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);
  void noAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);

  // Virtual methods' implementation
  void timeExpired(toolbox::task::TimerEvent& e); 
  void actionPerformed(xdata::Event & received ); // inherited from xdata::ActionListener
  vector<emu::base::WebReportItem> materialToReportOnPage1(); // inherited from WebReporter 
  emu::base::Fact           findFact( const emu::base::Component& component, const string& factType ); // inherited from FactFinder
  emu::base::FactCollection findFacts(); // inherited from FactFinder
};


      
}}} // namespace emu::ldaq::manager

#endif
