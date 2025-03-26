#include "emu/ldaq/manager/Watchdog.h"
#include "emu/soap/Messenger.h"
#include <algorithm>

emu::ldaq::manager::Watchdog::Watchdog( emu::ldaq::manager::Application *owner )
  : application_( owner ){
  insertAllApps( currentAppStatuses_ );
}

void
emu::ldaq::manager::Watchdog::insertAllApps( emu::ldaq::manager::AppStatuses& appStatuses ){
  std::cout << "[emu::ldaq::manager::Watchdog::insertAllApps] application_->buildEvents_.value_ = " << application_->buildEvents_.value_ << std::endl; std::cout.flush();
  if ( application_->buildEvents_.value_ ){ 
    appStatuses.insertApps( application_->evmDescriptors_.begin(), application_->evmDescriptors_.end() );
    appStatuses.insertApps( application_->buDescriptors_ .begin(), application_->buDescriptors_ .end() );
    appStatuses.insertApps( application_->ruDescriptors_ .begin(), application_->ruDescriptors_ .end() );
  }
  appStatuses.insertApps( application_->ruiDescriptors_.begin(), application_->ruiDescriptors_.end() );
}

void
emu::ldaq::manager::Watchdog::patrol(){

  emu::soap::Messenger m( application_ );

  // Make sure it's only updated if currentAppStatuses_ is not empty. (It may be empty during configuring, for instance.)
  if ( ! currentAppStatuses_.isEmpty() ) previousAppStatuses_ = currentAppStatuses_;

  std::set<const xdaq::ApplicationDescriptor*> apps = currentAppStatuses_.getApps();
  std::set<const xdaq::ApplicationDescriptor*>::iterator a;
  xdata::String state( "UNKNOWN" );
  xdata::UnsignedLong events( 0 );
  for ( a=apps.begin(); a!=apps.end(); ++a ){
    state = "UNKNOWN";
    events = 0;
    try
      {
	emu::soap::Parameters p;
	p.add( "stateName", &state );
	if      ( (*a)->getClassName() == "emu::ldaq::rui::Application"                   ) p.add( "nEventsRead"  , &events );
	else if ( (*a)->getClassName() == "evb::RU" || (*a)->getClassName() == "evb::EVM" ) p.add( "eventCount"   , &events );
	else if ( (*a)->getClassName() == "evb::BU"                                       ) p.add( "nbEventsBuilt", &events );
	m.getParameters( *a, p );
      }
    catch(xcept::Exception &e)
      {
	state = "UNKNOWN";
	events = 0;
	ostringstream oss;
	oss << "Failed to get event count and state of " << (*a)->getClassName() << "." << (*a)->getInstance() << " : " ;
	LOG4CPLUS_WARN(application_->logger_, oss.str() + xcept::stdformat_exception_history(e));
      }
    currentAppStatuses_.setAppStatus( *a, state, events, NULL, NULL );
  }
  //cout << "Previous " << previousAppStatuses_;
  //cout << "Current "  << currentAppStatuses_;
}

set<const xdaq::ApplicationDescriptor*>
emu::ldaq::manager::Watchdog::getAppsInStates( const std::vector<std::string>& states ) const {
  // The collection of apps that have been in <state> since the previous patrol
  set<const xdaq::ApplicationDescriptor*> apps;

  // No reliable conclusion until both previous and current app states are meaningful:
  if ( currentAppStatuses_.isEmpty() || previousAppStatuses_.isEmpty() ) return apps;

  // All the apps that are in any of the states given by the argument:
  set<const xdaq::ApplicationDescriptor*> prevApps;
  set<const xdaq::ApplicationDescriptor*> currApps;
  // Loop over all state names:
  for ( vector<string>::const_iterator s=states.begin(); s!=states.end(); ++s ){
    set<const xdaq::ApplicationDescriptor*> pa = previousAppStatuses_.getAppsInState( *s );
    set<const xdaq::ApplicationDescriptor*> ca =  currentAppStatuses_.getAppsInState( *s );
    // Add apps that are in this state:
    prevApps.insert( pa.begin(), pa.end() );
    currApps.insert( ca.begin(), ca.end() );
  }

  // Collect those that are in both sets (previous and current) (set_intersection is in <algorithm>):
  set_intersection( prevApps.begin(), prevApps.end(),
		    currApps.begin(), currApps.end(),
		    std::insert_iterator< set<const xdaq::ApplicationDescriptor*> >( apps, apps.begin()) );
  return apps;
}

ostream&
emu::ldaq::manager::operator<<( ostream& os,  emu::ldaq::manager::Watchdog& wd ){
  os << "Previous:" << endl << wd.previousAppStatuses_ << endl 
     << "Current :" << endl << wd.currentAppStatuses_  << endl; 
  return os;
}
