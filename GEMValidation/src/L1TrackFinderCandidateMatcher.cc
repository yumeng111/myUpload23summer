#include "GEMCode/GEMValidation/interface/L1TrackFinderCandidateMatcher.h"

L1TrackFinderCandidateMatcher::L1TrackFinderCandidateMatcher(SimHitMatcher& sh)
: BaseMatcher(sh.trk(), sh.vtx(), sh.conf(), sh.event(), sh.eventSetup())
{
  auto cscTfCand = conf().getParameter<edm::ParameterSet>("cscTfCand");
  auto dtTfCand = conf().getParameter<edm::ParameterSet>("dtTfCand");
  auto rpcfTfCand = conf().getParameter<edm::ParameterSet>("rpcfTfCand");
  auto rpcbTfCand = conf().getParameter<edm::ParameterSet>("rpcbTfCand");

  cscTfCandInputLabel_ = cscTfCand.getParameter<std::vector<edm::InputTag>>("validInputTags");
  dtTfCandInputLabel_ = dtTfCand.getParameter<std::vector<edm::InputTag>>("validInputTags");
  rpcfTfCandInputLabel_ = rpcfTfCand.getParameter<std::vector<edm::InputTag>>("validInputTags");
  rpcbTfCandInputLabel_ = rpcbTfCand.getParameter<std::vector<edm::InputTag>>("validInputTags");
  
  verboseCscTfCand_ = cscTfCand.getParameter<int>("verbose");
  verboseDtTfCand_ = dtTfCand.getParameter<int>("verbose");
  verboseRpcfTfCand_ = rpcfTfCand.getParameter<int>("verbose");
  verboseRpcbTfCand_ = rpcbTfCand.getParameter<int>("verbose");

  runCscTfCand_ = cscTfCand.getParameter<bool>("run");
  runDtTfCand_ = dtTfCand.getParameter<bool>("run");
  runRpcfTfCand_ = rpcfTfCand.getParameter<bool>("run");
  runRpcbTfCand_ = rpcbTfCand.getParameter<bool>("run");

  minBXCscTfCand_ = cscTfCand.getParameter<int>("minBX");
  minBXDtTfCand_ = dtTfCand.getParameter<int>("minBX");
  minBXRpcfTfCand_ = rpcfTfCand.getParameter<int>("minBX");
  minBXRpcbTfCand_ = rpcbTfCand.getParameter<int>("minBX");

  maxBXCscTfCand_ = cscTfCand.getParameter<int>("maxBX");
  maxBXDtTfCand_ = dtTfCand.getParameter<int>("maxBX");
  maxBXRpcfTfCand_ = rpcfTfCand.getParameter<int>("maxBX");
  maxBXRpcbTfCand_ = rpcbTfCand.getParameter<int>("maxBX");

  init();
}

L1TrackFinderCandidateMatcher::~L1TrackFinderCandidateMatcher()
{}


void 
L1TrackFinderCandidateMatcher::init()
{
  edm::Handle<L1MuRegionalCandCollection> hCscTfCand;
  if (gemvalidation::getByLabel(cscTfCandInputLabel_, hCscTfCand, event())) if (runCscTfCand_) matchCSCTfCandToSimTrack(*hCscTfCand.product());

  edm::Handle<L1MuRegionalCandCollection> hDtTfCand;
  if (gemvalidation::getByLabel(dtTfCandInputLabel_, hDtTfCand, event())) if (runDtTfCand_) matchDTTfCandToSimTrack(*hDtTfCand.product());

  edm::Handle<L1MuRegionalCandCollection> hRpcfTfCand;
  if (gemvalidation::getByLabel(rpcfTfCandInputLabel_, hRpcfTfCand, event())) if (runRpcfTfCand_) matchRPCfTfCandToSimTrack(*hRpcfTfCand.product());

  edm::Handle<L1MuRegionalCandCollection> hRpcbTfCand;
  if (gemvalidation::getByLabel(rpcbTfCandInputLabel_, hRpcbTfCand, event())) if (runRpcbTfCand_) matchRPCbTfCandToSimTrack(*hRpcbTfCand.product());
}

void 
L1TrackFinderCandidateMatcher::matchCSCTfCandToSimTrack(const L1MuRegionalCandCollection& cands)
{
  if (verboseCscTfCand_) std::cout << "Match SimTrack to CSC TFCands" << std::endl;
}

void 
L1TrackFinderCandidateMatcher::matchDTTfCandToSimTrack(const L1MuRegionalCandCollection& cands)
{  
  if (verboseDtTfCand_) std::cout << "Match SimTrack to DT TFCands" << std::endl;
}

void 
L1TrackFinderCandidateMatcher::matchRPCfTfCandToSimTrack(const L1MuRegionalCandCollection& cands)
{
  if (verboseRpcfTfCand_) std::cout << "Match SimTrack to RPCf TFCands" << std::endl;
}

void 
L1TrackFinderCandidateMatcher::matchRPCbTfCandToSimTrack(const L1MuRegionalCandCollection& cands)
{
  if (verboseRpcbTfCand_) std::cout << "Match SimTrack to RPCb TFCands" << std::endl;
}

