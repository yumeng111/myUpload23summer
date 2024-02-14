#ifndef GEMCode_GEMValidation_SimTrackStruct
#define GEMCode_GEMValidation_SimTrackStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct SimTrackStruct {
    // old code
    int run, ievent, time, bx, lumi, orbit;
    float pt, eta, phi, pz, px, py, vx, vy, vz;
    int charge;
    int endcap;
    int pdgid;
    int sim_iev;

    void init() {
      sim_iev = 0;
      run = 0;
      ievent = 0;
      time = 0;
      bx = 0;
      lumi = 0;
      orbit = 0;
      pt = 0.;
      phi = 0.;
      eta = -9.;
      px = 0.;
      py = 0.;
      pz = 0.;
      // in cm
      vx = -9999.;
      vy = -9999.;
      vz = -9999.;
      charge = -9;
      endcap = -9;
      pdgid = -9999;
    };

    void book(TTree* t) {
      t->Branch("sim_iev", &sim_iev);
      t->Branch("run", &run);
      t->Branch("ievent", &ievent);
      t->Branch("time", &time);
      t->Branch("bx", &bx);
      t->Branch("lumi", &lumi);
      t->Branch("orbit", &orbit);
      t->Branch("pt", &pt);
      t->Branch("px", &pz);
      t->Branch("py", &pz);
      t->Branch("pz", &pz);
      t->Branch("eta", &eta);
      t->Branch("phi", &phi);
      t->Branch("vx", &vz);
      t->Branch("vy", &vz);
      t->Branch("vz", &vz);
      t->Branch("charge", &charge);
      t->Branch("endcap", &endcap);
      t->Branch("pdgid", &pdgid);
    }
  };
}  // namespace

#endif
