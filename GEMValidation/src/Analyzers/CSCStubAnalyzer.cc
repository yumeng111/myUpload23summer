#include "GEMCode/GEMValidation/interface/Analyzers/CSCStubAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "L1Trigger/CSCTriggerPrimitives/interface/CSCPatternBank.h"
#include "DataFormats/Math/interface/deltaPhi.h"

CSCStubAnalyzer::CSCStubAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  //get detlaWG and deltaStrip for finding out the LCT match type
  const auto& wireDigi = conf.getParameter<edm::ParameterSet>("cscWireDigi");
  matchDeltaWG_ = wireDigi.getParameter<int>("matchDeltaWG");

  const auto& stripDigi = conf.getParameter<edm::ParameterSet>("cscStripDigi");
  matchDeltaStrip_ = stripDigi.getParameter<int>("matchDeltaStrip");

  const auto& cscPreCLCT = conf.getParameter<edm::ParameterSet>("cscPreCLCT");
  minBXPreCLCT_ = cscPreCLCT.getParameter<int>("minBX");
  maxBXPreCLCT_ = cscPreCLCT.getParameter<int>("maxBX");
  verbosePreCLCT_ = cscPreCLCT.getParameter<int>("verbose");

  const auto& cscCLCT = conf.getParameter<edm::ParameterSet>("cscCLCT");
  minBXCLCT_ = cscCLCT.getParameter<int>("minBX");
  maxBXCLCT_ = cscCLCT.getParameter<int>("maxBX");
  verboseCLCT_ = cscCLCT.getParameter<int>("verbose");

  const auto& cscALCT = conf.getParameter<edm::ParameterSet>("cscALCT");
  minBXALCT_ = cscALCT.getParameter<int>("minBX");
  maxBXALCT_ = cscALCT.getParameter<int>("maxBX");
  verboseALCT_ = cscALCT.getParameter<int>("verbose");

  const auto& cscLCT = conf.getParameter<edm::ParameterSet>("cscLCT");
  minBXLCT_ = cscLCT.getParameter<int>("minBX");
  maxBXLCT_ = cscLCT.getParameter<int>("maxBX");
  verboseLCT_ = cscLCT.getParameter<int>("verbose");

  const auto& cscMPLCT = conf.getParameter<edm::ParameterSet>("cscMPLCT");
  minBXMPLCT_ = cscMPLCT.getParameter<int>("minBX");
  maxBXMPLCT_ = cscMPLCT.getParameter<int>("maxBX");
  verboseMPLCT_ = cscMPLCT.getParameter<int>("verbose");

  const auto& cscShower = conf.getParameter<edm::ParameterSet>("cscShower");
  minBXShower_ = cscShower.getParameter<int>("minBX");
  maxBXShower_ = cscShower.getParameter<int>("maxBX");
  verboseShower_ = cscShower.getParameter<int>("verbose");

  preclctToken_ = iC.consumes<CSCCLCTPreTriggerDigiCollection>(cscPreCLCT.getParameter<edm::InputTag>("inputTag"));
  clctToken_ = iC.consumes<CSCCLCTDigiCollection>(cscCLCT.getParameter<edm::InputTag>("inputTag"));
  alctToken_ = iC.consumes<CSCALCTDigiCollection>(cscALCT.getParameter<edm::InputTag>("inputTag"));
  lctToken_ = iC.consumes<CSCCorrelatedLCTDigiCollection>(cscLCT.getParameter<edm::InputTag>("inputTag"));
  mplctToken_ = iC.consumes<CSCCorrelatedLCTDigiCollection>(cscMPLCT.getParameter<edm::InputTag>("inputTag"));
  showerToken_ = iC.consumes<CSCShowerDigiCollection>(cscShower.getParameter<edm::InputTag>("inputTag"));

  cscToken_ = iC.esConsumes<CSCGeometry, MuonGeometryRecord>();
  gemToken_ = iC.esConsumes<GEMGeometry, MuonGeometryRecord>();
}

void CSCStubAnalyzer::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //iSetup.get<MuonGeometryRecord>().get(csc_geom_);
  //if (csc_geom_.isValid()) {
  //cscGeometry_ = &*csc_geom_;
  //} else {
  //  std::cout << "+++ Info: CSC geometry is unavailable. +++\n";
  //}
  cscGeometry_ = &iSetup.getData(cscToken_);

  //iSetup.get<MuonGeometryRecord>().get(gem_geom_);
  //if (gem_geom_.isValid()) {
  //gemGeometry_ = &*gem_geom_;
  //} else {
  //  std::cout << "+++ Info: GEM geometry is unavailable. +++\n";
  //}
  gemGeometry_ = &iSetup.getData(gemToken_);
  iEvent.getByToken(showerToken_, showersH_);
}

