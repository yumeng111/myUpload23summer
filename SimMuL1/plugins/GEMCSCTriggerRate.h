#ifndef SimMuL1_GEMCSCTriggerRate_h
#define SimMuL1_GEMCSCTriggerRate_h

// system include files
#include <memory>
#include <cmath>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"

#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTChambPhContainer.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1CSCTrackCollection.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticleFwd.h"
#include "DataFormats/L1Trigger/interface/L1MuonParticle.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "L1Trigger/CSCCommonTrigger/interface/CSCConstants.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCTriggerGeometry.h"
#include "L1Trigger/CSCTrackFinder/interface/CSCTFPtLUT.h"
#include "L1Trigger/CSCTrackFinder/interface/CSCTFSectorProcessor.h"
#include "L1Trigger/CSCTrackFinder/interface/CSCSectorReceiverLUT.h"
#include "L1Trigger/CSCTrackFinder/interface/CSCTrackFinderDataTypes.h"
#include "L1Trigger/CSCTrackFinder/src/CSCTFDTReceiver.h"

#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerScalesRcd.h"
#include "CondFormats/DataRecord/interface/L1MuTriggerPtScaleRcd.h"

#include "GEMCode/SimMuL1/interface/MuGeometryHelpers.h"
#include "GEMCode/SimMuL1/interface/MatchCSCMuL1.h"

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"

struct MyALCT
{
  Int_t event, detId, bx, pattern;
  Float_t pt, eta, phi;
};

struct MyCLCT
{
  Int_t event, detId, bx, pattern;
  Float_t pt, eta, phi;
};

struct MyLCT
{
  Int_t event, detId, bx;
  Float_t pt, eta, phi;
  Int_t quality, strip;
  Char_t hasGEM;
};

struct MyMPCLCT
{
  Int_t event, detId, bx;
  Float_t pt, eta, phi;
  Int_t quality, strip;
  Char_t hasGEM;
};

struct MyTFTrack
{
  Int_t event, bx;
  Float_t pt, eta, phi;
  Char_t hasME1a, hasME1b, hasME12, hasME13;
  Char_t hasME21, hasME22;
  Char_t hasME31, hasME32;
  Char_t hasME41, hasME42;
  Char_t hasGEM;
};

struct MyTFCand
{
  Int_t event, bx;
  Float_t pt, eta, phi;
  Char_t hasME1a, hasME1b, hasME12, hasME13;
  Char_t hasME21, hasME22;
  Char_t hasME31, hasME32;
  Char_t hasME41, hasME42;
  Char_t hasGEM;
};

struct MyGMTReg
{
  Int_t event, bx;
  Float_t pt, eta, phi;
  Char_t hasCSC, hasRPC, hasDT, hasGEM;
};

struct MyGMT
{
  Int_t event, bx;
  Float_t pt, eta, phi;
  Char_t hasCSC, hasRPC, hasDT, hasGEM;
};

class GEMCSCTriggerRate : public edm::EDAnalyzer 
{
 public:
  
  explicit GEMCSCTriggerRate(const edm::ParameterSet&);
  
  ~GEMCSCTriggerRate();

  virtual void beginRun(const edm::Run&, const edm::EventSetup&);

  virtual void beginJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  enum trig_cscs {MAX_STATIONS = 4, CSC_TYPES = 10};
  //Various useful constants
  static const std::string csc_type[CSC_TYPES+1];
  static const std::string csc_type_[CSC_TYPES+1];
  static const std::string csc_type_a[CSC_TYPES+2];
  static const std::string csc_type_a_[CSC_TYPES+2];
  static const int pbend[CSCConstants::NUM_CLCT_PATTERNS];

  enum pt_thresh {N_PT_THRESHOLDS = 6};
  static const double PT_THRESHOLDS[N_PT_THRESHOLDS];
  static const double PT_THRESHOLDS_FOR_ETA[N_PT_THRESHOLDS];
  
 private:
  
  // functions
  int getCSCType(CSCDetId &id);
  int isME11(int t);
  int getCSCSpecsType(CSCDetId &id);
  int cscTriggerSubsector(CSCDetId &id);

