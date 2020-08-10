##
#
# This is the emuldaqrui Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)

#
# Packages to be built
#
Project = emu
Package = emu/ldaq/rui

Sources = \
	STEPEventCounter.cc \
	BadEventCount.cc \
	LogMessageCounter.cc \
	Application.cc \
	FactTypes.cc \
	EventBuffer.cc \
	EventBufferRing.cc \
	FEROLDataStream.cc \
	CDF_FED_Header.cc \
	CDF_FED_Trailer.cc \
	version.cc

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
	$(BUILD_HOME)/emu/soap/include \
	$(BUILD_HOME)/emu/base/include \
	$(BUILD_HOME)/emu/ldaq/server/include \
	$(BUILD_HOME)/emu/ldaq/writer/include \
	$(BUILD_HOME)/emu/ldaq/reader/include


LibraryDirs = $(XDAQ_ROOT)/lib

UserSourcePath =
UserCFlags =
UserCCFlags = -g -Wall -pedantic-errors -Wno-long-long -Wno-variadic-macros  -std=c++11
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
DynamicLibrary = emuldaqrui
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
