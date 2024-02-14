// CMSSW
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Private code
#include "GEMCode/GEMValidation/interface/MatcherManager.h"
#include "GEMCode/GEMValidation/interface/AnalyzerManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

#include <iomanip>
#include <sstream>
#include <memory>
#include <math.h>
#include <bitset>

class GEMCSCAnalyzer : public edm::one::EDAnalyzer<> {
public:
  explicit GEMCSCAnalyzer(const edm::ParameterSet&);

  ~GEMCSCAnalyzer() {}

  virtual void analyze(const edm::Event&, const edm::EventSetup&);

private:
  void analyze(const SimTrack& t, const SimVertex& v, const edm::Event& ev);
  bool isSimTrackGood(const SimTrack& t);

  edm::EDGetTokenT<edm::SimVertexContainer> simVertexInput_;
  edm::EDGetTokenT<edm::SimTrackContainer> simTrackInput_;

  int verboseSimTrack_;
  double simTrackMinPt_;
  double simTrackMinEta_;
  double simTrackMaxEta_;
  int verbose_;
  int iev_;

  std::unique_ptr<TreeManager> tree_;
  std::unique_ptr<MatcherManager> matcher_;
  std::unique_ptr<AnalyzerManager> analyzer_;
};

GEMCSCAnalyzer::GEMCSCAnalyzer(const edm::ParameterSet& ps) :
  verbose_(ps.getUntrackedParameter<int>("verbose", 0))
{
  const auto& simVertex = ps.getParameter<edm::ParameterSet>("simVertex");
  simVertexInput_ = consumes<edm::SimVertexContainer>(simVertex.getParameter<edm::InputTag>("inputTag"));

  const auto& simTrack = ps.getParameter<edm::ParameterSet>("simTrack");
  simTrackInput_ = consumes<edm::SimTrackContainer>(simTrack.getParameter<edm::InputTag>("inputTag"));
  simTrackMinPt_ = simTrack.getParameter<double>("minPt");
  simTrackMinEta_ = simTrack.getParameter<double>("minEta");
  simTrackMaxEta_ = simTrack.getParameter<double>("maxEta");

  // book the trees
  tree_.reset(new TreeManager());
  tree_->book();

  // define new matchers
  matcher_.reset(new MatcherManager(ps, consumesCollector()));

  // define new analyzers
  analyzer_.reset(new AnalyzerManager(ps, consumesCollector()));

  iev_ = 0;
}

void GEMCSCAnalyzer::analyze(const edm::Event& ev, const edm::EventSetup& es) {

  // set event and setup
  matcher_->init(ev, es);
  analyzer_->init(ev, es);

  edm::Handle<edm::SimTrackContainer> sim_tracks;
  ev.getByToken(simTrackInput_, sim_tracks);
  const edm::SimTrackContainer& sim_track = *sim_tracks.product();

  edm::Handle<edm::SimVertexContainer> sim_vertices;
  ev.getByToken(simVertexInput_, sim_vertices);
  const edm::SimVertexContainer& sim_vert = *sim_vertices.product();

  if (verbose_) {
    std::cout <<"iev "<< iev_ << " Total number of SimTrack in this event: " << sim_track.size() << std::endl;
  }

  edm::SimTrackContainer sim_track_selected;
  for (const auto& t : sim_track) {
    if (!isSimTrackGood(t))
      continue;
    sim_track_selected.push_back(t);
  }

  int trk_no = 0;

  for (const auto& t : sim_track_selected) {
    trk_no++;
    if (verbose_) {
      std::cout << "Processing selected SimTrack " << trk_no + 1 << std::endl;
      std::cout << "pT = " << t.momentum().pt()
                << "GeV, eta = " << t.momentum().eta()
                << ", phi = " << t.momentum().phi()
                << ", Q = " << t.charge()
                << ", PDGiD =  " << t.type() << std::endl;
    }
    analyze(t, sim_vert[t.vertIndex()], ev);
  }
  iev_++;
  // analyzer_->analyze(ev, es, tree_);
}

void GEMCSCAnalyzer::analyze(const SimTrack& track, const SimVertex& vertex, const edm::Event& ev)
{
  // reset all structs
  tree_->init();

  // match the track
  matcher_->match(track, vertex);

  // initialize the track analyzers
  analyzer_->setMatcher(*matcher_);
  analyzer_->setiev (iev_);

  // analyze the track
  analyzer_->analyze(*tree_, track, vertex, ev);

  // fill all trees
  tree_->fill();
}

bool GEMCSCAnalyzer::isSimTrackGood(const SimTrack& t) {
  // SimTrack selection
  if (t.noVertex())
    return false;
  if (t.noGenpart())
    return false;
  // only muons
  if (std::abs(t.type()) != 13)
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


DEFINE_FWK_MODULE(GEMCSCAnalyzer);
