#include "GEMCode/GEMValidation/interface/Analyzers/CSCSimHitAnalyzer.h"
#include "TF1.h"
#include "TGraph.h"
#include "TFitResult.h"

using namespace std;

CSCSimHitAnalyzer::CSCSimHitAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  simHitPSet_ = conf.getParameterSet("cscSimHit");
  verbose_ = simHitPSet_.getParameter<int>("verbose");
  minNHitsChamber_ = simHitPSet_.getParameter<int>("minNHitsChamber");
  simHitInput_ = iC.consumes<edm::PSimHitContainer>(simHitPSet_.getParameter<edm::InputTag>("inputTag"));
}

void CSCSimHitAnalyzer::setMatcher(const CSCSimHitMatcher& match_sh)
{
  match_.reset(new CSCSimHitMatcher(match_sh));
}

void CSCSimHitAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree)
{
  iEvent.getByToken(simHitInput_, simHitsH_);

  // get the digi collections
  const edm::PSimHitContainer& simhits = *simHitsH_.product();

  auto& cscTree = tree.cscSimHit();
  auto& simTree = tree.simTrack();

  int index = 0;
  // loop on all simhits
  for (const auto& simhit : simhits) {
    const CSCDetId& id(simhit.detUnitId());
    const bool isodd = (id.chamber()%2 == 1);

    if (verbose_) {
      std::cout << "Analyzing CSC simhit "
                << id << " " << simhit << " PDGID " << simhit.particleType()
                << std::endl;
    }

    // check if matched to a tracking particle!
    int tpidfound = -1;
    for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

      // get the matcher
      const auto& matcher = manager.matcher(tpid);

      // stop processing when the first invalid matcher is found
      if (matcher->isInValid()) break;

      // get the matched simhits for this simtrack (and child tracks)
      const auto& matchedSimHits = matcher->cscSimHits();

      for (const auto& matchedHit : matchedSimHits->simHits(0)) {
        // check if simhit is the same
        if (matchedHit.entryPoint() == simhit.entryPoint() ) {
          tpidfound = tpid;
        }
      }
    }
    // std::cout << "CSCSimHitAnalyzer::simhit " << p << " " << simhit.localPosition() << std::endl;
    // cscTree.csc_simhit_hs->push_back(digiIt->getHalfStrip());
    cscTree.csc_sh_isodd->push_back(isodd);
    cscTree.csc_sh_region->push_back(id.zendcap());
    cscTree.csc_sh_station->push_back(id.station());
    cscTree.csc_sh_ring->push_back(id.ring());
    cscTree.csc_sh_chamber->push_back(id.chamber());
    // cscTree.csc_sh_layer->push_back(id.layer());

    if (tpidfound != -1)
      ((*simTree.sim_id_csc_sh)[tpidfound]).push_back(index);

    index++;
  }
}

