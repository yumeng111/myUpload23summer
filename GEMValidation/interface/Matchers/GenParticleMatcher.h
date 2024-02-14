#ifndef GEMCode_GEMValidation_GenParticleMatcher_h
#define GEMCode_GEMValidation_GenParticleMatcher_h

/**\class GenParticleMatcher

 Description: Matching of displaced GEN muon to SimTrack

 Original Author:  "Sven Dildick"
*/

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/normalizedPhi.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

#include <vector>

class GenParticleMatcher
{
 public:

  // constructor
  GenParticleMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector && iC);

  // destructor
  ~GenParticleMatcher() {}

  // initialize the event
  void init(const edm::Event& e, const edm::EventSetup& eventSetup);

  // do the matching
  void match(const SimTrack& t, const SimVertex& v);

  std::shared_ptr<reco::GenParticle> getMatch() const { return match_; }
  std::shared_ptr<reco::GenParticle> getParent() const { return match_; }

 private:

  int verbose_;
  bool run_;

  edm::Handle<reco::GenParticleCollection> genParticlesHandle_;
  edm::EDGetTokenT<reco::GenParticleCollection> inputToken_;

  std::shared_ptr<reco::GenParticle> match_;
  std::shared_ptr<reco::GenParticle> parent_;
};

#endif
