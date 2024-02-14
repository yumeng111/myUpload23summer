#include "GEMCode/GEMValidation/interface/Analyzers/GEMStubAnalyzer.h"

GEMStubAnalyzer::GEMStubAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
  const auto& gemPad = conf.getParameterSet("gemPadDigi");
  minBXPad_ = gemPad.getParameter<int>("minBX");
  maxBXPad_ = gemPad.getParameter<int>("maxBX");
  verbosePad_ = gemPad.getParameter<int>("verbose");
  runPad_ = gemPad.getParameter<bool>("run");

  const auto& gemCluster = conf.getParameterSet("gemPadCluster");
  minBXCluster_ = gemCluster.getParameter<int>("minBX");
  maxBXCluster_ = gemCluster.getParameter<int>("maxBX");
  verboseCluster_ = gemCluster.getParameter<int>("verbose");
  runCluster_ = gemCluster.getParameter<bool>("run");

  const auto& gemCoPad = conf.getParameterSet("gemCoPadDigi");
  minBXCoPad_ = gemCoPad.getParameter<int>("minBX");
  maxBXCoPad_ = gemCoPad.getParameter<int>("maxBX");
  verboseCoPad_ = gemCoPad.getParameter<int>("verbose");
  runCoPad_ = gemCoPad.getParameter<bool>("run");

  gemPadToken_ = iC.consumes<GEMPadDigiCollection>(gemPad.getParameter<edm::InputTag>("inputTag"));
  gemClusterToken_ = iC.consumes<GEMPadDigiClusterCollection>(gemCluster.getParameter<edm::InputTag>("inputTag"));
  gemCoPadToken_ = iC.consumes<GEMCoPadDigiCollection>(gemCoPad.getParameter<edm::InputTag>("inputTag"));

  gemToken_ = iC.esConsumes<GEMGeometry, MuonGeometryRecord>();
}

void GEMStubAnalyzer::setMatcher(const GEMDigiMatcher& match_sh)
{
  match_.reset(new GEMDigiMatcher(match_sh));
}

void GEMStubAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup, const MatcherSuperManager& manager, my::TreeManager& tree)
{
  //iSetup.get<MuonGeometryRecord>().get(gem_geom_);
  //if (gem_geom_.isValid()) {
  //  gemGeometry_ = &*gem_geom_;
  //} else {
  //  std::cout << "+++ Info: GEM geometry is unavailable. +++\n";
  //}
  gemGeometry_ =  &iSetup.getData(gemToken_);

  // get the digi collections
  auto& gemTree = tree.gemStub();
  auto& simTree = tree.simTrack();
  const bool validTracks(simTree.sim_pt->size()>0);

  if (runPad_) {
    iEvent.getByToken(gemPadToken_, gemPadsH_);
    const GEMPadDigiCollection& gemPads = *gemPadsH_.product();
    int index = 0;
    for (auto detUnitIt = gemPads.begin(); detUnitIt != gemPads.end(); ++detUnitIt) {
      const GEMDetId& id = (*detUnitIt).first;
      const bool isodd = (id.chamber()%2 == 1);

      // Loop over the digis of this DetUnit
      const auto& range = (*detUnitIt).second;
      for (auto digiIt = range.first; digiIt != range.second; ++digiIt) {

        if (!digiIt->isValid())
          continue;

        int tpidfound = -1;
        for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

          // get the matcher
          const auto& matcher = manager.matcher(tpid);

          const auto& gemMatches = manager.matcher(tpid)->gemDigis()->padsInDetId(id.rawId());
          for (const auto& gemMatch : gemMatches) {
            // check if the same
            if (*digiIt == gemMatch) {
              tpidfound =  tpid;
            }
          }
        }

        gemTree.gem_pad_bx->push_back(digiIt->bx());
        gemTree.gem_pad_pad->push_back(digiIt->pad());
        gemTree.gem_pad_isodd->push_back(isodd);
        gemTree.gem_pad_region->push_back(id.region());
        gemTree.gem_pad_station->push_back(id.station());
        gemTree.gem_pad_chamber->push_back(id.chamber());
        gemTree.gem_pad_layer->push_back(id.layer());
        gemTree.gem_pad_roll->push_back(id.roll());
        gemTree.gem_pad_tpid->push_back(tpidfound);

        if (tpidfound != -1 and validTracks) {
          ((*simTree.sim_id_gem_pad)[tpidfound]).push_back(index);
        }
        index++;
      }
    }
  }

  if (runCluster_) {
    iEvent.getByToken(gemClusterToken_, gemClustersH_);
    const GEMPadDigiClusterCollection& gemClusters = *gemClustersH_.product();
    int index = 0;
    for (auto detUnitIt = gemClusters.begin(); detUnitIt != gemClusters.end(); ++detUnitIt) {
      const GEMDetId& id = (*detUnitIt).first;
      const bool isodd = (id.chamber()%2 == 1);

      // Loop over the digis of this DetUnit
      const auto& range = (*detUnitIt).second;
      for (auto digiIt = range.first; digiIt != range.second; ++digiIt) {

        if (!digiIt->isValid())
          continue;

        int tpidfound = -1;
        for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

          // get the matcher
          const auto& matcher = manager.matcher(tpid);

          const auto& gemMatches = manager.matcher(tpid)->gemDigis()->clustersInDetId(id.rawId());
          for (const auto& gemMatch : gemMatches) {
            // check if the same
            if (*digiIt == gemMatch) {
              tpidfound =  tpid;
            }
          }
        }

        gemTree.gem_cluster_bx->push_back(digiIt->bx());
        gemTree.gem_cluster_pad->push_back(digiIt->pads()[0]);
        gemTree.gem_cluster_size->push_back(digiIt->pads().size());
        gemTree.gem_cluster_isodd->push_back(isodd);
        gemTree.gem_cluster_region->push_back(id.region());
        gemTree.gem_cluster_station->push_back(id.station());
        gemTree.gem_cluster_chamber->push_back(id.chamber());
        gemTree.gem_cluster_layer->push_back(id.layer());
        gemTree.gem_cluster_roll->push_back(id.roll());
        gemTree.gem_cluster_tpid->push_back(tpidfound);

        if (tpidfound != -1 and validTracks)
          ((*simTree.sim_id_gem_cluster)[tpidfound]).push_back(index);

        index++;
      }
    }
  }

  if (runCoPad_) {
    iEvent.getByToken(gemCoPadToken_, gemCoPadsH_);
    const GEMCoPadDigiCollection& gemCoPads = *gemCoPadsH_.product();
    int index = 0;
    for (auto detUnitIt = gemCoPads.begin(); detUnitIt != gemCoPads.end(); ++detUnitIt) {
      const GEMDetId& id = (*detUnitIt).first;
      const bool isodd = (id.chamber()%2 == 1);

      // Loop over the digis of this DetUnit
      const auto& range = (*detUnitIt).second;
      for (auto digiIt = range.first; digiIt != range.second; ++digiIt) {

        if (!digiIt->isValid())
          continue;

        int tpidfound = -1;
        for (int tpid = 0; tpid < MAX_PARTICLES; tpid++) {

          // get the matcher
          const auto& matcher = manager.matcher(tpid);

          const auto& gemMatches = manager.matcher(tpid)->gemDigis()->coPadsInSuperChamber(id.rawId());
          for (const auto& gemMatch : gemMatches) {
            // check if the same
            if (*digiIt == gemMatch) {
              tpidfound =  tpid;
            }
          }
        }

        gemTree.gem_copad_bx->push_back(digiIt->bx(1));
        gemTree.gem_copad_pad->push_back(digiIt->pad(1));
        gemTree.gem_copad_isodd->push_back(isodd);
        gemTree.gem_copad_region->push_back(id.region());
        gemTree.gem_copad_station->push_back(id.station());
        gemTree.gem_copad_chamber->push_back(id.chamber());
        gemTree.gem_copad_roll->push_back(id.roll());
        gemTree.gem_copad_tpid->push_back(tpidfound);

        if (tpidfound != -1 and validTracks)
          ((*simTree.sim_id_gem_copad)[tpidfound]).push_back(index);

        index++;
      }
    }
  }
}

