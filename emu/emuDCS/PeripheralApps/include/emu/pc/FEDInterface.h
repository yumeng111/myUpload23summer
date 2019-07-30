#ifndef __emu_pc_FEDInterface_h__
#define __emu_pc_FEDInterface_h__

#include "emu/soap/Messenger.h"

#include "xdaq/Application.h"

#include <set>
#include <string>

using namespace std;

namespace emu { namespace pc {

    class FEDInterface
    {
    public:
      FEDInterface( xdaq::Application *parent, set<string> chamberLabels );
      ~FEDInterface();
      void startFED( bool inPassthroughMode );
      void haltFED();
    private:
      xdaq::Application *parent_;
      set<string> chamberLabels_;
      emu::soap::Messenger *messenger_;
    };

  }
}
#endif
