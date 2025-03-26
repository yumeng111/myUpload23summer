##
#
# This is the emusoap Makefile
#
##
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

#
# Packages to be built
#
Project=emu
Package=emu/soap
PackageName=soap
Description="SOAP messenger for Emu (CSC) applications."
Summary="SOAP messenger for Emu (CSC) applications."
Authors="Karoly Banicz"
Link=""

Sources = \
	QualifiedName.cc \
	Attachment.cc \
	Attributes.cc \
	Parameters.cc \
        Messenger.cc \
        ToolBox.cc \
	version.cc

IncludeDirs = \
	$(XDAQ_ROOT)/include \
	$(XDAQ_ROOT)/include/$(XDAQ_OS) \
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
DynamicLibrary = emusoap
StaticLibrary =

TestLibraries=
TestExecutables=

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
