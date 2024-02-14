#include "GEMCode/GEMValidation/interface/Analyzers/CSCDigiAnalyzer.h"

CSCDigiAnalyzer::CSCDigiAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& wireDigi = conf.getParameterSet("cscWireDigi");
  verboseWG_ = wireDigi.getParameter<int>("verbose");
  minBXWire_ = wireDigi.getParameter<int>("minBX");
  maxBXWire_ = wireDigi.getParameter<int>("maxBX");
  minNHitsChamber_ = wireDigi.getParameter<int>("minNHitsChamber");

  const auto& comparatorDigi = conf.getParameterSet("cscComparatorDigi");
  verboseComparator_ = comparatorDigi.getParameter<int>("verbose");
  minBXComparator_ = comparatorDigi.getParameter<int>("minBX");
  maxBXComparator_ = comparatorDigi.getParameter<int>("maxBX");

  const auto& stripDigi = conf.getParameterSet("cscStripDigi");
  verboseStrip_ = stripDigi.getParameter<int>("verbose");
  minBXStrip_ = stripDigi.getParameter<int>("minBX");
  maxBXStrip_ = stripDigi.getParameter<int>("maxBX");

  comparatorDigiInput_ =
      iC.consumes<CSCComparatorDigiCollection>(comparatorDigi.getParameter<edm::InputTag>("inputTag"));
  stripDigiInput_ = iC.consumes<CSCStripDigiCollection>(stripDigi.getParameter<edm::InputTag>("inputTag"));
  wireDigiInput_ = iC.consumes<CSCWireDigiCollection>(wireDigi.getParameter<edm::InputTag>("inputTag"));
}

void CSCDigiAnalyzer::setMatcher(const CSCDigiMatcher& match_sh)
{
  match_.reset(new CSCDigiMatcher(match_sh));
}

void CSCDigiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree)
{
  iEvent.getByToken(comparatorDigiInput_, comparatorDigisH_);
  iEvent.getByToken(wireDigiInput_, wireDigisH_);

  // get the digi collections
  const CSCComparatorDigiCollection& comps = *comparatorDigisH_.product();
  const CSCWireDigiCollection& wires = *wireDigisH_.product();

  auto& cscTree = tree.cscDigi();
  //auto& simTree = tree.simTrack();
  //auto& genTree = tree.genParticle();

  int index;
  for (auto detUnitIt = comps.begin(); detUnitIt != comps.end(); ++detUnitIt) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);

    // Loop over the digis of this DetUnit
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; ++digiIt) {

      //int tpidfound = -1;
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        //   const auto& gemMatches = manager.matcher(tpid)->gemDigis()->digisInDetId(id.rawId());
        //   for (const auto& gemMatch : gemMatches) {
        //     // check if the same
        //     if (*digiIt == gemMatch) {
        //       tpidfound =  tpid;
        //     }
        //   }
      }

      for (auto p : digiIt->getTimeBinsOn()) {
        cscTree.csc_comp_time->push_back(p);
        cscTree.csc_comp_hs->push_back(digiIt->getHalfStrip());
        cscTree.csc_comp_isodd->push_back(isodd);
        cscTree.csc_comp_region->push_back(id.zendcap());
        cscTree.csc_comp_station->push_back(id.station());
        cscTree.csc_comp_chamber->push_back(id.chamber());
        cscTree.csc_comp_layer->push_back(id.layer());
        cscTree.csc_comp_ring->push_back(id.ring());

        // if (tpidfound != -1)
        //   ((*simTree.sim_id_gem_dg)[tpidfound]).push_back(index);

        index++;
      }
    }
  }

  index = 0;

  for (auto detUnitIt = wires.begin(); detUnitIt != wires.end(); ++detUnitIt) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);

    // Loop over the digis of this DetUnit
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; ++digiIt) {

      //int tpidfound = -1;
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        //   const auto& gemMatches = manager.matcher(tpid)->gemDigis()->digisInDetId(id.rawId());
        //   for (const auto& gemMatch : gemMatches) {
        //     // check if the same
        //     if (*digiIt == gemMatch) {
        //       tpidfound =  tpid;
        //     }
        //   }
      }

      for (auto p : digiIt->getTimeBinsOn()) {
        cscTree.csc_wire_time->push_back(p);
        cscTree.csc_wire_keywg->push_back(digiIt->getWireGroup());
        cscTree.csc_wire_isodd->push_back(isodd);
        cscTree.csc_wire_region->push_back(id.zendcap());
        cscTree.csc_wire_station->push_back(id.station());
        cscTree.csc_wire_chamber->push_back(id.chamber());
        cscTree.csc_wire_layer->push_back(id.layer());
        cscTree.csc_wire_ring->push_back(id.ring());

        // if (tpidfound != -1)
        //   ((*simTree.sim_id_gem_dg)[tpidfound]).push_back(index);

        index++;
      }
    }
  }
}

