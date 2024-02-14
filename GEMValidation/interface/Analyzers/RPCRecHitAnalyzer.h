#ifndef GEMCode_GEMValidation_RPCRecHitAnalyzer_h
#define GEMCode_GEMValidation_RPCRecHitAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class RPCRecHitAnalyzer
{
public:

  // constructor
  RPCRecHitAnalyzer(const edm::ParameterSet& conf);

  // destructor
  ~RPCRecHitAnalyzer() {}

  void setMatcher(const RPCRecHitMatcher& match_sh);

  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es) {}
  void analyze(TreeManager& tree);

 private:

  std::unique_ptr<RPCRecHitMatcher> match_;
};

#endif
