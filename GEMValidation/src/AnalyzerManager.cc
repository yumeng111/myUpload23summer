#include "GEMCode/GEMValidation/interface/AnalyzerManager.h"
#include <iostream>

AnalyzerManager::AnalyzerManager(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC) {
  event_.reset(new EventAnalyzer(conf, std::move(iC)));
  gent_.reset(new GenParticleAnalyzer(conf, std::move(iC)));
  simt_.reset(new SimTrackAnalyzer(conf, std::move(iC)));
  gemsh_.reset(new GEMSimHitAnalyzer(conf, std::move(iC)));
  gemdg_.reset(new GEMDigiAnalyzer(conf, std::move(iC)));
  gemstub_.reset(new GEMStubAnalyzer(conf, std::move(iC)));
  cscsh_.reset(new CSCSimHitAnalyzer(conf, std::move(iC)));
  cscdg_.reset(new CSCDigiAnalyzer(conf, std::move(iC)));
  cscstub_.reset(new CSCStubAnalyzer(conf, std::move(iC)));
  l1mu_.reset(new L1MuAnalyzer(conf, std::move(iC)));
  runSim_ = conf.getParameter<bool>("runSim");
  useGEM_ = conf.getParameter<bool>("useGEMs");
  iev_ = 0;
  // l1track_.reset(new L1TrackAnalyzer(conf, std::move(iC)));
  // recotrack_.reset(new RecoTrackAnalyzer(conf));
}

void AnalyzerManager::init(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  // gent_->init(iEvent, iSetup);
  // gemsh_->init(iEvent, iSetup);
  // gemdg_->init(iEvent, iSetup);
  // gemstub_->init(iEvent, iSetup);
  // cscsh_->init(iEvent, iSetup);
  // cscdg_->init(iEvent, iSetup);
  cscstub_->init(iEvent, iSetup);
  // l1mu_->init(iEvent, iSetup);
  // l1track_->init(iEvent, iSetup);
}

void AnalyzerManager::setMatcher(const MatcherManager& manager) {
  gent_->setMatcher(*manager.genParticles());
  gemsh_->setMatcher(*manager.gemSimHits());
  gemdg_->setMatcher(*manager.gemDigis());
  gemstub_->setMatcher(*manager.gemDigis());
  cscsh_->setMatcher(*manager.cscSimHits());
  cscdg_->setMatcher(*manager.cscDigis());
  cscstub_->setMatcher(*manager.cscStubs());
  l1mu_->setMatcher(*manager.l1Muons());
  // l1track_->setMatcher(*manager.l1Tracks());
  // recotrack_->setMatcher(*manager.recoTracks());
}

void AnalyzerManager::analyze(const edm::Event& ev,
                              const edm::EventSetup& es,
                              const MatcherSuperManager& manager,
                              my::TreeManager& tree) {
  event_->analyze(ev, es, tree);
  if (runSim_) {
    simt_->analyze(ev, es, tree);
    gent_->analyze(ev, es, manager, tree);
    if (useGEM_) {
      gemsh_->analyze(ev, es, manager, tree);
    }
    cscsh_->analyze(ev, es, manager, tree);
  }
  if (useGEM_) {
    gemdg_->analyze(ev, es, manager, tree);
    gemstub_->analyze(ev, es, manager, tree);
  }
  cscdg_->analyze(ev, es, manager, tree);
  cscstub_->analyze(ev, es, manager, tree);
  l1mu_->analyze(ev, es, manager, tree);
  // l1track_->analyze(ev, es, manager, tree);
}


void
AnalyzerManager::analyze(TreeManager& tree, const SimTrack& t, const SimVertex& v, const edm::Event& ev) {
  gent_->setiev(iev_);
  simt_->setiev(iev_);
  gent_->analyze(tree);
  simt_->analyze(tree, t, v, ev);
  gemsh_->analyze(tree);
  gemdg_->analyze(tree);
  gemstub_->analyze(tree);
  cscsh_->analyze(tree);
  cscdg_->analyze(tree);
  cscstub_->analyze(tree);
  l1mu_->analyze(tree);
  // l1track_->analyze(tree);
}
