#include "GEMCode/GEMValidation/interface/Matchers/L1MuMatcher.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/normalizedPhi.h"

L1MuMatcher::L1MuMatcher(const edm::ParameterSet& ps, edm::ConsumesCollector&& iC)
{
  cscStubMatcher_.reset(new CSCStubMatcher(ps, std::move(iC)));

  const auto& emtfTrack = ps.getParameter<edm::ParameterSet>("emtfTrack");
  minBXEMTFTrack_ = emtfTrack.getParameter<int>("minBX");
  maxBXEMTFTrack_ = emtfTrack.getParameter<int>("maxBX");
  verboseEMTFTrack_ = emtfTrack.getParameter<int>("verbose");
  deltaREMTFTrack_ = emtfTrack.getParameter<double>("deltaR");
  runEMTFTrack_ = emtfTrack.getParameter<bool>("run");

  const auto& emtfCand = ps.getParameter<edm::ParameterSet>("emtfCand");
  minBXRegMuCand_ = emtfCand.getParameter<int>("minBX");
  maxBXRegMuCand_ = emtfCand.getParameter<int>("maxBX");
  verboseRegMuCand_ = emtfCand.getParameter<int>("verbose");
  deltaRRegMuCand_ = emtfCand.getParameter<double>("deltaR");
  runRegMuCand_ = emtfCand.getParameter<bool>("run");

  const auto& muon = ps.getParameter<edm::ParameterSet>("muon");
  minBXGMT_ = muon.getParameter<int>("minBX");
  maxBXGMT_ = muon.getParameter<int>("maxBX");
  verboseGMT_ = muon.getParameter<int>("verbose");
  deltaRGMT_ = muon.getParameter<double>("deltaR");
  runGMT_ = muon.getParameter<bool>("run");

  const auto& emtfShower = ps.getParameter<edm::ParameterSet>("emtfShower");
  minBXEMTFShower_ = emtfShower.getParameter<int>("minBX");
  maxBXEMTFShower_ = emtfShower.getParameter<int>("maxBX");
  verboseEMTFShower_ = emtfShower.getParameter<int>("verbose");
  runEMTFShower_ = emtfShower.getParameter<bool>("run");

  //const auto& muonShower = conf.getParameter<edm::ParameterSet>("muonShower");
  //minBXGMT_ = muonShower.getParameter<int>("minBX");
  //maxBXGMT_ = muonShower.getParameter<int>("maxBX");
  //verboseShower_ = muonShower.getParameter<int>("verbose");
  //runShower_ = muonShower.getParameter<bool>("run");

  emtfTrackToken_ = iC.consumes<l1t::EMTFTrackCollection>(emtfTrack.getParameter<edm::InputTag>("inputTag"));
  emtfCandToken_ = iC.consumes<l1t::RegionalMuonCandBxCollection>(emtfCand.getParameter<edm::InputTag>("inputTag"));
  muonToken_ = iC.consumes<l1t::MuonBxCollection>(muon.getParameter<edm::InputTag>("inputTag"));
  emtfShowerToken_ = iC.consumes<l1t::RegionalMuonShowerBxCollection>(emtfShower.getParameter<edm::InputTag>("inputTag"));
  //showerToken_ = iC.consumes<l1t::MuonShowerBxCollection>(muonShower.getParameter<edm::InputTag>("inputTag"));
}

void L1MuMatcher::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  cscStubMatcher_->init(iEvent, iSetup);

  iEvent.getByToken(emtfTrackToken_, emtfTrackHandle_);
  iEvent.getByToken(emtfCandToken_, emtfCandHandle_);
  iEvent.getByToken(muonToken_, muonHandle_);
  iEvent.getByToken(emtfShowerToken_, emtfShowerHandle_);
  //iEvent.getByToken(showerToken_, muonShowerHandle_);
}

void
L1MuMatcher::clear()
{
  emtfTrack_ = nullptr;
  emtfCand_ = nullptr;
  muon_ = nullptr;
  emtfShower_ = nullptr;
}


// do the matching
void L1MuMatcher::match(const SimTrack& t, const SimVertex& v)
{
  clear();

  cscStubMatcher_->match(t, v);

  if (runEMTFTrack_)
    matchEmtfTrackToSimTrack(t, *emtfTrackHandle_.product());

  if (runRegMuCand_)
    matchRegionalMuonCandToSimTrack(*emtfCandHandle_.product());

  if (runGMT_)
    matchGMTToSimTrack(*muonHandle_.product());

  if(runEMTFShower_)
    matchEmtfShowerToSimTrack(t, *emtfShowerHandle_.product());

}

