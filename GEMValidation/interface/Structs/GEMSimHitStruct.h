#ifndef GEMCode_GEMValidation_GEMSimHitStruct
#define GEMCode_GEMValidation_GEMSimHitStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct GEMSimHitStruct {

    static const int nStations = 3;

    // bools
    bool has_gem_sh_even[nStations];
    bool has_gem_sh_odd[nStations];

    bool has_gem_sh2_even[nStations];
    bool has_gem_sh2_odd[nStations];

    bool has_gemcopad_sh_even[nStations];
    bool has_gemcopad_sh_odd[nStations];

    // ints
    int chamber_sh_odd[nStations];
    int chamber_sh_even[nStations];

    // floats
    float dphi_sh_even[nStations];
    float dphi_sh_odd[nStations];

    float dphipositionpt_gem_sh_even[nStations];
    float dphipositionpt_gem_sh_odd[nStations];

    float bending_sh[nStations];

    float perp_gem_sh_even[nStations];
    float perp_gem_sh_odd[nStations];

    float centralperp_gem_sh_even[nStations];
    float centralperp_gem_sh_odd[nStations];

    float strip_gem_sh_odd[nStations];
    float strip_gem_sh_even[nStations];

    float eta_gem_sh_odd[nStations];
    float eta_gem_sh_even[nStations];

    float phi_gem_sh_odd[nStations];
    float phi_gem_sh_even[nStations];


    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {

        has_gem_sh_even[i] = false;
        has_gem_sh_odd[i] = false;

        has_gem_sh2_even[i] = false;
        has_gem_sh2_odd[i] = false;

        has_gemcopad_sh_even[i] = false;
        has_gemcopad_sh_odd[i] = false;

        chamber_sh_odd[i] = -1;
        chamber_sh_even[i] = -1;

        bending_sh[i] = -10;

        perp_gem_sh_odd[i] = -0.0;
        perp_gem_sh_even[i] = -0.0;

        centralperp_gem_sh_odd[i] = -0.0;
        centralperp_gem_sh_even[i] = -0.0;

        strip_gem_sh_odd[i] = -9.;
        strip_gem_sh_even[i] = -9.;

        eta_gem_sh_odd[i] = -9.;
        eta_gem_sh_even[i] = -9.;

        phi_gem_sh_odd[i] = -9.;
        phi_gem_sh_even[i] = -9.;

        dphi_sh_odd[i] = -9;
        dphi_sh_even[i] = -9;
      }
    };

    void book(TTree* t) {

      t->Branch("has_gem_sh_odd", has_gem_sh_odd, "has_gem_sh_odd[3]/O");
      t->Branch("has_gem_sh_even", has_gem_sh_even, "has_gem_sh_even[3]/O");

      t->Branch("has_gem_sh2_odd", has_gem_sh2_odd, "has_gem_sh2_odd[3]/O");
      t->Branch("has_gem_sh2_even", has_gem_sh2_even, "has_gem_sh2_even[3]/O");

      t->Branch("has_gemcopad_sh_even", has_gemcopad_sh_even, "has_gemcopad_sh_even[3]/O");
      t->Branch("has_gemcopad_sh_odd", has_gemcopad_sh_odd, "has_gemcopad_sh_odd[3]/O");

      t->Branch("chamber_sh_odd", chamber_sh_odd, "chamber_sh_odd[3]/I");
      t->Branch("chamber_sh_even", chamber_sh_even, "chamber_sh_even[3]/I");

      t->Branch("bending_sh", bending_sh, "bending_sh[3]/F");

      t->Branch("perp_gem_sh_odd", perp_gem_sh_odd, "perp_gem_sh_odd[3]/F");
      t->Branch("perp_gem_sh_even", perp_gem_sh_even, "perp_gem_sh_even[3]/F");

      t->Branch("centralperp_gem_sh_odd", centralperp_gem_sh_odd, "centralperp_gem_sh_odd[3]/F");
      t->Branch("centralperp_gem_sh_even", centralperp_gem_sh_even, "centralperp_gem_sh_even[3]/F");

      t->Branch("strip_gem_sh_odd", strip_gem_sh_odd, "strip_gem_sh_odd[3]/F");
      t->Branch("strip_gem_sh_even", strip_gem_sh_even, "strip_gem_sh_even[3]/F");

      t->Branch("eta_gem_sh_odd", eta_gem_sh_odd, "eta_gem_sh_odd[3]/F");
      t->Branch("eta_gem_sh_even", eta_gem_sh_even, "eta_gem_sh_even[3]/F");

      t->Branch("phi_gem_sh_odd", phi_gem_sh_odd, "phi_gem_sh_odd[3]/F");
      t->Branch("phi_gem_sh_even", phi_gem_sh_even, "phi_gem_sh_even[3]/F");

      t->Branch("dphi_sh_odd", dphi_sh_odd, "dphi_sh_odd[3]/F");
      t->Branch("dphi_sh_even", dphi_sh_even, "dphi_sh_even[3]/F");
    }
  };
}  // namespace

#endif
