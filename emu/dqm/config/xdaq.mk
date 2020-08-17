include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfAutoconf.rules
include $(XDAQ_ROOT)/$(BUILD_SUPPORT)/mfDefs.$(XDAQ_OS)

XdaqIncludeDirs = \
	$(XDAQ_ROOT)/include \

XdaqLibraryDirs = \
	$(XDAQ_ROOT)/lib \

XdaqLibraries = toolbox asyncresolv xdaq xoap xerces-c log4cplus xdata xgi xcept cgicc mimetic logxmlappender logudpappender peer i2outils uuid config
