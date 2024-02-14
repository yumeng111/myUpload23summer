#ifndef GEMCode_GEMValidation_RecoTrackMatcher_h
#define GEMCode_GEMValidation_RecoTrackMatcher_h

/**\class RecoTrackMatcher

 Description: Matching of tracks to SimTrack

 Original Author:  "Sven Dildick"
*/

#include "GEMCode/GEMValidation/interface/Matchers/CSCRecHitMatcher.h"
#include "GEMCode/GEMValidation/interface/Matchers/DTRecHitMatcher.h"
#include "GEMCode/GEMValidation/interface/Matchers/RPCRecHitMatcher.h"
#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "Validation/MuonGEMRecHits/interface/GEMRecHitMatcher.h"

#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "DataFormats/TrackReco/interface/TrackExtraFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

class RecoTrackMatcher
{
 public:
  /// constructor
  RecoTrackMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC);

  /// destructor
  ~RecoTrackMatcher() {}

  /// initialize the event
  void init(const edm::Event& e, const edm::EventSetup& eventSetup);

  /// do the matching
  void match(const SimTrack& t, const SimVertex& v);

  std::shared_ptr<reco::TrackExtra> getMatchedRecoTrackExtra() const {return matchedRecoTrackExtra_;}
  std::shared_ptr<reco::Track> getMatchedRecoTrack() const {return matchedRecoTrack_;}
  std::shared_ptr<reco::RecoChargedCandidate> getMatchedRecoChargedCandidate() const {return matchedRecoChargedCandidate_;}

  std::shared_ptr<GEMRecHitMatcher> gemRecHits() const { return gem_rechit_matcher_; }
  std::shared_ptr<DTRecHitMatcher> dtRecHits() const { return dt_rechit_matcher_; }
  std::shared_ptr<RPCRecHitMatcher> rpcRecHits() const { return rpc_rechit_matcher_; }
  std::shared_ptr<CSCRecHitMatcher> cscRecHits() const { return csc_rechit_matcher_; }

 private:

  void clear();

  void matchRecoTrackExtraToSimTrack(const reco::TrackExtraCollection&);
  void matchRecoTrackToSimTrack(const reco::TrackCollection&);
  void matchRecoChargedCandidateToSimTrack(const reco::RecoChargedCandidateCollection&);

  template<typename T>
  bool areRecoTrackSame(const T&, const T&) const;

  edm::Handle<reco::TrackExtraCollection> recoTrackExtrasHandle_;
  edm::Handle<reco::TrackCollection> recoTracksHandle_;
  edm::Handle<reco::RecoChargedCandidateCollection> recoChargedCandidatesHandle_;

  edm::EDGetTokenT<reco::TrackExtraCollection> recoTrackExtraToken_;
  edm::EDGetTokenT<reco::TrackCollection> recoTrackToken_;
  edm::EDGetTokenT<reco::RecoChargedCandidateCollection> recoChargedCandidateToken_;

  std::shared_ptr<GEMRecHitMatcher> gem_rechit_matcher_;
  std::shared_ptr<DTRecHitMatcher> dt_rechit_matcher_;
  std::shared_ptr<RPCRecHitMatcher> rpc_rechit_matcher_;
  std::shared_ptr<CSCRecHitMatcher> csc_rechit_matcher_;

  int minBXRecoTrackExtra_, maxBXRecoTrackExtra_;
  int verboseRecoTrackExtra_;
  bool runRecoTrackExtra_;

  int minBXRecoTrack_, maxBXRecoTrack_;
  int verboseRecoTrack_;
  bool runRecoTrack_;

  int minBXRecoChargedCandidate_, maxBXRecoChargedCandidate_;
  int verboseRecoChargedCandidate_;
  bool runRecoChargedCandidate_;

  std::shared_ptr<reco::TrackExtra> matchedRecoTrackExtra_;
  std::shared_ptr<reco::Track> matchedRecoTrack_;
  std::shared_ptr<reco::RecoChargedCandidate> matchedRecoChargedCandidate_;
};

#endif
