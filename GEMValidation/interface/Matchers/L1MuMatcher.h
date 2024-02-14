#ifndef GEMCode_GEMValidation_L1MuMatcher_h
#define GEMCode_GEMValidation_L1MuMatcher_h

/**\class L1MuMatcher

 Description: Matching of tracks to SimTrack

 Original Author:  "Sven Dildick"
*/

#include "Validation/MuonCSCDigis/interface/CSCStubMatcher.h"
#include "GEMCode/GEMValidation/interface/EMTFTrack.h"

class L1MuMatcher
{
 public:
  /// constructor
  L1MuMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC);

  /// destructor
  ~L1MuMatcher() {}

  /// initialize the event
  void init(const edm::Event& e, const edm::EventSetup& eventSetup);

  /// do the matching
  void match(const SimTrack& t, const SimVertex& v);

  // return best matching tracks
  std::shared_ptr<gem::EMTFTrack> emtfTrack() const { return emtfTrack_; }
  std::shared_ptr<gem::EMTFCand> emtfCand() const { return emtfCand_; }
  std::shared_ptr<gem::EMTFCand> muon() const { return muon_; }
  std::shared_ptr<l1t::RegionalMuonShower> emtfShower() const { return emtfShower_; }

  std::shared_ptr<CSCStubMatcher> cscStubMatcher() { return cscStubMatcher_; }
  void setCSCStubMatcher(std::shared_ptr<CSCStubMatcher> s) {cscStubMatcher_ = s;}

 private:

  void clear();

  void matchEmtfTrackToSimTrack(const SimTrack& t, const l1t::EMTFTrackCollection&);
  void matchRegionalMuonCandToSimTrack(const l1t::RegionalMuonCandBxCollection&);
  void matchGMTToSimTrack(const l1t::MuonBxCollection&);
  void matchEmtfShowerToSimTrack(const SimTrack& t, const l1t::RegionalMuonShowerBxCollection& emtfShowers);

  edm::EDGetTokenT<l1t::EMTFTrackCollection> emtfTrackToken_;
  edm::EDGetTokenT<l1t::RegionalMuonCandBxCollection> emtfCandToken_;
  edm::EDGetTokenT<l1t::MuonBxCollection> muonToken_;
  edm::EDGetTokenT<l1t::RegionalMuonShowerBxCollection> emtfShowerToken_;

  edm::Handle<l1t::EMTFTrackCollection> emtfTrackHandle_;
  edm::Handle<l1t::RegionalMuonCandBxCollection> emtfCandHandle_;
  edm::Handle<l1t::MuonBxCollection> muonHandle_;
  edm::Handle<l1t::RegionalMuonShowerBxCollection> emtfShowerHandle_;

  std::shared_ptr<CSCStubMatcher> cscStubMatcher_;

  int minBXEMTFTrack_, maxBXEMTFTrack_;
  int verboseEMTFTrack_;
  double deltaREMTFTrack_;
  bool runEMTFTrack_;

  int minBXRegMuCand_, maxBXRegMuCand_;
  int verboseRegMuCand_;
  double deltaRRegMuCand_;
  bool runRegMuCand_;

  int minBXGMT_, maxBXGMT_;
  int verboseGMT_;
  double deltaRGMT_;
  bool runGMT_;

  int minBXEMTFShower_, maxBXEMTFShower_;
  int verboseEMTFShower_;
  bool runEMTFShower_;

  int minBXShower_, maxBXShower_;
  int verboseShower_;
  bool runShower_;

  std::shared_ptr<gem::EMTFTrack> emtfTrack_;
  std::shared_ptr<gem::EMTFCand> emtfCand_;
  std::shared_ptr<gem::EMTFCand> muon_;

  std::shared_ptr<l1t::RegionalMuonShower> emtfShower_;
};

#endif
