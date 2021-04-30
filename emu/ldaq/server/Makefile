# $Id: Makefile,v 1.3 2011/12/13 17:28:53 banicz Exp $

##
#
# This is the emuldaqserver Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

#
# Packages to be built
#
Project = emu
Package = emu/ldaq/server

Sources = \
	Base.cc \
	I2O.cc \
	SOAP.cc \
	version.cc


IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
	$(BUILD_HOME)/emu/ldaq/reader/include \
	$(BUILD_HOME)/emu/ldaq/server/include \
	$(BUILD_HOME)/emu/soap/include


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
DynamicLibrary = emuldaqserver
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
