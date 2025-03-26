##
#
# This is the emuldaqmanager Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

#
# Packages to be built
#
Project = emu
Package = emu/ldaq/manager

Sources = \
	AppStatuses.cc \
	Watchdog.cc \
	FactTypes.cc \
	Application.cc \
	version.cc

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
	$(BUILD_HOME)/emu/base/include \
	$(BUILD_HOME)/emu/soap/include \
	$(BUILD_HOME)/emu/supervisor/include \
	$(BUILD_HOME)/emu/ldaq/rui/include


LibraryDirs = $(XDAQ_ROOT)/lib

UserSourcePath =
UserCFlags =
UserCCFlags = -g -O0 -Wall -pedantic-errors -Wno-long-long -Wno-variadic-macros  -std=c++11
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
DynamicLibrary = emuldaqmanager
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