  // From Ingo:
  // calculates the weight of the event to reproduce a min bias
  //spectrum, from G. Wrochna's note CMSS 1997/096
  void setupTFModeHisto(TH1D* h);
  void runCSCTFSP(const CSCCorrelatedLCTDigiCollection*, const L1MuDTChambPhContainer*);

  std::pair<float, float> intersectionEtaPhi(CSCDetId id, int wg, int hs);
  csctf::TrackStub buildTrackStub(const CSCCorrelatedLCTDigi &d, CSCDetId id);

  void bookALCTTree();
  void bookCLCTTree();
  void bookLCTTree();
  void bookMPCLCTTree();
  void bookTFTrackTree();
  void bookTFCandTree();
  void bookGMTRegionalTree();
  void bookGMTCandTree();

  void analyzeALCTRate(const edm::Event&);
  void analyzeCLCTRate(const edm::Event&);
  void analyzeLCTRate(const edm::Event&);
  void analyzeMPCLCTRate(const edm::Event&);
  void analyzeTFTrackRate(const edm::Event&);
  void analyzeTFCandRate(const edm::Event&);
  void analyzeGMTRegionalRate(const edm::Event&);
  void analyzeGMTCandRate(const edm::Event&);

  // parameters
  edm::ParameterSet CSCTFSPset;
  edm::ParameterSet ptLUTset;
  CSCTFPtLUT* ptLUT;
  CSCTFSectorProcessor* my_SPs[2][6];
  CSCSectorReceiverLUT* srLUTs_[5][6][2];
  CSCTFDTReceiver* my_dtrc;
  unsigned long long  muScalesCacheID_;
  unsigned long long  muPtScaleCacheID_;

  edm::ESHandle< L1MuTriggerScales > muScales;
  edm::ESHandle< L1MuTriggerPtScale > muPtScale;

  // config parameters:
  bool matchAllTrigPrimitivesInChamber_;
  int debugRATE;

  int minBX_;
  int maxBX_;
  int minTMBBX_;
  int maxTMBBX_;
  int minRateBX_;
  int maxRateBX_;

  int minBxALCT_;
  int maxBxALCT_;
  int minBxCLCT_;
  int maxBxCLCT_;
  int minBxLCT_;
  int maxBxLCT_;
  int minBxMPLCT_;
  int maxBxMPLCT_;
  int minBxGMT_;
  int maxBxGMT_;

  bool centralBxOnlyGMT_;
  bool doSelectEtaForGMTRates_;
  bool doME1a_;
  bool defaultME1a;

  const CSCGeometry* cscGeometry;

  TTree* alct_tree_;
  TTree* clct_tree_;
  TTree* lct_tree_;
  TTree* mplct_tree_;
  TTree* tftrack_tree_;
  TTree* tfcand_tree_;
  TTree* gmtreg_tree_;
  TTree* gmt_tree_;

  MyALCT alct_;
  MyCLCT clct_;
  MyLCT lct_;
  MyMPCLCT mpclct_;
  MyTFTrack tftrack_;
  MyTFCand tfcand_;
  MyGMTReg gmtreg_;
  MyGMT gmt_;

  // --- rate histograms ---

  TH1D * h_rt_lct_per_sector;
  TH2D * h_rt_lct_per_sector_vs_bx;
  TH1D * h_rt_mplct_per_sector;
  TH2D * h_rt_mplct_per_sector_vs_bx;
  TH1D * h_rt_lct_per_sector_st[MAX_STATIONS];
  TH2D * h_rt_lct_per_sector_vs_bx_st[MAX_STATIONS];
  TH1D * h_rt_mplct_per_sector_st[MAX_STATIONS];
  TH2D * h_rt_mplct_per_sector_vs_bx_st[MAX_STATIONS];
  TH2D * h_rt_lct_per_sector_vs_bx_st1t;
  TH2D * h_rt_mplct_per_sector_vs_bx_st1t;

