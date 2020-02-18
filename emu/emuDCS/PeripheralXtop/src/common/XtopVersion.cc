#include "config/PackageInfo.h"
#include "emu/x2p/XtopVersion.h"

namespace EmuPeripheralXtop {
	const std::string package  =  "EmuPeripheralXtop";
	const std::string versions =  xtopversion;
	const std::string summary = "emu/emuDCS/PeripheralXtop";
	const std::string description = "Emu Peripheral Crate Xmas to PVSS Library";
	const std::string authors = "Jinghua Liu for cms-emu-onlinesoftware";
	const std::string link = "http://cms.cern.ch";
	config::PackageInfo getPackageInfo();
        void checkPackageDependencies();
}

GETPACKAGEINFO(EmuPeripheralXtop);

void EmuPeripheralXtop::checkPackageDependencies() {}
