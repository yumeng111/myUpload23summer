#include "GEMCode/GEMValidation/interface/Matchers/GenParticleMatcher.h"

GenParticleMatcher::GenParticleMatcher(edm::ParameterSet const& iPS,
                                       edm::ConsumesCollector && iC)
{
  const auto& gen = iPS.getParameter<edm::ParameterSet>("genParticle");
  verbose_ = gen.getParameter<int>("verbose");
  run_ = gen.getParameter<bool>("run");

  inputToken_ = iC.consumes<reco::GenParticleCollection>(gen.getParameter<edm::InputTag>("inputTag"));
}

void
GenParticleMatcher::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  iEvent.getByToken(inputToken_, genParticlesHandle_);
}

void
GenParticleMatcher::match(const SimTrack& t, const SimVertex& v)
{
  // fetch the collection
  const reco::GenParticleCollection& genParticles = *genParticlesHandle_.product();

  float minDeltaR = 0.6;
  for(auto iGenParticle = genParticles.begin();  iGenParticle != genParticles.end();  ++iGenParticle) {

    // require stable particle
    if (iGenParticle->status() != 1) continue;

    // require that PDGId is the same
    if (iGenParticle->pdgId() != t.type()) continue;

    // require that charge is the same
    if (iGenParticle->charge() * t.charge() < 0) continue;

    const double deltaR(reco::deltaR(iGenParticle->eta(), iGenParticle->phi(), t.momentum().eta(), t.momentum().phi()));

    // eliminate candidates too far away
    if (deltaR > 0.3) continue;

    // get the closest matching one
    if (deltaR < minDeltaR) {
      minDeltaR = deltaR;
      match_.reset(new reco::GenParticle(*iGenParticle));
      // parent_.reset(new reco::GenParticle(*(iGenParticle->mother())));
    }
  }
}
