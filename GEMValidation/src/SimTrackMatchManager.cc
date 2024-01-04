#include "GEMCode/GEMValidation/interface/SimTrackMatchManager.h"

SimTrackMatchManager::SimTrackMatchManager(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es)
  // need additional protection
  : genMuons_(t, v, ps, ev, es)
  , simhits_(t, v, ps, ev, es)
  , gem_digis_(simhits_)
  , gem_rechits_(simhits_)
  , me0_digis_(simhits_)
  // , me0_rechits_(simhits_)
  , rpc_digis_(simhits_)
  , rpc_rechits_(simhits_)
  , csc_digis_(simhits_)
  , csc_stubs_(simhits_, csc_digis_, gem_digis_, rpc_digis_)
  , csc_rechits_(simhits_)
  , dt_digis_(simhits_)
  , dt_stubs_(simhits_)
  , dt_rechits_(simhits_)
  //, l1_tracks_(csc_stubs_, dt_digis_, rpc_digis_)
  , tracks_(simhits_, csc_digis_, gem_digis_, rpc_digis_, csc_stubs_)
  , l1_tf_tracks_(simhits_)
  , l1_tf_cands_(simhits_)
  , l1_gmt_cands_(simhits_)
  , hlt_tracks_(csc_rechits_, dt_rechits_, rpc_rechits_, gem_rechits_)
{
  //std::cout <<" simTrackMatcherManager constructor " << std::endl;
}

SimTrackMatchManager::~SimTrackMatchManager() {
 // std::cout <<" simTrackMatcherManager destructor " << std::endl;

}
