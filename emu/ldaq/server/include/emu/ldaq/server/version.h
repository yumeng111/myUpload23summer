#ifndef __emu_server_version_h__
#define __emu_server_version_h__

#include "config/PackageInfo.h"

namespace emudaqserver {
	const std::string package     = "emudaqserver";
	const std::string versions    = "14.00.00";
	const std::string description = "Emu (CSC) DAQ servers";

	const std::string summary     = "emu/ldaq/server";
	const std::string authors     = "Karoly Banicz";
	const std::string link        = "";

	config::PackageInfo getPackageInfo();
	void checkPackageDependencies()
			throw (config::PackageInfo::VersionException);
	std::set<std::string, std::less<std::string> > getPackageDependencies();
}

#endif
