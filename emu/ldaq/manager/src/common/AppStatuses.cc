#include "emu/ldaq/manager/AppStatuses.h"

#include "emu/utils/String.h"

#include "toolbox/net/URL.h"

#include "xercesc/dom/DOMElement.hpp"
#include "xercesc/dom/DOMAttr.hpp"
#include "xercesc/util/XMLString.hpp"

#include <sstream>
#include <iomanip>

using namespace std;
using namespace xercesc;

emu::ldaq::manager::AppStatuses::AppStatuses()
  : bSem_( toolbox::BSem::FULL )
  , timeOfUpdate_( 0 )
{}

emu::ldaq::manager::AppStatuses::AppStatuses( const emu::ldaq::manager::AppStatuses& other )
  : bSem_              ( toolbox::BSem::EMPTY          )
  , timeOfUpdate_      ( other.getUnixTimeOfUpdate()   )
  , synonyms_          ( other.getSynonyms()           )
  , appStates_         ( other.getAppStates()          )
  , appEventCounts_    ( other.getAppEventCounts()     )
  , appEventStatistics_( other.getAppEventStatistics() )
{
  bSem_.give();
}

emu::ldaq::manager::AppStatuses&
emu::ldaq::manager::AppStatuses::defineSynonyms( const string canonicalStateName, const string csvOfAlternativeStateNames ){
  set<string> alternatives( emu::utils::csvTo< set<string> >( csvOfAlternativeStateNames ) );
  alternatives.insert( canonicalStateName ); // Add canonical to set of synonyms, just in case it's not listed.
  if ( synonyms_.find( canonicalStateName ) != synonyms_.end() ) synonyms_[ canonicalStateName ].insert( alternatives.begin(), alternatives.end() );
  else                                                           synonyms_[ canonicalStateName ] = alternatives;
  return *this;
}

emu::ldaq::manager::AppStatuses&
emu::ldaq::manager::AppStatuses::setAppStatus( const xdaq::ApplicationDescriptor* ad, const string& state, const unsigned long eventCount, const rui::EventStatistics* statistics, const STEPInfo* step ){
  bSem_.take();
  appStates_[ad] = state;
  // getCanonicalStateName returns its argument if it's not yet member of any synonym class or it is the canonical name of a synonym class.
  // In the first case, defineSynonyms will create a new synonym class for it, in the second case it will do nothing:
  if ( getCanonicalStateName( state ) == state ) defineSynonyms( state, state );
  appEventCounts_[ad] = eventCount;
  if ( statistics != NULL ) appEventStatistics_[ad] = *statistics;
  if ( step       != NULL ) stepInfo_[ad]           = *step;
  time( &timeOfUpdate_ ); // time stamp this update
  bSem_.give();
  return *this;
}

emu::ldaq::manager::AppStatuses&
emu::ldaq::manager::AppStatuses::operator=( const emu::ldaq::manager::AppStatuses& other ){
  if ( this == &other ) return *this;
  bSem_.take();
  timeOfUpdate_       = other.getUnixTimeOfUpdate();
  appStates_          = other.getAppStates();
  appEventCounts_     = other.getAppEventCounts();
  appEventStatistics_ = other.getAppEventStatistics();
  stepInfo_           = other.getSTEPInfo();
  bSem_.give();
  return *this;
}

set<const xdaq::ApplicationDescriptor*>
emu::ldaq::manager::AppStatuses::getApps() const {
  set<const xdaq::ApplicationDescriptor*> apps;
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;
  bSem_.take();
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s ) apps.insert( s->first );
  bSem_.give();
  return apps;
}

set<const xdaq::ApplicationDescriptor*>
emu::ldaq::manager::AppStatuses::getAppsInState( const string& state ) const {
  set<const xdaq::ApplicationDescriptor*> apps;
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;
  bSem_.take();
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s ){
    if ( s->second == state ) apps.insert( s->first );
  }
  bSem_.give();
  return apps;
}

string
emu::ldaq::manager::AppStatuses::getCanonicalStateName( const string& state ) const {
  for ( map< string, set<string> >::const_iterator syn = synonyms_.begin(); syn != synonyms_.end(); ++syn ){
    if ( syn->second.find( state ) != syn->second.end() ) return syn->first;
  }
  return state;
}

