#ifndef GEMCode_GEMValidation_L1MuAnalyzer_h
#define GEMCode_GEMValidation_L1MuAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class L1MuAnalyzer
{
public:

  // constructor
  L1MuAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~L1MuAnalyzer() {}

  void setMatcher(const L1MuMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:

  edm::EDGetTokenT<l1t::EMTFTrackCollection> emtfTrackToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> bmtfCandToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> omtfCandToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> emtfCandToken_;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> emtfShowerToken_;
  edm::EDGetTokenT<l1t::MuonBxCollection> muonToken_;
  edm::EDGetTokenT<l1t::MuonShowerBxCollection> showerToken_;

  edm::Handle<l1t::EMTFTrackCollection> emtfTrackHandle_;
  edm::Handle<l1t::RegionalMuonCandBxCollection> bmtfCandHandle_;
  edm::Handle<l1t::RegionalMuonCandBxCollection> omtfCandHandle_;
  edm::Handle<l1t::RegionalMuonCandBxCollection> emtfCandHandle_;
  edm::Handle<l1t::RegionalMuonShowerBxCollection> emtfShowerHandle_;
  edm::Handle<l1t::MuonBxCollection> muonHandle_;
  edm::Handle<l1t::MuonShowerBxCollection> showerHandle_;

  std::shared_ptr<CSCStubMatcher> cscStubMatcher_;

  int minBXEMTFTrack_, maxBXEMTFTrack_;
  int verboseEMTFTrack_;
  bool runEMTFTrack_;

  int minBXEMTFCand_, maxBXEMTFCand_;
  int verboseEMTFCand_;
  bool runEMTFCand_;

  int minBXEMTFShower_, maxBXEMTFShower_;
  int verboseEMTFShower_;
  bool runEMTFShower_;

  int minBXBMTFCand_, maxBXBMTFCand_;
  int verboseBMTFCand_;
  bool runBMTFCand_;

  int minBXOMTFCand_, maxBXOMTFCand_;
  int verboseOMTFCand_;
  bool runOMTFCand_;

  int minBXGMT_, maxBXGMT_;
  int verboseGMT_;
  bool runGMT_;

  int minBXShower_, maxBXShower_;
  int verboseShower_;
  bool runShower_;

  //showers for each chamber
  std::vector<unsigned> anodeThresholds_;
  unsigned anodeMinLayers_;
  std::vector<unsigned> cathodeThresholds_;
  unsigned cathodeMinLayers_;

  std::unique_ptr<L1MuMatcher> match_;
};

#endif
