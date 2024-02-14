#include "GEMCode/GEMValidation/interface/Matchers/RecoTrackMatcher.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TLorentzVector.h"
#include <map>

RecoTrackMatcher::RecoTrackMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC)
{
  const auto& recoTrackExtra = iPS.getParameter<edm::ParameterSet>("recoTrackExtra");
  minBXRecoTrackExtra_ = recoTrackExtra.getParameter<int>("minBX");
  maxBXRecoTrackExtra_ = recoTrackExtra.getParameter<int>("minBX");
  verboseRecoTrackExtra_ = recoTrackExtra.getParameter<int>("verbose");
  runRecoTrackExtra_ = recoTrackExtra.getParameter<bool>("run");

  const auto& recoTrack = iPS.getParameter<edm::ParameterSet>("recoTrack");
  minBXRecoTrack_ = recoTrack.getParameter<int>("minBX");
  maxBXRecoTrack_ = recoTrack.getParameter<int>("minBX");
  verboseRecoTrack_ = recoTrack.getParameter<int>("verbose");
  runRecoTrack_ = recoTrack.getParameter<bool>("run");

  const auto& recoChargedCandidate = iPS.getParameter<edm::ParameterSet>("recoChargedCandidate");
  minBXRecoChargedCandidate_ = recoChargedCandidate.getParameter<int>("minBX");
  maxBXRecoChargedCandidate_ = recoChargedCandidate.getParameter<int>("minBX");
  verboseRecoChargedCandidate_ = recoChargedCandidate.getParameter<int>("verbose");
  runRecoChargedCandidate_ = recoChargedCandidate.getParameter<bool>("run");

  recoTrackExtraToken_ = iC.consumes<reco::TrackExtraCollection>(recoTrackExtra.getParameter<edm::InputTag>("inputTag"));
  recoTrackToken_ = iC.consumes<reco::TrackCollection>(recoTrack.getParameter<edm::InputTag>("inputTag"));
  recoChargedCandidateToken_ = iC.consumes<reco::RecoChargedCandidateCollection>(recoChargedCandidate.getParameter<edm::InputTag>("inputTag"));
}

void
RecoTrackMatcher::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  gem_rechit_matcher_->init(iEvent, iSetup);
  dt_rechit_matcher_->init(iEvent, iSetup);
  rpc_rechit_matcher_->init(iEvent, iSetup);
  csc_rechit_matcher_->init(iEvent, iSetup);

  iEvent.getByToken(recoTrackExtraToken_, recoTrackExtrasHandle_);
  iEvent.getByToken(recoTrackToken_, recoTracksHandle_);
  iEvent.getByToken(recoChargedCandidateToken_, recoChargedCandidatesHandle_);
}

void
RecoTrackMatcher::match(const SimTrack& t, const SimVertex& v)
{
  clear();

  gem_rechit_matcher_->match(t, v);
  dt_rechit_matcher_->match(t, v);
  rpc_rechit_matcher_->match(t, v);
  csc_rechit_matcher_->match(t, v);

  matchRecoTrackExtraToSimTrack(*recoTrackExtrasHandle_.product());
  matchRecoTrackToSimTrack(*recoTracksHandle_.product());
  matchRecoChargedCandidateToSimTrack(*recoChargedCandidatesHandle_.product());
}

void RecoTrackMatcher::clear()
{
  matchedRecoTrackExtra_ = nullptr;
  matchedRecoTrack_ = nullptr;
  matchedRecoChargedCandidate_ = nullptr;
}

