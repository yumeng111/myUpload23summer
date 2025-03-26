#include "config/PackageInfo.h"
#include "emu/pc/AppsVersion.h"

//namespace emu {
//  namespace pc {

    namespace EmuPeripheralApps {
	const std::string project = "emu";
	const std::string package  =  "EmuPeripheralApps";
	const std::string versions =  appsversion;
	const std::string summary = "emu/emuDCS/PeripheralApps";
	const std::string description = "Emu Peripheral Crate Application Library";
	const std::string authors = "Jinghua Liu for cms-emu-onlinesoftware";
	const std::string link = "http://cms.cern.ch";
	config::PackageInfo getPackageInfo();
        void checkPackageDependencies();
    }


GETPACKAGEINFO(EmuPeripheralApps);

void EmuPeripheralApps::checkPackageDependencies() {}

//  } // namespace emu::pc
// } // namespace emu
