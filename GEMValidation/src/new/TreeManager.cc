#include "GEMCode/GEMValidation/interface/new/TreeManager.h"

void my::TreeManager::book() {
  edm::Service<TFileService> fs;
  flatTree_ = fs->make<TTree>("FlatTree","FlatTree");
  eventSt_.book(flatTree_);
  genParticleSt_.book(flatTree_);
  simTrackSt_.book(flatTree_);
  gemSimHitSt_.book(flatTree_);
  gemDigiSt_.book(flatTree_);
  gemStubSt_.book(flatTree_);
  cscSimHitSt_.book(flatTree_);
  cscDigiSt_.book(flatTree_);
  cscStubSt_.book(flatTree_);
  l1MuSt_.book(flatTree_);
  l1TrackSt_.book(flatTree_);
  recoTrackSt_.book(flatTree_);
}

/// initialize
void my::TreeManager::init() {
  eventSt_.init();
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

void my::TreeManager::fill() {
  flatTree_->Fill();
}