void
RecoTrackMatcher::matchRecoTrackExtraToSimTrack(const reco::TrackExtraCollection& tracks)
{
  if (verboseRecoTrackExtra_) std::cout << "Number of RecoTrackExtras: " <<tracks.size() << std::endl;

  int maxMatchingSegments(0);
  for(const auto& track: tracks) {

    if (verboseRecoTrackExtra_) {
      std::cout << "RecoTrackExtra " << std::endl
                << "\tpT_inner: "<<track.innerMomentum().Rho()
                << ", eta_inner: "<<track.innerPosition().eta()
                << ", phi_inner: "<<track.innerPosition().phi()
                << "\tpT_outer: "<<track.outerMomentum().Rho()
                << ", eta_outer: "<<track.outerPosition().eta()
                << ", phi_outer: "<<track.outerPosition().phi()
                <<std::endl;
      std::cout << "\tRechits/Segments: " << track.recHitsSize()<< std::endl;
    }

    int matchingCSCSegments(0);
    int matchingRPCSegments(0);
    int matchingGEMSegments(0);
    int matchingDTSegments(0);
    int matchingSegments(0);
    int nValidSegments(0);

    for(auto rh = track.recHitsBegin(); rh != track.recHitsEnd(); rh++) {
      if (!(**rh).isValid()) continue;
      ++nValidSegments;
      const auto& id((**rh).rawId());
      if (gem::is_dt(id)) {
        const DTRecSegment4D *seg = dynamic_cast<const DTRecSegment4D*>(*rh);
        if (verboseRecoTrackExtra_) {
          std::cout << "\t\tDT  :: id :: " << DTChamberId(id) << std::endl;
          std::cout << "\t\t    :: segment :: " << *seg << std::endl;
        }
        if (dt_rechit_matcher_->isDTRecSegment4DMatched(*seg)) {
          if (verboseRecoTrackExtra_) std::cout << "\t\t    :: MATCHED!" << std::endl;
          ++matchingDTSegments;
          ++matchingSegments;
        }
      }
      if (gem::is_rpc(id)) {
        const RPCRecHit* rpcrh = dynamic_cast<const RPCRecHit*>(*rh);
        if (verboseRecoTrackExtra_) {
          std::cout << "\t\tRPC :: id :: " << RPCDetId(id) << std::endl;
          std::cout << "\t\t    :: rechit :: " << *rpcrh << std::endl;
        }
        if (rpc_rechit_matcher_->isRPCRecHitMatched(*rpcrh)) {
          if (verboseRecoTrackExtra_) std::cout << "\t\t    :: MATCHED!" << std::endl;
          ++matchingRPCSegments;
          // ++matchingSegments;
        }
      }
      if (gem::is_gem(id)) {
        const GEMRecHit* gemrh = dynamic_cast<const GEMRecHit*>(*rh);
        if (verboseRecoTrackExtra_) {
          std::cout << "\t\tGEM :: id :: " << GEMDetId(id) << std::endl;
          std::cout << "\t\t    :: rechit :: " << *gemrh << std::endl;
        }
        if (gem_rechit_matcher_->isGEMRecHitMatched(*gemrh)) {
          if (verboseRecoTrackExtra_) std::cout << "\t\t    :: MATCHED!" << std::endl;
          ++matchingGEMSegments;
          // ++matchingSegments;
        }
      }
      if (gem::is_csc(id)) {
        const CSCSegment *seg = dynamic_cast<const CSCSegment*>(*rh);
        if (verboseRecoTrackExtra_) {
          std::cout << "\t\tCSC :: id :: " << CSCDetId(id) << std::endl;
          std::cout << "\t\t    :: segment :: " << *seg << std::endl;
        }
        if (csc_rechit_matcher_->isCSCSegmentMatched(*seg)) {
          if (verboseRecoTrackExtra_) std::cout << "\t\t    :: MATCHED!" << std::endl;
          ++matchingCSCSegments;
          ++matchingSegments;
        }
      }
    }
    if (verboseRecoTrackExtra_) {
      std::cout << "\tValid Segments:    " << nValidSegments << std::endl;
      std::cout << "\tMatching Segments: " << matchingSegments << std::endl;
      std::cout << "\t              RPC: " << matchingRPCSegments << std::endl;
      std::cout << "\t              CSC: " << matchingCSCSegments << std::endl;
      std::cout << "\t              GEM: " << matchingGEMSegments << std::endl;
      std::cout << "\t               DT: " << matchingDTSegments << std::endl;
    }
    if (matchingSegments >= 2 and matchingSegments > maxMatchingSegments) {
      maxMatchingSegments = matchingSegments;
      // no copy constructor :-/
      matchedRecoTrackExtra_.reset(new reco::TrackExtra(track.outerPosition(),
                                                        track.outerMomentum(),
                                                        track.outerOk(),
                                                        track.innerPosition(),
                                                        track.innerMomentum(),
                                                        track.innerOk(),
                                                        track.outerStateCovariance(),
                                                        track.outerDetId(),
                                                        track.innerStateCovariance(),
                                                        track.innerDetId(),
                                                        track.seedDirection(),
                                                        track.seedRef()));

      if (verboseRecoTrackExtra_) {
        std::cout << "\tRecoTrackExtra was matched! " << std::endl;
      }
    }
  }
}


