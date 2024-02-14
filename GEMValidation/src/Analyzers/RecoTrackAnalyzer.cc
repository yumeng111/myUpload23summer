#include "GEMCode/GEMValidation/interface/Analyzers/RecoTrackAnalyzer.h"

RecoTrackAnalyzer::RecoTrackAnalyzer(const edm::ParameterSet& conf)
{
}

void RecoTrackAnalyzer::setMatcher(const RecoTrackMatcher& match_sh)
{
  match_.reset(new RecoTrackMatcher(match_sh));
}

void RecoTrackAnalyzer::analyze(TreeManager& tree)
{
  // RecoTrackExtra
  const auto& recoTrackExtra(match_->getMatchedRecoTrackExtra());
  if (recoTrackExtra != nullptr) {
    tree.recoTrack().has_recoTrackExtra = 1;
    tree.recoTrack().recoTrackExtra_pt_inner = recoTrackExtra->innerMomentum().Rho();
    tree.recoTrack().recoTrackExtra_eta_inner = recoTrackExtra->innerPosition().eta();
    tree.recoTrack().recoTrackExtra_phi_inner = recoTrackExtra->innerPosition().phi();
    tree.recoTrack().recoTrackExtra_pt_outer = recoTrackExtra->outerMomentum().Rho();
    tree.recoTrack().recoTrackExtra_eta_outer = recoTrackExtra->outerPosition().eta();
    tree.recoTrack().recoTrackExtra_phi_outer = recoTrackExtra->outerPosition().phi();
  }

  // RecoTrack
  const auto& recoTrack(match_->getMatchedRecoTrack());
  if (recoTrack != nullptr) {
    tree.recoTrack().has_recoTrack = 1;
    tree.recoTrack().recoTrack_pt_outer = recoTrack->outerPt();
    tree.recoTrack().recoTrack_eta_outer = recoTrack->outerEta();
    tree.recoTrack().recoTrack_phi_outer = recoTrack->outerPhi();
  }

  // RecoChargedCandidate
  const auto& recoChargedCandidate(match_->getMatchedRecoChargedCandidate());
  if (recoChargedCandidate != nullptr) {
    tree.recoTrack().has_recoChargedCandidate = 1;
    tree.recoTrack().recoChargedCandidate_pt = recoChargedCandidate->pt();
    tree.recoTrack().recoChargedCandidate_eta = recoChargedCandidate->eta();
    tree.recoTrack().recoChargedCandidate_phi = recoChargedCandidate->phi();
    tree.recoTrack().recoChargedCandidate_nValidDTHits = (recoChargedCandidate->track().get())->hitPattern().numberOfValidMuonDTHits();
    tree.recoTrack().recoChargedCandidate_nValidCSCHits = (recoChargedCandidate->track().get())->hitPattern().numberOfValidMuonCSCHits();
    tree.recoTrack().recoChargedCandidate_nValidRPCHits = (recoChargedCandidate->track().get())->hitPattern().numberOfValidMuonRPCHits();
    // if (verbose_) {
    //   std::cout << "recoChargedCandidate_pt " << tree.recoTrack().recoChargedCandidate_pt << std::endl;
    //   std::cout << "recoChargedCandidate_eta " << tree.recoTrack().recoChargedCandidate_eta << std::endl;
    //   std::cout << "recoChargedCandidate_phi " << tree.recoTrack().recoChargedCandidate_phi << std::endl;
    //   std::cout << "nValidHits:"
    //             << " DT " << tree.recoTrack().recoChargedCandidate_nValidDTHits
    //             << " CSC " << tree.recoTrack().recoChargedCandidate_nValidCSCHits
    //             << " RPC " << tree.recoTrack().recoChargedCandidate_nValidRPCHits << std::endl;
    // }
  }
}
