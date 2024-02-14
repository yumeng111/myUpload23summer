#ifndef GEMCode_GEMValidation_GEMDigiAnalyzer_h
#define GEMCode_GEMValidation_GEMDigiAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class GEMDigiAnalyzer
{
public:

  // constructor
  GEMDigiAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~GEMDigiAnalyzer() {}

  void setMatcher(const GEMDigiMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:

  edm::EDGetTokenT<GEMDigiCollection> gemDigiToken_;
  edm::Handle<GEMDigiCollection> gemDigisH_;

  int median(const GEMDigiContainer& digis) const;
  GlobalPoint meanPosition(const GEMDetId& id, const GEMDigiContainer& digis) const;

  std::unique_ptr<GEMDigiMatcher> match_;
  int minNHitsChamber_;

  //edm::ESHandle<GEMGeometry> gem_geom_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> gemToken_;
  const GEMGeometry* gemGeometry_;

  int minBXDigi_, maxBXDigi_;
  bool verboseDigi_;
  bool runDigi_;
};

#endif
