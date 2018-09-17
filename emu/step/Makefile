##
#
# This is the emustep Makefile
#
##

BUILD_HOME:=$(shell pwd)/../..

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
PACKAGE_VER_MAJOR=05
PACKAGE_VER_MINOR=01
PACKAGE_VER_PATCH=01

#
# Packages to be built
#
Project = emu
Package = step
PackageName= step
Description="Emu (CSC) STEP (System Test of Endcap Peripheral crate and chamber electronics)"
Summary="Emu (CSC) STEP"
Authors="Karoly Banicz, Alex Madorsky"
Link=""

Sources = \
	Application.cc \
	Manager.cc \
	Tester.cc \
	Configuration.cc \
	TestParameters.cc	\
	Test.cc	\
	ChamberMap.cc	\
	Dumper.cc	\
	PeriodicDumper.cc	\
	TimingOptions.cc \
	version.cc
TestSources =
TestIncludeDirs =

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(BUILD_HOME)/emu/soap/include \
	$(BUILD_HOME)/emu/base/include \
	$(BUILD_HOME)/emu/emuDCS/PeripheralCore/include \
	$(BUILD_HOME)/emu/emuDCS/PeripheralApps/include \
	$(BUILD_HOME)/emu/step/include \
	/opt/dip/include
LibraryDirs = /opt/dip/lib64

UserSourcePath =
UserCFlags = $(ROOTCFLAGS)
# At GIF++ we may want to use the SPS timing DIP signals. To do that:
#   Get these DIP packages and install them (see https://readthedocs.web.cern.ch/display/ICKB/DIP+API):
#     https://readthedocs.web.cern.ch/download/attachments/21178175/dip-5.6.3.3-1.x86_64.rpm
#     https://readthedocs.web.cern.ch/download/attachments/21178175/dip-devel-5.6.3.3-1.x86_64.rpm
#   Add to XDAQ configuration the module /opt/dip/lib64/libdip.so
#   Add -DDIP to UserCCFlags if the DIP library is available and needed.
UserCCFlags = -O0 -g -Wall -Wno-long-long $(ROOTCFLAGS) # -DDIP
UserDynamicLinkFlags = $(ROOTLIBS)
UserStaticLinkFlags =
UserExecutableLinkFlags =

# These libraries can be platform specific and
# potentially need conditional processing
#
Libraries =
ExternalObjects =

#
# Compile the source files and create a shared library
#
DynamicLibrary = emustep
StaticLibrary =

# To build tests:
#     export BUILD_HOME=... 
#     make tests
# To run it:
#     LD_LIBRARY_PATH=$BUILD_HOME/emu/lib/$XDAQ_OS/$XDAQ_PLATFORM:$XDAQ_ROOT/lib $BUILD_HOME/emu/step/$XDAQ_OS/$XDAQ_PLATFORM/*.exe
TestLibraryDirs=
TestLibraries=xcept toolbox log4cplus xdata xerces-c xalan-c xdaq xalanMsg xoap xgi cgicc pthttp peer asyncresolv mimetic config logudpappender logxmlappender uuid emubase emusoap tstoreclient EmuOnlineDB EmuPeripheralCore EmuPeripheralApps
TestExecutables=configTest.cc

include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules
