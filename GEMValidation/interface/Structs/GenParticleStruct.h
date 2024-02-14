#ifndef GEMCode_GEMValidation_GenParticleStruct
#define GEMCode_GEMValidation_GenParticleStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct GenParticleStruct {
    // old code
    float pt, eta, phi, pz, dxy;
    int charge;
    int endcap;
    int pdgid;
    int genp_iev;

    void init() {
      genp_iev = 0;
      pt = 0.;
      phi = 0.;
      eta = -9.;
      dxy = -999.;
      charge = -9;
      endcap = -9;
      pdgid = -9999;
    };

    void book(TTree* t) {
      t->Branch("genp_iev", &genp_iev);
      t->Branch("pt", &pt);
      t->Branch("pz", &pz);
      t->Branch("eta", &eta);
      t->Branch("dxy", &dxy);
      t->Branch("phi", &phi);
      t->Branch("charge", &charge);
      t->Branch("endcap", &endcap);
      t->Branch("pdgid", &pdgid);
    }
  };
}  // namespace

#endif