void CSCDigiAnalyzer::analyze(TreeManager& tree)
{
  // CSC strip digis
  for(const auto& d: match_->chamberIdsStrip(0)) {
    CSCDetId id(d);

    const int st(gem::detIdToMEStation(id.station(),id.ring()));

    const int nlayers(match_->nLayersWithStripInChamber(d));

    if (nlayers < minNHitsChamber_) continue;

    const bool odd(id.chamber()%2==1);

    int nstrips = 0; int ncomparators = 0;
    float compara_avgbx = 0.0;
    int totalstrips = 0; int totalcomparators = 0;
    for (unsigned int ilayer=1; ilayer<= 6; ilayer++){
      const CSCDetId& csc_id = CSCDetId(id.endcap(), id.station(), id.ring(), id.chamber(), ilayer);
      const auto&  hitstrips = match_->stripsInDetId(csc_id.rawId());
      const auto&  hitcomparators = match_->comparatorsInDetId(csc_id.rawId());
      nstrips += hitstrips.size();
      ncomparators += hitcomparators.size();
      totalcomparators += match_->totalComparators(csc_id.rawId());
      totalstrips += match_->totalStrips(csc_id.rawId());
    }
    for (const auto& comp : match_->comparatorDigisInChamber(id.rawId())){
      compara_avgbx += comp.getTimeBin();
    }
    compara_avgbx = 1.0*compara_avgbx/ncomparators;

    if (odd){
      tree.cscDigi().nstrips_dg_odd[st]      = nstrips;
      tree.cscDigi().ncomparators_dg_odd[st] = ncomparators;
      tree.cscDigi().ncompsbx_dg_odd[st] = compara_avgbx;
      tree.cscDigi().totalstrips_dg_odd[st]      = totalstrips;
      tree.cscDigi().totalcomparators_dg_odd[st] = totalcomparators;
    }else{
      tree.cscDigi().nstrips_dg_even[st] = nstrips;
      tree.cscDigi().ncomparators_dg_even[st] = ncomparators;
      tree.cscDigi().ncompsbx_dg_even[st] = compara_avgbx;
      tree.cscDigi().totalstrips_dg_even[st]      = totalstrips;
      tree.cscDigi().totalcomparators_dg_even[st] = totalcomparators;
    }
    if (odd) tree.cscDigi().has_csc_strips_odd[st] = true;
    else     tree.cscDigi().has_csc_strips_even[st] = true;

    if (odd) tree.cscDigi().nlayers_st_dg_odd[st] = nlayers;
    else     tree.cscDigi().nlayers_st_dg_even[st] = nlayers;

    // case ME11
    if (st==1 or st==2){
      if (odd) tree.cscDigi().has_csc_strips_odd[0] = true;
      else     tree.cscDigi().has_csc_strips_even[0] = true;

      if (odd) tree.cscDigi().nlayers_st_dg_odd[0] = nlayers;
      else     tree.cscDigi().nlayers_st_dg_even[0] = nlayers;

      if (odd){
        tree.cscDigi().nstrips_dg_odd[0] += nstrips;
        tree.cscDigi().ncomparators_dg_odd[0] += ncomparators;
        tree.cscDigi().totalstrips_dg_odd[0] += totalstrips;
        tree.cscDigi().ncompsbx_dg_odd[0] = compara_avgbx;
        tree.cscDigi().totalcomparators_dg_odd[0] += totalcomparators;
      }else{
        tree.cscDigi().nstrips_dg_even[0] += nstrips;
        tree.cscDigi().ncomparators_dg_even[0] += ncomparators;
        tree.cscDigi().totalstrips_dg_even[0] += totalstrips;
        tree.cscDigi().ncompsbx_dg_even[0] = compara_avgbx;
        tree.cscDigi().totalcomparators_dg_even[0] += totalcomparators;
      }
    }
  }

  // CSC wire digis
  for(const auto& d: match_->chamberIdsWire(0)) {
    CSCDetId id(d);
    const int st(gem::detIdToMEStation(id.station(),id.ring()));

    const int nlayers(match_->nLayersWithWireInChamber(d));
    if (nlayers < minNHitsChamber_) continue;

    const bool odd(id.chamber()%2==1);

    int nwires = 0;
    float wires_avgbx = 0.0;
    int totalwires = 0;
    for (unsigned int ilayer=1; ilayer<= 6; ilayer++){
      const CSCDetId& csc_id = CSCDetId(id.endcap(), id.station(), id.ring(), id.chamber(), ilayer);
      const auto&  hitwires  = match_->wiregroupsInDetId(csc_id.rawId());
      nwires  += hitwires.size();
      totalwires += match_->totalWires(csc_id.rawId());
    }
    for (const auto& w : match_->wireDigisInChamber(id)){
        wires_avgbx += w.getTimeBin();
    }
    wires_avgbx = 1.0*wires_avgbx/nwires;

    if (odd){
        tree.cscDigi().has_csc_wires_odd[st] = true;
        tree.cscDigi().nwires_dg_odd[st] = nwires;
        tree.cscDigi().nwiresbx_dg_odd[st] = wires_avgbx;
        tree.cscDigi().totalwires_dg_odd[st] = totalwires;
    }else{
        tree.cscDigi().has_csc_wires_even[st] = true;
        tree.cscDigi().nwires_dg_even[st] = nwires;
        tree.cscDigi().nwiresbx_dg_even[st] = wires_avgbx;
        tree.cscDigi().totalwires_dg_even[st] = totalwires;
    }

    if (odd) tree.cscDigi().nlayers_wg_dg_odd[st] = nlayers;
    else tree.cscDigi().nlayers_wg_dg_even[st] = nlayers;

    // case ME11
    if (st==1 or st==2){
      if (odd){
          tree.cscDigi().has_csc_wires_odd[0] = true;
          tree.cscDigi().nlayers_wg_dg_odd[0] = nlayers;
          tree.cscDigi().nwires_dg_odd[0] += nwires;
          tree.cscDigi().nwiresbx_dg_odd[0] = wires_avgbx;
          tree.cscDigi().totalwires_dg_odd[0] += totalwires;
      }else {
          tree.cscDigi().has_csc_wires_even[0] = true;
          tree.cscDigi().nlayers_wg_dg_even[0] = nlayers;
          tree.cscDigi().nwires_dg_even[0] += nwires;
          tree.cscDigi().nwiresbx_dg_even[0] = wires_avgbx;
          tree.cscDigi().totalwires_dg_even[0] += totalwires;
      }
    }
  }
}
