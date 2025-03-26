#ifndef __emu_ldaq_rui_EventStatistics_h__
#define __emu_ldaq_rui_EventStatistics_h__

#include "xdata/Float.h"
#include "xdata/Bag.h"
#include <string>
#include <iomanip>
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
      oss << std::scientific << std::setfill('0') << std::setprecision(3)
	  << "{ dataRate[B/s]: "   << float( dataRate        )
	  << ", eventRate[1/s]: "  << float( eventRate       )
	  << ", sampledFraction: " << float( sampledFraction )
	  << ", sizeMean[B]: "     << float( sizeMean        )
	  << ", sizeStD[B]: "      << float( sizeStD         )
	  << " }";
      return oss.str();
    }
    xdata::Float dataRate;        ///< rate of data volume read out [byte/s]
    xdata::Float eventRate;       ///< rate of events read out [1/s]
    xdata::Float sampledFraction; ///< fraction of events read out
    xdata::Float sizeMean;        ///< mean of event data size [byte]
    xdata::Float sizeStD;         ///< std dev of event data size [byte]
  };
}}} // namespace emu::ldaq::rui

#endif