void GEMStubAnalyzer::analyze(TreeManager& tree)
{
  for(const auto& d: match_->chamberIdsPad()) {
    GEMDetId id(d);
    GEMDetId scid(id.superChamberId());

    const int st = id.station();
    const bool odd(id.chamber()%2==1);

    if (match_->padsInChamber(d).size() >= 1) {
      if (odd) {
        tree.gemStub().has_gem_pad_odd[st] = true;
      }
      else {
        tree.gemStub().has_gem_pad_even[st] = true;
      }
    }

    if (match_->nLayersWithPadsInSuperChamber(scid.rawId()) == 2) {
      if (odd) tree.gemStub().has_gem_pad2_odd[st] = true;
      else     tree.gemStub().has_gem_pad2_even[st] = true;
    }
  }

  // best matching pad in a chamber
  for(const auto& d: match_->chamberIdsPad()) {
    GEMDetId id(d);
    const int st = id.station();
    const bool odd(id.chamber()%2==1);
    const int ilayer(id.layer());

    // best matching pad
    const auto& bestP(bestPad(id, match_->padsInChamber(id)).first);
    const auto& bestGP(bestPad(id, match_->padsInChamber(id)).second);

    if (ilayer == 1) {
      if (odd) {
        tree.gemStub().pad1_odd[st] = bestP.pad();
        tree.gemStub().bx_pad1_odd[st] = bestP.bx();
        tree.gemStub().z_pad1_odd[st] = bestGP.z();
        tree.gemStub().eta_pad1_odd[st] = bestGP.eta();
        tree.gemStub().phi_pad1_odd[st] = bestGP.phi();
        tree.gemStub().dphi_pad1_odd[st] =
          reco::deltaPhi(float(tree.gemStub().phi_pad1_odd[st]),
                         float(tree.gemSimHit().phi_gem_sh_odd[st]));
      }
      else {
        tree.gemStub().pad1_even[st] = bestP.pad();
        tree.gemStub().bx_pad1_even[st] = bestP.bx();
        tree.gemStub().z_pad1_even[st] = bestGP.z();
        tree.gemStub().eta_pad1_even[st] = bestGP.eta();
        tree.gemStub().phi_pad1_even[st] = bestGP.phi();
        tree.gemStub().dphi_pad1_even[st] =
          reco::deltaPhi(float(tree.gemStub().phi_pad1_even[st]),
                         float(tree.gemSimHit().phi_gem_sh_even[st]));
      }
    }
    else {
      if (odd) {
        tree.gemStub().pad2_odd[st] = bestP.pad();
        tree.gemStub().bx_pad2_odd[st] = bestP.bx();
        tree.gemStub().z_pad2_odd[st] = bestGP.z();
        tree.gemStub().eta_pad2_odd[st] = bestGP.eta();
        tree.gemStub().phi_pad2_odd[st] = bestGP.phi();
        tree.gemStub().dphi_pad2_odd[st] =
          reco::deltaPhi(float(tree.gemStub().phi_pad2_odd[st]),
                         float(tree.gemSimHit().phi_gem_sh_odd[st]));
      }
      else {
        tree.gemStub().pad2_even[st] = bestP.pad();
        tree.gemStub().bx_pad2_even[st] = bestP.bx();
        tree.gemStub().z_pad2_even[st] = bestGP.z();
        tree.gemStub().eta_pad2_even[st] = bestGP.eta();
        tree.gemStub().phi_pad2_even[st] = bestGP.phi();
        tree.gemStub().dphi_pad2_even[st] =
          reco::deltaPhi(float(tree.gemStub().phi_pad2_even[st]),
                         float(tree.gemSimHit().phi_gem_sh_even[st]));
      }
    }
  }

  // best matching coincidence pads
  for(const auto& d: match_->superChamberIdsCoPad()) {
    GEMDetId id(d);
    const int st = id.station();
    const bool odd(id.chamber()%2==1);
    if (odd) tree.gemStub().has_gem_copad_odd[st] = true;
    else     tree.gemStub().has_gem_copad_even[st] = true;
    /*
    // best matching coincidence pad
    const auto& bestP(bestCoPad(id, match_->coPadsInSuperChamber(id)).first);
    const auto& bestGP(bestCoPad(id, match_->coPadsInSuperChamber(id)).second);

    if (odd) {
      tree.gemStub().copad_odd[st] = bestP.pad(1);
      tree.gemStub().bx_copad_odd[st] = bestP.bx(1);
      tree.gemStub().z_copad_odd[st] = bestGP.z();
      tree.gemStub().eta_copad_odd[st] = bestGP.eta();
      tree.gemStub().phi_copad_odd[st] = bestGP.phi();
    }
    else {
      tree.gemStub().copad_even[st] = bestP.pad(1);
      tree.gemStub().bx_copad_even[st] = bestP.bx(1);
      tree.gemStub().z_copad_even[st] = bestGP.z();
      tree.gemStub().eta_copad_even[st] = bestGP.eta();
      tree.gemStub().phi_copad_even[st] = bestGP.phi();
    }
  */
  }


  // GEM clusters in single chambers or super chambers
  for(const auto& d: match_->superChamberIdsCluster()) {
    GEMDetId id(d);
    const int st = id.station();
    const bool odd(id.chamber()%2==1);
    const int nlayers(match_->nLayersWithDigisInSuperChamber(d));

    if (odd) tree.gemStub().has_gem_cluster_odd[st] = true;
    else     tree.gemStub().has_gem_cluster_even[st] = true;

    if (nlayers >= 2) {
      if (odd) tree.gemStub().has_gem_cluster2_odd[st] = true;
      else     tree.gemStub().has_gem_cluster2_even[st] = true;
    }
  }

}

