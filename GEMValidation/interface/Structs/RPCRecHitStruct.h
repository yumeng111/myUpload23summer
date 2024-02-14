#ifndef GEMCode_GEMValidation_RPCRecHitStruct
#define GEMCode_GEMValidation_RPCRecHitStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct RPCRecHitStruct {

    static const int nStations = 10;

    bool has_rpc_rh_even[nStations];
    bool has_rpc_rh_odd[nStations];

    int rpc_BunchX[nStations];
    int rpc_firstClusterStrip[nStations];
    int rpc_clusterSize[nStations];
    float rpc_time[nStations];
    float rpc_timeError[nStations];

    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {
        has_rpc_rh_even[i] = 0;
        has_rpc_rh_odd[i] = 0;
      }
    };

    void book(TTree* t) {
      t->Branch("has_rpc_rh_odd", has_rpc_rh_odd, "has_rpc_rh_odd[10]/O");
      t->Branch("has_rpc_rh_even", has_rpc_rh_even, "has_rpc_rh_even[10]/O");
    }
  };
}  // namespace

#endif