  TH1D * h_rt_nalct;
  TH1D * h_rt_nclct;
  TH1D * h_rt_nlct;
  TH1D * h_rt_nmplct;
  TH1D * h_rt_ntftrack;
  TH1D * h_rt_ntfcand;
  TH1D * h_rt_ntfcand_pt10;
  TH1D * h_rt_ngmt_csc;
  TH1D * h_rt_ngmt_csc_pt10;
  TH1D * h_rt_ngmt_csc_per_bx;
  TH1D * h_rt_ngmt_rpcf;
  TH1D * h_rt_ngmt_rpcf_pt10;
  TH1D * h_rt_ngmt_rpcf_per_bx;
  TH1D * h_rt_ngmt_rpcb;
  TH1D * h_rt_ngmt_rpcb_pt10;
  TH1D * h_rt_ngmt_rpcb_per_bx;
  TH1D * h_rt_ngmt_dt;
  TH1D * h_rt_ngmt_dt_pt10;
  TH1D * h_rt_ngmt_dt_per_bx;
  TH1D * h_rt_ngmt;
  TH1D * h_rt_nxtra;

  TH1D * h_rt_nalct_per_bx;
  TH1D * h_rt_nclct_per_bx;
  TH1D * h_rt_nlct_per_bx;

  TH1D * h_rt_alct_bx;
  TH1D * h_rt_clct_bx;
  TH1D * h_rt_lct_bx;
  TH1D * h_rt_mplct_bx;
  TH1D * h_rt_csctype_alct_bx567;
  TH1D * h_rt_csctype_clct_bx567;
  TH1D * h_rt_csctype_lct_bx567;
  TH1D * h_rt_csctype_mplct_bx567;
  TH1D * h_rt_alct_bx_cscdet[CSC_TYPES+1];
  TH1D * h_rt_clct_bx_cscdet[CSC_TYPES+1];
  TH1D * h_rt_lct_bx_cscdet[CSC_TYPES+1];
  TH1D * h_rt_mplct_bx_cscdet[CSC_TYPES+1];

  TH2D * h_rt_lct_qu_vs_bx;
  TH2D * h_rt_mplct_qu_vs_bx;

  TH2D * h_rt_nalct_vs_bx;
  TH2D * h_rt_nclct_vs_bx;
  TH2D * h_rt_nlct_vs_bx;
  TH2D * h_rt_nmplct_vs_bx;
  
  TH2D * h_rt_n_per_ch_alct_vs_bx_cscdet[CSC_TYPES+1];
  TH2D * h_rt_n_per_ch_clct_vs_bx_cscdet[CSC_TYPES+1];
  TH2D * h_rt_n_per_ch_lct_vs_bx_cscdet[CSC_TYPES+1];

  TH1D * h_rt_n_ch_alct_per_bx_cscdet[CSC_TYPES+1];
  TH1D * h_rt_n_ch_clct_per_bx_cscdet[CSC_TYPES+1];
  TH1D * h_rt_n_ch_lct_per_bx_cscdet[CSC_TYPES+1];

  TH1D * h_rt_n_ch_alct_per_bx_st[MAX_STATIONS];
  TH1D * h_rt_n_ch_clct_per_bx_st[MAX_STATIONS];
  TH1D * h_rt_n_ch_lct_per_bx_st[MAX_STATIONS];

  TH1D * h_rt_n_ch_alct_per_bx;
  TH1D * h_rt_n_ch_clct_per_bx;
  TH1D * h_rt_n_ch_lct_per_bx;

  TH1D * h_rt_lct_qu;
  TH1D * h_rt_mplct_qu;
  
  TH2D * h_rt_qu_vs_bxclct__lct;

  TH1D * h_rt_mplct_pattern;
  TH1D * h_rt_mplct_pattern_cscdet[CSC_TYPES+1];

  TH1D * h_rt_tftrack_pt;
  TH1D * h_rt_tfcand_pt;

  TH1D * h_rt_tfcand_pt_2st;
  TH1D * h_rt_tfcand_pt_3st;

  TH1D * h_rt_tfcand_pt_h42_2st;
  TH1D * h_rt_tfcand_pt_h42_3st;

  TH1D * h_rt_tftrack_bx;
  TH1D * h_rt_tfcand_bx;

