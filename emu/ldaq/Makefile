include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

Project=emu
Package=emu/ldaq
PackageName=ldaq

Description="The Emu (CSC) local DAQ"
Summary="The Emu (CSC) local DAQ"
Authors="Steven Murray, Karoly banicz"
Link="https://gitlab.cern.ch/CMS-Emu/CERN/TriDAS"

Packages= \
           emu/ldaq/reader \
           emu/ldaq/writer \
           emu/ldaq/server \
           emu/ldaq/rui \
           emu/ldaq/manager

# Order is important. If Makefile.rules came before mfRPM.rules,
# it would try to build in the subdirectories listed in Packages.
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules

# Override rpm-related targets
rpm:	_rpmall

.PHONY: _rpmall
_rpmall: spec_update spec_local_update makerpm
	@echo "Building emu-ldaq rpm: done" 

PackagesForRPM= \
           ldaq/reader \
           ldaq/writer \
           ldaq/server \
           ldaq/rui \
           ldaq/manager

.PHONY: spec_local_update
spec_local_update:
	perl -p -i -e 's#__packages__#$(PackagesForRPM)#' $(PackagePath)/rpm/$(PackageName).spec
