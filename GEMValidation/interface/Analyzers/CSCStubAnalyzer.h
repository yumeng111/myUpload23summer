#ifndef GEMCode_GEMValidation_CSCStubAnalyzer_h
#define GEMCode_GEMValidation_CSCStubAnalyzer_h

#include "GEMCode/GEMValidation/interface/Helpers.h"
#include "GEMCode/GEMValidation/interface/MatcherSuperManager.h"
#include "GEMCode/GEMValidation/interface/TreeManager.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTPreTriggerDigiCollection.h"

class CSCStubAnalyzer
{
public:

  // constructor
  CSCStubAnalyzer(const edm::ParameterSet& conf, edm::ConsumesCollector&& iC);

  // destructor
  ~CSCStubAnalyzer() {}

 void init(const edm::Event& iEvent, const edm::EventSetup& iSetup);

  void setMatcher(const CSCStubMatcher& match_sh);
//    virtual void beginJob();
//      virtual void beginRun(const edm::Run&,   const edm::EventSetup&);
//        virtual void analyze (const edm::Event&, const edm::EventSetup&);
//	  virtual void endRun  (const edm::Run&,   const edm::EventSetup&);
//	    virtual void endJob  ();
//
  // initialize the event
  void analyze(const edm::Event& ev, const edm::EventSetup& es, const MatcherSuperManager& manager, my::TreeManager& tree);
  void analyze(TreeManager& tree);

  //convert Run2 pattern into Run3 slope
  int convertRun2PIDToRun3Slope(int run2pid) {
    int lut[11] = {-1, -1, 12, 12, 9, 9, 6, 6, 3, 3, 0};
    int run3slope = (run2pid > 0 and run2pid <= 10) ? lut[run2pid] : -99;
    return run3slope; 
  }

 private:

  edm::EDGetTokenT<CSCCLCTPreTriggerDigiCollection> preclctToken_;
  edm::EDGetTokenT<CSCCLCTDigiCollection> clctToken_;
  edm::EDGetTokenT<CSCALCTDigiCollection> alctToken_;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> lctToken_;
  edm::EDGetTokenT<CSCCorrelatedLCTDigiCollection> mplctToken_;
  edm::EDGetTokenT<CSCShowerDigiCollection> showerToken_;

  edm::Handle<CSCCLCTPreTriggerDigiCollection> preclctsH_;
  edm::Handle<CSCCLCTDigiCollection> clctsH_;
  edm::Handle<CSCALCTDigiCollection> alctsH_;
  edm::Handle<CSCCorrelatedLCTDigiCollection> lctsH_;
  edm::Handle<CSCCorrelatedLCTDigiCollection> mplctsH_;
  edm::Handle<CSCShowerDigiCollection> showersH_;

  bool verboseALCT_;
  bool verbosePreCLCT_;
  bool verboseCLCT_;
  bool verboseLCT_;
  bool verboseMPLCT_;
  bool verboseShower_;

  int minBXPreCLCT_, maxBXPreCLCT_;
  int minBXCLCT_, maxBXCLCT_;
  int minBXALCT_, maxBXALCT_;
  int minBXLCT_, maxBXLCT_;
  int minBXMPLCT_, maxBXMPLCT_;
  int minBXShower_, maxBXShower_;
  int matchDeltaStrip_, matchDeltaWG_;

  // best here means "closest in phi"
  std::pair<GEMDigi, GlobalPoint>
  bestGEMDigi(const GEMDetId& gemId,
              const GEMDigiContainer& gem_digis,
              const GlobalPoint& csc_gp) const;

  std::pair<GEMPadDigi, GlobalPoint>
  bestGEMPadDigi(const GEMDetId& gemId,
                 const GEMPadDigiContainer& gem_digis,
                 const GlobalPoint& csc_gp) const;

  std::pair<GEMCoPadDigi, GlobalPoint>
  bestGEMCoPadDigi(const GEMDetId& gemId,
                   const GEMCoPadDigiContainer& gem_digis,
                   const GlobalPoint& csc_gp) const;

  //edm::ESHandle<CSCGeometry> csc_geom_;
  //edm::ESHandle<GEMGeometry> gem_geom_;
  edm::ESGetToken<CSCGeometry, MuonGeometryRecord> cscToken_;
  edm::ESGetToken<GEMGeometry, MuonGeometryRecord> gemToken_;

  const CSCGeometry* cscGeometry_;
  const GEMGeometry* gemGeometry_;

  std::unique_ptr<CSCStubMatcher> match_;
  int minNHitsChamber_;
};

#endif
