#ifndef GEMCode_GEMValidation_UpgradeL1TrackMatcher_h
#define GEMCode_GEMValidation_UpgradeL1TrackMatcher_h

/**\class UpgradeL1TrackMatcher

 Description: Matching of tracks to SimTrack

 Original Author:  "Sven Dildick"
*/

#include "GEMCode/GEMValidation/interface/Matchers/L1MuMatcher.h"
#include "DataFormats/L1TrackTrigger/interface/TTTypes.h"
#include "DataFormats/L1TCorrelator/interface/TkMuon.h"
#include "DataFormats/L1TCorrelator/interface/TkMuonFwd.h"

typedef TTTrack< Ref_Phase2TrackerDigi_ >  L1TTTrackType;
typedef std::vector< L1TTTrackType > L1TTTrackCollectionType;

class L1TrackMatcher
{
 public:

  /// constructor
  L1TrackMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC);

  /// destructor
  ~L1TrackMatcher() {}

  /// initialize the event
  void init(const edm::Event& e, const edm::EventSetup& eventSetup);

  /// do the matching
  void match(const SimTrack& t, const SimVertex& v);

  std::shared_ptr<L1TTTrackType> l1Track() const { return l1Track_; }
  std::shared_ptr<l1t::TkMuon> l1TrackMuon() const { return l1TrackMuon_; }
  std::shared_ptr<L1MuMatcher> l1MuonMatcher() const { return l1MuonMatcher_; }

 private:

  void clear();

  void matchL1TrackToSimTrack(const L1TTTrackCollectionType&, const SimTrack& t);
  void matchTrackMuonToSimTrack(const l1t::TkMuonCollection&);

  edm::EDGetTokenT<L1TTTrackCollectionType> l1TrackToken_;
  edm::EDGetTokenT<l1t::TkMuonCollection> l1TrackMuonToken_;

  edm::Handle<L1TTTrackCollectionType> l1TrackHandle_;
  edm::Handle<l1t::TkMuonCollection> l1TrackMuonHandle_;

  int minBXTrack_, maxBXTrack_;
  int verboseTrack_;
  double deltaRTrack_;
  bool runTrack_;

  int minBXTrackMuon_, maxBXTrackMuon_;
  int verboseTrackMuon_;
  double deltaRTrackMuon_;
  bool runTrackMuon_;

  std::shared_ptr<L1TTTrackType> l1Track_;
  std::shared_ptr<l1t::TkMuon> l1TrackMuon_;
  std::shared_ptr<L1MuMatcher> l1MuonMatcher_;
};

#endif