void CSCStubAnalyzer::setMatcher(const CSCStubMatcher& match_sh)
{
  match_.reset(new CSCStubMatcher(match_sh));
}

void CSCStubAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup, const MatcherSuperManager& manager, my::TreeManager& tree)
{

  cscGeometry_ = &iSetup.getData(cscToken_);
  gemGeometry_ = &iSetup.getData(gemToken_);
  iEvent.getByToken(preclctToken_, preclctsH_);
  iEvent.getByToken(clctToken_, clctsH_);
  iEvent.getByToken(alctToken_, alctsH_);
  iEvent.getByToken(lctToken_, lctsH_);
  iEvent.getByToken(mplctToken_, mplctsH_);
  iEvent.getByToken(showerToken_, showersH_);

  const CSCCLCTPreTriggerDigiCollection& preclcts = *preclctsH_.product();
  const CSCCLCTDigiCollection& clcts = *clctsH_.product();
  const CSCALCTDigiCollection& alcts = *alctsH_.product();
  const CSCCorrelatedLCTDigiCollection& lcts = *lctsH_.product();
  const CSCCorrelatedLCTDigiCollection& mplcts = *mplctsH_.product();
  const CSCShowerDigiCollection& showers = *showersH_.product();

  auto& cscTree = tree.cscStub();
  auto& simTree = tree.simTrack();
  auto& genTree = tree.genParticle();
  const bool validTracks(simTree.sim_pt->size()>0);

  bool oneLLP = false;

  if (genTree.gen_tpid->size() > 0)
    oneLLP = oneLLP or genTree.gen_llp_in_acceptance->at(0)==1;
  if (genTree.gen_tpid->size() > 1)
    oneLLP = oneLLP or genTree.gen_llp_in_acceptance->at(1)==1;

  int index = 0;
  // CSC ALCTs
  for (auto detUnitIt = alcts.begin(); detUnitIt != alcts.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      int tpidfound = -1;
      // check if it was matched to a simtrack
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        const auto& lctMatch = matcher->cscStubs()->bestAlctInChamber(id.rawId());
        // check if the same
        if (*digiIt == lctMatch) {
          tpidfound = tpid;
          break;
        }
      }

      cscTree.csc_alct_bx->push_back(digiIt->getBX());
      cscTree.csc_alct_wg->push_back(digiIt->getKeyWG());
      cscTree.csc_alct_quality->push_back(digiIt->getQuality());
      cscTree.csc_alct_isodd->push_back(isodd);
      cscTree.csc_alct_region->push_back(id.zendcap());
      cscTree.csc_alct_station->push_back(id.station());
      cscTree.csc_alct_ring->push_back(id.ring());
      cscTree.csc_alct_chamber->push_back(id.chamber());
      cscTree.csc_alct_tpid->push_back(tpidfound);
      cscTree.csc_alct_sector->push_back(id.triggerSector());

      if (tpidfound != -1 and validTracks) {
        ((*simTree.sim_id_csc_alct)[tpidfound]).push_back(index);
      }

      index++;
    }
  }

  index = 0;
  // CSC PreCLCTs
  for (auto detUnitIt = preclcts.begin(); detUnitIt != preclcts.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      cscTree.csc_preclct_hs->push_back(digiIt->getKeyStrip());
      cscTree.csc_preclct_pattern->push_back(digiIt->getPattern());

      cscTree.csc_preclct_bx->push_back(digiIt->getBX());
      cscTree.csc_preclct_quality->push_back(digiIt->getQuality());
      cscTree.csc_preclct_isodd->push_back(isodd);
      cscTree.csc_preclct_region->push_back(id.zendcap());
      cscTree.csc_preclct_station->push_back(id.station());
      cscTree.csc_preclct_ring->push_back(id.ring());
      cscTree.csc_preclct_chamber->push_back(id.chamber());
      cscTree.csc_preclct_sector->push_back(id.triggerSector());

      index++;
    }
  }

  // CSC CLCTs
  for (auto detUnitIt = clcts.begin(); detUnitIt != clcts.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      int tpidfound = -1;
      // check if it was matched to a simtrack
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        const auto& lctMatch = matcher->cscStubs()->bestClctInChamber(id.rawId());
        // check if the same
        if (*digiIt == lctMatch) {
          tpidfound = tpid;
          break;
        }
      }

      cscTree.csc_clct_hs->push_back(digiIt->getKeyStrip(2));
      cscTree.csc_clct_qs->push_back(digiIt->getKeyStrip(4));
      cscTree.csc_clct_es->push_back(digiIt->getKeyStrip(8));
      cscTree.csc_clct_pattern->push_back(digiIt->getPattern());
      cscTree.csc_clct_pattern_run3->push_back(digiIt->getRun3Pattern());

      cscTree.csc_clct_bx->push_back(digiIt->getBX());
      cscTree.csc_clct_quality->push_back(digiIt->getQuality());
      cscTree.csc_clct_isodd->push_back(isodd);
      cscTree.csc_clct_region->push_back(id.zendcap());
      cscTree.csc_clct_station->push_back(id.station());
      cscTree.csc_clct_ring->push_back(id.ring());
      cscTree.csc_clct_chamber->push_back(id.chamber());
      cscTree.csc_clct_tpid->push_back(tpidfound);
      cscTree.csc_clct_sector->push_back(id.triggerSector());

      if (tpidfound != -1 and validTracks)
        ((*simTree.sim_id_csc_clct)[tpidfound]).push_back(index);

      index++;

    }
  }

  index = 0;
  // CSC LCTs
  for (auto detUnitIt = lcts.begin(); detUnitIt != lcts.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      int tpidfound = -1;
      // check if it was matched to a simtrack
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        const auto& lctMatch = matcher->cscStubs()->bestLctInChamber(id.rawId());
        // check if the same
        if (*digiIt == lctMatch) {
          tpidfound = tpid;
          break;
        }
      }

      cscTree.csc_lct_hs->push_back(digiIt->getStrip(2));
      cscTree.csc_lct_qs->push_back(digiIt->getStrip(4));
      cscTree.csc_lct_es->push_back(digiIt->getStrip(8));
      cscTree.csc_lct_pattern->push_back(digiIt->getPattern());
      cscTree.csc_lct_pattern_run3->push_back(digiIt->getRun3Pattern());

      cscTree.csc_lct_bx->push_back(digiIt->getBX());
      cscTree.csc_lct_quality->push_back(digiIt->getQuality());
      cscTree.csc_lct_isodd->push_back(isodd);
      cscTree.csc_lct_region->push_back(id.zendcap());
      cscTree.csc_lct_station->push_back(id.station());
      cscTree.csc_lct_ring->push_back(id.ring());
      cscTree.csc_lct_chamber->push_back(id.chamber());
      cscTree.csc_lct_tpid->push_back(tpidfound);
      cscTree.csc_lct_sector->push_back(id.triggerSector());

      if (tpidfound != -1 and validTracks)
        ((*simTree.sim_id_csc_lct)[tpidfound]).push_back(index);

      index++;
    }
  }

  index = 0;
  // CSC MPLCTs
  for (auto detUnitIt = mplcts.begin(); detUnitIt != mplcts.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      // check that the BX for this stub wasn't too early or too late
      if (digiIt->getBX() < minBXMPLCT_ || digiIt->getBX() > maxBXMPLCT_)
        continue;

      int tpidfound = -1;
      // check if it was matched to a simtrack
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        // stop processing when the first invalid matcher is found
        if (matcher->isInValid()) break;

        const auto& lctMatch = matcher->cscStubs()->bestLctInChamber(id.rawId());
        // check if the same
        if (*digiIt == lctMatch) {
          tpidfound = tpid;
          break;
        }
      }

      cscTree.csc_mplct_hs->push_back(digiIt->getStrip(2));
      cscTree.csc_mplct_qs->push_back(digiIt->getStrip(4));
      cscTree.csc_mplct_es->push_back(digiIt->getStrip(8));
      cscTree.csc_mplct_pattern->push_back(digiIt->getPattern());
      cscTree.csc_mplct_pattern_run3->push_back(digiIt->getRun3Pattern());
      cscTree.csc_mplct_bx->push_back(digiIt->getBX());
      cscTree.csc_mplct_quality->push_back(digiIt->getQuality());
      cscTree.csc_mplct_isodd->push_back(isodd);
      cscTree.csc_mplct_region->push_back(id.zendcap());
      cscTree.csc_mplct_station->push_back(id.station());
      cscTree.csc_mplct_ring->push_back(id.ring());
      cscTree.csc_mplct_chamber->push_back(id.chamber());
      cscTree.csc_mplct_tpid->push_back(tpidfound);
      cscTree.csc_mplct_sector->push_back(id.triggerSector());

      if (tpidfound != -1 and validTracks)
        ((*simTree.sim_id_csc_mplct)[tpidfound]).push_back(index);

      index++;
    }
  }

  // CSC Shower
  for (auto detUnitIt = showers.begin(); detUnitIt != showers.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    //const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      if (verboseShower_) {
        std::cout << ">>>Analyzing CSC Shower in " << id << std::endl;
        std::cout << "\tShower data: "
                  << " LooseOOT: " << digiIt->isLooseOutOfTime()
                  << " LooseIT: " << digiIt->isLooseInTime()
                  << " NominalOOT: " << digiIt->isNominalOutOfTime()
                  << " NominalIT: " << digiIt->isNominalInTime()
                  << " TightOOT: " << digiIt->isTightOutOfTime()
                  << " TightIT: " << digiIt->isTightInTime()
                  << std::endl;

        if (!oneLLP)
          std::cout << "Without LLP in acceptance" << std::endl;
      }
      cscTree.csc_shower_region->push_back(id.zendcap());
      cscTree.csc_shower_station->push_back(id.station());
      cscTree.csc_shower_ring->push_back(id.ring());
      cscTree.csc_shower_chamber->push_back(id.chamber());
      cscTree.csc_shower_sector->push_back(id.triggerSector());
      cscTree.csc_shower_bx->push_back(digiIt->getBX());
      cscTree.csc_shower_anodehits->push_back(digiIt->getWireNHits());
      cscTree.csc_shower_cathodehits->push_back(digiIt->getComparatorNHits());
      cscTree.csc_shower_isLooseInTime->push_back(digiIt->isLooseInTime());
      cscTree.csc_shower_isNominalInTime->push_back(digiIt->isNominalInTime());
      cscTree.csc_shower_isTightInTime->push_back(digiIt->isTightInTime());
      cscTree.csc_shower_isLooseOutOfTime->push_back(digiIt->isLooseOutOfTime());
      cscTree.csc_shower_isNominalOutOfTime->push_back(digiIt->isNominalOutOfTime());
      cscTree.csc_shower_isTightOutOfTime->push_back(digiIt->isTightOutOfTime());
    }
  }
}

