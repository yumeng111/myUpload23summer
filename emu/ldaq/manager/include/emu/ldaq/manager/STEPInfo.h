#ifndef __emu_ldaq_manager_STEPInfo_h__
#define __emu_ldaq_manager_STEPInfo_h__

#include "xdata/UnsignedInteger64.h"
#include "xdata/Boolean.h"
#include "xdata/String.h"
#include "xdata/Vector.h"

namespace emu { namespace ldaq { namespace manager {
      struct STEPInfo{
	STEPInfo(){
	  persistentDDUError = "";
	  eventsRead         = 0;
	  totalCount         = 0;
	  lowestCount        = 0;
	}
	xdata::String                           persistentDDUError;
	xdata::UnsignedInteger64                eventsRead;
	xdata::UnsignedInteger64                totalCount;
	xdata::UnsignedInteger64                lowestCount;
	xdata::Vector<xdata::UnsignedInteger64> counts;
	xdata::Vector<xdata::UnsignedInteger64> readCounts;
	xdata::Vector<xdata::Boolean>           masks;
	xdata::Vector<xdata::Boolean>           liveInputs;
      };
}}} // namespace emu::ldaq::manager

#endif
