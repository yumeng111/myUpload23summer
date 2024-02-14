#ifndef GEMCode_GEMValidation_MatcherSuperManager_h
#define GEMCode_GEMValidation_MatcherSuperManager_h

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "GEMCode/GEMValidation/interface/MatcherManager.h"
#include "GEMCode/GEMValidation/interface/new/TreeManager.h"

// shower objects
#include "DataFormats/CSCDigi/interface/CSCShowerDigiCollection.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"
#include "DataFormats/L1Trigger/interface/MuonShower.h"

// container class with managers

class MatcherSuperManager
{
public:
  MatcherSuperManager(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC);

  ~MatcherSuperManager() {}

  // initialize
  void init();

  /// do the matching
  void match(const edm::Event& e, const edm::EventSetup&);

  bool isSimTrackGood(const SimTrack& t);

  // accessors
  std::vector<std::shared_ptr<MatcherManager> > matchers() const { return matchers_; }
  std::shared_ptr<MatcherManager> matcher(unsigned index) const {return matchers_.at(index); }

private:
  // one manager per Sim-level particle we're fully analyzing
  std::vector<std::shared_ptr<MatcherManager> > matchers_;

  edm::EDGetTokenT<edm::SimVertexContainer> simVertexInput_;
  edm::EDGetTokenT<edm::SimTrackContainer> simTrackInput_;

  int simTrackVerbose_;
  double simTrackMinPt_;
  double simTrackMinEta_;
  double simTrackMaxEta_;
  int verbose_;
  bool runSim_;
  std::vector<int> pdgIds_;
};

#endif