  TH1D * h_rt_tfcand_eta;
  TH1D * h_rt_tfcand_eta_pt5;
  TH1D * h_rt_tfcand_eta_pt10;
  TH1D * h_rt_tfcand_eta_pt15;
  
  TH1D * h_rt_tfcand_eta_3st;
  TH1D * h_rt_tfcand_eta_pt5_3st;
  TH1D * h_rt_tfcand_eta_pt10_3st;
  TH1D * h_rt_tfcand_eta_pt15_3st;

  TH1D * h_rt_tfcand_eta_3st1a;
  TH1D * h_rt_tfcand_eta_pt5_3st1a;
  TH1D * h_rt_tfcand_eta_pt10_3st1a;
  TH1D * h_rt_tfcand_eta_pt15_3st1a;

  TH2D * h_rt_tfcand_pt_vs_eta;
  TH2D * h_rt_tfcand_pt_vs_eta_3st;
  TH2D * h_rt_tfcand_pt_vs_eta_3st1a;

  TH1D * h_rt_gmt_csc_pt;
  TH1D * h_rt_gmt_csc_ptmax_2s;
  TH1D * h_rt_gmt_csc_ptmax_2s_1b;
  TH1D * h_rt_gmt_csc_ptmax_2s_no1a;
  TH1D * h_rt_gmt_csc_ptmax_3s;
  TH1D * h_rt_gmt_csc_ptmax_3s_1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_no1a;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s123_1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s13_1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s123_no1a;
  TH1D * h_rt_gmt_csc_ptmax_3s_2s13_no1a;
  TH1D * h_rt_gmt_csc_ptmax_3s_3s1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_3s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax_3s_3s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax_2q;
  TH1D * h_rt_gmt_csc_ptmax_3q;
  TH1D * h_rt_gmt_csc_pt_2s42;
  TH1D * h_rt_gmt_csc_pt_3s42;
  TH1D * h_rt_gmt_csc_ptmax_2s42;
  TH1D * h_rt_gmt_csc_ptmax_3s42;
  TH1D * h_rt_gmt_csc_pt_2q42;
  TH1D * h_rt_gmt_csc_pt_3q42;
  TH1D * h_rt_gmt_csc_ptmax_2q42;
  TH1D * h_rt_gmt_csc_ptmax_3q42;
  TH1D * h_rt_gmt_csc_pt_2s42r;
  TH1D * h_rt_gmt_csc_pt_3s42r;
  TH1D * h_rt_gmt_csc_ptmax_2s42r;
  TH1D * h_rt_gmt_csc_ptmax_3s42r;
  TH1D * h_rt_gmt_csc_pt_2q42r;
  TH1D * h_rt_gmt_csc_pt_3q42r;
  TH1D * h_rt_gmt_csc_ptmax_2q42r;
  TH1D * h_rt_gmt_csc_ptmax_3q42r;
  

  TH1D * h_rt_gmt_rpcf_pt;
  TH1D * h_rt_gmt_rpcf_pt_42;
  TH1D * h_rt_gmt_rpcf_ptmax;
  TH1D * h_rt_gmt_rpcf_ptmax_42;

  TH1D * h_rt_gmt_rpcb_pt;
  TH1D * h_rt_gmt_rpcb_ptmax;
  
  TH1D * h_rt_gmt_dt_pt;
  TH1D * h_rt_gmt_dt_ptmax;

