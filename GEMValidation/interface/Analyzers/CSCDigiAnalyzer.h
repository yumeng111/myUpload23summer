#ifndef GEMCode_GEMValidation_CSCDigiAnalyzer_h
#define GEMCode_GEMValidation_CSCDigiAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class CSCDigiAnalyzer
{
public:

  // constructor
  CSCDigiAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~CSCDigiAnalyzer() {}

  void init(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  void setMatcher(const CSCDigiMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

 private:

  edm::EDGetTokenT<CSCComparatorDigiCollection> comparatorDigiInput_;
  edm::EDGetTokenT<CSCStripDigiCollection> stripDigiInput_;
  edm::EDGetTokenT<CSCWireDigiCollection> wireDigiInput_;

  edm::Handle<CSCComparatorDigiCollection> comparatorDigisH_;
  edm::Handle<CSCStripDigiCollection> stripDigisH_;
  edm::Handle<CSCWireDigiCollection> wireDigisH_;

  std::unique_ptr<CSCDigiMatcher> match_;
  int minNHitsChamber_;

  int minBXComparator_, maxBXComparator_;
  int minBXStrip_, maxBXStrip_;
  int minBXWire_, maxBXWire_;

  int verboseComparator_;
  int verboseStrip_;
  int verboseWG_;
};

#endif
