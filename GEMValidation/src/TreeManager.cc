#include "GEMCode/GEMValidation/interface/TreeManager.h"

void TreeManager::book() {
  edm::Service<TFileService> fs;
  genParticleTree_ = fs->make<TTree>("genParticle","genParticle");
  simTrackTree_ = fs->make<TTree>("simTrack","simTrack");
  gemSimHitTree_ = fs->make<TTree>("gemSimHit","gemSimHit");
  gemDigiTree_ = fs->make<TTree>("gemDigi","gemDigi");
  gemStubTree_ = fs->make<TTree>("gemStub","gemStub");
  cscSimHitTree_ = fs->make<TTree>("cscSimHit","cscSimHit");
  cscDigiTree_ = fs->make<TTree>("cscDigi","cscDigi");
  cscStubTree_ = fs->make<TTree>("cscStub","cscStub");
  l1MuTree_ = fs->make<TTree>("l1Mu","l1Mu");
  l1TrackTree_ = fs->make<TTree>("l1Track","l1Track");
  recoTrackTree_ = fs->make<TTree>("recoTrack","recoTrack");

  genParticleSt_.book(genParticleTree_);
  simTrackSt_.book(simTrackTree_);
  gemSimHitSt_.book(gemSimHitTree_);
  gemDigiSt_.book(gemDigiTree_);
  gemStubSt_.book(gemStubTree_);
  cscSimHitSt_.book(cscSimHitTree_);
  cscDigiSt_.book(cscDigiTree_);
  cscStubSt_.book(cscStubTree_);
  l1MuSt_.book(l1MuTree_);
  l1TrackSt_.book(l1TrackTree_);
  recoTrackSt_.book(recoTrackTree_);
}

/// initialize
void TreeManager::init() {
  genParticleSt_.init();
  simTrackSt_.init();
  gemSimHitSt_.init();
  gemDigiSt_.init();
  gemStubSt_.init();
  cscSimHitSt_.init();
  cscDigiSt_.init();
  cscStubSt_.init();
  l1MuSt_.init();
  l1TrackSt_.init();
  recoTrackSt_.init();
}

void TreeManager::fill() {
  genParticleTree_->Fill();
  simTrackTree_->Fill();
  gemSimHitTree_->Fill();
  gemDigiTree_->Fill();
  gemStubTree_->Fill();
  cscSimHitTree_->Fill();
  cscDigiTree_->Fill();
  cscStubTree_->Fill();
  l1MuTree_->Fill();
  l1TrackTree_->Fill();
  recoTrackTree_->Fill();
}
