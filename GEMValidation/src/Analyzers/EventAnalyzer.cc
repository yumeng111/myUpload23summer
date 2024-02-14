#include "GEMCode/GEMValidation/interface/Analyzers/EventAnalyzer.h"
#include <algorithm>

EventAnalyzer::EventAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC)
{
}

void EventAnalyzer::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
}

void EventAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup, my::TreeManager& tree)
{
  auto& evTree = tree.event();

  evTree.event_run->push_back(iEvent.id().run());
  evTree.event_event->push_back(iEvent.id().event());
  evTree.event_time->push_back(iEvent.time().value());
  evTree.event_bx->push_back(iEvent.bunchCrossing());
  evTree.event_lumi->push_back(iEvent.luminosityBlock());
  evTree.event_orbit->push_back(iEvent.orbitNumber());
}
