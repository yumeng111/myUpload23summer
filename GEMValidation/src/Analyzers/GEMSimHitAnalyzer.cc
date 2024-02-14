#include "GEMCode/GEMValidation/interface/Analyzers/GEMSimHitAnalyzer.h"

GEMSimHitAnalyzer::GEMSimHitAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  simHitPSet_ = conf.getParameterSet("gemSimHit");
  verbose_ = simHitPSet_.getParameter<int>("verbose");
  simHitInput_ = iC.consumes<edm::PSimHitContainer>(simHitPSet_.getParameter<edm::InputTag>("inputTag"));
}

void GEMSimHitAnalyzer::setMatcher(const GEMSimHitMatcher& match_sh)
{
  match_.reset(new GEMSimHitMatcher(match_sh));
}

void GEMSimHitAnalyzer::analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree)
{
}

void GEMSimHitAnalyzer::analyze(TreeManager& tree)
{
  for(const auto& d: match_->superChamberIds()) {
    GEMDetId id(d);
    const int st = id.station();

    const bool odd(id.chamber()%2==1);

    const auto& gem_simhits = match_->hitsInSuperChamber(d);
    const auto& gem_simhits_gp = match_->simHitsMeanPosition(gem_simhits);
    //const auto& gem_simhits_gv = match_->simHitsMeanMomentum(gem_simhits);

    if (gem_simhits.size() > 0) {
      const float mean_strip(match_->simHitsMeanStrip(match_->hitsInChamber(d)));
      if (odd) {
        tree.gemSimHit().has_gem_sh_odd[st] = true;
        tree.gemSimHit().strip_gem_sh_odd[st] = mean_strip;
        tree.gemSimHit().phi_gem_sh_odd[st] = float(gem_simhits_gp.phi());
      }
      else {
        tree.gemSimHit().has_gem_sh_even[st] = true;
        tree.gemSimHit().strip_gem_sh_even[st] = mean_strip;
        tree.gemSimHit().phi_gem_sh_even[st] = float(gem_simhits_gp.phi());
      }
    }

    if (match_->nLayersWithHitsInSuperChamber(d) >= 2) {
      if (odd) tree.gemSimHit().has_gem_sh2_odd[st] = true;
      else     tree.gemSimHit().has_gem_sh2_even[st] = true;
    }

    const auto& copad_superids(match_->superChamberIdsCoincidences());
    if (copad_superids.find(d) != copad_superids.end()) {
      if (odd) tree.gemSimHit().has_gemcopad_sh_odd[st] = true;
      else     tree.gemSimHit().has_gemcopad_sh_even[st] = true;
    }
  };
}
