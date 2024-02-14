#include "GEMCode/GEMValidation/interface/Analyzers/RPCRecHitAnalyzer.h"

RPCRecHitAnalyzer::RPCRecHitAnalyzer(const edm::ParameterSet& conf)
{
}

void RPCRecHitAnalyzer::setMatcher(const RPCRecHitMatcher& match_sh)
{
  match_.reset(new RPCRecHitMatcher(match_sh));
}

void RPCRecHitAnalyzer::analyze(TreeManager& tree)
{
  /*
  for (const auto& d: match_->detIds()) {

    RPCDetId id(d);
    const int st = id.station();//(detIdToMEStation(id.station(), id.ring()));

    // convert sector and subsector to chamber
    const int chamber = CSCTriggerNumbering::chamberFromTriggerLabels(id.sector(), 0, id.station(), id.subsector());

    const auto& rpcrechits = match_->recHitsInDetId(id);

    // pick the first rechit now
    const auto& candidate_rechit = rpcrechits.at(0);

    const bool odd(chamber%2 == 1);
    tree.rpcRecHit().rpc_BunchX[st] = candidate_rechit.BunchX();
    tree.rpcRecHit().rpc_firstClusterStrip[st] = candidate_rechit.firstClusterStrip();
    tree.rpcRecHit().rpc_clusterSize[st] = candidate_rechit.clusterSize();
    tree.rpcRecHit().rpc_time[st] = candidate_rechit.time();
    tree.rpcRecHit().rpc_timeError[st] = candidate_rechit.timeError();

    if (odd) {
     tree.rpcRecHit().has_rpc_rh_odd[st] = true;
    }
    else {
      tree.rpcRecHit().has_rpc_rh_even[st] = true;
    }
  }
  */
}