void CSCStubAnalyzer::analyze(TreeManager& tree)
{
  // CSC CLCTs
  const CSCShowerDigiCollection& showers = *showersH_.product();
  for(const auto& d: match_->chamberIdsCLCT(0)) {
    CSCDetId id(d);

    const int st(gem::detIdToMEStation(id.station(),id.ring()));

    const bool odd(id.chamber()%2==1);
    const auto& clct = match_->bestClctInChamber(d);

    if (!clct.isValid()) continue;

    // in half-strips per layer
    const float slopeHalfStrip(clct.getFractionalSlope());
    const float slopeStrip(slopeHalfStrip / 2.);
    const int run3Slope = clct.isRun3() ? clct.getSlope() : convertRun2PIDToRun3Slope(clct.getPattern());

    int deltaStrip = 0;
    if (id.station() == 1 and id.ring() == 4 and clct.getKeyStrip() > CSCConstants::MAX_HALF_STRIP_ME1B)
      deltaStrip = CSCConstants::NUM_STRIPS_ME1B;

    float fpos = -9;

    auto fill = [clct, odd, slopeStrip, tree, deltaStrip, id, fpos, run3Slope](gem::CSCStubStruct& cscStubTree, int st) mutable {
      if (odd) {
        cscStubTree.has_clct_odd[st] = true;
        cscStubTree.quality_clct_odd[st] = clct.getQuality();
        cscStubTree.pattern_clct_odd[st] = clct.getPattern();
        cscStubTree.run3pattern_clct_odd[st] = clct.isRun3() ? clct.getRun3Pattern() : -9;
        //cscStubTree.run3slope_clct_odd[st]  =  clct.isRun3() ? clct.getSlope() : convertRun2PIDToRun3Slope(clct.getPattern());
        cscStubTree.run3slope_clct_odd[st]  =  run3Slope; 
        cscStubTree.bx_clct_odd[st] = clct.getBX();
        cscStubTree.bend_clct_odd[st] = clct.getBend();
        cscStubTree.endcap_clct_odd[st] = id.zendcap();
        cscStubTree.hs_clct_odd[st] = clct.getKeyStrip();
        cscStubTree.qs_clct_odd[st] = clct.getKeyStrip(4);
        cscStubTree.es_clct_odd[st] = clct.getKeyStrip(8);
        cscStubTree.slope_clct_odd[st] = slopeStrip;
        cscStubTree.ffhs_clct_odd[st] = fpos/2.;
        cscStubTree.fhs_clct_odd[st] = clct.getFractionalStrip(2);
        cscStubTree.fqs_clct_odd[st] = clct.getFractionalStrip(4);
        cscStubTree.fes_clct_odd[st] = clct.getFractionalStrip(8);
        // position deltas
        cscStubTree.delta_ffhs_clct_odd[st] = cscStubTree.ffhs_clct_odd[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_odd[st];
        cscStubTree.delta_fhs_clct_odd[st] = cscStubTree.fhs_clct_odd[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_odd[st];
        cscStubTree.delta_fqs_clct_odd[st] = cscStubTree.fqs_clct_odd[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_odd[st];
        cscStubTree.delta_fes_clct_odd[st] = cscStubTree.fes_clct_odd[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_odd[st];
        // bending deltas
        cscStubTree.dslope_clct_odd[st] = cscStubTree.slope_clct_odd[st] - tree.cscSimHit().bend_csc_sh_odd[st];
        // std::cout << "CSCStubAnalyzer " << id << " " << clct << " " << cscStubTree.slope_clct_odd[st] << " " << tree.cscSimHit().bend_csc_sh_odd[st] << " " <<  cscStubTree.dslope_clct_odd[st] << std::endl;
      }
      else {
        cscStubTree.has_clct_even[st] = true;
        cscStubTree.quality_clct_even[st] = clct.getQuality();
        cscStubTree.pattern_clct_even[st] = clct.getPattern();
        cscStubTree.run3pattern_clct_even[st] = clct.isRun3() ? clct.getRun3Pattern() : -9;
        //cscStubTree.run3slope_clct_even[st]  =  clct.isRun3() ? clct.getSlope() : convertRun2PIDToRun3Slope(clct.getPattern());
        cscStubTree.run3slope_clct_even[st]  =  run3Slope; 
        cscStubTree.bx_clct_even[st] = clct.getBX();
        cscStubTree.bend_clct_even[st] = clct.getBend();
        cscStubTree.endcap_clct_even[st] = id.zendcap();
        cscStubTree.hs_clct_even[st] = clct.getKeyStrip();
        cscStubTree.qs_clct_even[st] = clct.getKeyStrip(4);
        cscStubTree.es_clct_even[st] = clct.getKeyStrip(8);
        cscStubTree.slope_clct_even[st] = slopeStrip;
        cscStubTree.ffhs_clct_even[st] = fpos / 2.;
        cscStubTree.fhs_clct_even[st] = clct.getFractionalStrip(2);
        cscStubTree.fqs_clct_even[st] = clct.getFractionalStrip(4);
        cscStubTree.fes_clct_even[st] = clct.getFractionalStrip(8);
        // position deltas
        cscStubTree.delta_ffhs_clct_even[st] = cscStubTree.ffhs_clct_even[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_even[st];
        cscStubTree.delta_fhs_clct_even[st] = cscStubTree.fhs_clct_even[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_even[st];
        cscStubTree.delta_fqs_clct_even[st] = cscStubTree.fqs_clct_even[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_even[st];
        cscStubTree.delta_fes_clct_even[st] = cscStubTree.fes_clct_even[st] - deltaStrip - tree.cscSimHit().strip_csc_sh_even[st];
        // bending deltas
        cscStubTree.dslope_clct_even[st] = cscStubTree.slope_clct_even[st] - tree.cscSimHit().bend_csc_sh_even[st];
        // std::cout << "CSCStubAnalyzer " << id << " " << clct << " " << cscStubTree.slope_clct_even[st] << " " << tree.cscSimHit().bend_csc_sh_even[st] << " " <<  cscStubTree.dslope_clct_even[st] << std::endl;
      }
    };

    fill(tree.cscStub(), st);
    // case ME11
    if (st==1 or st==2) {
      fill(tree.cscStub(), 0);
    }
  }

  // CSC ALCTs
  for(const auto& d: match_->chamberIdsALCT(0)) {
    CSCDetId id(d);
    const int st(gem::detIdToMEStation(id.station(),id.ring()));

    const bool odd(id.chamber()%2==1);
    const auto& alct = match_->bestAlctInChamber(d);

    if (!alct.isValid()) continue;

    auto fill = [alct, odd](gem::CSCStubStruct& cscStubTree, int st) mutable {
      if (odd) {
        cscStubTree.has_alct_odd[st] = true;
        cscStubTree.wg_alct_odd[st] = alct.getKeyWG();
        cscStubTree.quality_alct_odd[st] = alct.getQuality();
        cscStubTree.bx_alct_odd[st] = alct.getBX();
      }
      else {
        cscStubTree.has_alct_even[st] = true;
        cscStubTree.wg_alct_even[st] = alct.getKeyWG();
        cscStubTree.quality_alct_even[st] = alct.getQuality();
        cscStubTree.bx_alct_even[st] = alct.getBX();
      }
    };

    fill(tree.cscStub(), st);
    // case ME11
    if (st==1 or st==2){
      fill(tree.cscStub(), 0);
    }
  }

  // CSC LCTs
  for(const auto& d: match_->chamberIdsLCT(0)) {
    CSCDetId id(d);
    const int st(gem::detIdToMEStation(id.station(),id.ring()));
    const int gemstation(id.station());

    const auto& lct = match_->bestLctInChamber(d);

    if (!lct.isValid()) continue;

    const GlobalPoint& gp = match_->getGlobalPosition(d, lct);

    const bool odd(id.chamber()%2==1);
    // when  matchTypeTight = false, then it would allow to do  LCT-simtrack match with ALCT=only or CLCT-only 
    int matchType = 0; //1:ALCTonly, 2:CLCTonly, 3:ALCT_CLCT or ALCT_GEMCopad or CLCT_GEMCopad
    const auto& alct = match_->bestAlctInChamber(d);
    const auto& clct = match_->bestClctInChamber(d);
    //GEM station: 0=ME0, 1=GE11, 2=GE21
    //cuts on dphi: 0.004, on deta: 0.2
    float gem_phi = odd ? tree.gemStub().phi_pad1_odd[1] : tree.gemStub().phi_pad1_even[1];
    float gem_eta = odd ? tree.gemStub().eta_pad1_odd[1] : tree.gemStub().eta_pad1_even[1];
    bool CLCTCopad = lct.getType() == 5 and abs(deltaPhi(gem_phi, gp.phi())<=0.004);
    bool ALCTCopad = lct.getType() == 4 and abs(gem_eta - gp.eta())<=0.1;
    //assume that ALCT+2GEM and CLCT+2GEM is good
    if (ALCTCopad or CLCTCopad or (alct.isValid() and clct.isValid() and (abs(lct.getStrip()-clct.getKeyStrip())<=matchDeltaStrip_*2) \
            and abs(lct.getKeyWG() - alct.getKeyWG())<=matchDeltaWG_))
      matchType = 3;
    else if (clct.isValid() and abs(lct.getStrip()-clct.getKeyStrip())<=matchDeltaStrip_*2)
      matchType = 2;
    else if (alct.isValid() and abs(lct.getKeyWG() - alct.getKeyWG()) <=matchDeltaWG_)
      matchType = 1;
    const int run3slope = lct.isRun3() ? lct.getSlope() : convertRun2PIDToRun3Slope(lct.getPattern());

    auto fill = [lct, gp, odd, id, matchType, run3slope](gem::CSCStubStruct& cscStubTree, int st) mutable {
      if (odd) {
        cscStubTree.has_lct_odd[st] = true;
        cscStubTree.bend_lct_odd[st] = lct.getPattern();
        cscStubTree.run3slope_lct_odd[st]  =  run3slope; 
        cscStubTree.phi_lct_odd[st] = gp.phi();
        cscStubTree.eta_lct_odd[st] = gp.eta();
        cscStubTree.perp_lct_odd[st] = gp.perp();
        cscStubTree.bx_lct_odd[st] = lct.getBX();
        cscStubTree.alctbx_lct_odd[st] = lct.getALCT().isValid() ? lct.getALCT().getBX() : 0;
        cscStubTree.clctbx_lct_odd[st] = lct.getCLCT().isValid() ? lct.getCLCT().getBX() : 0;
        cscStubTree.clctpattern_lct_odd[st] = lct.getCLCT().isValid() ? lct.getCLCT().getPattern() : -1;
        cscStubTree.clctquality_lct_odd[st] = lct.getCLCT().isValid() ? lct.getCLCT().getQuality() : -1;
        cscStubTree.bxdiff_lct_odd[st] = (lct.getCLCT().isValid() and lct.getALCT().isValid()) ? \
         (lct.getALCT().getBX()-lct.getCLCT().getBX() ) : -9;
        cscStubTree.matchtype_lct_odd[st] = matchType;
        cscStubTree.simtype_lct_odd[st] = lct.getType();
        cscStubTree.hs_lct_odd[st] = lct.getStrip();
        cscStubTree.qs_lct_odd[st] = lct.getStrip(4);
        cscStubTree.es_lct_odd[st] = lct.getStrip(8);
        cscStubTree.wg_lct_odd[st] = lct.getKeyWG();
        cscStubTree.chamber_lct_odd[st] = id.chamber();
        cscStubTree.quality_lct_odd[st] = lct.getQuality();
      }
      else {
        cscStubTree.has_lct_even[st] = true;
        cscStubTree.bend_lct_even[st] = lct.getPattern();
        cscStubTree.run3slope_lct_even[st]  =  run3slope; 
        cscStubTree.phi_lct_even[st] = gp.phi();
        cscStubTree.eta_lct_even[st] = gp.eta();
        cscStubTree.perp_lct_even[st] = gp.perp();
        cscStubTree.bx_lct_even[st] = lct.getBX();
        cscStubTree.alctbx_lct_even[st] = lct.getALCT().isValid() ? lct.getALCT().getBX() : 0;
        cscStubTree.clctbx_lct_even[st] = lct.getCLCT().isValid() ? lct.getCLCT().getBX() : 0;
        cscStubTree.clctpattern_lct_even[st] = lct.getCLCT().isValid() ? lct.getCLCT().getPattern() : -1;
        cscStubTree.clctquality_lct_even[st] = lct.getCLCT().isValid() ? lct.getCLCT().getQuality() : -1;
        cscStubTree.bxdiff_lct_even[st] = (lct.getCLCT().isValid() and lct.getALCT().isValid()) ? \
         (lct.getALCT().getBX()-lct.getCLCT().getBX() ) : -9;
        cscStubTree.matchtype_lct_even[st] = matchType;
        cscStubTree.simtype_lct_even[st] = lct.getType();
        cscStubTree.hs_lct_even[st] = lct.getStrip();
        cscStubTree.qs_lct_even[st] = lct.getStrip(4);
        cscStubTree.es_lct_even[st] = lct.getStrip(8);
        cscStubTree.wg_lct_even[st] = lct.getKeyWG();
        cscStubTree.chamber_lct_even[st] = id.chamber();
        cscStubTree.quality_lct_even[st] = lct.getQuality();
      }
    };

    fill(tree.cscStub(), st);
    // case ME11
    if (st==1 or st==2){
      fill(tree.cscStub(), 0);
    }

    // only for ME1/1 and ME2/1
    if (st==1 or st==2 or st==5) continue;

    // require at least one GEM pad present...
    if (! (tree.gemStub().has_gem_pad_even[gemstation] or
           tree.gemStub().has_gem_pad_odd[gemstation] ) ) continue;

    if (odd) {
      tree.gemStub().dphi_lct_pad1_odd[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_pad1_odd[st]),
                         float(tree.cscStub().phi_lct_odd[gemstation]));
      tree.gemStub().dphi_lct_pad2_odd[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_pad2_odd[st]),
                         float(tree.cscStub().phi_lct_odd[gemstation]));
    } else {
      tree.gemStub().dphi_lct_pad1_even[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_pad1_even[st]),
                         float(tree.cscStub().phi_lct_even[gemstation]));
      tree.gemStub().dphi_lct_pad2_even[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_pad2_even[st]),
                         float(tree.cscStub().phi_lct_even[gemstation]));
    }

    // require at least one GEM pad present...
    if (! (tree.gemStub().has_gem_copad_even[gemstation] or
           tree.gemStub().has_gem_copad_odd[gemstation] ) ) continue;

    if (odd) {
      tree.gemStub().dphi_lct_copad_odd[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_copad_odd[st]),
                         float(tree.cscStub().phi_lct_odd[gemstation]));
    } else {
      tree.gemStub().dphi_lct_copad_even[gemstation]
        = reco::deltaPhi(float(tree.gemStub().phi_copad_even[st]),
                         float(tree.cscStub().phi_lct_even[gemstation]));
    }
  }

  // CSC Shower
  for (auto detUnitIt = showers.begin(); detUnitIt != showers.end(); detUnitIt++) {
    const CSCDetId& id = (*detUnitIt).first;
    //const bool isodd = (id.chamber()%2 == 1);
    const auto& range = (*detUnitIt).second;
    for (auto digiIt = range.first; digiIt != range.second; digiIt++) {

      if (!(*digiIt).isValid())
        continue;

      if (verboseShower_) {
        std::cout << ">>>Analyzing CSC Shower in " << id << std::endl;
        std::cout << "\tShower data: "
                  << " LooseOOT: " << digiIt->isLooseOutOfTime()
                  << " LooseIT: " << digiIt->isLooseInTime()
                  << " NominalOOT: " << digiIt->isNominalOutOfTime()
                  << " NominalIT: " << digiIt->isNominalInTime()
                  << " TightOOT: " << digiIt->isTightOutOfTime()
                  << " TightIT: " << digiIt->isTightInTime()
                  << std::endl;
      }
    }
  }
}

std::pair<GEMDigi, GlobalPoint>
CSCStubAnalyzer::bestGEMDigi(const GEMDetId& gemId,
                             const GEMDigiContainer& gem_digis,
                             const GlobalPoint& csc_gp) const
{
  GlobalPoint gp;
  GEMDigi best_digi;
  auto emptyValue = make_pair(best_digi, gp);

  // no valid GEM digis
  if (gem_digis.empty()) return emptyValue;

  // invalid CSC stub
  if (std::abs(csc_gp.z()) < 0.001) return emptyValue;

  float bestDR2 = 999.;
  for (const auto& d: gem_digis) {
    const LocalPoint& lp = gemGeometry_->etaPartition(gemId)->centreOfStrip(d.strip());
    const GlobalPoint& gem_gp = gemGeometry_->idToDet(gemId)->surface().toGlobal(lp);

    // in deltaR calculation, give x20 larger weight to deltaPhi to make them comparable
    // but with slight bias towards dphi:
    float dphi = 20. * reco::deltaPhi(float(csc_gp.phi()), float(gem_gp.phi()));
    float deta = csc_gp.eta() - gem_gp.eta();
    float dR2 = dphi*dphi + deta*deta;
    // current gp is closer in phi then the previous
    if (dR2 < bestDR2) {
      gp = gem_gp;
      best_digi = d;
      bestDR2 = dR2;
    }
  }
  return emptyValue;
}


std::pair<GEMPadDigi, GlobalPoint>
CSCStubAnalyzer::bestGEMPadDigi(const GEMDetId& gemId,
                                const GEMPadDigiContainer& gem_digis,
                                const GlobalPoint& csc_gp) const
{
  GlobalPoint gp;
  GEMPadDigi best_digi;
  auto emptyValue = make_pair(best_digi, gp);

  // no valid GEM digis
  if (gem_digis.empty()) return emptyValue;

  // invalid CSC stub
  if (std::abs(csc_gp.z()) < 0.001) return emptyValue;

  float bestDR2 = 999.;
  for (const auto& d: gem_digis) {
    const LocalPoint& lp = gemGeometry_->etaPartition(gemId)->centreOfPad(d.pad());
    const GlobalPoint& gem_gp = gemGeometry_->idToDet(gemId)->surface().toGlobal(lp);

    // in deltaR calculation, give x20 larger weight to deltaPhi to make them comparable
    // but with slight bias towards dphi:
    float dphi = 20. * reco::deltaPhi(float(csc_gp.phi()), float(gem_gp.phi()));
    float deta = csc_gp.eta() - gem_gp.eta();
    float dR2 = dphi*dphi + deta*deta;
    // current gp is closer in phi then the previous
    if (dR2 < bestDR2) {
      gp = gem_gp;
      best_digi = d;
      bestDR2 = dR2;
    }
  }
  return emptyValue;
}


std::pair<GEMCoPadDigi, GlobalPoint>
CSCStubAnalyzer::bestGEMCoPadDigi(const GEMDetId& gemId,
                                  const GEMCoPadDigiContainer& gem_digis,
                                  const GlobalPoint& csc_gp) const
{
  GlobalPoint gp;
  GEMCoPadDigi best_digi;
  auto emptyValue = make_pair(best_digi, gp);

  // no valid GEM digis
  if (gem_digis.empty()) return emptyValue;

  // invalid CSC stub
  if (std::abs(csc_gp.z()) < 0.001) return emptyValue;

  float bestDR2 = 999.;
  for (const auto& d: gem_digis) {
    const LocalPoint& lp = gemGeometry_->etaPartition(gemId)->centreOfPad(d.pad(1));
    const GlobalPoint& gem_gp = gemGeometry_->idToDet(gemId)->surface().toGlobal(lp);

    // in deltaR calculation, give x20 larger weight to deltaPhi to make them comparable
    // but with slight bias towards dphi:
    float dphi = 20. * reco::deltaPhi(float(csc_gp.phi()), float(gem_gp.phi()));
    float deta = csc_gp.eta() - gem_gp.eta();
    float dR2 = dphi*dphi + deta*deta;
    // current gp is closer in phi then the previous
    if (dR2 < bestDR2) {
      gp = gem_gp;
      best_digi = d;
      bestDR2 = dR2;
    }
  }
  return emptyValue;
}
