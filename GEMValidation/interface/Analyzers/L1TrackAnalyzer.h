#ifndef GEMCode_GEMValidation_L1TrackAnalyzer_h
#define GEMCode_GEMValidation_L1TrackAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class L1TrackAnalyzer
{
public:

  // constructor
  L1TrackAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~L1TrackAnalyzer() {}

  void setMatcher(const L1TrackMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:

  edm::EDGetTokenT<L1TTTrackCollectionType> l1TrackToken_;
  edm::EDGetTokenT<l1t::TkMuonCollection> l1TrackMuonToken_;

  edm::Handle<L1TTTrackCollectionType> l1TrackHandle_;
  edm::Handle<l1t::TkMuonCollection> l1TrackMuonHandle_;

  int minBXTrack_, maxBXTrack_;
  int verboseTrack_;
  bool runTrack_;

  int minBXTrackMuon_, maxBXTrackMuon_;
  int verboseTrackMuon_;
  bool runTrackMuon_;

  std::unique_ptr<L1TrackMatcher> match_;
};

#endif
