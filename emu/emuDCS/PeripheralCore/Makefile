##
#
# This is the TriDAS/emu/emuDCS/PeripheralCore Package Makefile
#
##

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

#
# Packages to be built
#
Project=emu
Package=emu/emuDCS/PeripheralCore
PackageName=PeripheralCore
Description="Core libraries for Peripheral Crates"

Sources=\
	CoreVersion.cc \
	ALCTController.cc \
	CCB.cc \
	RAT.cc \
	CFEB.cc \
	Crate.cc \
	DAQMB.cc \
	MPC.cc \
	TMBTester.cc \
	TMB.cc \
	VMECC.cc \
	VMEController.cc \
	VMEController_jtag.cc \
	VMEModule.cc \
	Chamber.cc \
	EMUjtag.cc \
	EmuEndcap.cc \
        EmuLogger.cc \
        PROBLEM.cc \
        DDU.cc \
	Unpacker.cc

IncludeDirs = 

LibraryDirs = 

TestLibraryDirs =

UserCFlags  = 
UserCCFlags = -std=c++11
UserDynamicLinkFlags =
UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing
#
Libraries =
TestLibraries =

#
# Compile the source files and create a shared library
#
DynamicLibrary= EmuPeripheralCore
StaticLibrary= 

Executables=
TestExecutables= 

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
