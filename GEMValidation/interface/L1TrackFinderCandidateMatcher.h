#ifndef GEMCode_GEMValidation_L1TrackFinderCandidateMatcher_h
#define GEMCode_GEMValidation_L1TrackFinderCandidateMatcher_h

#include "GEMCode/GEMValidation/interface/SimHitMatcher.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"

typedef std::vector<L1MuRegionalCand> L1MuRegionalCandCollection;

class L1TrackFinderCandidateMatcher : public BaseMatcher
{
 public:
  /// constructor
  L1TrackFinderCandidateMatcher(SimHitMatcher& sh);
  /// destructor
  ~L1TrackFinderCandidateMatcher();
  
 private:
  
  void clear();
  void init(); 
  
  void matchCSCTfCandToSimTrack(const L1MuRegionalCandCollection&); 
  void matchDTTfCandToSimTrack(const L1MuRegionalCandCollection&); 
  void matchRPCfTfCandToSimTrack(const L1MuRegionalCandCollection&); 
  void matchRPCbTfCandToSimTrack(const L1MuRegionalCandCollection&); 

  std::vector<edm::InputTag> cscTfCandInputLabel_; 
  std::vector<edm::InputTag> dtTfCandInputLabel_; 
  std::vector<edm::InputTag> rpcfTfCandInputLabel_; 
  std::vector<edm::InputTag> rpcbTfCandInputLabel_; 

  int verboseCscTfCand_;
  int verboseDtTfCand_;
  int verboseRpcfTfCand_;
  int verboseRpcbTfCand_;

  bool runCscTfCand_;
  bool runDtTfCand_;
  bool runRpcfTfCand_;
  bool runRpcbTfCand_;

  int minBXCscTfCand_;
  int minBXDtTfCand_;
  int minBXRpcfTfCand_;
  int minBXRpcbTfCand_;

  int maxBXCscTfCand_;
  int maxBXDtTfCand_;
  int maxBXRpcfTfCand_;
  int maxBXRpcbTfCand_;
};

#endif
