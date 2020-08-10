##
#
# This is the emuldaqreader test application Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)

#
# Packages to be built
#
Project = emu
Package = emu/ldaq/reader

Standalone = no

Sources = \
	Base.cc \
	RawDataFile.cc \
	Spy.cc \
	Clock.cc 

ifeq ($(Standalone),no)
	Sources += version.cc
endif

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
	$(BUILD_HOME)/emu/ldaq/reader/include \
	$(BUILD_HOME)/emu/base/include \
        $(BUILD_HOME)/emu/drivers/e1000e_emu/eth_hook_2_daq

LibraryDirs = $(XDAQ_ROOT)/lib

UserSourcePath =
UserCFlags =
#UserCCFlags = -g -Wall -pedantic-errors -Wno-long-long
UserCCFlags = -DUSE_DDU2004 -g -Wall -Wno-long-long -std=c++11
UserDynamicLinkFlags =
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
ifeq ($(Standalone),no)
	DynamicLibrary = emuldaqreader
	StaticLibrary = 
else
	DynamicLibrary = 
	StaticLibrary = emuldaqreader_sa
endif

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
