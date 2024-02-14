#include "GEMCode/GEMValidation/interface/Analyzers/RPCSimHitAnalyzer.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"

RPCSimHitAnalyzer::RPCSimHitAnalyzer(const edm::ParameterSet& conf)
{
}

void RPCSimHitAnalyzer::setMatcher(const RPCSimHitMatcher& match_sh)
{
  match_.reset(new RPCSimHitMatcher(match_sh));
}

void RPCSimHitAnalyzer::analyze(TreeManager& tree)
{
  for (const auto& d: match_->chamberIds()) {
    RPCDetId id(d);

    const int st = id.station();//(detIdToMEStation(id.station(), id.ring()));

    int cscchamber = CSCTriggerNumbering::chamberFromTriggerLabels(id.sector(), 0, id.station(), id.subsector());

    if ( (match_->hitsInChamber(d)).size() >0 ) {
      bool odd(cscchamber%2 == 1);

      if (odd)   tree.rpcSimHit().has_rpc_sh_odd[st] = true;
      else       tree.rpcSimHit().has_rpc_sh_even[st] = true;
    }
  }
}
