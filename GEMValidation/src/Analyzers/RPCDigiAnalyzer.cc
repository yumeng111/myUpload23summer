#include "GEMCode/GEMValidation/interface/Analyzers/RPCDigiAnalyzer.h"

RPCDigiAnalyzer::RPCDigiAnalyzer(const edm::ParameterSet& conf)
{
}

void RPCDigiAnalyzer::setMatcher(const RPCDigiMatcher& match_sh)
{
  match_.reset(new RPCDigiMatcher(match_sh));
}

void RPCDigiAnalyzer::analyze(TreeManager& tree)
{
  for (const auto& d: match_->detIds()) {
    RPCDetId id(d);
    //const int st(detIdToMEStation(id.station(), id.ring()));
    const int st = id.station();

    //meanstrip in rpc
    //const auto& rpcdigis = match_->digisInDetId(id);
    const int rpc_medianstrip =0;//(match_->median(rpcdigis));

    // convert sector and subsector to chamber
    const int chamber = CSCTriggerNumbering::chamberFromTriggerLabels(id.sector(), 0, id.station(), id.subsector());

    const bool odd(chamber%2 == 1);
    if (odd) {
      tree.rpcDigi().has_rpc_dg_odd[st] = true;;
      tree.rpcDigi().strip_rpcdg_odd[st] = rpc_medianstrip;
    }
    else {
      tree.rpcDigi().has_rpc_dg_even[st] = true;
      tree.rpcDigi().strip_rpcdg_even[st] = rpc_medianstrip;
    }
  }
}