uint64_t
emu::ldaq::manager::AppStatuses::getLowestOfLowestSTEPCount() const {
  // Return the lowest of the DDUs' lowest input event counts.
  uint64_t lowest = numeric_limits<uint64_t>::max();
  bSem_.take();
  if ( ! hasSTEPInfo() ){
    bSem_.give();
    return lowest;
  }
  // Cannot use STEPInfo::lowestCount as it will remain zero for DDUs whose all inputs are killed. Those should disregarded, but we don't know that.
  // We check each individual input of every DDU here.
  for ( map<const xdaq::ApplicationDescriptor*, STEPInfo>::const_iterator s=stepInfo_.begin(); s!=stepInfo_.end(); ++s ){
    for ( size_t i = 0; i < s->second.counts.size(); i++ ){
      // if ( s->second.lowestCount < lowestLowest ) lowestLowest = s->second.lowestCount;
      // Only consider non-masked live inputs:
      if ( ! bool ( *dynamic_cast<const xdata::Boolean*>( &s->second.masks     .at(i) ) ) &&
	     bool ( *dynamic_cast<const xdata::Boolean*>( &s->second.liveInputs.at(i) ) )    ){

	uint64_t count = uint64_t( *dynamic_cast<const xdata::UnsignedInteger64*>( &s->second.counts.at(i) ) );
	if ( count < lowest ) lowest = count;

      }
    }
  }
  bSem_.give();
  return ( lowest == numeric_limits<uint64_t>::max() ? 0 : lowest );
}

uint64_t
emu::ldaq::manager::AppStatuses::getLowestCount( const string appName ) const {
  uint64_t lowest = numeric_limits<uint64_t>::max();
  bSem_.take();
  for ( map<const xdaq::ApplicationDescriptor*, unsigned long>::const_iterator ac=appEventCounts_.begin(); ac!=appEventCounts_.end(); ++ac ){
    if ( ( appName.size() == 0 || ac->first->getClassName() == appName ) && ac->second < lowest ) lowest = ac->second;
  }
  bSem_.give();
  return lowest;
}

string
emu::ldaq::manager::AppStatuses::getTimeOfUpdate() const {
  struct tm lt;
  localtime_r( &timeOfUpdate_, &lt ); // reentrant version for thread safety

  stringstream ss;
  ss << setfill('0') << setw(4) << lt.tm_year+1900 << "-"
     << setfill('0') << setw(2) << lt.tm_mon+1     << "-"
     << setfill('0') << setw(2) << lt.tm_mday      << " "
     << setfill('0') << setw(2) << lt.tm_hour      << ":"
     << setfill('0') << setw(2) << lt.tm_min       << ":"
     << setfill('0') << setw(2) << lt.tm_sec;
  return ss.str();
}

string
emu::ldaq::manager::AppStatuses::getCombinedStateNTS( const string& className ) const {
  // Not thread-safe method, to be used in private only, invoked from within thread-safe methods.
  // Combine states:
  // If one is failed, the combined state will also be failed.
  // Else, if one is unknown, the combined state will also be unknown.
  // Else, if all are known but not the same or not synonymous, the combined state will be indefinite.
  string combinedState("UNKNOWN");
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;
  // First check if any failed:
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s )
    if ( className.size() == 0 || s->first->getClassName() == className ){
      if ( s->second == "Failed" ){
	combinedState = s->second;
	bSem_.give();
	return combinedState;
      }
    }
  // If none failed:
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s ){
    if ( className.size() == 0 || s->first->getClassName() == className ){
      if ( s->second == "UNKNOWN" ){
	combinedState = s->second;
	break;
      }
      else if ( ! areSynonymous( s->second, combinedState ) && combinedState != "UNKNOWN" ){
	combinedState = "INDEFINITE";
	break;
      }
      else{
	// if ( s->second.find( "Mismatch", 0 ) != string::npos )
	// 	// DAQ is still "enabled" while RU is seeing mismatch but has not timed out TODO: Can this happen in EVB? Is there such a state?
	// 	combinedState = "Enabled";
	// else
	combinedState = getCanonicalStateName( s->second );
      }
    }
  }
  return combinedState;
}

string
emu::ldaq::manager::AppStatuses::getCombinedState() const {
  // Thread-safe method, to be used in public.
  // Combine states:
  // If one is failed, the combined state will also be failed.
  // Else, if one is unknown, the combined state will also be unknown.
  // Else, if all are known but not the same or not synonymous, the combined state will be indefinite.
  bSem_.take();
  string combinedState( getCombinedStateNTS( string() ) );
  bSem_.give();
  return combinedState;
}

string
emu::ldaq::manager::AppStatuses::getCombinedState( const string& className ) const {
  // Thread-safe method, to be used in public.
  // Combine states:
  // If one is failed, the combined state will also be failed.
  // Else, if one is unknown, the combined state will also be unknown.
  // Else, if all are known but not the same or not synonymous, the combined state will be indefinite.
  bSem_.take();
  string combinedState( getCombinedStateNTS( className ) );
  bSem_.give();
  return combinedState;
}

time_t
emu::ldaq::manager::AppStatuses::getAgeInSeconds() const {
  bSem_.take();
  time_t ageInSec = getAgeInSecondsNTS();
  bSem_.give();
  return ageInSec;
}