std::pair<GEMPadDigi, GlobalPoint>
GEMStubAnalyzer::bestPad(const GEMDetId& id,
                         const GEMPadDigiContainer& digis) const
{
  const auto& mean(meanPosition(id, digis));
  float bestDPhi = 99;
  GEMPadDigi bestDigi;
  GlobalPoint bestGP;
  for(const auto& d: match_->padsInChamber(id.rawId())) {
    // does not work for GE2/1!!!
    for (int partition = 1; partition <= 8; ++partition) {

      GEMDetId partId(id.region(), 1, id.station(), id.layer(), id.chamber(), partition);

      const GlobalPoint& gp = match_->getGlobalPointPad(partId, d);
      const float dPhi = reco::deltaPhi(float(gp.phi()),float(mean.phi()));
      if (dPhi < bestDPhi) {
        bestDigi = d;
        bestDPhi = dPhi;
        bestGP = gp;
    }
  }
}
  return std::make_pair(bestDigi, bestGP);
}


std::pair<GEMCoPadDigi, GlobalPoint>
GEMStubAnalyzer::bestCoPad(const GEMDetId& id,
                           const GEMCoPadDigiContainer& digis) const
{
  const auto& mean(meanPosition(id, digis));
  float bestDPhi = 99;
  GEMCoPadDigi bestDigi;
  GlobalPoint bestGP;
  for(const auto& d: match_->coPadsInSuperChamber(id.rawId())) {
    const GlobalPoint& gp = match_->getGlobalPointPad(id, d.first());
    const float dPhi = reco::deltaPhi(float(gp.phi()),float(mean.phi()));
    if (dPhi < bestDPhi) {
      bestDigi = d;
      bestDPhi = dPhi;
      bestGP = gp;
    }
  }
  return std::make_pair(bestDigi, bestGP);
}


GlobalPoint
GEMStubAnalyzer::meanPosition(const GEMDetId& id,
                              const GEMPadDigiContainer& digis) const
{
  GlobalPoint point_zero;
  if (digis.empty()) return point_zero; // point "zero"

  return point_zero;

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.f;
  size_t n = 0;
  for (auto& d: digis){
    const GlobalPoint& gp = match_->getGlobalPointPad(id, d);
    if (gp == point_zero) continue;

    sumx += gp.x();
    sumy += gp.y();
    sumz += gp.z();
    ++n;
  }
  if (n == 0) return GlobalPoint();
  return GlobalPoint(sumx/n, sumy/n, sumz/n);
}


GlobalPoint
GEMStubAnalyzer::meanPosition(const GEMDetId& id,
                              const GEMCoPadDigiContainer& digis) const
{
  GlobalPoint point_zero;
  if (digis.empty()) return point_zero; // point "zero"

  return point_zero;

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.f;
  size_t n = 0;
  for (auto& d: digis){
    const GlobalPoint& gp = match_->getGlobalPointPad(id, d.first());
    if (gp == point_zero) continue;

    sumx += gp.x();
    sumy += gp.y();
    sumz += gp.z();
    ++n;
  }
  if (n == 0) return GlobalPoint();
  return GlobalPoint(sumx/n, sumy/n, sumz/n);
}