void
L1MuMatcher::matchEmtfTrackToSimTrack(const SimTrack& simtrack, const l1t::EMTFTrackCollection& tracks)
{
  if (deltaREMTFTrack_ < 0) {
    bool isRun3 = true;
    for (const auto& trk : tracks) {
      int bx = trk.BX();
      if ( bx < minBXEMTFTrack_ or bx > maxBXEMTFTrack_) continue;
      int nMatchingStubs = 0;
      int nMaxMatchingStubs = 0;
      if (verboseEMTFTrack_)
        std::cout <<"track BX "<< trk.BX()
                  <<  " pt "<< trk.Pt()
                  <<" eta "<< trk.Eta()
                  <<" phi "<< emtf::deg_to_rad(trk.Phi_glob())
                  <<" phi_local "<< emtf::deg_to_rad(trk.Phi_loc()) << std::endl;
      for (const auto& stub : trk.Hits()){
        const CSCCorrelatedLCTDigi& csc_stub = stub.CreateCSCCorrelatedLCTDigi(isRun3);
        const CSCDetId& csc_id = stub.CSC_DetId();
        if (verboseEMTFTrack_)
          std::cout << "\tCSCDetId " << csc_id << " CSC stub "
                    << csc_stub << " available stubs in this chamber: "
                    << cscStubMatcher_->lctsInChamber(csc_id.rawId()).size() << std::endl;

        for (const auto& sim_stub: cscStubMatcher_->lctsInChamber(csc_id.rawId())){
          if (verboseEMTFTrack_)
            std::cout << "\tSIM " << csc_id << " " << sim_stub << std::endl;
          if (csc_stub == sim_stub) {
            nMatchingStubs++;
          }
        }
        if (nMatchingStubs>=2) {
          if (nMatchingStubs > nMaxMatchingStubs){
            emtfTrack_.reset(new gem::EMTFTrack(trk));
            nMaxMatchingStubs = nMatchingStubs;
          }
        }
      }
    }
  }
  else {
    float mindREMTFTrack = 10.0;
    for (const auto& trk : tracks) {
      if (verboseEMTFTrack_)
        std::cout <<"track BX "<< trk.BX()
                  <<  " pt "<< trk.Pt()
                  <<" eta "<< trk.Eta()
                  <<" phi "<< emtf::deg_to_rad(trk.Phi_glob())
                  <<" phi_local "<< emtf::deg_to_rad(trk.Phi_loc()) << std::endl;
      int bx = trk.BX();
      if ( bx < minBXEMTFTrack_ or bx > maxBXEMTFTrack_) continue;

      float dR = deltaR(float(simtrack.momentum().eta()), float(reco::reduceRange(simtrack.momentum().phi())),
                        trk.Eta(), emtf::deg_to_rad(trk.Phi_glob()));

      if (verboseEMTFTrack_)
        std::cout <<"dR (track, sim) "<< dR <<" deltaREMTFTrack_ "
                  << deltaREMTFTrack_ << std::endl;
      if (dR < deltaREMTFTrack_){
        if (dR < mindREMTFTrack){
          if (verboseEMTFTrack_)
            std::cout <<"...is matched" << std::endl;
          emtfTrack_.reset(new gem::EMTFTrack(trk));
          emtfTrack_->setDR(dR);
          mindREMTFTrack = dR;
        }
      }
    }
  }
}

void L1MuMatcher::matchRegionalMuonCandToSimTrack(const l1t::RegionalMuonCandBxCollection& regMuCands)
{
  if (emtfTrack_ == nullptr) return;

  // EMTF track properties
  float track_pt = emtfTrack_->pt();
  float track_eta = emtfTrack_->eta();
  float track_phi = emtfTrack_->phi();

  if (verboseRegMuCand_)
    std::cout << "Matched EMTF track "<< track_pt << " " << track_eta << " " << track_phi << std::endl;

  //float mindPtRel = 0.5;
  float mindRRegMuCand = 1;

  for (int bx = regMuCands.getFirstBX(); bx <= regMuCands.getLastBX(); bx++ ){
    if ( bx < minBXRegMuCand_ or bx > maxBXRegMuCand_) continue;
    for (auto emtfCand = regMuCands.begin(bx); emtfCand != regMuCands.end(bx); ++emtfCand ){

      const gem::EMTFCand& cand(*emtfCand);

      // EMTF candidate properties
      float cand_pt = cand.pt();
      float cand_eta = cand.eta();
      float cand_phi = cand.phi();

      if (verboseRegMuCand_)
        std::cout << "\tcandidate regional muon " << cand_pt << " " << cand_eta << " " << cand_phi << std::endl;

      float dR = deltaR(track_eta, track_phi, cand_eta, cand_phi);
      float dPtRel = std::fabs(track_pt - cand_pt)/cand_pt;

      if (verboseRegMuCand_)
        std::cout << "\tdR " << dR << " dPtRel " << dPtRel << std::endl<< std::endl;

      if (dR < mindRRegMuCand and dR < 0.1){
        if (verboseRegMuCand_)
          std::cout <<"...is matched" << std::endl;
        mindRRegMuCand = dR;
        emtfCand_.reset(new gem::EMTFCand(*emtfCand));
      }
    }
  }
}

