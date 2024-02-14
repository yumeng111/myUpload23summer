#ifndef GEMCode_GEMValidation_L1TrackStruct
#define GEMCode_GEMValidation_L1TrackStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct L1TrackStruct {

    // Track properties
    bool has_L1Track;
    bool has_L1TrackMuon;

    float L1Track_pt;
    float L1Track_eta;
    float L1Track_phi;
    int L1Track_charge;
    int L1Track_endcap;

    float L1TrackMuon_pt;
    float L1TrackMuon_eta;
    float L1TrackMuon_phi;
    int L1TrackMuon_charge;
    int L1TrackMuon_endcap;

    void init() {
      has_L1Track = false;
      has_L1TrackMuon = false;
      L1Track_pt = 0.;
      L1Track_phi = -9;
      L1Track_eta = -9.;
      L1Track_charge = -9;
      L1Track_endcap = -9;

      L1TrackMuon_pt = 0.;
      L1TrackMuon_phi = -9;
      L1TrackMuon_eta = -9.;
      L1TrackMuon_charge = -9;
      L1TrackMuon_endcap = -9;
    };

    void book(TTree* t) {
      t->Branch("has_L1Track", &has_L1Track);
      t->Branch("has_L1TrackMuon", &has_L1TrackMuon);

      t->Branch("L1Track_pt", &L1Track_pt);
      t->Branch("L1Track_eta", &L1Track_eta);
      t->Branch("L1Track_phi", &L1Track_phi);
      t->Branch("L1Track_charge", &L1Track_charge);
      t->Branch("L1Track_endcap", &L1Track_endcap);

      t->Branch("L1TrackMuon_pt", &L1TrackMuon_pt);
      t->Branch("L1TrackMuon_eta", &L1TrackMuon_eta);
      t->Branch("L1TrackMuon_phi", &L1TrackMuon_phi);
      t->Branch("L1TrackMuon_charge", &L1TrackMuon_charge);
      t->Branch("L1TrackMuon_endcap", &L1TrackMuon_endcap);
    }
  };
}  // namespace

#endif
