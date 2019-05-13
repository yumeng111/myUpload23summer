#ifndef __emu_pc_FEDInterface_h__
#define __emu_pc_FEDInterface_h__

#include "emu/soap/Messenger.h"
#include "xdata/Vector.h"
#include "emu/utils/String.h"

#include "xdaq/Application.h"

#include <set>
#include <string>

using namespace std;

namespace emu { namespace pc {

    class FEDInterface
    {
    public:
      FEDInterface( xdaq::Application *parent );
      ~FEDInterface();
      void startFED( bool inPassthroughMode );
      void haltFED();
    private:
      xdaq::Application *parent_;
      emu::soap::Messenger *messenger_;
      set<string> dataFileNames_; ///< The names of all data files written since the last configure
    };

  }
}
#endif
