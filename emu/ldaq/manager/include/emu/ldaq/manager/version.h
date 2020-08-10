#ifndef __emu_ldaq_manager_version_h__
#define __emu_ldaq_manager_version_h__

#include "config/PackageInfo.h"

namespace emuldaqmanager {
	const std::string project     = "emu";
	const std::string package     = "emuldaqmanager";
	const std::string versions    = "14.07.00";
	const std::string description = "Emu (CSC) Local DAQ Manager";

	const std::string summary     = "emu/ldaq/manager";
	const std::string authors     = "Karoly Banicz";
	const std::string link        = "";

	config::PackageInfo getPackageInfo();
	void checkPackageDependencies()
			throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif

