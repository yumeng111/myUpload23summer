#include "GEMCode/GEMValidation/interface/Analyzers/L1MuAnalyzer.h"

L1MuAnalyzer::L1MuAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& emtfTrack = conf.getParameter<edm::ParameterSet>("emtfTrack");
  minBXEMTFTrack_ = emtfTrack.getParameter<int>("minBX");
  maxBXEMTFTrack_ = emtfTrack.getParameter<int>("maxBX");
  verboseEMTFTrack_ = emtfTrack.getParameter<int>("verbose");
  runEMTFTrack_ = emtfTrack.getParameter<bool>("run");

  const auto& emtfCand = conf.getParameter<edm::ParameterSet>("emtfCand");
  minBXEMTFCand_ = emtfCand.getParameter<int>("minBX");
  maxBXEMTFCand_ = emtfCand.getParameter<int>("maxBX");
  verboseEMTFCand_ = emtfCand.getParameter<int>("verbose");
  runEMTFCand_ = emtfCand.getParameter<bool>("run");

  const auto& omtfCand = conf.getParameter<edm::ParameterSet>("omtfCand");
  minBXOMTFCand_ = omtfCand.getParameter<int>("minBX");
  maxBXOMTFCand_ = omtfCand.getParameter<int>("maxBX");
  verboseOMTFCand_ = omtfCand.getParameter<int>("verbose");
  runOMTFCand_ = omtfCand.getParameter<bool>("run");

  const auto& bmtfCand = conf.getParameter<edm::ParameterSet>("bmtfCand");
  minBXBMTFCand_ = bmtfCand.getParameter<int>("minBX");
  maxBXBMTFCand_ = bmtfCand.getParameter<int>("maxBX");
  verboseBMTFCand_ = bmtfCand.getParameter<int>("verbose");
  runBMTFCand_ = bmtfCand.getParameter<bool>("run");

  const auto& muon = conf.getParameter<edm::ParameterSet>("muon");
  minBXGMT_ = muon.getParameter<int>("minBX");
  maxBXGMT_ = muon.getParameter<int>("maxBX");
  verboseGMT_ = muon.getParameter<int>("verbose");
  runGMT_ = muon.getParameter<bool>("run");

  const auto& emtfShower = conf.getParameter<edm::ParameterSet>("emtfShower");
  minBXEMTFShower_ = emtfShower.getParameter<int>("minBX");
  maxBXEMTFShower_ = emtfShower.getParameter<int>("maxBX");
  verboseEMTFShower_ = emtfShower.getParameter<int>("verbose");
  runEMTFShower_ = emtfShower.getParameter<bool>("run");

  const auto& muonShower = conf.getParameter<edm::ParameterSet>("muonShower");
  minBXGMT_ = muonShower.getParameter<int>("minBX");
  maxBXGMT_ = muonShower.getParameter<int>("maxBX");
  verboseShower_ = muonShower.getParameter<int>("verbose");
  runShower_ = muonShower.getParameter<bool>("run");

// Shower Pset from cmssw/L1Trigger/CSCTriggerPrimitives/python/params/showerParams.py 
  const auto& anodeShower = conf.getParameter<edm::ParameterSet>("anodeShower");
  anodeThresholds_ = anodeShower.getParameter<std::vector<unsigned>>("showerThresholds");
  anodeMinLayers_ = anodeShower.getParameter<unsigned>("minLayersCentralTBin");
  const auto& cathodeShower = conf.getParameter<edm::ParameterSet>("cathodeShower");
  cathodeThresholds_ = cathodeShower.getParameter<std::vector<unsigned>>("showerThresholds");
  cathodeMinLayers_ = cathodeShower.getParameter<unsigned>("minLayersCentralTBin");



  if (runEMTFTrack_)
    emtfTrackToken_ = iC.consumes<l1t::EMTFTrackCollection>(emtfTrack.getParameter<edm::InputTag>("inputTag"));

  if (runEMTFCand_)
    emtfCandToken_ = iC.consumes<l1t::RegionalMuonCandBxCollection>(emtfCand.getParameter<edm::InputTag>("inputTag"));

  if (runGMT_)
    muonToken_ = iC.consumes<l1t::MuonBxCollection>(muon.getParameter<edm::InputTag>("inputTag"));

  if (runEMTFShower_)
    emtfShowerToken_ = iC.consumes<l1t::RegionalMuonShowerBxCollection>(emtfShower.getParameter<edm::InputTag>("inputTag"));

  if (runShower_)
    showerToken_ = iC.consumes<l1t::MuonShowerBxCollection>(muonShower.getParameter<edm::InputTag>("inputTag"));
}

void L1MuAnalyzer::setMatcher(const L1MuMatcher& match_sh)
{
  match_.reset(new L1MuMatcher(match_sh));
}

void L1MuAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup,
                           const MatcherSuperManager& manager, my::TreeManager& tree) {

  auto& trkTree = tree.l1mu();
  auto& simTree = tree.simTrack();
  const bool validTracks(simTree.sim_pt->size()>0);

  if (runEMTFTrack_) {
    iEvent.getByToken(emtfTrackToken_, emtfTrackHandle_);
    const l1t::EMTFTrackCollection& emtfTracks = *emtfTrackHandle_.product();
    if (verboseEMTFTrack_)
      std::cout << "Analyzing " << emtfTracks.size() << " EMTF tracks" << std::endl;

    int index = 0;
    for (const auto& trk : emtfTracks) {

      int bx = trk.BX();
      if ( bx < minBXEMTFTrack_ or bx > maxBXEMTFTrack_) continue;

      const gem::EMTFTrack& gemtrk(trk);

      if (verboseEMTFTrack_)
        std::cout << gemtrk << std::endl;

      int tpidfound = -1;
      // check if it was matched to a simtrack
      for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

        // get the matcher
        const auto& matcher = manager.matcher(tpid);

        index++;

        const auto& trackMatch = matcher->l1Muons()->emtfTrack();
        if (trackMatch) {

          if (verboseEMTFTrack_)
            std::cout << "Candidate match " << *trackMatch
                      << std::endl;

          //check if the same
          if (gemtrk == *trackMatch) {
            tpidfound = tpid;
            if (verboseEMTFTrack_)
              std::cout << "...matched! With index " << tpidfound << std::endl;
            break;
          }
        }
      }
      trkTree.emtftrack_pt->push_back(gemtrk.pt());
      trkTree.emtftrack_eta->push_back(gemtrk.eta());
      trkTree.emtftrack_phi->push_back(gemtrk.phi());
      trkTree.emtftrack_charge->push_back(gemtrk.charge());
      trkTree.emtftrack_bx->push_back(gemtrk.bx());
      trkTree.emtftrack_tpid->push_back(tpidfound);

      if (tpidfound != -1 and validTracks) {
        ((*simTree.sim_id_emtf_track)[tpidfound]).push_back(index);
      }
    }

  }

  if (runEMTFCand_) {
    iEvent.getByToken(emtfCandToken_, emtfCandHandle_);
    const l1t::RegionalMuonCandBxCollection& emtfCands = *emtfCandHandle_.product();

    if (verboseEMTFCand_)
      std::cout << "Analyzing " << int(emtfCands.end(0) - emtfCands.begin(0)) << " EMTF cands" << std::endl;

    int index = 0;
    for (int bx = emtfCands.getFirstBX(); bx <= emtfCands.getLastBX(); bx++ ){
      if ( bx < minBXEMTFCand_ or bx > maxBXEMTFCand_) continue;
      for (auto emtfCand = emtfCands.begin(bx); emtfCand != emtfCands.end(bx); ++emtfCand ){

        const gem::EMTFCand& gemtrk(*emtfCand);

        if (verboseEMTFCand_)
          std::cout << gemtrk << std::endl;

        int tpidfound = -1;
        // check if it was matched to a simtrack
        for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

          // get the matcher
          const auto& matcher = manager.matcher(tpid);

          const auto& trackMatch = matcher->l1Muons()->emtfCand();
          if (trackMatch) {

            if (verboseEMTFCand_)
              std::cout << "Candidate match " << *trackMatch
                        << std::endl;

            //check if the same
            if (gemtrk == *trackMatch) {
              tpidfound = tpid;
              if (verboseEMTFCand_)
                std::cout << "...matched! With index " << tpidfound << std::endl;
              break;
            }
          }
        }
        trkTree.emtfcand_pt->push_back(gemtrk.pt());
        trkTree.emtfcand_eta->push_back(gemtrk.eta());
        trkTree.emtfcand_phi->push_back(gemtrk.phi());
        trkTree.emtfcand_charge->push_back(gemtrk.charge());
        trkTree.emtfcand_bx->push_back(gemtrk.bx());
        trkTree.emtfcand_tpid->push_back(tpidfound);

        if (tpidfound != -1 and validTracks) {
          ((*simTree.sim_id_emtf_cand)[tpidfound]).push_back(index);
        }
      }
    }
  }

  if (runGMT_) {
    iEvent.getByToken(muonToken_, muonHandle_);
    const l1t::MuonBxCollection& gmtCands = *muonHandle_.product();

    if (verboseGMT_)
      std::cout << "Analyzing " << int(gmtCands.end(0) - gmtCands.begin(0)) << " GMT cands" << std::endl;

    int index = 0;
    for (int bx = gmtCands.getFirstBX(); bx <= gmtCands.getLastBX(); bx++ ){
      if ( bx < minBXGMT_ or bx > maxBXGMT_) continue;
      for (auto emtfCand = gmtCands.begin(bx); emtfCand != gmtCands.end(bx); ++emtfCand ){

        const gem::EMTFCand& gemtrk(*emtfCand);

        if (verboseGMT_)
          std::cout << gemtrk << std::endl;

        int tpidfound = -1;
        // check if it was matched to a simtrack
        for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

          // get the matcher
          const auto& matcher = manager.matcher(tpid);

          const auto& trackMatch = matcher->l1Muons()->muon();
          if (trackMatch) {

            if (verboseGMT_)
              std::cout << "Candidate match " << *trackMatch
                        << std::endl;

            //check if the same
            if (gemtrk == *trackMatch) {
              tpidfound = tpid;
              if (verboseGMT_)
                std::cout << "...matched! With index " << tpidfound << std::endl;
              break;
            }
          }
        }
        trkTree.l1mu_pt->push_back(gemtrk.pt());
        trkTree.l1mu_eta->push_back(gemtrk.eta());
        trkTree.l1mu_phi->push_back(gemtrk.phi());
        trkTree.l1mu_charge->push_back(gemtrk.charge());
        trkTree.l1mu_bx->push_back(gemtrk.bx());
        trkTree.l1mu_tpid->push_back(tpidfound);

        if (tpidfound != -1 and validTracks) {
          ((*simTree.sim_id_l1mu)[tpidfound]).push_back(index);
        }
      }
    }
  }

  if (runEMTFShower_) {
    iEvent.getByToken(emtfShowerToken_, emtfShowerHandle_);
    //DataFormats/​L1TMuon/​interface/​RegionalMuonShower.h
    const l1t::RegionalMuonShowerBxCollection& emtfShowers = *emtfShowerHandle_.product();

    if (verboseEMTFShower_)
      std::cout << "Analyzing " << int(emtfShowers.end(0) - emtfShowers.begin(0)) << " EMTF showers" << std::endl;

    for (int bx = emtfShowers.getFirstBX(); bx <= emtfShowers.getLastBX(); bx++ ){
      if ( bx < minBXEMTFShower_ or bx > maxBXEMTFShower_) continue;
      for (auto emtfShower = emtfShowers.begin(bx); emtfShower != emtfShowers.end(bx); ++emtfShower ){

        if (not emtfShower->isValid()) continue;
        // enum tftype { bmtf, omtf_neg, omtf_pos, emtf_neg, emtf_pos };
        if (not (emtfShower->trackFinderType() == l1t::tftype::emtf_neg or emtfShower->trackFinderType() == l1t::tftype::emtf_pos))
          continue;

        trkTree.emtfshower_bx->push_back(0);
        //1= positive endcap, 0=negative endcap
        trkTree.emtfshower_region->push_back(emtfShower->trackFinderType() == l1t::tftype::emtf_pos);
        trkTree.emtfshower_processor->push_back(emtfShower->processor());
        trkTree.emtfshower_isOneNominalInTime->push_back(emtfShower->isOneNominalInTime());
        trkTree.emtfshower_isTwoLooseInTime->push_back(emtfShower->isTwoLooseInTime());
        trkTree.emtfshower_isOneNominalOutOfTime->push_back(emtfShower->isOneNominalOutOfTime());
        trkTree.emtfshower_isTwoLooseOutOfTime->push_back(emtfShower->isTwoLooseOutOfTime());
        if (verboseEMTFShower_)
          std::cout << "\tShower data: "
                    << " TFtype: " << emtfShower->trackFinderType()
                    << " TwoLooseOOT: " << emtfShower->isTwoLooseOutOfTime()
                    << " TwoLooseIT: " << emtfShower->isTwoLooseInTime()
                    << " OneNominalOOT: " << emtfShower->isOneNominalOutOfTime()
                    << " OneNominalIT: " << emtfShower->isOneNominalInTime()
                    << std::endl;
      }
    }

  }

  if (runShower_) {
    iEvent.getByToken(showerToken_, showerHandle_);
    const l1t::MuonShowerBxCollection& gmtShowers = *showerHandle_.product();

    if (verboseShower_)
      std::cout << "Analyzing " << int(gmtShowers.end(0) - gmtShowers.begin(0)) << " GMT shower" << std::endl;

    for (int bx = gmtShowers.getFirstBX(); bx <= gmtShowers.getLastBX(); bx++ ){
      if ( bx < minBXGMT_ or bx > maxBXGMT_) continue;
      for (auto gmtShower = gmtShowers.begin(bx); gmtShower != gmtShowers.end(bx); ++gmtShower ){

        trkTree.l1mushower_bx->push_back(0);
        trkTree.l1mushower_isOneNominalInTime->push_back(gmtShower->isOneNominalInTime());
        trkTree.l1mushower_isTwoLooseInTime->push_back(gmtShower->isTwoLooseInTime());
        trkTree.l1mushower_isOneNominalOutOfTime->push_back(gmtShower->isOneNominalOutOfTime());
        trkTree.l1mushower_isTwoLooseOutOfTime->push_back(gmtShower->isTwoLooseOutOfTime());
        if (verboseShower_)
          std::cout << "\tShower data: "
                    << " TwoLooseOOT: " << gmtShower->isTwoLooseOutOfTime()
                    << " TwoLooseIT: " << gmtShower->isTwoLooseInTime()
                    << " OneNominalOOT: " << gmtShower->isOneNominalOutOfTime()
                    << " OneNominalIT: " << gmtShower->isOneNominalInTime()
                    << std::endl;
      }
    }
  }
}

  void L1MuAnalyzer::analyze(TreeManager& tree)
  {
  const auto& emtfTrack = match_->emtfTrack();
  const auto& muon = match_->muon();
  const auto& emtfShower  = match_->emtfShower();

  if (emtfTrack != nullptr) {
    unsigned int nMatchingStubs = 0;
    unsigned int nMatchingbestStubs = 0;
    bool stubMatched[4] = {false, false, false, false};
    bool beststubMatched[4] = {false, false, false, false};
    const auto& cscStubMatcher_ = match_->cscStubMatcher();
    bool isRun3 = true;
    //stub created by EMTF hit has following conventions:
    //stub from ME1a would have halfstrip from 0-96 and with ring number = 4
    //however, LCT from ME1A would have halfstrip from 128-223 with ring number = 1
    for (const auto& stub : *emtfTrack->emtfHits()){
      if (not stub.Is_CSC()) continue;
      const CSCCorrelatedLCTDigi& csc_stub = stub.CreateCSCCorrelatedLCTDigi(isRun3);
      const CSCDetId& csc_id1 = stub.CSC_DetId();
      uint16_t emtfhit_halfstrip = csc_id1.ring() == 4 ? csc_stub.getStrip()+128 : csc_stub.getStrip();
      const CSCDetId& csc_id = CSCDetId(csc_id1.endcap(), csc_id1.station(), csc_id1.ring()==4 ? 1 : csc_id1.ring(), 
        csc_id1.chamber(), 0);
      const CSCCorrelatedLCTDigi& csc_stub1 = CSCCorrelatedLCTDigi(1, csc_stub.isValid(), csc_stub.getQuality(), csc_stub.getKeyWG(), 
        emtfhit_halfstrip, csc_stub.getPattern(), csc_stub.getBend(), csc_stub.getBX(), 0, 0, 0, 1);
      const GlobalPoint& stub_gp = cscStubMatcher_->getGlobalPosition(csc_id1, csc_stub1);
      float emtfhit_phi = stub_gp.phi(); 
      if (verboseEMTFTrack_)
          std::cout <<"EMTFhit "<< csc_id <<" stub "<< csc_stub1 << std::endl;
      //if (csc_id1.ring() == 4) std::cout <<"EMTFhit in ring4 " << csc_id1 <<" "<< csc_stub << std::endl;
      for (const auto& sim_stub: cscStubMatcher_->lctsInChamber(csc_id.rawId())){
        bool matched = sim_stub.isValid() == csc_stub.isValid() && sim_stub.getQuality() == csc_stub.getQuality() 
        && sim_stub.getPattern() == csc_stub.getPattern() && sim_stub.getStrip() == emtfhit_halfstrip 
        && sim_stub.getKeyWG() == csc_stub.getKeyWG() && sim_stub.getBend() == csc_stub.getBend() 
        && sim_stub.getBX() == csc_stub.getBX();
        if (matched) {
          if (not stubMatched[csc_id.station() - 1])
              nMatchingStubs++;
          stubMatched[csc_id.station() - 1] = true;
          if (verboseEMTFTrack_)
            std::cout <<"EMTF matching, stub in EMTF is matched to simStub, CSCid "<< csc_id <<" stub "<< sim_stub << std::endl;
        }
      }
      const int st(gem::detIdToMEStation(csc_id1.station(), csc_id1.ring()));
      const auto& lct = cscStubMatcher_->bestLctInChamber(csc_id);
      float cscstub_phi = -9.0;
      beststubMatched[csc_id.station()-1] = (lct.isValid() == csc_stub.isValid() && lct.getQuality() == csc_stub.getQuality() 
        && lct.getPattern() == csc_stub.getPattern() && lct.getStrip() == emtfhit_halfstrip 
        && lct.getKeyWG() == csc_stub.getKeyWG() && lct.getBend() == csc_stub.getBend() 
        && lct.getBX() == csc_stub.getBX());
      if (beststubMatched[csc_id.station()-1]) nMatchingbestStubs++;
      if (lct.isValid()){
        const GlobalPoint& simstub_gp = cscStubMatcher_->getGlobalPosition(csc_id, lct);
        cscstub_phi = simstub_gp.phi();
      }

      bool digiMatcher_chamber = (csc_id.chamber()%2) ?  csc_id.chamber() == tree.cscDigi().chamber_dg_odd[st] : csc_id.chamber() == tree.cscDigi().chamber_dg_even[st];
      // convert station and ring number to index
      // index runs from 2 to 10, subtract 2
      unsigned csc_idx = CSCDetId::iChamberType(csc_id.station(), csc_id1.ring()==4 ? 1 : csc_id1.ring()) - 2;
       std::vector<unsigned> athresholds = {
      anodeThresholds_[csc_idx * 3], anodeThresholds_[csc_idx * 3 + 1], anodeThresholds_[csc_idx * 3 + 2]};
       std::vector<unsigned> cthresholds = {
      cathodeThresholds_[csc_idx * 3], cathodeThresholds_[csc_idx * 3 + 1], cathodeThresholds_[csc_idx * 3 + 2]};
      unsigned anodeHMT =0;  unsigned cathodeHMT = 0;
      //if (digiMatcher_chamber){
          // assign the bits
      unsigned anodeHits = (csc_id.chamber()%2) ? tree.cscDigi().nwires_dg_odd[st] : tree.cscDigi().nwires_dg_even[st]; 
      unsigned cathodeHits = (csc_id.chamber()%2) ? tree.cscDigi().ncomparators_dg_odd[st] : tree.cscDigi().ncomparators_dg_even[st]; 
      //unsigned anodeHits = std::max(tree.cscDigi().nwires_dg_odd[st], tree.cscDigi().nwires_dg_even[st]); 
      //unsigned cathodeHits = std::max(tree.cscDigi().ncomparators_dg_odd[st], tree.cscDigi().ncomparators_dg_even[st]); 
      for (unsigned i = 0; i < athresholds.size(); i++) {
          if (anodeHits >= athresholds[i]) {
              anodeHMT = i + 1;
              if (verboseEMTFTrack_)
                std::cout<<"CSCid "<< csc_id <<" anode hits "<< anodeHits <<" anode HMT "<< anodeHMT 
                <<" this thresh "<<athresholds[i] << std::endl;
          }
          if (cathodeHits >= cthresholds[i]) {
              cathodeHMT = i + 1;
              if (verboseEMTFTrack_)
                std::cout<<"CSCid "<< csc_id <<" cathode hits "<< cathodeHits <<" cathode HMT "<< cathodeHMT 
                << " this thresh "<< cthresholds[i] << std::endl;
          }
        }
      //}
      
      switch (csc_id.station()){
        case 1:
          tree.l1mu().emtfhit_st1_ring = csc_id.ring();
          tree.l1mu().emtfhit_st1_pattern = csc_stub.getPattern();
          tree.l1mu().emtfhit_st1_wire = csc_stub.getKeyWG();
          tree.l1mu().emtfhit_st1_halfstrip = emtfhit_halfstrip;
          tree.l1mu().emtfhit_st1_phi = emtfhit_phi;
          tree.l1mu().simhits_st1_phi = (csc_id.chamber()%2) ? tree.cscSimHit().phi_csc_sh_odd[st] : tree.cscSimHit().phi_csc_sh_even[st];
          tree.l1mu().emtfhit_st1_nwiredigis = (csc_id.chamber()%2) ? tree.cscDigi().nwires_dg_odd[st] : tree.cscDigi().nwires_dg_even[st];
          tree.l1mu().emtfhit_st1_ncomparatordigis = (csc_id.chamber()%2) ? tree.cscDigi().ncomparators_dg_odd[st] : tree.cscDigi().ncomparators_dg_even[st];
          tree.l1mu().emtfhit_st1_nstripdigis = (csc_id.chamber()%2) ? tree.cscDigi().nstrips_dg_odd[st] : tree.cscDigi().nstrips_dg_even[st];
          tree.l1mu().emtfhit_st1_anodeHMT = anodeHMT;
          tree.l1mu().emtfhit_st1_cathodeHMT = cathodeHMT;
          tree.l1mu().emtfhit_st1_digiChmatch = digiMatcher_chamber;
        if (lct.isValid()){
          tree.l1mu().cscstub_st1_found = true;
          tree.l1mu().cscstub_st1_ring = csc_id.ring();
          tree.l1mu().cscstub_st1_pattern = lct.getPattern();
          tree.l1mu().cscstub_st1_wire = lct.getKeyWG();
          tree.l1mu().cscstub_st1_halfstrip = lct.getStrip();
          tree.l1mu().cscstub_st1_phi = cscstub_phi;
        }
        break;
        case 2:
          tree.l1mu().emtfhit_st2_ring = csc_id.ring();
          tree.l1mu().emtfhit_st2_pattern = csc_stub.getPattern();
          tree.l1mu().emtfhit_st2_wire =  csc_stub.getKeyWG();
          tree.l1mu().emtfhit_st2_halfstrip =  csc_stub.getStrip();
          tree.l1mu().emtfhit_st2_phi = emtfhit_phi;
          tree.l1mu().simhits_st2_phi = (csc_id.chamber()%2) ? tree.cscSimHit().phi_csc_sh_odd[st] : tree.cscSimHit().phi_csc_sh_even[st];
          tree.l1mu().emtfhit_st2_nwiredigis = (csc_id.chamber()%2) ? tree.cscDigi().nwires_dg_odd[st] : tree.cscDigi().nwires_dg_even[st];
          tree.l1mu().emtfhit_st2_ncomparatordigis = (csc_id.chamber()%2) ? tree.cscDigi().ncomparators_dg_odd[st] : tree.cscDigi().ncomparators_dg_even[st];
          tree.l1mu().emtfhit_st2_nstripdigis = (csc_id.chamber()%2) ? tree.cscDigi().nstrips_dg_odd[st] : tree.cscDigi().nstrips_dg_even[st];
          tree.l1mu().emtfhit_st2_anodeHMT = anodeHMT;
          tree.l1mu().emtfhit_st2_cathodeHMT = cathodeHMT;
          tree.l1mu().emtfhit_st2_digiChmatch = digiMatcher_chamber;
        if (lct.isValid()){
          tree.l1mu().cscstub_st2_found = true;
          tree.l1mu().cscstub_st2_ring = csc_id.ring();
          tree.l1mu().cscstub_st2_pattern = lct.getPattern();
          tree.l1mu().cscstub_st2_wire = lct.getKeyWG();
          tree.l1mu().cscstub_st2_halfstrip = lct.getStrip();
          tree.l1mu().cscstub_st2_phi = cscstub_phi;
        }
        break;
        case 3:
          tree.l1mu().emtfhit_st3_ring = csc_id.ring();
          tree.l1mu().emtfhit_st3_pattern =  csc_stub.getPattern();
          tree.l1mu().emtfhit_st3_wire =  csc_stub.getKeyWG();
          tree.l1mu().emtfhit_st3_halfstrip =  csc_stub.getStrip();
          tree.l1mu().emtfhit_st3_phi = emtfhit_phi;
          tree.l1mu().simhits_st3_phi = (csc_id.chamber()%2) ? tree.cscSimHit().phi_csc_sh_odd[st] : tree.cscSimHit().phi_csc_sh_even[st];
          tree.l1mu().emtfhit_st3_nwiredigis = (csc_id.chamber()%2) ? tree.cscDigi().nwires_dg_odd[st] : tree.cscDigi().nwires_dg_even[st];
          tree.l1mu().emtfhit_st3_ncomparatordigis = (csc_id.chamber()%2) ? tree.cscDigi().ncomparators_dg_odd[st] : tree.cscDigi().ncomparators_dg_even[st];
          tree.l1mu().emtfhit_st3_nstripdigis = (csc_id.chamber()%2) ? tree.cscDigi().nstrips_dg_odd[st] : tree.cscDigi().nstrips_dg_even[st];
          tree.l1mu().emtfhit_st3_anodeHMT = anodeHMT;
          tree.l1mu().emtfhit_st3_cathodeHMT = cathodeHMT;
          tree.l1mu().emtfhit_st3_digiChmatch = digiMatcher_chamber;
        if (lct.isValid()){
          tree.l1mu().cscstub_st3_found = true;
          tree.l1mu().cscstub_st3_ring = csc_id.ring();
          tree.l1mu().cscstub_st3_pattern = lct.getPattern();
          tree.l1mu().cscstub_st3_wire = lct.getKeyWG();
          tree.l1mu().cscstub_st3_halfstrip = lct.getStrip();
          tree.l1mu().cscstub_st3_phi = cscstub_phi;
        }
        break;
        case 4:
          tree.l1mu().emtfhit_st4_ring = csc_id.ring();
          tree.l1mu().emtfhit_st4_pattern =  csc_stub.getPattern();
          tree.l1mu().emtfhit_st4_wire =  csc_stub.getKeyWG();
          tree.l1mu().emtfhit_st4_halfstrip =  csc_stub.getStrip();
          tree.l1mu().emtfhit_st4_phi = emtfhit_phi;
          tree.l1mu().simhits_st4_phi = (csc_id.chamber()%2) ? tree.cscSimHit().phi_csc_sh_odd[st] : tree.cscSimHit().phi_csc_sh_even[st];
          tree.l1mu().emtfhit_st4_nwiredigis = (csc_id.chamber()%2) ? tree.cscDigi().nwires_dg_odd[st] : tree.cscDigi().nwires_dg_even[st];
          tree.l1mu().emtfhit_st4_ncomparatordigis = (csc_id.chamber()%2) ? tree.cscDigi().ncomparators_dg_odd[st] : tree.cscDigi().ncomparators_dg_even[st];
          tree.l1mu().emtfhit_st4_nstripdigis = (csc_id.chamber()%2) ? tree.cscDigi().nstrips_dg_odd[st] : tree.cscDigi().nstrips_dg_even[st];
          tree.l1mu().emtfhit_st4_anodeHMT = anodeHMT;
          tree.l1mu().emtfhit_st4_cathodeHMT = cathodeHMT;
          tree.l1mu().emtfhit_st4_digiChmatch = digiMatcher_chamber;
        if (lct.isValid()){
          tree.l1mu().cscstub_st4_found = true;
          tree.l1mu().cscstub_st4_ring = csc_id.ring();
          tree.l1mu().cscstub_st4_pattern = lct.getPattern();
          tree.l1mu().cscstub_st4_wire = lct.getKeyWG();
          tree.l1mu().cscstub_st4_halfstrip = lct.getStrip();
          tree.l1mu().cscstub_st4_phi = cscstub_phi;
        }
        break;
        default: std::cout<<"Error!! station from CSC id is > 4 "<<csc_id << std::endl; 
      }
    }
    tree.l1mu().cscstub_st1_matched = stubMatched[0];
    tree.l1mu().cscstub_st2_matched = stubMatched[1];
    tree.l1mu().cscstub_st3_matched = stubMatched[2];
    tree.l1mu().cscstub_st4_matched = stubMatched[3];
    tree.l1mu().nstubs_matched_TF   = nMatchingStubs;
    tree.l1mu().allstubs_matched_TF = nMatchingStubs == emtfTrack->nStubs();
    tree.l1mu().cscstub_st1_bestmatched = beststubMatched[0];
    tree.l1mu().cscstub_st2_bestmatched = beststubMatched[1];
    tree.l1mu().cscstub_st3_bestmatched = beststubMatched[2];
    tree.l1mu().cscstub_st4_bestmatched = beststubMatched[3];
    tree.l1mu().nstubs_bestmatched_TF   = nMatchingbestStubs;
    tree.l1mu().allstubs_bestmatched_TF = nMatchingbestStubs == emtfTrack->nStubs();
    tree.l1mu().has_emtfTrack = 1;
    tree.l1mu().emtf_pt = emtfTrack->pt();
    tree.l1mu().emtf_eta = emtfTrack->eta();
    tree.l1mu().emtf_phi = emtfTrack->phi();

    tree.l1mu().hasME1 = emtfTrack->hasStub(1);
    tree.l1mu().hasME2 = emtfTrack->hasStub(2);
    tree.l1mu().hasME3 = emtfTrack->hasStub(3);
    tree.l1mu().hasME4 = emtfTrack->hasStub(4);
    tree.l1mu().nstubs = emtfTrack->nStubs();
    tree.l1mu().deltaR = emtfTrack->dR();
    tree.l1mu().chargesign = emtfTrack->charge();
    tree.l1mu().quality = emtfTrack->quality();
    tree.l1mu().mode    = emtfTrack->mode();
    tree.l1mu().deltaphi12 = emtfTrack->deltaphi(0); 
    tree.l1mu().deltaphi13 = emtfTrack->deltaphi(1); 
    tree.l1mu().deltaphi14 = emtfTrack->deltaphi(2); 
    tree.l1mu().deltaphi23 = emtfTrack->deltaphi(3); 
    tree.l1mu().deltaphi24 = emtfTrack->deltaphi(4); 
    tree.l1mu().deltaphi34 = emtfTrack->deltaphi(5); 
    tree.l1mu().deltatheta12 = emtfTrack->deltatheta(0); 
    tree.l1mu().deltatheta13 = emtfTrack->deltatheta(1); 
    tree.l1mu().deltatheta14 = emtfTrack->deltatheta(2); 
    tree.l1mu().deltatheta23 = emtfTrack->deltatheta(3); 
    tree.l1mu().deltatheta24 = emtfTrack->deltatheta(4); 
    tree.l1mu().deltatheta34 = emtfTrack->deltatheta(5); 
    tree.l1mu().lctslope1 = emtfTrack->lctslope(0); 
    tree.l1mu().lctslope2 = emtfTrack->lctslope(1); 
    tree.l1mu().lctslope3 = emtfTrack->lctslope(2); 
    tree.l1mu().lctslope4 = emtfTrack->lctslope(3); 
    tree.l1mu().lctpattern1 = emtfTrack->lctpattern(0); 
    tree.l1mu().lctpattern2 = emtfTrack->lctpattern(1); 
    tree.l1mu().lctpattern3 = emtfTrack->lctpattern(2); 
    tree.l1mu().lctpattern4 = emtfTrack->lctpattern(3); 


  }

  if (muon != nullptr) {
    tree.l1mu().has_gmtCand = 1;
    tree.l1mu().bestdRGmtCand = muon->dR();
    tree.l1mu().L1Mu_pt = muon->pt();
    tree.l1mu().L1Mu_eta = muon->eta();
    tree.l1mu().L1Mu_phi = muon->phi();
    tree.l1mu().L1Mu_charge = muon->charge();
    tree.l1mu().L1Mu_bx = muon->bx();
    tree.l1mu().L1Mu_quality = muon->quality();
  }

  if (emtfShower != nullptr){
    tree.l1mu().emtfShower_valid = emtfShower->isValid();
    tree.l1mu().emtfShower_tftype = emtfShower->trackFinderType();
    tree.l1mu().emtfShower_processor = emtfShower->processor();
    tree.l1mu().emtfShower_nominal = emtfShower->isOneNominalInTime();
    tree.l1mu().emtfShower_tight = emtfShower->isOneTightInTime();
    tree.l1mu().emtfShower_twoloose = emtfShower->isTwoLooseInTime();
  }

}
