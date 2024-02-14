#ifndef GEMCode_GEMValidation_CSCDigiStruct
#define GEMCode_GEMValidation_CSCDigiStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {
  /*
    "CSC_ME11", "CSC_ME1a", "CSC_ME1b", "CSC_ME12", "CSC_ME13",
    "CSC_ME21", "CSC_ME22", "CSC_ME31",
    "CSC_ME32", "CSC_ME41", "CSC_ME42"
  */

  struct CSCDigiStruct {

    static const int nStations = 11;

    // bool
    bool has_csc_strips_even[nStations];
    bool has_csc_strips_odd[nStations];

    bool has_csc_wires_even[nStations];
    bool has_csc_wires_odd[nStations];

    // int
    Int_t nlayers_wg_dg_odd[nStations];
    Int_t nlayers_st_dg_odd[nStations];

    Int_t nlayers_wg_dg_even[nStations];
    Int_t nlayers_st_dg_even[nStations];

    int chamber_dg_odd[nStations];
    int chamber_dg_even[nStations];

    int nwires_dg_odd[nStations];
    int nwires_dg_even[nStations];
    int nstrips_dg_odd[nStations];
    int nstrips_dg_even[nStations];
    int ncomparators_dg_even[nStations];
    int ncomparators_dg_odd[nStations];
    float nwiresbx_dg_odd[nStations];
    float nwiresbx_dg_even[nStations];
    float ncompsbx_dg_even[nStations];
    float ncompsbx_dg_odd[nStations];

    int totalwires_dg_odd[nStations];
    int totalwires_dg_even[nStations];
    int totalstrips_dg_odd[nStations];
    int totalstrips_dg_even[nStations];
    int totalcomparators_dg_even[nStations];
    int totalcomparators_dg_odd[nStations];

    Int_t wiregroup_odd[nStations];
    Int_t wiregroup_even[nStations];

    Int_t halfstrip_odd[nStations];
    Int_t halfstrip_even[nStations];

    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {

        has_csc_strips_even[i] = 0;
        has_csc_strips_odd[i] = 0;

        has_csc_wires_even[i] = 0;
        has_csc_wires_odd[i] = 0;

        nlayers_wg_dg_odd[i] = -1;
        nlayers_wg_dg_even[i] = -1;

        nlayers_st_dg_odd[i] = -1;
        nlayers_st_dg_even[i] = -1;

        chamber_dg_odd[i] = -1;
        chamber_dg_even[i] = -1;

        wiregroup_odd[i] = -1;
        wiregroup_even[i] = -1;

        halfstrip_odd[i] = -1;
        halfstrip_even[i] = -1;

        nwires_dg_odd[i] = 0;
        nwires_dg_even[i] = 0;
        nstrips_dg_even[i] = 0;
        nstrips_dg_odd[i] = 0;
        ncomparators_dg_odd[i] = 0;
        ncomparators_dg_even[i] = 0;
        nwiresbx_dg_odd[i]  = 0.0;
        nwiresbx_dg_even[i] = 0.0;
        ncompsbx_dg_odd[i]  = 0.0;
        ncompsbx_dg_even[i] = 0.0;
        totalwires_dg_odd[i] = 0;
        totalwires_dg_even[i] = 0;
        totalstrips_dg_even[i] = 0;
        totalstrips_dg_odd[i] = 0;
        totalcomparators_dg_odd[i] = 0;
        totalcomparators_dg_even[i] = 0;
      }
    };

   void book(TTree* t) {

      t->Branch("has_csc_strips_odd", has_csc_strips_odd, "has_csc_strips_odd[11]/O");
      t->Branch("has_csc_strips_even", has_csc_strips_even, "has_csc_strips_even[11]/O");

      t->Branch("has_csc_wires_odd", has_csc_wires_odd, "has_csc_wires_odd[11]/O");
      t->Branch("has_csc_wires_even", has_csc_wires_even, "has_csc_wires_even[11]/O");

      t->Branch("chamber_dg_odd", chamber_dg_odd, "chamber_dg_odd[11]/I");
      t->Branch("chamber_dg_even", chamber_dg_even, "chamber_dg_even[11]/I");
      t->Branch("nwires_dg_odd",          nwires_dg_odd,         "nwires_dg_odd[11]/I");
      t->Branch("nwires_dg_even",         nwires_dg_even,        "nwires_dg_even[11]/I");
      t->Branch("nstrips_dg_odd",         nstrips_dg_odd,        "nstrips_dg_odd[11]/I");
      t->Branch("nstrips_dg_even",        nstrips_dg_even,       "nstrips_dg_even[11]/I");
      t->Branch("ncomparators_dg_even",   ncomparators_dg_even,  "ncomparators_dg_even[11]/I");
      t->Branch("ncomparators_dg_odd",    ncomparators_dg_odd,   "ncomparators_dg_odd[11]/I");
      t->Branch("nwiresbx_dg_odd",        nwiresbx_dg_odd,       "nwiresbx_dg_odd[11]/F");
      t->Branch("nwiresbx_dg_even",       nwiresbx_dg_even,      "nwiresbx_dg_even[11]/F");
      t->Branch("ncompsbx_dg_even",       ncompsbx_dg_even,      "ncompsbx_dg_even[11]/F");
      t->Branch("ncompsbx_dg_odd",        ncompsbx_dg_odd,       "ncompsbx_dg_odd[11]/F");
      t->Branch("totalwires_dg_odd",         totalwires_dg_odd,         "totalwires_dg_odd[11]/I");
      t->Branch("totalwires_dg_even",        totalwires_dg_even,        "totalwires_dg_even[11]/I");
      t->Branch("totalstrips_dg_odd",        totalstrips_dg_odd,        "totalstrips_dg_odd[11]/I");
      t->Branch("totalstrips_dg_even",       totalstrips_dg_even,       "totalstrips_dg_even[11]/I");
      t->Branch("totalcomparators_dg_even",  totalcomparators_dg_even,  "totalcomparators_dg_even[11]/I");
      t->Branch("totalcomparators_dg_odd",   totalcomparators_dg_odd,   "totalcomparators_dg_odd[11]/I");


      t->Branch("nlayers_wg_dg_odd", nlayers_wg_dg_odd, "nlayers_wg_dg_odd[11]/I");
      t->Branch("nlayers_wg_dg_even", nlayers_wg_dg_even, "nlayers_wg_dg_even[11]/I");

      t->Branch("nlayers_st_dg_odd", nlayers_st_dg_odd, "nlayers_st_dg_odd[11]/I");
      t->Branch("nlayers_st_dg_even", nlayers_st_dg_even, "nlayers_st_dg_even[11]/I");

      t->Branch("wiregroup_odd", wiregroup_odd, "wiregroup_odd[11]/I");
      t->Branch("wiregroup_even", wiregroup_even, "wiregroup_even[11]/I");

      t->Branch("halfstrip_odd", halfstrip_odd, "halfstrip_odd[11]/I");
      t->Branch("halfstrip_even", halfstrip_even, "halfstrip_even[11]/I");
    }
  };  // namespace
}
#endif
