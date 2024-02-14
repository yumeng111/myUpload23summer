#ifndef GEMCode_GEMValidation_EventAnalyzer_h
#define GEMCode_GEMValidation_EventAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class EventAnalyzer
{
public:

  // constructor
  EventAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~EventAnalyzer() {}

  void init(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, my::TreeManager& tree);
};

#endif
