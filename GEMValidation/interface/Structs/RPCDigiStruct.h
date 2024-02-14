#ifndef GEMCode_GEMValidation_RPCDigiStruct
#define GEMCode_GEMValidation_RPCDigiStruct

#include "TTree.h"
#include <string>

namespace gem {

  struct RPCDigiStruct {

    static const int nStations = 10;

    bool has_rpc_dg_even[nStations];
    bool has_rpc_dg_odd[nStations];

    Int_t strip_rpcdg_odd[nStations];
    Int_t strip_rpcdg_even[nStations];

    int bx_dg_odd[nStations];
    int bx_dg_even[nStations];

    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {
        has_rpc_dg_even[i] = 0;
        has_rpc_dg_odd[i] = 0;

        strip_rpcdg_odd[i] = -9;
        strip_rpcdg_even[i] = -9;

        bx_dg_odd[i] = -9;
        bx_dg_even[i] = -9;
      }
    };

    void book(TTree* t) {
      t->Branch("has_rpc_dg_odd", has_rpc_dg_odd, "has_rpc_dg_odd[3]/O");
      t->Branch("has_rpc_dg_even", has_rpc_dg_even, "has_rpc_dg_even[3]/O");

      t->Branch("strip_rpcdg_odd", strip_rpcdg_odd, "strip_rpcdg_odd[3]/I");
      t->Branch("strip_rpcdg_even", strip_rpcdg_even, "strip_rpcdg_even[3]/I");

      t->Branch("bx_dg_odd", bx_dg_odd, "bx_dg_odd[3]/I");
      t->Branch("bx_dg_even", bx_dg_even, "bx_dg_even[3]/I");
    }
  };
}  // namespace

#endif
