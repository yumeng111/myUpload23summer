#ifndef __emu_pc_TCDSInterface_h__
#define __emu_pc_TCDSInterface_h__

#include "emu/supervisor/CIControl.h"
#include "emu/supervisor/PIControl.h"

#include "xdaq/Application.h"

using namespace std;

namespace emu { namespace pc {

    class TCDSInterface
    {
    public:
      TCDSInterface( xdaq::Application *parent );
      ~TCDSInterface();
      emu::pc::TCDSInterface& configureCCB();
    private:
      void createControllers();
      xdaq::Application *parent_;
      emu::supervisor::CIControl *ci_;
      emu::supervisor::PIControl *pi_;
    };

  }
}
#endif
