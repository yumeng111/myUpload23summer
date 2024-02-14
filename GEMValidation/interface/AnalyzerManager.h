#ifndef GEMCode_GEMValidation_AnalyzerManager_h
#define GEMCode_GEMValidation_AnalyzerManager_h

#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/new/TreeManager.h"
#include "GEMCode/GEMValidation/interface/Analyzers/EventAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/GenParticleAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/SimTrackAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/GEMSimHitAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/GEMDigiAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/GEMStubAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/CSCSimHitAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/CSCDigiAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/CSCStubAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/RPCSimHitAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/RPCDigiAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/RPCRecHitAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/L1MuAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/L1TrackAnalyzer.h"
#include "GEMCode/GEMValidation/interface/Analyzers/RecoTrackAnalyzer.h"

class AnalyzerManager
{
 public:
  AnalyzerManager(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  ~AnalyzerManager() {}

  void init(const edm::Event& ev, const edm::EventSetup& es);

  /// initialize
  void setMatcher(const MatcherManager&);
  ///
  void setiev (int i) {iev_ = i;} 

  /// do the matching
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree, const SimTrack& t, const SimVertex& v, const edm::Event& ev);

 private:

  // analyzers
  std::unique_ptr<EventAnalyzer> event_;
  std::unique_ptr<GenParticleAnalyzer> gent_;
  std::unique_ptr<SimTrackAnalyzer> simt_;
  std::unique_ptr<GEMSimHitAnalyzer> gemsh_;
  std::unique_ptr<GEMDigiAnalyzer> gemdg_;
  std::unique_ptr<GEMStubAnalyzer> gemstub_;
  std::unique_ptr<CSCSimHitAnalyzer> cscsh_;
  std::unique_ptr<CSCDigiAnalyzer> cscdg_;
  std::unique_ptr<CSCStubAnalyzer> cscstub_;
  std::unique_ptr<RPCSimHitAnalyzer> rpcsh_;
  std::unique_ptr<RPCDigiAnalyzer> rpcdg_;
  std::unique_ptr<RPCRecHitAnalyzer> rpcrh_;
  std::unique_ptr<L1MuAnalyzer> l1mu_;
  std::unique_ptr<L1TrackAnalyzer> l1track_;
  std::unique_ptr<RecoTrackAnalyzer> recotrack_;
  int iev_;
  bool runSim_;
  bool useGEM_;
};

#endif
