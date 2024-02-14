#include "GEMCode/GEMValidation/interface/Analyzers/L1TrackAnalyzer.h"

L1TrackAnalyzer::L1TrackAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& l1Track = conf.getParameter<edm::ParameterSet>("l1Track");
  minBXTrack_ = l1Track.getParameter<int>("minBX");
  maxBXTrack_ = l1Track.getParameter<int>("maxBX");
  verboseTrack_ = l1Track.getParameter<int>("verbose");
  runTrack_ = l1Track.getParameter<bool>("run");

  const auto& l1TrackMuon = conf.getParameter<edm::ParameterSet>("l1TkMuon");
  minBXTrackMuon_ = l1TrackMuon.getParameter<int>("minBX");
  maxBXTrackMuon_ = l1TrackMuon.getParameter<int>("maxBX");
  verboseTrackMuon_ = l1TrackMuon.getParameter<int>("verbose");
  runTrackMuon_ = l1TrackMuon.getParameter<bool>("run");

  l1TrackToken_ = iC.consumes<L1TTTrackCollectionType>(l1Track.getParameter<edm::InputTag>("inputTag"));
  l1TrackMuonToken_ = iC.consumes<l1t::TkMuonCollection>(l1TrackMuon.getParameter<edm::InputTag>("inputTag"));
}

void L1TrackAnalyzer::setMatcher(const L1TrackMatcher& match_sh)
{
  match_.reset(new L1TrackMatcher(match_sh));
}

void L1TrackAnalyzer::analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree) {}

void L1TrackAnalyzer::analyze(TreeManager& tree)
{
  const auto& track = match_->l1Track();
  const auto& muon = match_->l1TrackMuon();

  if (track != nullptr) {
    tree.l1track().has_L1Track = true;
    tree.l1track().L1Track_pt = track->momentum().perp();
    tree.l1track().L1Track_eta = track->momentum().eta();
    tree.l1track().L1Track_phi = track->momentum().barePhi();
  }

  if (muon != nullptr) {
    tree.l1track().has_L1TrackMuon = true;
    tree.l1track().L1TrackMuon_pt = muon->pt();
    tree.l1track().L1TrackMuon_eta = muon->eta();
    tree.l1track().L1TrackMuon_phi = muon->phi();
  }

  /*
    L1TrackTriggerVeto trkVeto2(cfg_, match_sh.eventSetup(), match_sh.event(), trackInputLabel_,
    etrk_[0].L1Mu_eta, normalizedPhi((float)etrk_[0].L1Mu_phi));
    etrk_[0].isL1LooseVeto  = trkVeto2.isLooseVeto();
    etrk_[0].isL1MediumVeto = trkVeto2.isMediumVeto();
    etrk_[0].isL1TightVeto  = trkVeto2.isTightVeto();
  */
}
