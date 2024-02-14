// CMSSW
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Private code
#include "GEMCode/GEMValidation/interface/AnalyzerManager.h"

class MuonNtuplizer : public edm::one::EDAnalyzer<> {
public:
  explicit MuonNtuplizer(const edm::ParameterSet&);

  ~MuonNtuplizer() {}

  virtual void beginJob();
  virtual void endJob();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

private:
  std::unique_ptr<my::TreeManager> tree_;
  std::unique_ptr<MatcherSuperManager> matcher_;
  std::unique_ptr<AnalyzerManager> analyzer_;

  bool runSim_;
};

MuonNtuplizer::MuonNtuplizer(const edm::ParameterSet& ps) {
  // book the trees
  tree_.reset(new my::TreeManager());

  // reset he matchers
  matcher_.reset(new MatcherSuperManager(ps, consumesCollector()));

  // reset the analyzers
  analyzer_.reset(new AnalyzerManager(ps, consumesCollector()));

  runSim_ = ps.getParameter<bool>("runSim");
}

void MuonNtuplizer::beginJob(){
    tree_->book();
}

void MuonNtuplizer::endJob()
{
    std::cerr << "NtupleMaker::endJob" << endl;
}

void MuonNtuplizer::analyze(const edm::Event& ev, const edm::EventSetup& es) {
  // reset all structs
  tree_->init();

  // match the tracks
  if (runSim_) {
    matcher_->init();
    matcher_->match(ev, es);
  }

  // analyze the track objects
  analyzer_->analyze(ev, es, *matcher_, *tree_);

  // fill all trees
  tree_->fill();
}

DEFINE_FWK_MODULE(MuonNtuplizer);
