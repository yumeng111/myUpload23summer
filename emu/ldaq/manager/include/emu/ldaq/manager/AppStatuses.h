#ifndef __emu_ldaq_manager_AppStatuses_h__
#define __emu_ldaq_manager_AppStatuses_h__

#include "emu/ldaq/manager/STEPInfo.h"

#include "toolbox/BSem.h"
#include "xdaq/ApplicationDescriptor.h"

#include "xercesc/dom/DOMDocument.hpp"
#include "xercesc/dom/DOMDocumentFragment.hpp"

#include <string>
#include <ostream>
#include <map>
#include <set>
#include <utility>
#include <time.h>

using namespace std;

namespace emu { namespace ldaq { namespace manager {

  class AppStatuses {
  public:
    friend ostream& operator<<( ostream& os,  emu::ldaq::manager::AppStatuses& as );
    
    AppStatuses();
    AppStatuses( const emu::ldaq::manager::AppStatuses& other );
    emu::ldaq::manager::AppStatuses& defineSynonyms( const string canonicalStateName, const string csvOfAlternativeStateNames );
    emu::ldaq::manager::AppStatuses& setAppStatus( const xdaq::ApplicationDescriptor* ad, const string& state, const unsigned long eventCount, const STEPInfo* step );
    // emu::ldaq::manager::AppStatuses& setAppState( const xdaq::ApplicationDescriptor* ad, const string& state );
    // emu::ldaq::manager::AppStatuses& setAppEventCount( const xdaq::ApplicationDescriptor* ad, const unsigned long eventCount );
    emu::ldaq::manager::AppStatuses& operator=( const emu::ldaq::manager::AppStatuses& other );
    // emu::ldaq::manager::AppStatuses& operator=( const map<const xdaq::ApplicationDescriptor*, string>& appStates );
    map< string, set<string> > getSynonyms() const { return synonyms_; }
    map<const xdaq::ApplicationDescriptor*, string> getAppStates() const { return appStates_; }
    map<const xdaq::ApplicationDescriptor*, unsigned long> getAppEventCounts() const { return appEventCounts_; }
    map<const xdaq::ApplicationDescriptor*, STEPInfo> getSTEPInfo() const { return stepInfo_; }
    uint64_t getLowestOfLowestSTEPCount() const;
    time_t getUnixTimeOfUpdate() const { return timeOfUpdate_; }
    set<const xdaq::ApplicationDescriptor*> getApps() const;
    set<const xdaq::ApplicationDescriptor*> getAppsInState( const string& state ) const;
    string getCanonicalStateName( const string& state ) const;
    time_t getAgeInSeconds() const;
    string getCombinedState() const;
    string getCombinedState( const string& className ) const;
    bool isEmpty() const; /// Empty if it contains no apps or any app it contains has no state assigned to it.
    void clear();
    xercesc::DOMDocumentFragment* toDOM( xercesc::DOMDocument* doc ) const;
    
    template <class Iterator>
    void insertApps( Iterator begin, Iterator end ){
      bSem_.take();
      for ( Iterator i=begin; i!=end; ++i ){
	appStates_     [*i] = "";
	appEventCounts_[*i] = 0;
      }
      bSem_.give();
    }
    
  private:
    string getCombinedStateNTS( const string& className ) const; /// Not thread safe. Only invoke it from within thread-safe methods.
    string getTimeOfUpdate() const; /// Not thread safe. Only invoke it from within thread-safe methods.
    time_t getAgeInSecondsNTS() const; /// Not thread safe. Only invoke it from within thread-safe methods.
    bool   areSynonymous( const string& stateName1, const string& stateName2 ) const;
    bool   hasSTEPInfo() const; /// Not thread safe. Only invoke it from within thread-safe methods.

    mutable toolbox::BSem bSem_; // exempt from constness
    time_t timeOfUpdate_;
    map< string, set<string> > synonyms_; // state name --> set of alternative state names
    map<const xdaq::ApplicationDescriptor*, string> appStates_;
    map<const xdaq::ApplicationDescriptor*, unsigned long> appEventCounts_;
    map<const xdaq::ApplicationDescriptor*, STEPInfo> stepInfo_;
  };

  ostream& operator<<( ostream& os, emu::ldaq::manager::AppStatuses& as );

}}} // namespace emu::ldaq::manager

#endif