  TH1D * h_rt_gmt_pt;
  TH1D * h_rt_gmt_pt_2s42;
  TH1D * h_rt_gmt_pt_3s42;
  TH1D * h_rt_gmt_ptmax_2s42;
  TH1D * h_rt_gmt_ptmax_3s42;
  TH1D * h_rt_gmt_ptmax_2s42_sing;
  TH1D * h_rt_gmt_ptmax_3s42_sing;
  TH1D * h_rt_gmt_pt_2s42r;
  TH1D * h_rt_gmt_pt_3s42r;
  TH1D * h_rt_gmt_ptmax_2s42r;
  TH1D * h_rt_gmt_ptmax_3s42r;
  TH1D * h_rt_gmt_ptmax_2s42r_sing;
  TH1D * h_rt_gmt_ptmax_3s42r_sing;
  TH1D * h_rt_gmt_ptmax;
  TH1D * h_rt_gmt_ptmax_sing;
  TH1D * h_rt_gmt_ptmax_sing_3s;
  TH1D * h_rt_gmt_ptmax_sing_csc;
  TH1D * h_rt_gmt_ptmax_sing_1b;
  TH1D * h_rt_gmt_ptmax_sing_no1a;
  TH1D * h_rt_gmt_ptmax_sing6;
  TH1D * h_rt_gmt_ptmax_sing6_3s;
  TH1D * h_rt_gmt_ptmax_sing6_csc;
  TH1D * h_rt_gmt_ptmax_sing6_1b;
  TH1D * h_rt_gmt_ptmax_sing6_no1a;
  TH1D * h_rt_gmt_ptmax_sing6_3s1b_no1a;
  TH1D * h_rt_gmt_ptmax_dbl;
  TH1D * h_rt_gmt_pt_2q42;
  TH1D * h_rt_gmt_pt_3q42;
  TH1D * h_rt_gmt_ptmax_2q42;
  TH1D * h_rt_gmt_ptmax_3q42;
  TH1D * h_rt_gmt_ptmax_2q42_sing;
  TH1D * h_rt_gmt_ptmax_3q42_sing;
  TH1D * h_rt_gmt_pt_2q42r;
  TH1D * h_rt_gmt_pt_3q42r;
  TH1D * h_rt_gmt_ptmax_2q42r;
  TH1D * h_rt_gmt_ptmax_3q42r;
  TH1D * h_rt_gmt_ptmax_2q42r_sing;
  TH1D * h_rt_gmt_ptmax_3q42r_sing;

  TH1D * h_rt_gmt_csc_eta;
  TH1D * h_rt_gmt_csc_ptmax10_eta_2s;
  TH1D * h_rt_gmt_csc_ptmax10_eta_2s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_no1a;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s123_1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s13_1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s123_no1a;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_2s13_no1a;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_3s1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_3s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3s_3s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax10_eta_2q;
  TH1D * h_rt_gmt_csc_ptmax10_eta_3q;

  TH1D * h_rt_gmt_csc_ptmax20_eta_2s;
  TH1D * h_rt_gmt_csc_ptmax20_eta_2s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_1ab;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_no1a;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s123_1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s13_1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s123_no1a;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_2s13_no1a;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_3s1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_3s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_3s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3s_3s1ab;
  TH1D * h_rt_gmt_csc_ptmax20_eta_2q;
  TH1D * h_rt_gmt_csc_ptmax20_eta_3q;

  TH1D * h_rt_gmt_csc_ptmax30_eta_2s;
  TH1D * h_rt_gmt_csc_ptmax30_eta_2s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_1ab;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_no1a;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s123_1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s13_1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s123_no1a;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_2s13_no1a;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_3s1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_3s1b_1b;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_3s1b_no1a;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3s_3s1ab;
  TH1D * h_rt_gmt_csc_ptmax30_eta_2q;
  TH1D * h_rt_gmt_csc_ptmax30_eta_3q;

  TH1D * h_rt_gmt_csc_mode_2s1b_1b[6];

