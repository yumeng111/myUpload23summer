#ifndef GEMCode_GEMValidation_GEMDigiStruct
#define GEMCode_GEMValidation_GEMDigiStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct GEMDigiStruct {

    static const int nStations = 3;

    bool has_gem_dg_even[nStations];
    bool has_gem_dg2_even[nStations];

    bool has_gem_dg_odd[nStations];
    bool has_gem_dg2_odd[nStations];

    Int_t strip_gem_dg_odd[nStations];
    Int_t strip_gem_dg_even[nStations];

    int bx_dg_odd[nStations];
    int bx_dg_even[nStations];

    int total_dg_odd[nStations];
    int total_dg_even[nStations];

    void init() {

      for (unsigned i = 0 ; i < nStations; i++) {
        has_gem_dg_even[i] = 0;
        has_gem_dg2_even[i] = 0;

        has_gem_dg_odd[i] = 0;
        has_gem_dg2_odd[i] = 0;

        strip_gem_dg_odd[i] = -9;
        strip_gem_dg_even[i] = -9;

        bx_dg_odd[i] = -9;
        bx_dg_even[i] = -9;

        total_dg_odd[i] = 0;
        total_dg_even[i] = 0;
      }
    };

    void book(TTree* t) {

      t->Branch("has_gem_dg_odd", has_gem_dg_odd, "has_gem_dg_odd[3]/O");
      t->Branch("has_gem_dg2_odd", has_gem_dg2_odd, "has_gem_dg2_odd[3]/O");

      t->Branch("has_gem_dg_even", has_gem_dg_even, "has_gem_dg_even[3]/O");
      t->Branch("has_gem_dg2_even", has_gem_dg2_even, "has_gem_dg2_even[3]/O");

      t->Branch("strip_gem_dg_odd", strip_gem_dg_odd, "strip_gem_dg_odd[3]/I");
      t->Branch("strip_gem_dg_even", strip_gem_dg_even, "strip_gem_dg_even[3]/I");

      t->Branch("bx_dg_odd", bx_dg_odd, "bx_dg_odd[3]/I");
      t->Branch("bx_dg_even", bx_dg_even, "bx_dg_even[3]/I");
      t->Branch("total_dg_odd",  total_dg_odd,  "total_dg_odd[3]/I");
      t->Branch("total_dg_even", total_dg_even, "total_dg_even[3]/I");
    }
  };
}  // namespace

#endif
