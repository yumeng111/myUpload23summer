#ifndef GEMCode_GEMValidation_RPCDigiAnalyzer_h
#define GEMCode_GEMValidation_RPCDigiAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class RPCDigiAnalyzer
{
public:

  // constructor
  RPCDigiAnalyzer(const edm::ParameterSet& conf);

  // destructor
  ~RPCDigiAnalyzer() {}

  void setMatcher(const RPCDigiMatcher& match_sh);

  // initialize the event
  void analyze(TreeManager& tree);

 private:

  int median(const RPCDigiContainer& digis) const;

  std::unique_ptr<RPCDigiMatcher> match_;
};

#endif
