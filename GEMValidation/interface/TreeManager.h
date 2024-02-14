#ifndef GEMCode_GEMValidation_TreeManager_h
#define GEMCode_GEMValidation_TreeManager_h

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "GEMCode/GEMValidation/interface/Structs/GenParticleStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/SimTrackStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/GEMSimHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/GEMDigiStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/GEMStubStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/CSCSimHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/CSCDigiStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/CSCStubStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RPCSimHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RPCDigiStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RPCRecHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/L1MuStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/L1TrackStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RecoTrackStruct.h"

#include "TTree.h"
#include <vector>
#include <string>

class TreeManager
{
 public:
  TreeManager() {}

  ~TreeManager() {}

  void book();

  void init();

  void fill();

  gem::GenParticleStruct& genParticle() { return genParticleSt_; }
  gem::SimTrackStruct& simTrack() { return simTrackSt_; }
  gem::GEMSimHitStruct& gemSimHit() { return gemSimHitSt_; }
  gem::GEMDigiStruct& gemDigi() { return gemDigiSt_; }
  gem::GEMStubStruct& gemStub() { return gemStubSt_; }
  gem::CSCSimHitStruct& cscSimHit() { return cscSimHitSt_; }
  gem::CSCDigiStruct& cscDigi() { return cscDigiSt_; }
  gem::CSCStubStruct& cscStub() { return cscStubSt_; }
  gem::RPCSimHitStruct& rpcSimHit() { return rpcSimHitSt_; }
  gem::RPCDigiStruct& rpcDigi() { return rpcDigiSt_; }
  gem::RPCRecHitStruct& rpcRecHit() { return rpcRecHitSt_; }
  gem::L1MuStruct& l1mu() { return l1MuSt_; }
  gem::L1TrackStruct& l1track() { return l1TrackSt_; }
  gem::RecoTrackStruct& recoTrack() { return recoTrackSt_; }

 private:

  TTree* genParticleTree_;
  TTree* simTrackTree_;
  TTree* gemSimHitTree_;
  TTree* gemDigiTree_;
  TTree* gemStubTree_;
  TTree* cscSimHitTree_;
  TTree* cscDigiTree_;
  TTree* cscStubTree_;
  TTree* rpcSimHitTree_;
  TTree* rpcDigiTree_;
  TTree* rpcRecHitTree_;
  TTree* l1MuTree_;
  TTree* l1TrackTree_;
  TTree* recoTrackTree_;

  gem::GenParticleStruct genParticleSt_;
  gem::SimTrackStruct simTrackSt_;
  gem::GEMSimHitStruct gemSimHitSt_;
  gem::GEMDigiStruct gemDigiSt_;
  gem::GEMStubStruct gemStubSt_;
  gem::CSCSimHitStruct cscSimHitSt_;
  gem::CSCDigiStruct cscDigiSt_;
  gem::CSCStubStruct cscStubSt_;
  gem::RPCSimHitStruct rpcSimHitSt_;
  gem::RPCDigiStruct rpcDigiSt_;
  gem::RPCRecHitStruct rpcRecHitSt_;
  gem::L1MuStruct l1MuSt_;
  gem::L1TrackStruct l1TrackSt_;
  gem::RecoTrackStruct recoTrackSt_;
};

#endif
