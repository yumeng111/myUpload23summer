#ifndef GEMCode_GEMValidation_RecoTrackAnalyzer_h
#define GEMCode_GEMValidation_RecoTrackAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"

class RecoTrackAnalyzer
{
public:

  // constructor
  RecoTrackAnalyzer(const edm::ParameterSet& conf);

  // destructor
  ~RecoTrackAnalyzer() {}

  void setMatcher(const RecoTrackMatcher& match_sh);

  // initialize the event
  void analyze(TreeManager& tree);

 private:

  std::unique_ptr<RecoTrackMatcher> match_;
};

#endif
