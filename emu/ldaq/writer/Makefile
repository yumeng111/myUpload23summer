##
#
# This is the emuldaqwriter Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)

#
# Packages to be built
#
Project = emu
Package = emu/ldaq/writer

Sources = \
	RawDataFile.cc \
	RateLimiter.cc \
	TransferJSON.cc \
	version.cc

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
	$(BUILD_HOME)/emu/base/include \
	$(BUILD_HOME)/emu/ldaq/writer/include


LibraryDirs = $(XDAQ_ROOT)/lib

UserSourcePath =
UserCFlags =
UserCCFlags = -g -Wall -pedantic-errors -Wno-long-long -Wno-variadic-macros -std=c++11
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
DynamicLibrary = emuldaqwriter
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
