#ifndef GEMCode_GEMValidation_RPCSimHitAnalyzer_h
#define GEMCode_GEMValidation_RPCSimHitAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class RPCSimHitAnalyzer
{
public:

  // constructor
  RPCSimHitAnalyzer(const edm::ParameterSet& conf);

  // destructor
  ~RPCSimHitAnalyzer() {}

  void setMatcher(const RPCSimHitMatcher& match_sh);

  // initialize the event
  void analyze(TreeManager& tree);

 private:

  std::shared_ptr<RPCSimHitMatcher> match_;
};

#endif