time_t
emu::ldaq::manager::AppStatuses::getAgeInSecondsNTS() const {
  time_t ageInSec;
  time_t now;
  time( &now );
  ageInSec = now - timeOfUpdate_;
  return ageInSec;
}

bool
emu::ldaq::manager::AppStatuses::areSynonymous( const string& stateName1, const string& stateName2 ) const {
  for ( map< string, set<string> >::const_iterator syn = synonyms_.begin(); syn != synonyms_.end(); ++syn ){
    if ( syn->second.find( stateName1 ) != syn->second.end() && syn->second.find( stateName2 ) != syn->second.end() ) return true;
  }
  return false;
}

bool
emu::ldaq::manager::AppStatuses::hasSTEPInfo() const {
  // Not thread safe. To be invoked from within therad-safe methods only.
  if ( stepInfo_.size() == 0 ) return false;
  map<const xdaq::ApplicationDescriptor*, STEPInfo>::const_iterator s;  
  for ( s=stepInfo_.begin(); s!=stepInfo_.end(); ++s ) if ( s->second.counts.size() > 0 ) return true;
  return false;
}

bool
emu::ldaq::manager::AppStatuses::isEmpty() const {
  if ( appStates_.size() == 0 ) return true;
  bSem_.take();
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;  
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s ){
    if ( s->second.size() == 0 ){
      bSem_.give();
      return true;
    }
  }
  bSem_.give();
  return false;
}

void
emu::ldaq::manager::AppStatuses::clear(){
  bSem_.take();
  appStates_.clear();
  appEventCounts_.clear();
  appEventStatistics_.clear();
  stepInfo_.clear();
  synonyms_.clear();
  bSem_.give();
}

