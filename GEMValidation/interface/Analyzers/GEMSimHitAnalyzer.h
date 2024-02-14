#ifndef GEMCode_GEMValidation_GEMSimHitAnalyzer_h
#define GEMCode_GEMValidation_GEMSimHitAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class GEMSimHitAnalyzer
{
public:

  // constructor
  GEMSimHitAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~GEMSimHitAnalyzer() {}

  void setMatcher(const GEMSimHitMatcher& match_sh);

  void init(const edm::Event& iEvent, const edm::EventSetup& iSetup);

 // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:
  edm::EDGetTokenT<edm::PSimHitContainer> simHitInput_;
  edm::Handle<edm::PSimHitContainer> simHitsH_;

  std::shared_ptr<GEMSimHitMatcher> match_;
  int minNHitsChamber_;

  bool verbose_;
  edm::ParameterSet simHitPSet_;
};

#endif
