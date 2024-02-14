#ifndef GEMCode_GEMValidation_RPCSimHitStruct
#define GEMCode_GEMValidation_RPCSimHitStruct

#include "TTree.h"

namespace gem {

  struct RPCSimHitStruct {

    static const int nStations = 10;

    // bools
    bool has_rpc_sh_even[nStations];
    bool has_rpc_sh_odd[nStations];

    // ints
    int chamber_sh_odd[nStations];
    int chamber_sh_even[nStations];

    // floats
    float perp_rpcsh_even[nStations];
    float perp_rpcsh_odd[nStations];

    float strip_rpcsh_odd[nStations];
    float strip_rpcsh_even[nStations];

    float eta_rpcsh_odd[nStations];
    float eta_rpcsh_even[nStations];

    float phi_rpcsh_odd[nStations];
    float phi_rpcsh_even[nStations];


    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {

        has_rpc_sh_even[i] = false;
        has_rpc_sh_odd[i] = false;

        chamber_sh_odd[i] = -1;
        chamber_sh_even[i] = -1;

        perp_rpcsh_odd[i] = -0.0;
        perp_rpcsh_even[i] = -0.0;

        strip_rpcsh_odd[i] = -9.;
        strip_rpcsh_even[i] = -9.;

        eta_rpcsh_odd[i] = -9.;
        eta_rpcsh_even[i] = -9.;

        phi_rpcsh_odd[i] = -9.;
        phi_rpcsh_even[i] = -9.;
      }
    };

    void book(TTree* t) {

      t->Branch("has_rpc_sh_odd", has_rpc_sh_odd, "has_rpc_sh_odd[10]/O");
      t->Branch("has_rpc_sh_even", has_rpc_sh_even, "has_rpc_sh_even[10]/O");

      t->Branch("chamber_sh_odd", chamber_sh_odd, "chamber_sh_odd[10]/I");
      t->Branch("chamber_sh_even", chamber_sh_even, "chamber_sh_even[10]/I");

      t->Branch("perp_rpcsh_odd", perp_rpcsh_odd, "perp_rpcsh_odd[10]/F");
      t->Branch("perp_rpcsh_even", perp_rpcsh_even, "perp_rpcsh_even[10]/F");

      t->Branch("strip_rpcsh_odd", strip_rpcsh_odd, "strip_rpcsh_odd[10]/F");
      t->Branch("strip_rpcsh_even", strip_rpcsh_even, "strip_rpcsh_even[10]/F");

      t->Branch("eta_rpcsh_odd", eta_rpcsh_odd, "eta_rpcsh_odd[10]/F");
      t->Branch("eta_rpcsh_even", eta_rpcsh_even, "eta_rpcsh_even[10]/F");

      t->Branch("phi_rpcsh_odd", phi_rpcsh_odd, "phi_rpcsh_odd[10]/F");
      t->Branch("phi_rpcsh_even", phi_rpcsh_even, "phi_rpcsh_even[10]/F");
    }
  };
}  // namespace

#endif
