#ifndef GEMCode_GEMValidation_MY_TreeManager_h
#define GEMCode_GEMValidation_MY_TreeManager_h

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "GEMCode/GEMValidation/interface/Structs/FlatStruct.h"

#include "GEMCode/GEMValidation/interface/Structs/RPCSimHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RPCDigiStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RPCRecHitStruct.h"
#include "GEMCode/GEMValidation/interface/Structs/RecoTrackStruct.h"

#include "TTree.h"
#include <vector>
#include <string>

namespace my {

class TreeManager
{
 public:
  TreeManager() { book(); }

  ~TreeManager() {}

  void book();

  void init();

  void fill();

  my::EventStruct& event() { return eventSt_; }
  my::GenParticleStruct& genParticle() { return genParticleSt_; }
  my::SimTrackStruct& simTrack() { return simTrackSt_; }
  my::GEMSimHitStruct& gemSimHit() { return gemSimHitSt_; }
  my::GEMDigiStruct& gemDigi() { return gemDigiSt_; }
  my::GEMStubStruct& gemStub() { return gemStubSt_; }
  my::CSCSimHitStruct& cscSimHit() { return cscSimHitSt_; }
  my::CSCDigiStruct& cscDigi() { return cscDigiSt_; }
  my::CSCStubStruct& cscStub() { return cscStubSt_; }
  gem::RPCSimHitStruct& rpcSimHit() { return rpcSimHitSt_; }
  gem::RPCDigiStruct& rpcDigi() { return rpcDigiSt_; }
  gem::RPCRecHitStruct& rpcRecHit() { return rpcRecHitSt_; }
  my::L1MuStruct& l1mu() { return l1MuSt_; }
  my::L1TrackStruct& l1track() { return l1TrackSt_; }
  gem::RecoTrackStruct& recoTrack() { return recoTrackSt_; }

 private:

  my::EventStruct eventSt_;
  my::GenParticleStruct genParticleSt_;
  my::SimTrackStruct simTrackSt_;
  my::GEMSimHitStruct gemSimHitSt_;
  my::GEMDigiStruct gemDigiSt_;
  my::GEMStubStruct gemStubSt_;
  my::CSCSimHitStruct cscSimHitSt_;
  my::CSCDigiStruct cscDigiSt_;
  my::CSCStubStruct cscStubSt_;
  gem::RPCSimHitStruct rpcSimHitSt_;
  gem::RPCDigiStruct rpcDigiSt_;
  gem::RPCRecHitStruct rpcRecHitSt_;
  my::L1MuStruct l1MuSt_;
  my::L1TrackStruct l1TrackSt_;
  gem::RecoTrackStruct recoTrackSt_;

  // new stuff
  TTree* flatTree_;
};

};

#endif
