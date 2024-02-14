#ifndef GEMCode_GEMValidation_GEMStubAnalyzer_h
#define GEMCode_GEMValidation_GEMStubAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class GEMStubAnalyzer
{
public:

  // constructor
  GEMStubAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~GEMStubAnalyzer() {}

  void setMatcher(const GEMDigiMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:

  edm::EDGetTokenT<GEMPadDigiCollection> gemPadToken_;
  edm::EDGetTokenT<GEMPadDigiClusterCollection> gemClusterToken_;
  edm::EDGetTokenT<GEMCoPadDigiCollection> gemCoPadToken_;

  edm::Handle<GEMPadDigiCollection> gemPadsH_;
  edm::Handle<GEMPadDigiClusterCollection> gemClustersH_;
  edm::Handle<GEMCoPadDigiCollection> gemCoPadsH_;

  int minBXPad_, maxBXPad_;
  int minBXCluster_, maxBXCluster_;
  int minBXCoPad_, maxBXCoPad_;

  bool verbosePad_;
  bool verboseCluster_;
  bool verboseCoPad_;

  bool runPad_;
  bool runCluster_;
  bool runCoPad_;

  std::pair<GEMPadDigi, GlobalPoint>
  bestPad(const GEMDetId& id,
          const GEMPadDigiContainer& digis) const;

  std::pair<GEMCoPadDigi, GlobalPoint>
  bestCoPad(const GEMDetId& id,
            const GEMCoPadDigiContainer& digis) const;

  GlobalPoint
  meanPosition(const GEMDetId& id,
               const GEMPadDigiContainer& digis) const;

  GlobalPoint
  meanPosition(const GEMDetId& id,
               const GEMCoPadDigiContainer& digis) const;

  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> gemToken_;
  const GEMGeometry* gemGeometry_;

  std::unique_ptr<GEMDigiMatcher> match_;
  int minNHitsChamber_;
};

#endif
