#ifndef __emu_pc_LocalDAQInterface_h__
#define __emu_pc_LocalDAQInterface_h__

#include "emu/soap/Messenger.h"
#include "xdata/Vector.h"
#include "emu/utils/String.h"

#include "xdaq/Application.h"

#include <set>
#include <string>

using namespace std;

namespace emu { namespace pc {

    class LocalDAQInterface
    {
    public:
      LocalDAQInterface( xdaq::Application *parent );
      ~LocalDAQInterface();
      void takeRun( string type, unsigned int durationInSec );
      bool waitForDAQToExecute( const string command, const uint64_t seconds );      
      void updateDataFileNames();
    private:
      xdaq::Application *parent_;
      emu::soap::Messenger *messenger_;
      set<string> dataFileNames_; ///< The names of all data files written since the last configure
    };

  }
}
#endif
