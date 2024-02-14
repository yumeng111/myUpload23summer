#ifndef GEMCode_GEMValidation_CSCSimHitStruct
#define GEMCode_GEMValidation_CSCSimHitStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct CSCSimHitStruct {

    static const int nStations = 11;

    // bools
    bool has_csc_sh_odd[nStations];
    bool has_csc_sh_even[nStations];

    // ints
    int nlayers_csc_sh_even[nStations];
    int nlayers_csc_sh_odd[nStations];

    int chamber_sh_odd[nStations];
    int chamber_sh_even[nStations];

    int nstrips_sh_odd[nStations];
    int nstrips_sh_even[nStations];
    int nwires_sh_odd[nStations];
    int nwires_sh_even[nStations];

    int totalnum_csc_sh_odd[nStations];
    int totalnum_csc_sh_even[nStations];
    // floats

    float dphi_sh_even[nStations];
    float dphi_sh_odd[nStations];

    float dphipositionpt_csc_sh_even[nStations];
    float dphipositionpt_csc_sh_odd[nStations];

    float bend_csc_sh_even[nStations];
    float bend_csc_sh_odd[nStations];

    float phi_csc_sh_even[nStations];
    float phi_csc_sh_odd[nStations];
    float eta_csc_sh_even[nStations];
    float eta_csc_sh_odd[nStations];
    float perp_csc_sh_even[nStations];
    float perp_csc_sh_odd[nStations];

    float phi_layer1_sh_even[nStations];
    float eta_layer1_sh_even[nStations];
    float phi_layer1_sh_odd[nStations];
    float eta_layer1_sh_odd[nStations];
    float perp_layer1_sh_odd[nStations];
    float perp_layer1_sh_even[nStations];
    float z_layer1_sh_odd[nStations];
    float z_layer1_sh_even[nStations];

    float phi_layer6_sh_even[nStations];
    float eta_layer6_sh_even[nStations];
    float phi_layer6_sh_odd[nStations];
    float eta_layer6_sh_odd[nStations];
    float perp_layer6_sh_odd[nStations];
    float perp_layer6_sh_even[nStations];
    float z_layer6_sh_odd[nStations];
    float z_layer6_sh_even[nStations];

    float strip_csc_sh_even[nStations];
    float strip_csc_sh_odd[nStations];

    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {

        has_csc_sh_even[i] = false;
        has_csc_sh_odd[i] = false;

        chamber_sh_odd[i] = -1;
        chamber_sh_even[i] = -1;

        nstrips_sh_odd[i] = 0;
        nstrips_sh_even[i] = 0;
        nwires_sh_odd[i] = 0;
        nwires_sh_even[i] = 0;

        totalnum_csc_sh_odd[i] = 0;
        totalnum_csc_sh_even[i] = 0;

        nlayers_csc_sh_odd[i] = -1;
        nlayers_csc_sh_even[i] = -1;

        perp_csc_sh_odd[i] = -0.0;
        perp_csc_sh_even[i] = -0.0;

        phi_csc_sh_even[i] = -9.0;
        phi_csc_sh_odd[i] = -9.0;

        eta_csc_sh_even[i] = -9.0;
        eta_csc_sh_odd[i] = -9.0;

        dphi_sh_odd[i] = -9;
        dphi_sh_even[i] = -9;

        bend_csc_sh_even[i] = -10;
        bend_csc_sh_odd[i] = -10;

        strip_csc_sh_even[i] = -100;
        strip_csc_sh_odd[i] = -100;
      }
    };

    void book(TTree* t) {

      t->Branch("has_csc_sh_even", has_csc_sh_even, "has_csc_sh_even[11]/O");
      t->Branch("has_csc_sh_odd", has_csc_sh_odd, "has_csc_sh_odd[11]/O");

      t->Branch("chamber_sh_odd", chamber_sh_odd, "chamber_sh_odd[11]/I");
      t->Branch("chamber_sh_even", chamber_sh_even, "chamber_sh_even[11]/I");

      t->Branch("nstrips_sh_odd",  nstrips_sh_odd,  "nstrips_sh_odd[11]/I");
      t->Branch("nstrips_sh_even", nstrips_sh_even, "nstrips_sh_even[11]/I");
      t->Branch("nwires_sh_odd",  nwires_sh_odd,  "nwires_sh_odd[11]/I");
      t->Branch("nwires_sh_even", nwires_sh_even, "nwires_sh_even[11]/I");
      t->Branch("totalnum_csc_sh_odd", totalnum_csc_sh_odd, "totalnum_csc_sh_odd[11]/I");
      t->Branch("totalnum_csc_sh_even", totalnum_csc_sh_even, "totalnum_csc_sh_even[11]/I");

      t->Branch("nlayers_csc_sh_odd", nlayers_csc_sh_odd, "nlayers_csc_sh_odd[11]/I");
      t->Branch("nlayers_csc_sh_even", nlayers_csc_sh_even, "nlayers_csc_sh_even[11]/I");

      t->Branch("bend_csc_sh_even", bend_csc_sh_even, "bend_csc_sh_even[11]/F");
      t->Branch("bend_csc_sh_odd", bend_csc_sh_odd, "bend_csc_sh_odd[11]/F");

      t->Branch("perp_csc_sh_odd", perp_csc_sh_odd, "perp_csc_sh_odd[11]/F");
      t->Branch("perp_csc_sh_even", perp_csc_sh_even, "perp_csc_sh_even[11]/F");

      t->Branch("phi_csc_sh_even", phi_csc_sh_even, "phi_csc_sh_even[11]/F");
      t->Branch("phi_csc_sh_odd", phi_csc_sh_odd, "phi_csc_sh_odd[11]/F");

      t->Branch("eta_csc_sh_even", eta_csc_sh_even, "eta_csc_sh_even[11]/F");
      t->Branch("eta_csc_sh_odd", eta_csc_sh_odd, "eta_csc_sh_odd[11]/F");

      t->Branch("dphi_sh_odd", dphi_sh_odd, "dphi_sh_odd[11]/F");
      t->Branch("dphi_sh_even", dphi_sh_even, "dphi_sh_even[11]/F");

      t->Branch("strip_csc_sh_even", strip_csc_sh_even, "strip_csc_sh_even[11]/F");
      t->Branch("strip_csc_sh_odd", strip_csc_sh_odd, "strip_csc_sh_odd[11]/F");
    }
  };
}  // namespace

#endif