DOMDocumentFragment* 
emu::ldaq::manager::AppStatuses::toDOM( DOMDocument* doc ) const {
  bSem_.take();
  bool hasSTEP = hasSTEPInfo(); // If it has STEP info, we'll add that to the DOM, too.
  DOMDocumentFragment* fragment = doc->createDocumentFragment();
  DOMElement *appsElem = doc->createElement( xercesc::XMLString::transcode( "applications" ) );

  DOMAttr *attr = doc->createAttribute( xercesc::XMLString::transcode( "combinedState" ) );
  attr->setValue( xercesc::XMLString::transcode( getCombinedStateNTS( string() ).c_str() ) );
  appsElem->setAttributeNode( attr );

  attr = doc->createAttribute( xercesc::XMLString::transcode( "updateTime" ) );
  attr->setValue( xercesc::XMLString::transcode( getTimeOfUpdate().c_str() ) );
  appsElem->setAttributeNode( attr );

  DOMElement *appElem = NULL;
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;  
  for ( s=appStates_.begin(); s!=appStates_.end(); ++s ){
    appElem = doc->createElement( xercesc::XMLString::transcode( "application" ) );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "class" ) );
    attr->setValue( xercesc::XMLString::transcode( s->first->getClassName().c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "instance" ) );
    attr->setValue( xercesc::XMLString::transcode( emu::utils::stringFrom<unsigned int>( s->first->getInstance() ).c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "state" ) );
    attr->setValue( xercesc::XMLString::transcode( s->second.c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "canonicalState" ) );
    attr->setValue( xercesc::XMLString::transcode( getCanonicalStateName( s->second ).c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "host" ) );
    attr->setValue( xercesc::XMLString::transcode( toolbox::net::URL( s->first->getContextDescriptor()->getURL() ).getHost().c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "port" ) );
    attr->setValue( xercesc::XMLString::transcode( emu::utils::stringFrom<unsigned int>( toolbox::net::URL( s->first->getContextDescriptor()->getURL() ).getPort() ).c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "appURN" ) );
    attr->setValue( xercesc::XMLString::transcode( s->first->getURN().c_str() ) );
    appElem->setAttributeNode( attr );

    attr = doc->createAttribute( xercesc::XMLString::transcode( "eventCount" ) );
    attr->setValue( xercesc::XMLString::transcode( emu::utils::stringFrom<unsigned long>( appEventCounts_.at( s->first ) ).c_str() ) );
    appElem->setAttributeNode( attr );

    if ( appEventStatistics_.find( s->first ) != appEventStatistics_.end() ){
      attr = doc->createAttribute( xercesc::XMLString::transcode( "dataRate" ) );
      attr->setValue( xercesc::XMLString::transcode( appEventStatistics_.at( s->first ).dataRate.toString().c_str() ) );
      appElem->setAttributeNode( attr );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "eventRate" ) );
      attr->setValue( xercesc::XMLString::transcode( appEventStatistics_.at( s->first ).eventRate.toString().c_str() ) );
      appElem->setAttributeNode( attr );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "sampledFraction" ) );
      attr->setValue( xercesc::XMLString::transcode( appEventStatistics_.at( s->first ).sampledFraction.toString().c_str() ) );
      appElem->setAttributeNode( attr );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "sizeMean" ) );
      attr->setValue( xercesc::XMLString::transcode( appEventStatistics_.at( s->first ).sizeMean.toString().c_str() ) );
      appElem->setAttributeNode( attr );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "sizeStD" ) );
      attr->setValue( xercesc::XMLString::transcode( appEventStatistics_.at( s->first ).sizeStD.toString().c_str() ) );
      appElem->setAttributeNode( attr );
    }
    
    if ( hasSTEP && stepInfo_.find( s->first ) != stepInfo_.end() ){
      DOMElement *stepElem = doc->createElement( xercesc::XMLString::transcode( "STEP" ) );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "eventsRead" ) );
      attr->setValue( xercesc::XMLString::transcode( stepInfo_.at( s->first ).eventsRead.toString().c_str() ) );
      stepElem->setAttributeNode( attr );
      
      attr = doc->createAttribute( xercesc::XMLString::transcode( "totalCount" ) );
      attr->setValue( xercesc::XMLString::transcode( stepInfo_.at( s->first ).totalCount.toString().c_str() ) );
      stepElem->setAttributeNode( attr );

      attr = doc->createAttribute( xercesc::XMLString::transcode( "lowestCount" ) );
      attr->setValue( xercesc::XMLString::transcode( stepInfo_.at( s->first ).lowestCount.toString().c_str() ) );
      stepElem->setAttributeNode( attr );

      DOMElement *errorElem = doc->createElement( xercesc::XMLString::transcode( "persistentDDUError" ) );
      errorElem->setTextContent( xercesc::XMLString::transcode( stepInfo_.at( s->first ).persistentDDUError.c_str() ) );
      stepElem->appendChild( errorElem );
      
      for ( size_t i = 0; i < stepInfo_.at( s->first ).counts.size(); ++i ){
	DOMElement *inputElem = doc->createElement( xercesc::XMLString::transcode( "input" ) );

	attr = doc->createAttribute( xercesc::XMLString::transcode( "channel" ) );
	attr->setValue( xercesc::XMLString::transcode( emu::utils::stringFrom<size_t>( i ).c_str() ) );
	inputElem->setAttributeNode( attr );

	attr = doc->createAttribute( xercesc::XMLString::transcode( "count" ) );
	attr->setValue( xercesc::XMLString::transcode( ( dynamic_cast<const xdata::UnsignedInteger64*>( &stepInfo_.at( s->first ).counts.at(i) ) )->toString().c_str() ) );
	inputElem->setAttributeNode( attr );

	attr = doc->createAttribute( xercesc::XMLString::transcode( "readCount" ) );
	attr->setValue( xercesc::XMLString::transcode( ( dynamic_cast<const xdata::UnsignedInteger64*>( &stepInfo_.at( s->first ).readCounts.at(i) ) )->toString().c_str() ) );
	inputElem->setAttributeNode( attr );

	attr = doc->createAttribute( xercesc::XMLString::transcode( "masked" ) );
	attr->setValue( xercesc::XMLString::transcode( ( dynamic_cast<const xdata::Boolean*>( &stepInfo_.at( s->first ).masks.at(i) ) )->toString().c_str() ) );
	inputElem->setAttributeNode( attr );

	attr = doc->createAttribute( xercesc::XMLString::transcode( "live" ) );
	attr->setValue( xercesc::XMLString::transcode( ( dynamic_cast<const xdata::Boolean*>( &stepInfo_.at( s->first ).liveInputs.at(i) ) )->toString().c_str() ) );
	inputElem->setAttributeNode( attr );

	stepElem->appendChild( inputElem );
      } // for ( size_t i = 0; i < stepInfo_.at( s->first ).counts.size(); ++i )
      appElem->appendChild( stepElem );
    } // if ( hasSTEP )
    appsElem->appendChild( appElem );
  } // for ( s=appStates_.begin(); s!=appStates_.end(); ++s )
  fragment->appendChild( appsElem );

  bSem_.give();
  return fragment;
}

ostream&
emu::ldaq::manager::operator<<( ostream& os, emu::ldaq::manager::AppStatuses& as ){
  as.bSem_.take();
  os << "Application states and counts updated at " << as.getTimeOfUpdate()
     << " (" << as.timeOfUpdate_
     << "), " << as.getAgeInSecondsNTS()
     << " seconds ago:" << endl;
  map<const xdaq::ApplicationDescriptor*, string>::const_iterator s;
  for ( s=as.appStates_.begin(); s!=as.appStates_.end(); ++s ){
    os << "   " << s->first->getClassName() << s->first->getInstance()
       << " " << s->second
       << " " << as.appEventCounts_[ s->first ]
       << endl;
  }
  as.bSem_.give();
  os << "Combined state is '" << as.getCombinedState() << "'" << endl;
  return os;
}
