#ifndef GEMCode_GEMValidation_DisplacedMuonTriggerPtassignment_h
#define GEMCode_GEMValidation_DisplacedMuonTriggerPtassignment_h

/**\class DisplacedMuonTriggerPtassignment

  Displaced Muon Trigger Design based on Muon system

Author: tao.huang@cern.ch

*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"

#include "Geometry/Records/interface/MuonGeometryRecord.h"
#include "Geometry/GEMGeometry/interface/GEMGeometry.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "Geometry/RPCGeometry/interface/RPCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCGeometry.h"
#include "Geometry/CSCGeometry/interface/CSCLayerGeometry.h"
#include "Geometry/DTGeometry/interface/DTGeometry.h"

#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "L1Trigger/CSCCommonTrigger/interface/CSCPatternLUT.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/CSCDigi/interface/CSCComparatorDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCALCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCLCTDigiCollection.h"
#include "DataFormats/CSCDigi/interface/CSCCorrelatedLCTDigiCollection.h"
//#include "DataFormats/GEMDigi/interface/GEMDigiCollection.h"
#include "DataFormats/GEMDigi/interface/GEMCSCPadDigiCollection.h"

#include "GEMCode/GEMValidation/interface/Helpers.h"

#include <vector>
#include <map>

//=================================
//this class take DT stubs or CSC stubs to assign displaced muons L1 pt
// 2016.10.05: calcualte variables used in displaced muon trigger
// |eta|<0.9 ok, stub alignment, region_=0
// 0.9<|eta|<1.2 not yet, region_=1
// 1.2<|eta|<1.6 ok, position-based and stub alignment, region_=2
// 1.6<|eta|<2.4 ok, position-based and stub alignment, region_=2
typedef std::vector<CSCCorrelatedLCTDigi> CSCCorrelatedLCTDigiContainer;
typedef std::vector<GEMCSCPadDigi> GEMCSCPadDigiContainer;
typedef std::vector<CSCComparatorDigi> CSCComparatorDigiContainer;
typedef std::vector<std::pair<CSCDetId, CSCComparatorDigiContainer> > CSCComparatorDigiContainerIds;
typedef std::vector<CSCDetId> CSCDetIdContainer;

class DisplacedMuonTriggerPtassignment
{
public:
  
  //endcap, we need LCTs and associated cscid, gempads and associated gemid, and all gemometry
  //to get position from fitting, we also need all comparator digis
  //step0 get LCTs and associated cscids, GEMPads and associated gemids, and geometry. 
  //step1 get fitting positions from fitting compara digis after assoicating comparator digis to LCTs
  //step2 calculate all variables used pt assignment, requires eta,phi,radius,Z
  //step3 assgin L1 pt according to LUTs (in short future)
  DisplacedMuonTriggerPtassignment(const CSCCorrelatedLCTDigiCollection* lcts, const edm::EventSetup& es, const edm::Event& iEvent); // region_=2, not used yet
  DisplacedMuonTriggerPtassignment(const CSCCorrelatedLCTDigiContainer lcts, const CSCDetIdContainer cscids, const edm::EventSetup& es,const edm::Event& iEvent);//not used yet

  DisplacedMuonTriggerPtassignment(std::map<unsigned int, CSCCorrelatedLCTDigiContainer> chamberid_lcts, std::map<unsigned int, GEMCSCPadDigiContainer> detid_pads, const edm::EventSetup& es,const edm::Event& iEvent);

  DisplacedMuonTriggerPtassignment(GlobalPoint gp1, GlobalPoint gp2, GlobalPoint gp3, GlobalPoint gp4, GlobalPoint gp_ge11, GlobalPoint gp_ge21, int npar,const edm::EventSetup& es,const  edm::Event& iEvent); //sim level
  DisplacedMuonTriggerPtassignment(); //test constructor

  ~DisplacedMuonTriggerPtassignment();

  // non-copyable
  DisplacedMuonTriggerPtassignment(const DisplacedMuonTriggerPtassignment&) = delete;
  DisplacedMuonTriggerPtassignment& operator=(const DisplacedMuonTriggerPtassignment&) = delete;

  //const edm::ParameterSet& conf() const {return conf_;}
  //const edm::Event& event() const {return ev_;}
  //const edm::EventSetup& eventSetup() const {return es_;}
  
  bool runPositionbased();
  bool runDirectionbased(bool useGE21){ 
      if (useGE21 and meRing==1) return runDirectionbasedGE21(); 
      else return runDirectionbasedCSConly();
   } 
  bool runDirectionbasedGE21();
  bool runDirectionbasedCSConly();
  void setVerbose(int v) { verbose_ = v; }
  int verbose() const { return verbose_; }

 
  /// geometry ?? should we used geometry
  void setGEMGeometry(const GEMGeometry *geom) {gemGeometry_ = geom;}
  void setRPCGeometry(const RPCGeometry *geom) {rpcGeometry_ = geom;}
  void setME0Geometry(const ME0Geometry *geom) {me0Geometry_ = geom;}
  void setCSCGeometry(const CSCGeometry *geom) {cscGeometry_ = geom;}
  void setDTGeometry(const DTGeometry *geom) {dtGeometry_ = geom;}
  const GEMGeometry* getGEMGeometry() const {return gemGeometry_;}
  const RPCGeometry* getRPCGeometry() const {return rpcGeometry_;}
  const ME0Geometry* getME0Geometry() const {return me0Geometry_;}
  const CSCGeometry* getCSCGeometry() const {return cscGeometry_;}
  const DTGeometry* getDTGeometry() const {return dtGeometry_;}


 protected:
  // should use geometry
  bool hasGEMGeometry_;
  bool hasRPCGeometry_;
  bool hasME0Geometry_;
  bool hasCSCGeometry_;
  bool hasDTGeometry_; 
  
  //edm::ParameterSet simTrackPSet_;
  const CSCGeometry* cscGeometry_;
  const RPCGeometry* rpcGeometry_;
  const GEMGeometry* gemGeometry_;
  const ME0Geometry* me0Geometry_;
  const DTGeometry* dtGeometry_;

 private:


  //const edm::ParameterSet& conf_;

  const edm::Event& ev_;
  const edm::EventSetup& es_;
  
  void initBoolVariables();
  void setupGeometry(const edm::EventSetup& es );
  int verbose_;
  unsigned int region_;



  //edm::ESHandle<MagneticField> magfield_;
  //edm::ESHandle<Propagator> propagator_;
  //edm::ESHandle<Propagator> propagatorOpposite_;
  edm::ESHandle<CSCGeometry> csc_geom_;
  edm::ESHandle<RPCGeometry> rpc_geom_;
  edm::ESHandle<GEMGeometry> gem_geom_;
  edm::ESHandle<ME0Geometry> me0_geom_;
  edm::ESHandle<DTGeometry> dt_geom_;
  
  //extra collection to get better CSC positions
 private:
  edm::Handle< CSCComparatorDigiCollection > hCSCComparators;

 private:
  void fitComparatorsLCT(const CSCComparatorDigiCollection&, const CSCCorrelatedLCTDigi& tp, 
	                          CSCDetId chid, float& fit_phi_layer1, float& fit_phi_layer3, float& fit_phi_layer6, 
				  float& fit_z_layer1, float& fit_z_layer3, float& fit_z_layer6, float& perp); 

  void globalPositionOfLCT(const CSCCorrelatedLCTDigi stub, CSCDetId chid);
  void globalPositionOfGEMPad(const GEMCSCPadDigi gempad, GEMDetId gemid);
//pt assignment 
 private:
  int getEtaPartition(float eta) const;
  float deltaYcalculation(GlobalPoint gp1, GlobalPoint gp2) const;
  float deltadeltaYcalculation(GlobalPoint gp1, GlobalPoint gp2, GlobalPoint gp3, float eta, int par) const;
  //float PhiMomentum(float dphi, float phi_position, int st, bool evenodd);
  //float PhiMomentum_Radius(float dphi, float phi_position, float radius_csc, float radius_gem);
  //float PhiMomentum_Xfactor(float dphi, float phi_position, float xfactor);
  //float xFactocalculation(float r1, float r2, float r3);//?
  float phiMomentum_Xfactor(float phi_CSC, float phi_GEM, float xfactor) const;
 public:
  int getNParity() const {return npar; } 
  int getMeRing() const {return meRing; } 
  float assignedPositionPt();
  float assignedDirectionPt();
  float getdeltaY12() const { return deltaY12; }
  float getdeltaY23() const { return deltaY23; }
  float getdeltaY123() const { return ddY123; }
  float getlocalPhiDirection(int st) const;
  float getdeltaPhiDirection(int st1, int st2) const;


 public:
  
  void setRadiusSt1(float x) { radius_st1 = x; }
  void setRadiusSt2(float x) { radius_st2 = x; }
  void setRadiusSt3(float x) { radius_st3 = x; }
  void setNParity(int x) { npar=x; }
  void setxfactor(float x) { xfactor = x; }
  void setPhiGE11(float x) { phi_ge11 = x; }
  void setPhiGE21(float x) { phi_ge21 = x; }
 private:
  //endcap, direction based
  bool hasStub_st1;
  bool hasStub_st2;
  bool hasStub_st3;
  bool hasStub_st4;
  bool hasGEMPad_st1;
  bool hasGEMPad_st2;
  float radius_st1;
  float radius_st2;
  float radius_st3;
  float xfactor;
  int npar;
  int meRing ;
  float eta_st1;
  float eta_st2;
  float eta_st3;
  float phi_ge11;
  float phi_ge21;
  float phi_st1_layer1;
  float phi_st1_layer3;
  float phi_st1_layer6;
  float phi_st2_layer1;
  float phi_st2_layer3;
  float phi_st2_layer6;
  float phi_st3_layer1;
  float phi_st3_layer3;
  float phi_st3_layer6;
  //GlobalPoint gp_st1, gp_st2, gp_st3, gp_ge11, gp_ge21; 
  GlobalPoint gp_st1, gp_st2, gp_st3, gp_ge11;
  GlobalPoint gp_ge21, gp_st1_layer1, gp_st1_layer6, gp_st2_layer1, gp_st2_layer6;

  //position-based
  float ddY123;
  float deltaY12;
  float deltaY23;

  //direction-based
  float phiM_st1;
  float phiM_st2;
  float phiM_st12;
  float phiM_st23;
  float dPhi_dir_st1_st2;
  float dPhi_dir_st1_st12;
  float dPhi_dir_st2_st23;
  float dPhi_dir_st12_st23;

 private:
  const CSCCorrelatedLCTDigiContainer lcts;
  const CSCDetIdContainer cscids;
  std::map<unsigned int, CSCCorrelatedLCTDigiContainer> chamberid_lcts_;
  std::map<unsigned int, GEMCSCPadDigiContainer> detid_pads_;

};

#endif