  TH1D * h_rt_gmt_rpcf_eta;
  TH1D * h_rt_gmt_rpcf_ptmax10_eta;
  TH1D * h_rt_gmt_rpcf_ptmax20_eta;
  TH1D * h_rt_gmt_rpcb_eta;
  TH1D * h_rt_gmt_rpcb_ptmax10_eta;
  TH1D * h_rt_gmt_rpcb_ptmax20_eta;
  TH1D * h_rt_gmt_dt_eta;
  TH1D * h_rt_gmt_dt_ptmax10_eta;
  TH1D * h_rt_gmt_dt_ptmax20_eta;
  TH1D * h_rt_gmt_eta;
  TH1D * h_rt_gmt_ptmax10_eta;
  TH1D * h_rt_gmt_ptmax10_eta_sing;
  TH1D * h_rt_gmt_ptmax10_eta_sing_3s;
  TH1D * h_rt_gmt_ptmax10_eta_dbl;
  TH1D * h_rt_gmt_ptmax20_eta;
  TH1D * h_rt_gmt_ptmax20_eta_sing;
  TH1D * h_rt_gmt_ptmax20_eta_sing_csc;
  TH1D * h_rt_gmt_ptmax20_eta_sing_dtcsc;
  TH1D * h_rt_gmt_ptmax20_eta_sing_3s;
  TH1D * h_rt_gmt_ptmax30_eta_sing;
  TH1D * h_rt_gmt_ptmax30_eta_sing_csc;
  TH1D * h_rt_gmt_ptmax30_eta_sing_dtcsc;
  TH1D * h_rt_gmt_ptmax30_eta_sing_3s;
  TH1D * h_rt_gmt_ptmax10_eta_sing6;
  TH1D * h_rt_gmt_ptmax10_eta_sing6_3s;
  TH1D * h_rt_gmt_ptmax20_eta_sing6;
  TH1D * h_rt_gmt_ptmax20_eta_sing6_csc;
  TH1D * h_rt_gmt_ptmax20_eta_sing6_dtcsc;
  TH1D * h_rt_gmt_ptmax20_eta_sing6_3s;
  TH1D * h_rt_gmt_ptmax30_eta_sing6;
  TH1D * h_rt_gmt_ptmax30_eta_sing6_csc;
  TH1D * h_rt_gmt_ptmax30_eta_sing6_dtcsc;
  TH1D * h_rt_gmt_ptmax30_eta_sing6_3s;
  TH1D * h_rt_gmt_ptmax20_eta_dbl;

  TH1D * h_rt_gmt_csc_pt_2st;
  TH1D * h_rt_gmt_csc_pt_3st;
  TH1D * h_rt_gmt_csc_pt_2q;
  TH1D * h_rt_gmt_csc_pt_3q;
  TH1D * h_rt_gmt_pt_2st;
  TH1D * h_rt_gmt_pt_3st;
  TH1D * h_rt_gmt_pt_2q;
  TH1D * h_rt_gmt_pt_3q;

  TH1D * h_rt_gmt_csc_bx;
  TH1D * h_rt_gmt_rpcf_bx;
  TH1D * h_rt_gmt_rpcb_bx;
  TH1D * h_rt_gmt_dt_bx;
  TH1D * h_rt_gmt_bx;
  
  TH1D * h_rt_gmt_csc_q;
  TH1D * h_rt_gmt_csc_q_42;
  TH1D * h_rt_gmt_csc_q_42r;
  TH1D * h_rt_gmt_rpcf_q;
  TH1D * h_rt_gmt_rpcf_q_42;
  TH1D * h_rt_gmt_rpcb_q;
  TH1D * h_rt_gmt_dt_q;
  TH1D * h_rt_gmt_gq;
  TH1D * h_rt_gmt_gq_42;
  TH1D * h_rt_gmt_gq_42r;
  TH2D * h_rt_gmt_gq_vs_pt_42r;
  TH2D * h_rt_gmt_gq_vs_type_42r;
  TH1D * h_rt_tftrack_mode;


  TH1D * h_gmt_mindr;
  TH1D * h_gmt_dr_maxrank;

  TH1D * h_gmt_pt_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dt_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_csc_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dtcsc_initial_gpt[N_PT_THRESHOLDS];

  TH1D * h_gmt_pt_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_2s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_3s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dt_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_csc_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_csc_2s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_csc_3s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dtcsc_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dtcsc_2s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_pt_dtcsc_3s_sing_gpt[N_PT_THRESHOLDS];


  TH1D * h_gmt_eta_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_dt_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_rpcf_initial_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_rpcb_initial_gpt[N_PT_THRESHOLDS];

  TH1D * h_gmt_eta_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_2s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_3s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_2q_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_3q_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_2s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_3s_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_2q_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_csc_3q_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_dt_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_rpcf_sing_gpt[N_PT_THRESHOLDS];
  TH1D * h_gmt_eta_rpcb_sing_gpt[N_PT_THRESHOLDS];
};

#endif
