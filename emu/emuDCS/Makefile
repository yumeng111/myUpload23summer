##
#
# This is the TriDAS/emu/emuDCS Project Makefile
#  (this Makefile will soon become obsolete:
#   only project and package Makefiles should remain)
#
##

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)
include $(BUILD_HOME)/emu/rpm_version

Project=emu

ifeq ($(Set),framework)
Packages=\
	emu/emuDCS/PeripheralCore \
	emu/emuDCS/PeripheralApps \
        emu/emuDCS/PeripheralXtop \
        emu/emuDCS/PCSwitches \
	emu/emuDCS/OnlineDB \
	emu/emuDCS/FEDCore \
	emu/emuDCS/FEDUtils \
	emu/emuDCS/FEDApps
endif

ifeq ($(Set),PeripheralApps)
Packages= \
	emu/emuDCS/PeripheralCore \
	emu/emuDCS/PeripheralApps \
        emu/emuDCS/PeripheralXtop \
	emu/emuDCS/OnlineDB
endif

ifeq ($(Set),FEDCrate)
Packages= \
        emu/emuDCS/FEDCore \
	emu/emuDCS/FEDUtils \
	emu/emuDCS/FEDApps \
	emu/emuDCS/OnlineDB 
endif

include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/Makefile.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfRPM.rules
