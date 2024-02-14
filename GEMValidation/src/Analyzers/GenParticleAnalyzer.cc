#include "GEMCode/GEMValidation/interface/Analyzers/GenParticleAnalyzer.h"
#include <algorithm>

GenParticleAnalyzer::GenParticleAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& gen = conf.getParameter<edm::ParameterSet>("genParticle");
  verbose_ = gen.getParameter<int>("verbose");
  run_ = gen.getParameter<bool>("run");
  pdgIds_ = gen.getParameter<std::vector<int> >("pdgIds");
  stableParticle_ = gen.getParameter<bool>("stableParticle");
  etaMin_ = gen.getParameter<double>("etaMin");
  etaMax_ = gen.getParameter<double>("etaMax");

  inputToken_ = iC.consumes<reco::GenParticleCollection>(gen.getParameter<edm::InputTag>("inputTag"));
}

void GenParticleAnalyzer::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  iEvent.getByToken(inputToken_, genParticlesHandle_);
}

void GenParticleAnalyzer::setMatcher(const GenParticleMatcher& match_sh)
{
  match_.reset(new GenParticleMatcher(match_sh));
}

void GenParticleAnalyzer::analyze(TreeManager& tree)
{
  // genparticle properties
  tree.genParticle().genp_iev = iev_;
  tree.genParticle().pt = match_->getMatch()->pt();
  tree.genParticle().pz = match_->getMatch()->pz();
  tree.genParticle().phi = match_->getMatch()->phi();
  tree.genParticle().eta = match_->getMatch()->eta();
  tree.genParticle().charge = match_->getMatch()->charge();
  tree.genParticle().endcap = (tree.genParticle().eta > 0.) ? 1 : -1;
  tree.genParticle().pdgid = match_->getMatch()->pdgId();
}

void GenParticleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup, const MatcherSuperManager& manager, my::TreeManager& tree)
{
  if (!run_) return;

  iEvent.getByToken(inputToken_, genParticlesHandle_);

  // fetch the collection
  const reco::GenParticleCollection& genParticles = *genParticlesHandle_.product();

  auto& simTree = tree.simTrack();
  auto& genTree = tree.genParticle();

  for(auto iGenParticle = genParticles.begin();  iGenParticle != genParticles.end();  ++iGenParticle) {

    int index = int(iGenParticle - genParticles.begin());

    // require stable particle
    if (iGenParticle->status() != 1 and stableParticle_) continue;

    // require pdgId
    if (!std::count(pdgIds_.begin(), pdgIds_.end(), iGenParticle->pdgId())) continue;

    // add a few more selections
    if (iGenParticle->pt() < 2) continue;

    const float vx(iGenParticle->daughter(0)->vx());
    const float vy(iGenParticle->daughter(0)->vy());
    const float vz(iGenParticle->daughter(0)->vz());
    const float eta(iGenParticle->eta());

    // particle decays in the CSC system
    const double radius( std::sqrt(std::pow(vx, 2.0) + std::pow(vy, 2.0) ) );

    // eta selection
    if (std::abs(eta) < etaMin_) continue;
    if (std::abs(eta) > etaMax_) continue;

    if (verbose_)
      std::cout << "Check gen particle pt: " <<  iGenParticle->pt()
                << "V: ("
                << iGenParticle->daughter(0)->vx() << ", "
                << iGenParticle->daughter(0)->vy() << ", "
                << iGenParticle->daughter(0)->vz() << "), eta: "
                << eta << " phi: "
                << iGenParticle->phi() << " R: " << radius
                << " PDGID: " << iGenParticle->pdgId() << " "
                << " Idx " << index << std::endl;

    int tpidfound = -1;
    // check if it was matched to a simtrack
    for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {
      const auto& genMatch = manager.matcher(tpid)->genParticles()->getMatch();
      if (genMatch) {
        // check if the same
        if (verbose_)
          std::cout << genMatch->p4() <<  " " << iGenParticle->p4() << std::endl;
        if (genMatch->p4() == iGenParticle->p4()) {
          tpidfound =  tpid;
          break;
        }
      }
    }

    // genparticle properties
    genTree.gen_pt->push_back(iGenParticle->pt());
    genTree.gen_pz->push_back(iGenParticle->pz());
    genTree.gen_eta->push_back(eta);
    genTree.gen_phi->push_back(iGenParticle->phi());
    genTree.gen_charge->push_back(iGenParticle->charge());
    genTree.gen_pdgid->push_back(iGenParticle->pdgId());
    genTree.gen_tpid->push_back(tpidfound);

    // LLP decay (if applicable)
    genTree.gen_vx->push_back(vx);
    genTree.gen_vy->push_back(vy);
    genTree.gen_vz->push_back(vz);
    genTree.gen_r->push_back(radius);

    // particle decays in the CSC system
    bool inAcceptance(false);
    if (std::abs(eta) > 0.9 &&
        std::abs(eta) < 2.4 &&
        std::abs(vz) > 568. &&
        std::abs(vz) < 1100. &&
        radius < 695.5)
      inAcceptance = true;

    genTree.gen_llp_in_acceptance->push_back(inAcceptance);

    if (inAcceptance)
      std::cout << "Accept gen particle " <<  iGenParticle->p4() << " " << eta << std::endl;

    // store the index of the particle that was matched to it
    if (tpidfound != -1) {
      (*simTree.sim_id_gen)[tpidfound] = index;
    }
  }
}
