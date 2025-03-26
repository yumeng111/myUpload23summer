#ifndef __emu_ldaq_manager_Watchdog_h__
#define __emu_ldaq_manager_Watchdog_h__

#include "emu/ldaq/manager/Application.h"
#include "emu/ldaq/manager/AppStatuses.h"
#include <set>
#include <vector>
#include <string>


namespace emu { namespace ldaq { namespace manager {

      class Application;

      class Watchdog {
	friend ostream& operator<<( ostream& os,  emu::ldaq::manager::Watchdog& wd );
      public:
	Watchdog( emu::ldaq::manager::Application *owner );
	void patrol();
	std::set<const xdaq::ApplicationDescriptor*> getAppsInStates( const std::vector<std::string>& states ) const;
      private:
	void insertAllApps( emu::ldaq::manager::AppStatuses& appStatuses );
	emu::ldaq::manager::Application *application_; ///< Pointer to the owner XDAQ application.
	emu::ldaq::manager::AppStatuses currentAppStatuses_;
	emu::ldaq::manager::AppStatuses previousAppStatuses_;
      };

      ostream& operator<<( ostream& os,  emu::ldaq::manager::Watchdog& wd );

}}} // namespace emu::ldaq::manager

#endif
