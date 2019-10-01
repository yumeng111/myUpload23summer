#ifndef __emu_pc_PipelineDepthScan_h__
#define __emu_pc_PipelineDepthScan_h__

#include "emu/pc/DAQMB.h"
#include "emu/pc/Crate.h"
#include "emu/pc/EmuEndcap.h"

#include "xdaq/Application.h"

#include <set>
#include <string>

using namespace std;

namespace emu { namespace pc {

    class PipelineDepthScan
    {
    public:
      
      PipelineDepthScan( xdaq::Application *parent, emu::pc::DAQMB* dmb );
      PipelineDepthScan( xdaq::Application *parent, emu::pc::Crate*     crate , set<unsigned int> hwVersions );
      PipelineDepthScan( xdaq::Application *parent, emu::pc::EmuEndcap* endcap, set<unsigned int> hwVersions );
      ~PipelineDepthScan();
      void run( int fromDepth, int toDepth, int incrementDepth, unsigned int durationInSec, bool isSingleLayerTrigger );
      
    private:
      xdaq::Application *parent_;
      set<unsigned int> hwVersions_;
      set<emu::pc::DAQMB*> DMBs_;
      set<emu::pc::Crate*> crates_;
      void setPipelineDepth( const int pipelineDepth );
      void setSingleLayerTrigger();
      void pauseL1A();
      void resumeL1A();
    };

  }
}
#endif
