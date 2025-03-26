##
#
# This is the emusupervisor Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

#
# Packages to be built
#
Project = emu
Package = emu/farmer
PackageName=farmer
Description="Emu (CSC) Farmer"
Summary="Emu (CSC) Farmer"
Authors="Karoly Banicz"
Link=""

Sources = \
	Application.cc \
	Cgi.cc \
	Xalan.cc \
	FileSystem.cc \
	SOAP.cc \
	ProcessDescriptor.cc \
	RunningExecutives.cc \
	version.cc

IncludeDirs = \
	$(XDAQ_ROOT)/include

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
DynamicLibrary = emufarmer
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
