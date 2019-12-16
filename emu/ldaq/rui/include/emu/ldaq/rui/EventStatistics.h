#ifndef __emu_ldaq_rui_EventStatistics_h__
#define __emu_ldaq_rui_EventStatistics_h__

#include "xdata/Double.h"
#include "xdata/Bag.h"
#include <string>
#include <sstream>

namespace emu { namespace ldaq { namespace rui {
  class EventStatistics{
  public:
    void zero(){
      dataRate        = 0.;
      eventRate       = 0.;
      sampledFraction = 0.;
      sizeMean        = 0.;
      sizeStD         = 0.;
    }
    void registerFields(xdata::Bag<EventStatistics> *bag){
      zero();
      bag->addField( "dataRate"       , &dataRate        );
      bag->addField( "eventRate"      , &eventRate       );
      bag->addField( "sampledFraction", &sampledFraction );
      bag->addField( "sizeMean"       , &sizeMean        );
      bag->addField( "sizeStD"        , &sizeStD         );
    }
    std::string toString(){
      std::ostringstream oss;
      oss << "{ dataRate[B/s]: "   << dataRate       .toString()
	  << ", eventRate[1/s]: "  << eventRate      .toString()
	  << ", sampledFraction: " << sampledFraction.toString()
	  << ", sizeMean[B]: "     << sizeMean       .toString()
	  << ", sizeStD[B]: "      << sizeStD        .toString()
	  << " }";
      return oss.str();
    }
    xdata::Double dataRate;        ///< rate of data volume read out [byte/s]
    xdata::Double eventRate;       ///< rate of events read out [1/s]
    xdata::Double sampledFraction; ///< fraction of events read out
    xdata::Double sizeMean;        ///< mean of event data size [byte]
    xdata::Double sizeStD;         ///< std dev of event data size [byte]
  };
}}} // namespace emu::ldaq::rui

#endif
