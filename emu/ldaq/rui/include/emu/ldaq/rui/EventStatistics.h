#ifndef __emu_daq_rui_EventStatistics_h__
#define __emu_daq_rui_EventStatistics_h__

#include "xdata/Double.h"

namespace emu { namespace ldaq { namespace rui {
  struct EventStatistics{
    EventStatistics(){
      zero();
    }
    void zero(){
      dataRate        = 0.;
      eventRate       = 0.;
      sampledFraction = 0.;
      sizeMean        = 0.;
      sizeStD         = 0.;
    }
    xdata::Double dataRate;        ///< rate of data volume read out [byte/s]
    xdata::Double eventRate;       ///< rate of events read out [1/s]
    xdata::Double sampledFraction; ///< fraction of events read out
    xdata::Double sizeMean;        ///< mean of event data size [byte]
    xdata::Double sizeStD;         ///< std dev of event data size [byte]
  };
}}} // namespace emu::ldaq::rui

#endif
