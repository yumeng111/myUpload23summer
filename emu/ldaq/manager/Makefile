##
#
# This is the emuldaqmanager Makefile
#
##
include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)

#
# Packages to be built
#
Project = emu
Package = ldaq/manager

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
UserCCFlags = -g -O0 -Wall -pedantic-errors -Wno-long-long -Wno-variadic-macros
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

include $(XDAQ_ROOT)/config/Makefile.rules