void L1MuMatcher::matchGMTToSimTrack(const BXVector<l1t::Muon>& gmtCands)
{
  if (emtfCand_ == nullptr) return;

  // EMTF candidate properties
  float cand_pt = emtfCand_->pt();
  float cand_eta = emtfCand_->eta();
  float cand_phi = emtfCand_->phi();

  if (verboseGMT_)
    std::cout << "Matched regional muon " << cand_pt << " " << cand_eta << " " << cand_phi << std::endl;

  //float mindPtRel = 1;
  float mindRGMT = 1;

  for (int bx = gmtCands.getFirstBX(); bx <= gmtCands.getLastBX(); bx++ ){
    if ( bx < minBXGMT_ or bx > maxBXGMT_) continue;
    for (auto emtfCand = gmtCands.begin(bx); emtfCand != gmtCands.end(bx); ++emtfCand ){

      const gem::EMTFCand& cand(*emtfCand);

      // Muon properties
      float muon_pt = cand.pt();
      float muon_eta = cand.eta();
      float muon_phi = cand.phi();

      if (verboseGMT_)
        std::cout << "\tcandidate muon " << muon_pt << " " << muon_eta << " " << muon_phi << std::endl;

      float dR = deltaR(muon_eta, muon_phi, cand_eta, cand_phi);
      float dPtRel = std::fabs(cand_pt - muon_pt)/muon_pt;

      if (verboseGMT_)
        std::cout << "\tdR " << dR << " dPtRel " << dPtRel << std::endl << std::endl;

      if (dR < mindRGMT and dR < 0.1){
        if (verboseGMT_)
          std::cout <<"...is matched" << std::endl;
        mindRGMT = dR;
        muon_.reset(new gem::EMTFCand(*emtfCand));
      }
    }
  }
}

void  L1MuMatcher::matchEmtfShowerToSimTrack(const SimTrack& simtrack, const l1t::RegionalMuonShowerBxCollection& emtfShowers){
  float simEta = simtrack.momentum().eta();
  unsigned emtfshower_type = 0;
  emtfShower_ = nullptr; 
  if (verboseEMTFShower_)
    std::cout << "Analyzing " << int(emtfShowers.end(0) - emtfShowers.begin(0)) << " EMTF showers" << std::endl;

  for (int bx = emtfShowers.getFirstBX(); bx <= emtfShowers.getLastBX(); bx++ ){
    if ( bx < minBXEMTFShower_ or bx > maxBXEMTFShower_) continue;
    for (auto emtfShower = emtfShowers.begin(bx); emtfShower != emtfShowers.end(bx); ++emtfShower ){

      if (not emtfShower->isValid()) continue;
      if (not ((simEta < 0 and emtfShower->trackFinderType() == l1t::tftype::emtf_neg) or (simEta>0 and emtfShower->trackFinderType() == l1t::tftype::emtf_pos)))
        continue;
      unsigned showerType = 0;
      if (emtfShower->isOneNominalInTime()) showerType = 1;
      else if (emtfShower->isOneTightInTime()) showerType = 2;
      else if (emtfShower->isTwoLooseInTime()) showerType = 3;
      if (showerType > emtfshower_type){
        emtfShower_.reset(new l1t::RegionalMuonShower(*emtfShower));
        emtfshower_type = showerType;
      }
      
      if (verboseEMTFShower_)
        std::cout << "\tShower data: "
                  << " TFtype: " << emtfShower->trackFinderType()
                  << " TwoLooseOOT: " << emtfShower->isTwoLooseOutOfTime()
                  << " TwoLooseIT: " << emtfShower->isTwoLooseInTime()
                  << " OneNominalOOT: " << emtfShower->isOneNominalOutOfTime()
                  << " OneNominalIT: " << emtfShower->isOneNominalInTime()
                  << " SimEta "<< simEta <<" matched! "
                  << std::endl;
    }
  }


}