void
RecoTrackMatcher::matchRecoTrackToSimTrack(const reco::TrackCollection& tracks)
{
  if (verboseRecoTrack_) std::cout << "Number of RecoTracks: " <<tracks.size() << std::endl;
  for(const auto& track: tracks) {
    if (verboseRecoTrack_) {
      std::cout<< "RecoTrack pT: " <<track.outerPt()
               <<", eta: "<<track.outerEta()
               <<", phi: "<<track.outerPhi()
               << std::endl;
    }
    // check if the associated RecoTrackExtra was matched!
    if (areRecoTrackSame(*(track.extra()), *matchedRecoTrackExtra_)) {
      if (verboseRecoTrack_) std::cout << "\tRecoTrack was matched!" << std::endl;
      // no copy constructor :-/
      matchedRecoTrack_.reset(new reco::Track(track.chi2(),
                                              track.ndof(),
                                              track.referencePoint(),
                                              track.momentum(),
                                              track.charge(),
                                              track.covariance(),
                                              track.algo(),
                                              reco::TrackBase::TrackQuality::undefQuality, // quality undefined!
                                              track.t0(),
                                              track.beta(),
                                              track.covt0t0(),
                                              track.covBetaBeta()));
      matchedRecoTrack_->setExtra(track.extra());
      return;
    }
  }
}


void
RecoTrackMatcher::matchRecoChargedCandidateToSimTrack(const reco::RecoChargedCandidateCollection& candidates)
{
  if (verboseRecoTrack_) std::cout << "Number of RecoChargedCandidates: " <<candidates.size() << std::endl;
  for(const auto& candidate: candidates) {
    if (verboseRecoChargedCandidate_) {
      std::cout<< "RecoCandidate - pT: "<<candidate.pt()
               <<", eta: "<<candidate.eta()
               <<", phi: "<<candidate.phi()
               << std::endl;
    }
    // check if the associated RecoTrack was matched!
    if (areRecoTrackSame(*(candidate.track()), *matchedRecoTrack_)) {
     	if (verboseRecoTrack_) std::cout << "\tRecoChargedCandidate was matched!" << std::endl;
      // no copy constructor :-/
      matchedRecoChargedCandidate_.reset(new reco::RecoChargedCandidate(candidate.charge(),
                                                                         candidate.p4(),
                                                                         candidate.vertex(),
                                                                         candidate.pdgId(),
                                                                         candidate.status()));
      matchedRecoChargedCandidate_->setTrack(candidate.track());
      return;
    }
  }
}


template<typename T>
bool
RecoTrackMatcher::areRecoTrackSame(const T& l, const T& r) const
{
  return (l.outerPosition()==r.outerPosition() and
          l.outerMomentum()==r.outerMomentum() and
          l.outerDetId()==r.outerDetId() and
          l.outerDetId()==r.outerDetId() and
          l.innerPosition()==r.innerPosition() and
          l.innerMomentum()==r.innerMomentum() and
          l.innerOk()==r.innerOk() and
          l.outerDetId()==r.outerDetId());
}
