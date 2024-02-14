#include "GEMCode/GEMValidation/interface/Analyzers/SimTrackAnalyzer.h"

SimTrackAnalyzer::SimTrackAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& simVertex = conf.getParameter<edm::ParameterSet>("simVertex");
  simVertexInput_ = iC.consumes<edm::SimVertexContainer>(simVertex.getParameter<edm::InputTag>("inputTag"));

  const auto& simTrack = conf.getParameter<edm::ParameterSet>("simTrack");
  simTrackInput_ = iC.consumes<edm::SimTrackContainer>(simTrack.getParameter<edm::InputTag>("inputTag"));
  simTrackMinPt_ = simTrack.getParameter<double>("minPt");
  simTrackMinEta_ = simTrack.getParameter<double>("minEta");
  simTrackMaxEta_ = simTrack.getParameter<double>("maxEta");
  pdgIds_ = simTrack.getParameter<std::vector<int> >("pdgIds");
  iev_ = 0;
}

void SimTrackAnalyzer::init()
{
}

void SimTrackAnalyzer::analyze(TreeManager& tree, const SimTrack& t, const SimVertex& v, const edm::Event& ev)
{
  //event information for tracking
  tree.simTrack().sim_iev = iev_;
  tree.simTrack().run = ev.id().run();
  tree.simTrack().ievent = ev.id().event();
  tree.simTrack().time = ev.time().value();
  tree.simTrack().bx = ev.bunchCrossing();
  tree.simTrack().lumi = ev.luminosityBlock();
  tree.simTrack().orbit = ev.orbitNumber();
  // simtrack properties
  tree.simTrack().pt = t.momentum().pt();
  tree.simTrack().px = t.momentum().px();
  tree.simTrack().py = t.momentum().py();
  tree.simTrack().pz = t.momentum().pz();
  tree.simTrack().phi = t.momentum().phi();
  tree.simTrack().eta = t.momentum().eta();
  tree.simTrack().charge = t.charge();
  tree.simTrack().endcap = (tree.simTrack().eta > 0.) ? 1 : -1;
  tree.simTrack().pdgid = t.type();

  tree.simTrack().vx = v.position().x();
  tree.simTrack().vy = v.position().y();
  tree.simTrack().vz = v.position().z();
}

void SimTrackAnalyzer::analyze(const edm::Event& ev, const edm::EventSetup& eventSetup, my::TreeManager& tree)
{
  edm::Handle<edm::SimTrackContainer> sim_tracks;
  ev.getByToken(simTrackInput_, sim_tracks);
  const edm::SimTrackContainer& sim_track = *sim_tracks.product();

  edm::Handle<edm::SimVertexContainer> sim_vertices;
  ev.getByToken(simVertexInput_, sim_vertices);
  const edm::SimVertexContainer& sim_vert = *sim_vertices.product();

  auto& treeSim = tree.simTrack();

  edm::SimTrackContainer sim_track_selected;
  for (const auto& t : sim_track) {
    if (!isSimTrackGood(t))
      continue;
    sim_track_selected.push_back(t);
  }

  int index;
  for (const auto& t : sim_track_selected) {

    const auto& vertex = sim_vert[t.vertIndex()];
    // simtrack properties
    treeSim.sim_index->push_back(index);
    treeSim.sim_pt->push_back(t.momentum().pt());
    treeSim.sim_px->push_back(t.momentum().px());
    treeSim.sim_py->push_back(t.momentum().py());
    treeSim.sim_pz->push_back(t.momentum().pz());
    treeSim.sim_phi->push_back(t.momentum().phi());
    treeSim.sim_eta->push_back(t.momentum().eta());
    treeSim.sim_charge->push_back(t.charge());
    treeSim.sim_pdgid->push_back(t.type());
    treeSim.sim_vx->push_back(vertex.position().x());
    treeSim.sim_vy->push_back(vertex.position().y());
    treeSim.sim_vz->push_back(vertex.position().z());

    treeSim.sim_id_gen->push_back(-1);

    treeSim.sim_id_gem_dg->push_back(std::vector<int>());
    treeSim.sim_id_gem_pad->push_back(std::vector<int>());
    treeSim.sim_id_gem_copad->push_back(std::vector<int>());
    treeSim.sim_id_gem_cluster->push_back(std::vector<int>());
    treeSim.sim_id_csc_wire->push_back(std::vector<int>());
    treeSim.sim_id_csc_strip->push_back(std::vector<int>());
    treeSim.sim_id_csc_clct->push_back(std::vector<int>());
    treeSim.sim_id_csc_alct->push_back(std::vector<int>());
    treeSim.sim_id_csc_lct->push_back(std::vector<int>());
    treeSim.sim_id_csc_mplct->push_back(std::vector<int>());
    treeSim.sim_id_emtf_track->push_back(std::vector<int>());
    treeSim.sim_id_emtf_cand->push_back(std::vector<int>());
    treeSim.sim_id_l1mu->push_back(std::vector<int>());
    treeSim.sim_id_l1track->push_back(std::vector<int>());
    treeSim.sim_id_l1trackmu->push_back(std::vector<int>());

    index++;
  }
}

bool SimTrackAnalyzer::isSimTrackGood(const SimTrack& t) {
  // SimTrack selection
  if (t.noVertex())
    return false;
  if (t.noGenpart())
    return false;
  // require pdgid match
  if (!std::count(pdgIds_.begin(), pdgIds_.end(), t.type()))
    return false;
  // pt selection
  if (t.momentum().pt() < simTrackMinPt_)
    return false;
  // eta selection
  const float eta(std::abs(t.momentum().eta()));
  if (eta > simTrackMaxEta_ || eta < simTrackMinEta_)
    return false;
  return true;
}
