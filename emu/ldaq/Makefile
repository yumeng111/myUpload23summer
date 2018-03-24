#BUILD_HOME:=$(shell pwd)/../..

include $(XDAQ_ROOT)/config/mfAutoconf.rules
include $(XDAQ_ROOT)/config/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

Project=emu
Package=ldaq
PackageName=ldaq
Description="The Emu (CSC) local DAQ"
Summary="The Emu (CSC) local DAQ"
Authors="Steven Murray, Karoly banicz"
Link=""

include $(XDAQ_ROOT)/config/Makefile.rules
include $(XDAQ_ROOT)/config/mfRPM.rules

Packages= \
          ldaq/reader \
          ldaq/writer \
          ldaq/server \
          ldaq/rui \
          ldaq/manager

include $(XDAQ_ROOT)/config/Makefile.rules

.PHONY: _rpmall
_rpmall: spec_update spec_local_update makerpm
	@echo "Building emuldaq rpm: done"

.PHONY: spec_local_update
spec_local_update:
	perl -p -i -e 's#__packages__#$(Packages)#' $(PackagePath)/rpm/$(PackageName).spec


# componentsrpm:
# 	for x in $(Packages); do \
# 	  make -C ../$$x rpm; \
# 	done
# 
# installcomponentsrpm:
# 	for x in $(Packages); do \
# 	  make -C ../$$x installrpm; \
# 	done
# 
# cleancomponentsrpm:
# 	for x in $(Packages); do \
# 	  make -C ../$$x cleanrpm; \
# 	done