void CSCSimHitAnalyzer::analyze(TreeManager& tree)
{
  const auto& csc_simhits(match_->chamberIds(0));

  for(const auto& d: match_->chamberIds(0)) {

    CSCDetId id(d);

    const int st(gem::detIdToMEStation(id.station(),id.ring()));

    // calculate hit layers
    int nlayers(match_->nLayersWithHitsInChamber(d));

    // case ME11
    const bool isME11(id.station()==1 and (id.ring()==4 or id.ring()==1));

    if (isME11){
      // get the detId of the pairing subchamber
      int other_ring(id.ring()==4 ? 1 : 4);
      CSCDetId co_id(id.endcap(), id.station(), other_ring, id.chamber());
      // check if co_id occurs in the list
      // add the hit layers

      const auto& rawId(co_id.rawId());
      if (csc_simhits.find(rawId) != csc_simhits.end()) {
        nlayers = nlayers+match_->nLayersWithHitsInChamber(rawId);
      }
    }

    // layer requirement (typically 4)
    if (nlayers < minNHitsChamber_) continue;

    const bool odd(id.chamber()%2==1);
    const auto& simhits = match_->hitsInChamber(id);
    const auto& keygp(match_->simHitsMeanPosition(simhits));
    float stripIntercept, stripSlope;
    match_->fitHitsInChamber(d, stripIntercept, stripSlope);

    // add offset of +0.25 strips for non-ME1/1 chambers
    if (!isME11){
      stripIntercept -= 0.25;
    }

    int nstrips = 0; int nwires = 0;
    for (unsigned int ilayer=1; ilayer<= 6; ilayer++){
      const CSCDetId& csc_id = CSCDetId(id.endcap(), id.station(), id.ring(), id.chamber(), ilayer);
      const auto&  hitstrips = match_->hitStripsInDetId(csc_id.rawId());
      const auto&  hitwires  = match_->hitWiregroupsInDetId(csc_id.rawId());
      nstrips += hitstrips.size();
      nwires  += hitwires.size();
    }

    if (odd) {
      tree.cscSimHit().chamber_sh_odd[st] = id.chamber();
      tree.cscSimHit().nstrips_sh_odd[st]  = nstrips;
      tree.cscSimHit().nwires_sh_odd[st] = nwires;
      tree.cscSimHit().nlayers_csc_sh_odd[st] = nlayers;
      tree.cscSimHit().has_csc_sh_odd[st] = true;
      tree.cscSimHit().eta_csc_sh_odd[st] = keygp.eta();
      tree.cscSimHit().phi_csc_sh_odd[st] = keygp.phi();
      tree.cscSimHit().perp_csc_sh_odd[st] = keygp.perp();
      tree.cscSimHit().bend_csc_sh_odd[st] = stripSlope;
      tree.cscSimHit().strip_csc_sh_odd[st] = stripIntercept;
    }
    else {
      tree.cscSimHit().chamber_sh_even[st] = id.chamber();
      tree.cscSimHit().nstrips_sh_even[st]  = nstrips;
      tree.cscSimHit().nwires_sh_even[st] = nwires;
      tree.cscSimHit().nlayers_csc_sh_even[st] = nlayers;
      tree.cscSimHit().has_csc_sh_even[st] = true;
      tree.cscSimHit().eta_csc_sh_even[st] = keygp.eta();
      tree.cscSimHit().phi_csc_sh_even[st] = keygp.phi();
      tree.cscSimHit().perp_csc_sh_even[st] = keygp.perp();
      tree.cscSimHit().bend_csc_sh_even[st] = stripSlope;
      tree.cscSimHit().strip_csc_sh_even[st] = stripIntercept;
    }

    // case ME11
    if (st==1 or st==2){
      if (odd) {
        tree.cscSimHit().chamber_sh_odd[0] = id.chamber();
        tree.cscSimHit().nstrips_sh_odd[0]  += nstrips;
        tree.cscSimHit().nwires_sh_odd[0] += nwires;
        tree.cscSimHit().nlayers_csc_sh_odd[0] = nlayers;
        tree.cscSimHit().has_csc_sh_odd[0] = true;
        tree.cscSimHit().eta_csc_sh_odd[0] = keygp.eta();
        tree.cscSimHit().phi_csc_sh_odd[0] = keygp.phi();
        tree.cscSimHit().perp_csc_sh_odd[0] = keygp.perp();
        tree.cscSimHit().bend_csc_sh_odd[0] = stripSlope;
        tree.cscSimHit().strip_csc_sh_odd[0] = stripIntercept;
      }
      else {
        tree.cscSimHit().chamber_sh_even[0] = id.chamber();
        tree.cscSimHit().nstrips_sh_even[0]  += nstrips;
        tree.cscSimHit().nwires_sh_even[0] += nwires;
        tree.cscSimHit().nlayers_csc_sh_even[0] = nlayers;
        tree.cscSimHit().has_csc_sh_even[0] = true;
        tree.cscSimHit().eta_csc_sh_even[0] = keygp.eta();
        tree.cscSimHit().phi_csc_sh_even[0] = keygp.phi();
        tree.cscSimHit().perp_csc_sh_even[0] = keygp.perp();
        tree.cscSimHit().bend_csc_sh_even[0] = stripSlope;
        tree.cscSimHit().strip_csc_sh_even[0] = stripIntercept;
      }
    }
  }
}


