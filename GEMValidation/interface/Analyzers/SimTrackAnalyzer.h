#ifndef GEMCode_GEMValidation_SimTrackAnalyzer_h
#define GEMCode_GEMValidation_SimTrackAnalyzer_h

#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"
#include "GEMCode/GEMValidation/interface/new/TreeManager.h"

class SimTrackAnalyzer
{
public:

  // constructor
  SimTrackAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~SimTrackAnalyzer() {}

  void init();
  void setiev (int i) {iev_ = i;} 

  bool isSimTrackGood(const SimTrack& t);

  // initialize the event
  void analyze(TreeManager& tree, const SimTrack& t, const SimVertex& v, const edm::Event& ev);
  void analyze(const edm::Event& ev, const edm::EventSetup& es, my::TreeManager&);

 private:
  edm::EDGetTokenT<edm::SimVertexContainer> simVertexInput_;
  edm::EDGetTokenT<edm::SimTrackContainer> simTrackInput_;

  double simTrackMinPt_;
  double simTrackMinEta_;
  double simTrackMaxEta_;
  std::vector<int> pdgIds_;
  int iev_;
};

#endif
