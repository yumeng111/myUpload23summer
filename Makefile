##
#
# This is the TriDAS/emu Project Makefile
#
##

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

Project=emu

### this will include other packages, like DAQ and DQM
ifeq ($(Set),framework)
Packages=\
	emu/base \
        emu/supervisor \
        emu/soap \
	emu/emuDCS/PeripheralCore \
	emu/emuDCS/PeripheralApps \
	emu/emuDCS/PeripheralXtop \
	emu/emuDCS/OnlineDB \
	emu/emuDCS/FEDCore \
	emu/emuDCS/FEDUtils \
	emu/emuDCS/FEDApps \
        emu/emuDCS/CSCGEMTestStand
endif

### specific package target builds
ifeq ($(Set),emuDCS)
Packages= \
        emu/emuDCS/PeripheralCore \
        emu/emuDCS/PeripheralApps \
	emu/emuDCS/PeripheralXtop \
	emu/emuDCS/OnlineDB \
	emu/emuDCS/FEDCore \
	emu/emuDCS/FEDUtils \
	emu/emuDCS/FEDApps
endif

ifeq ($(Set),ldaq)
Packages= \
        emu/farmer \
	emu/ldaq
endif

ifeq ($(Set),dqm)
Packages= \
          emu/dqm
endif

ifeq ($(Set),supervisor) 
Packages= \
          emu/base \
          emu/supervisor
endif 

ifeq ($(Set),drivers)
Packages = \
	emu/drivers
endif

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