float //std::pair<float,float>
CSCSimHitAnalyzer::fitBendingPositionInChamber(unsigned int detid) const
{

  const CSCDetId cscid(detid);

  if (match_->nLayersWithHitsInChamber(detid) < 6) return -100;

  float phi_layer1=-10;
  float phi_layer6=10;

  if (cscid.station()==1 and (cscid.ring()==1 or cscid.ring()==4)){
    const CSCDetId cscid1a(cscid.endcap(), cscid.station(), 4, cscid.chamber(), 1);
    const CSCDetId cscid1b(cscid.endcap(), cscid.station(), 1, cscid.chamber(), 1);

    const edm::PSimHitContainer& hits1a = match_->hitsInDetId(cscid1a.rawId());
    const edm::PSimHitContainer& hits1b = match_->hitsInDetId(cscid1b.rawId());

    const GlobalPoint& gp1a = match_->simHitsMeanPosition( match_->hitsInDetId(cscid1a.rawId()));
    const GlobalPoint& gp1b = match_->simHitsMeanPosition( match_->hitsInDetId(cscid1b.rawId()));
    if (hits1a.size()>0 and hits1b.size()>0)
      //phi_layer1 = (gp1a.phi()*hits1a.size()+gp1b.phi()*hits1b.size())/(hits1a.size()+hits1b.size());
      phi_layer1 = (gp1a.phi()+gp1b.phi())/2.0;
    else if (hits1a.size()>0) phi_layer1 = gp1a.phi();
    else if (hits1b.size()>0) phi_layer1 = gp1b.phi();
    else std::cerr <<" no hits in layer1, cant not find global phi of hits " << std::endl;

    const CSCDetId cscid6a(cscid.endcap(), cscid.station(), 4, cscid.chamber(), 6);
    const CSCDetId cscid6b(cscid.endcap(), cscid.station(), 1, cscid.chamber(), 6);

    const edm::PSimHitContainer& hits6a = match_->hitsInDetId(cscid6a.rawId());
    const edm::PSimHitContainer& hits6b = match_->hitsInDetId(cscid6b.rawId());

    const GlobalPoint& gp6a = match_->simHitsMeanPosition(match_->hitsInDetId(cscid6a.rawId()));
    const GlobalPoint& gp6b = match_->simHitsMeanPosition(match_->hitsInDetId(cscid6b.rawId()));
    if (hits6a.size()>0 and hits6b.size()>0)
      //phi_layer6 = (gp6a.phi()*hits6a.size()+gp6b.phi()*hits6b.size())/(hits6a.size()+hits6b.size());
      phi_layer6 = (gp6a.phi()+gp6b.phi())/2.0;
    else if (hits6a.size()>0) phi_layer6 = gp6a.phi();
    else if (hits6b.size()>0) phi_layer6 = gp6b.phi();
    else std::cerr <<" no hits in layer6, cant not find global phi of hits " << std::endl;


  }
  else {
    const CSCDetId cscid1(cscid.endcap(), cscid.station(), cscid.ring(), cscid.chamber(), 1);
    const CSCDetId cscid6(cscid.endcap(), cscid.station(), cscid.ring(), cscid.chamber(), 6);

    const edm::PSimHitContainer& hits1 =  match_->hitsInDetId(cscid1.rawId());
    const edm::PSimHitContainer& hits6 =  match_->hitsInDetId(cscid6.rawId());

    if (hits1.size()==0) std::cerr <<" no hits in layer1, cant not find global phi of hits " << std::endl;
    const GlobalPoint& gp1 =  match_->simHitsMeanPosition( match_->hitsInDetId(cscid1.rawId()));
    phi_layer1 = gp1.phi();

    if (hits6.size()==0) std::cerr <<" no hits in layer6, cant not find global phi of hits " << std::endl;
    const GlobalPoint& gp6 =  match_->simHitsMeanPosition( match_->hitsInDetId(cscid6.rawId()));
    phi_layer6 = gp6.phi();
  }
  //std::cout <<" phi1 "<< phi_layer1 <<" phi6 " << phi_layer6 << std::endl;
  return deltaPhi(phi_layer6,phi_layer1);
}
