#ifndef GEMCode_GEMValidation_RecoTrackStruct
#define GEMCode_GEMValidation_RecoTrackStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct RecoTrackStruct {

    // reco track properties
    Int_t has_recoTrackExtra;
    Float_t recoTrackExtra_pt_inner;
    Float_t recoTrackExtra_eta_inner;
    Float_t recoTrackExtra_phi_inner;
    Float_t recoTrackExtra_pt_outer;
    Float_t recoTrackExtra_eta_outer;
    Float_t recoTrackExtra_phi_outer;
    Int_t has_recoTrack;
    Float_t recoTrack_pt_outer;
    Float_t recoTrack_eta_outer;
    Float_t recoTrack_phi_outer;
    Int_t has_recoChargedCandidate;
    Float_t recoChargedCandidate_pt;
    Float_t recoChargedCandidate_eta;
    Float_t recoChargedCandidate_phi;
    Int_t recoChargedCandidate_nValidCSCHits;
    Int_t recoChargedCandidate_nValidRPCHits;
    Int_t recoChargedCandidate_nValidDTHits;

    void init() {
      has_recoTrackExtra = 0;
      recoTrackExtra_pt_inner = - 99.;
      recoTrackExtra_eta_inner = - 99.;
      recoTrackExtra_phi_inner = - 99.;
      recoTrackExtra_pt_outer = - 99.;
      recoTrackExtra_eta_outer = - 99.;
      recoTrackExtra_phi_outer = - 99.;
      has_recoTrack = 0;
      recoTrack_pt_outer = - 99.;
      recoTrack_eta_outer = - 99.;
      recoTrack_phi_outer = - 99.;
      has_recoChargedCandidate = 0;
      recoChargedCandidate_pt = - 99.;
      recoChargedCandidate_eta = - 99.;
      recoChargedCandidate_phi = - 99.;
      recoChargedCandidate_nValidCSCHits = 0;
      recoChargedCandidate_nValidRPCHits = 0;
      recoChargedCandidate_nValidDTHits = 0;
    };

    void book(TTree* t) {

      t->Branch("has_recoTrackExtra", &has_recoTrackExtra);
      t->Branch("recoTrackExtra_pt_inner", &recoTrackExtra_pt_inner);
      t->Branch("recoTrackExtra_eta_inner", &recoTrackExtra_eta_inner);
      t->Branch("recoTrackExtra_phi_inner", &recoTrackExtra_phi_inner);
      t->Branch("recoTrackExtra_pt_outer", &recoTrackExtra_pt_outer);
      t->Branch("recoTrackExtra_eta_outer", &recoTrackExtra_eta_outer);
      t->Branch("recoTrackExtra_phi_outer", &recoTrackExtra_phi_outer);
      t->Branch("has_recoTrack", &has_recoTrack);
      t->Branch("recoTrack_pt_outer", &recoTrack_pt_outer);
      t->Branch("recoTrack_eta_outer", &recoTrack_eta_outer);
      t->Branch("recoTrack_phi_outer", &recoTrack_phi_outer);
      t->Branch("has_recoChargedCandidate", &has_recoChargedCandidate);
      t->Branch("recoChargedCandidate_pt", &recoChargedCandidate_pt);
      t->Branch("recoChargedCandidate_eta", &recoChargedCandidate_eta);
      t->Branch("recoChargedCandidate_phi", &recoChargedCandidate_phi);

      t->Branch("recoChargedCandidate_nValidDTHits", &recoChargedCandidate_nValidDTHits);
      t->Branch("recoChargedCandidate_nValidCSCHits", &recoChargedCandidate_nValidCSCHits);
      t->Branch("recoChargedCandidate_nValidRPCHits", &recoChargedCandidate_nValidRPCHits);
    }
  };
}  // namespace

#endif
