#ifndef GEMCode_GEMValidation_DisplacedMuonTriggerPtassignment_cc
#define GEMCode_GEMValidation_DisplacedMuonTriggerPtassignment_cc

/**\class DisplacedMuonTriggerPtassignment

  Displaced Muon Trigger Design based on Muon system

Author: tao.huang@cern.ch

*/
#include "DataFormats/Math/interface/deltaPhi.h"
#include "GEMCode/GEMValidation/interface/DisplacedMuonTriggerPtassignment.h"
#include "GEMCode/GEMValidation/interface/PtassignmentHelper.h"
#include "GEMCode/GEMValidation/interface/CSCStubPatterns.h"
//#include "MuJetAnalysis/DisplacedL1MuFilter/plugins/CSCStubPatterns.h"


int
getHalfStrip(const CSCComparatorDigi& digi) 
{
  return (digi.getStrip() - 1) * 2 + digi.getComparator();
}

float 
getFractionalStrip(const CSCComparatorDigi&d)
{
  return d.getStrip() + d.getComparator()/2. - 3/4.;
}

  //endcap, we need LCTs and associated cscid, gempads and associated gemid, and all gemometry
  //to get position from fitting, we also need all comparator digis
  //step0 get LCTs and associated cscids, GEMPads and associated gemids, and geometry. 
  //step1 get fitting positions from fitting compara digis after assoicating comparator digis to LCTs
  //step2 calculate all variables used pt assignment, requires eta,phi,radius,Z
  //step3 assgin L1 pt according to LUTs (in short future)
DisplacedMuonTriggerPtassignment::DisplacedMuonTriggerPtassignment(const CSCCorrelatedLCTDigiCollection* lcts, const edm::EventSetup& es, const edm::Event& ev)
: ev_(ev), es_(es), verbose_(0)
{

  setupGeometry(es);
}

DisplacedMuonTriggerPtassignment::DisplacedMuonTriggerPtassignment(const CSCCorrelatedLCTDigiContainer lcts, const CSCDetIdContainer cscids, const edm::EventSetup& es, const edm::Event& ev)
: ev_(ev), es_(es), verbose_(0)
{

  setupGeometry(es);
}

//chamberid_lcts: LCTs matched to simmuon and their associated chamberid, detid_pads: gempads matched to simmuon and their associated detid_pads
DisplacedMuonTriggerPtassignment::DisplacedMuonTriggerPtassignment(std::map<unsigned int, CSCCorrelatedLCTDigiContainer> chamberid_lcts, std::map<unsigned int, GEMCSCPadDigiContainer> detid_pads, const edm::EventSetup& es, const edm::Event& ev)
: ev_(ev), es_(es), verbose_(0)
{

  setupGeometry(es);
  chamberid_lcts_ = chamberid_lcts;
  detid_pads_ = detid_pads;
  ev.getByLabel("simMuonCSCDigis", "MuonCSCComparatorDigi", hCSCComparators);

  initBoolVariables(); 
  bool isEven[4]={false, false, false, false};
  for (auto idlcts : chamberid_lcts_){
  	CSCDetId chid(idlcts.first);
	if (chid.station()==2 ) meRing = chid.ring();
	if (chid.chamber()%2 == 0) isEven[chid.station()-1] = true;
	if (chid.station() == 1 and idlcts.second.size()>0 ) hasStub_st1 = true;
	else if (chid.station() == 2 and idlcts.second.size()>0 ) hasStub_st2 = true;
	else if (chid.station() == 3 and idlcts.second.size()>0 ) hasStub_st3 = true;
	else if (chid.station() == 4 and idlcts.second.size()>0 ) hasStub_st4 = true;
	else {
	    std::cout <<" chid "<< chid <<"  number of lcts "<< idlcts.second.size() << std::endl;
	    continue;
        }
	globalPositionOfLCT(idlcts.second[0], chid);
	if (chid.station() == 1 or chid.station()==2){
        //find GEMPads	    
		for (auto idgempads : detid_pads){
			GEMDetId gemid(idgempads.first);
			if (((chid.station() == 1 and gemid.station() == 1) or (chid.station()==2 and gemid.station() ==3)) 
				and chid.chamber() == gemid.chamber()){
			    if (gemid.station() == 1 ) hasGEMPad_st1 = true;
			    else if (gemid.station() == 3) hasGEMPad_st2 = true;
			    else 
				std::cout <<" gemid "<< gemid <<" CSC chamber id "<< chid << std::endl;
			    //maybe also check the dR(csc, gem)
			    globalPositionOfGEMPad(idgempads.second[0], gemid);
			}
		}
	}
  }
  //npar>=0 is the flag to do pt assignment
  if (hasStub_st1 and hasStub_st2 and hasStub_st3){
  	if (not(isEven[0]) and isEven[1] and isEven[2]) npar = 0;
	else if (not(isEven[0]) and not(isEven[1]) and not(isEven[2])) npar = 1;
	else if (isEven[0] and isEven[1] and isEven[2]) npar = 2;
	else if (isEven[0] and not(isEven[1]) and not(isEven[2])) npar = 3;
	else {
	    std::cout <<" hasStub in station 1 2 3  but npar = -1 "<< std::endl;
	    npar= -1;
	}
  }else
      npar = -1;


}

DisplacedMuonTriggerPtassignment::DisplacedMuonTriggerPtassignment(GlobalPoint gp1, GlobalPoint gp2, GlobalPoint gp3, GlobalPoint gp4, GlobalPoint gp5, GlobalPoint gp6, int npar_in, const edm::EventSetup& es, const edm::Event& ev)
: ev_(ev), es_(es), verbose_(0)
{ //sim level

  setupGeometry(es);
  initBoolVariables(); 
  gp_st1 = GlobalPoint(gp1);
  gp_st2 = GlobalPoint(gp2);
  gp_st3 = GlobalPoint(gp3);
  gp_ge11 = GlobalPoint(gp4);
  gp_ge21 = GlobalPoint(gp5);
  npar = npar_in;
  phi_ge21 = gp_ge21.phi();
}


//DisplacedMuonTriggerPtassignment::DisplacedMuonTriggerPtassignment(){ //test constructor
//}

DisplacedMuonTriggerPtassignment::~DisplacedMuonTriggerPtassignment(){


}

void DisplacedMuonTriggerPtassignment::initBoolVariables()
{

  hasStub_st1 = false; 
  hasStub_st2 = false; 
  hasStub_st3 = false; 
  hasStub_st4 = false; 
  hasGEMPad_st1 = false; 
  hasGEMPad_st2 = false; 

}


void DisplacedMuonTriggerPtassignment::setupGeometry(const edm::EventSetup& es)
{

  /// get the geometry
  hasGEMGeometry_ = true;
  hasRPCGeometry_ = true;
  hasCSCGeometry_ = true;
  hasME0Geometry_ = true;
  hasDTGeometry_ = true;

  try {
    es.get<MuonGeometryRecord>().get(gem_geom_);
    gemGeometry_ = &*gem_geom_;
  } catch (edm::eventsetup::NoProxyException<GEMGeometry>& e) {
    hasGEMGeometry_ = false;
    std::cout << "+++ Info: GEM geometry is unavailable. +++\n";
  }

  try {
    es.get<MuonGeometryRecord>().get(me0_geom_);
    me0Geometry_ = &*me0_geom_;
  } catch (edm::eventsetup::NoProxyException<ME0Geometry>& e) {
    hasME0Geometry_ = false;
    std::cout << "+++ Info: ME0 geometry is unavailable. +++\n";
  }

  try {
    es.get<MuonGeometryRecord>().get(csc_geom_);
    cscGeometry_ = &*csc_geom_;
  } catch (edm::eventsetup::NoProxyException<CSCGeometry>& e) {
    hasCSCGeometry_ = false;
    std::cout << "+++ Info: CSC geometry is unavailable. +++\n";
  }

  try {
    es.get<MuonGeometryRecord>().get(rpc_geom_);
    rpcGeometry_ = &*rpc_geom_;
  } catch (edm::eventsetup::NoProxyException<RPCGeometry>& e) {
    hasRPCGeometry_ = false;
    std::cout << "+++ Info: RPC geometry is unavailable. +++\n";
  }

  try {
    es.get<MuonGeometryRecord>().get(dt_geom_);
    dtGeometry_ = &*dt_geom_;
  } catch (edm::eventsetup::NoProxyException<DTGeometry>& e) {
    hasDTGeometry_ = false;
    std::cout << "+++ Info: DT geometry is unavailable. +++\n";
  }


}

void DisplacedMuonTriggerPtassignment::fitComparatorsLCT(const CSCComparatorDigiCollection& hCSCComparators, const CSCCorrelatedLCTDigi& stub,
	                                  CSCDetId ch_id, float& fit_phi_layer1, float& fit_phi_layer3, float& fit_phi_layer6, 
					  float& fit_z_layer1, float& fit_z_layer3, float& fit_z_layer6, float& perp)
{

  auto cscChamber = cscGeometry_->chamber(ch_id);
  
  // fetch the CSC comparator digis in this chamber
  CSCComparatorDigiContainerIds compDigisIds;
  for (int iLayer=1; iLayer<=6; ++iLayer){
    CSCDetId layerId(ch_id.endcap(), ch_id.station(), ch_id.ring(), ch_id.chamber(), iLayer);
    // get the digis per layer
    auto compRange = hCSCComparators.get(layerId);
    CSCComparatorDigiContainer compDigis;
    for (auto compDigiItr = compRange.first; compDigiItr != compRange.second; compDigiItr++) {
      auto compDigi = *compDigiItr;
      //if (stub.getTimeBin() < 4 or stub.getTimeBin() > 8) continue;
      int stubHalfStrip(getHalfStrip(compDigi));
      // these comparator digis never fit the pattern anyway!
      if (std::abs(stubHalfStrip-stub.getStrip())>5) continue;
      // check if this comparator digi fits the pattern
      //if(verbose) std::cout << "Comparator digi L1Mu " << layerId << " " << compDigi << " HS " << stubHalfStrip << " stubKeyHS " << stub.getStrip() << std::endl; 
      if (comparatorInLCTPattern(stub.getStrip(), stub.getPattern(), iLayer, stubHalfStrip)) {
        //if(verbose) std::cout<<"\tACCEPT"<<std::endl;
        compDigis.push_back(compDigi);
      }
      // else{
      //   if(verbose) std::cout<<"\tDECLINE!"<<std::endl;
      // }
    }
    // if(verbose) if (compDigis.size() > 2) std::cout << ">>> INFO: " << compDigis.size() << " matched comp digis in this layer!" << std::endl;
    compDigisIds.push_back(std::make_pair(layerId, compDigis));
  }
  
  // get the z and phi positions
  perp = 0.0;
  std::vector<float> phis;
  std::vector<float> zs;
  std::vector<float> ephis;
  std::vector<float> ezs;
  std::vector<float> status;
  for (auto p: compDigisIds){
    auto detId = p.first;
    float phi_tmp = 0.0;
    float perp_tmp = 0.0;
    float z_tmp = 0.0;
    for (auto hit: p.second){
      float fractional_strip = getFractionalStrip(hit);
      auto layer_geo = cscChamber->layer(detId.layer())->geometry();
      float wire = layer_geo->middleWireOfGroup(stub.getKeyWG() + 1);
      LocalPoint csc_intersect = layer_geo->intersectionOfStripAndWire(fractional_strip, wire);
      GlobalPoint csc_gp = cscGeometry_->idToDet(detId)->surface().toGlobal(csc_intersect);
      float gpphi = csc_gp.phi();
      
      if (phis.size()>0 and gpphi>0 and phis[0]<0 and  (gpphi-phis[0])>3.1416)
        phi_tmp += (gpphi-2*3.1415926);
      else if (phis.size()>0 and gpphi<0 and phis[0]>0 and (gpphi-phis[0])<-3.1416)
        phi_tmp += (gpphi+2*3.1415926);
      else
        phi_tmp += (csc_gp.phi());

      z_tmp = csc_gp.z();
      perp_tmp += csc_gp.perp();
    }
    perp = perp_tmp/(p.second).size();
    phi_tmp = phi_tmp/(p.second).size();
    std::cout <<"detid "<< detId <<" perp "<< perp <<" phi "<< phi_tmp <<" z "<< z_tmp << std::endl;
    phis.push_back(phi_tmp);
    zs.push_back(z_tmp);
    ezs.push_back(0);
    // phis.push_back(csc_gp.phi());
    ephis.push_back(gemvalidation::cscHalfStripWidth(detId)/sqrt(12));
  }
  
  perp = perp/phis.size();
  // do a fit to the comparator digis
  float alpha = 0., beta = 0.;
  calculateAlphaBeta(zs, phis, ezs, ephis, status, alpha, beta);
  fit_z_layer3 = cscChamber->layer(CSCConstants::KEY_CLCT_LAYER)->centerOfStrip(20).z();
  
  fit_phi_layer3 = normalizePhi(alpha + beta * fit_z_layer3);
  
  if(verbose_>0) {
    std::cout << "Number of comparator digis used in the fit " << ezs.size() << std::endl;
    std::cout << "best CSC stub fit phi position (L1Only) " << fit_z_layer3 << " " << fit_phi_layer3 << std::endl;
  }

  // calculate the z position in L1 and L6
  float l1_z = cscChamber->layer(1)->centerOfStrip(20).z();
  float l6_z = cscChamber->layer(6)->centerOfStrip(20).z();
  fit_z_layer1 = l1_z;
  fit_z_layer6 = l6_z;
  fit_phi_layer1 = normalizePhi(alpha + beta * l1_z);
  fit_phi_layer6 = normalizePhi(alpha + beta * l6_z);
}



void DisplacedMuonTriggerPtassignment::globalPositionOfLCT(const CSCCorrelatedLCTDigi stub, CSCDetId chid)
{
  float fit_phi_layer1, fit_phi_layer3, fit_phi_layer6;
  float fit_z_layer1, fit_z_layer3, fit_z_layer6;
  float perp;
  fitComparatorsLCT(*hCSCComparators.product(), stub, chid, fit_phi_layer1, fit_phi_layer3, fit_phi_layer6, fit_z_layer1, fit_z_layer3, fit_z_layer6, perp);
  //gp calculated here can have negative Z!!!
  if (chid.station() == 1){
      gp_st1_layer1 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer1, fit_z_layer1));
      gp_st1 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer3, fit_z_layer3));
      gp_st1_layer6 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer6, fit_z_layer6));
      std::cout <<"LCT position st1 chid "<< chid <<" gp eta "<< gp_st1.eta()<<" phi "<<gp_st1.phi() << std::endl;
  }else if (chid.station() == 2){
      gp_st2_layer1 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer1, fit_z_layer1));
      gp_st2 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer3, fit_z_layer3));
      gp_st2_layer6 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer6, fit_z_layer6));
      std::cout <<"LCT position st2 chid "<< chid <<" gp eta "<< gp_st2.eta()<<" phi "<<gp_st2.phi() << std::endl;
  }
  else if (chid.station() == 3){
      gp_st3 = GlobalPoint(GlobalPoint::Cylindrical(perp, fit_phi_layer3, fit_z_layer3));
      std::cout <<"LCT position st3 chid "<< chid <<" gp eta "<< gp_st3.eta()<<" phi "<<gp_st3.phi() << std::endl;
  }else 
      std::cout <<" not in CSC station 1 , 2 ,3 , chamber id  "<< chid << std::endl;
  

}

void DisplacedMuonTriggerPtassignment::globalPositionOfGEMPad(const GEMCSCPadDigi gempad, GEMDetId gemid)
{
  GEMDetId ch_id(gemid.region(), gemid.ring(), gemid.station(), 1, gemid.chamber(), 0);
  const GEMChamber* gemChamber(gemGeometry_->chamber(ch_id.rawId()));
  auto gemRoll(gemChamber->etaPartition(gemid.roll()));//any roll
  const int nGEMPads(gemRoll->npads());
  if (gempad.pad() > nGEMPads or gempad.pad() < 0){
      std::cout <<" gempad.pad() is within pad range gempad "<< gempad <<" npad "<< nGEMPads << std::endl;
      return;
  }

  const LocalPoint lpGEM(gemRoll->centreOfPad(gempad.pad()));
  if (gemid.station() == 1)
  	gp_ge11 = GlobalPoint(gemRoll->toGlobal(lpGEM));
  else if (gemid.station() == 3)
  	gp_ge21 = GlobalPoint(gemRoll->toGlobal(lpGEM));
  else 
      std::cout <<" gemid "<< gemid  <<" not in station 1 or 3" << std::endl;

}



int DisplacedMuonTriggerPtassignment::getEtaPartition(float eta) const
{
    int neta=-1;
    if (fabs(eta)>=1.2 and fabs(eta)<1.4)
	neta=0;
    else if (fabs(eta)>=1.4 and fabs(eta)<1.6)
	neta=1;
    else if (fabs(eta)>=1.6 and fabs(eta)<1.8)
	neta=2;
    else if (fabs(eta)>=1.8 and fabs(eta)<2.0)
	neta=3;
    else if (fabs(eta)>=2.0 and fabs(eta)<2.2)
	neta=4;
    else if (fabs(eta)>=2.2 and fabs(eta)<2.4)
	neta=5;

    return neta;

}

float DisplacedMuonTriggerPtassignment::deltaYcalculation(GlobalPoint gp1, GlobalPoint gp2) const
{
   float anglea = gp2.phi();
   float newyst1 = -gp1.x()*sin(anglea) + gp1.y()*cos(anglea);
   float newyst2 = -gp2.x()*sin(anglea) + gp2.y()*cos(anglea);
   return (newyst2-newyst1);

}


float DisplacedMuonTriggerPtassignment::deltadeltaYcalculation(GlobalPoint gp1, GlobalPoint gp2, GlobalPoint gp3, float eta, int par) const
{

   float anglea = gp2.phi();
   float newyst1 = -gp1.x()*sin(anglea) + gp1.y()*cos(anglea);
   float newyst2 = -gp2.x()*sin(anglea) + gp2.y()*cos(anglea);
	//float newxst3 = gp3.x()*cos(anglea) + gp3.y()*sin(anglea);
   float newyst3 = -gp3.x()*sin(anglea) + gp3.y()*cos(anglea);
   float deltay12 = newyst2-newyst1;
   float deltay23 = newyst3-newyst2;
   //std::cout <<" angle in st2 "<< anglea <<" newyst1 "<< newyst1 <<" newyst2 "<< newyst2 << " newyst3 "<< newyst3 << std::endl;
   int neta = GetEtaPartition(eta);
   
   if (par<0 or par>3 or neta==-1) return -99;
   return (deltay23-PositionEpLUT[par][neta][0]*deltay12);
   
}

float DisplacedMuonTriggerPtassignment::phiMomentum_Xfactor(float phi_CSC, float phi_GEM, float xfactor) const
{


   if (fabs(phi_CSC) > PI or fabs(phi_GEM) > PI) return -9;
   float dphi = deltaPhi(phi_CSC,phi_GEM);
   float y = 1.0-cos(dphi)- xfactor;
      
   float phi_diff = 0.0;
   if (fabs(y) > 0.0) phi_diff = atan(sin(dphi)/y);
   else phi_diff = PI/2.0;

   if (phi_diff <= -PI) phi_diff = phi_diff+2*PI;
   else if (phi_diff > PI) phi_diff = phi_diff-2*PI;

   float phiM = phi_GEM-phi_diff;
   if (phiM <= -PI) phiM = phiM+2*PI;
   else if (phiM > PI) phiM = phiM-2*PI;
     
   //std::cout <<"PhiMomentum_Xfactor: dphi "<< dphi <<" phi_poistion1 "<< phi_GEM <<" phi_position2 "<< phi_CSC <<" Xfactor "<<X <<" phi_diff "<< phi_diff <<" phiM "<< phiM << std::endl;

   return phiM;
}


bool DisplacedMuonTriggerPtassignment::runPositionbased()
{
   if (npar<0)
   	return false;
   std::cout <<" gp1 eta "<< gp_st1.eta() <<" phi "<< gp_st1.phi()<<" gp2 eta "<< gp_st2.eta()<<" phi "<< gp_st2.phi()<<" gp3 eta "<< gp_st3.eta()
       <<" phi "<< gp_st3.phi() << std::endl;
   ddY123 = deltadeltaYcalculation(gp_st1, gp_st2, gp_st3, gp_st2.eta(), npar);
   deltaY12 = deltaYcalculation(gp_st1, gp_st2); 
   deltaY23 = -deltaYcalculation(gp_st3, gp_st2); 
   return true;
}


bool DisplacedMuonTriggerPtassignment::runDirectionbasedGE21()
{
   if (not (npar<4 and npar>=0 and hasGEMPad_st1 and hasGEMPad_st2)) return false; 
   if (fabs(phi_ge21)>4) return false;
   float xfactor_st1 = xfactor*fabs(gp_ge11.z() - gp_st1.z());
   float xfactor_st2 = xfactor*fabs(gp_ge21.z() - gp_st2.z())/(xfactor*fabs(gp_st1.z() - gp_st2.z())+1);
   float xfactor_st12 = xfactor*fabs(gp_st1.z() - gp_st2.z())/(xfactor*fabs(gp_st1.z() - gp_st2.z())+1);
   float xfactor_st23 = xfactor*fabs(gp_st2.z() - gp_st3.z())/(xfactor*fabs(gp_st1.z() - gp_st3.z())+1);
   std::cout <<"DisplacedMuonTrigger meRing "<< meRing <<" xfactor st1 "<< xfactor_st1 <<" xfactor st2 "<< xfactor_st2 << std::endl;
   phiM_st1 = phiMomentum_Xfactor(gp_st1.phi(), gp_ge11.phi(), xfactor_st1);//
   phiM_st2 = phiMomentum_Xfactor(gp_st2.phi(), phi_ge21, xfactor_st2);
   phiM_st12 = phiMomentum_Xfactor(gp_st2.phi(), gp_st1.phi(), xfactor_st12);
   phiM_st23 = phiMomentum_Xfactor(gp_st3.phi(), gp_st2.phi(), xfactor_st23);
   dPhi_dir_st1_st2 = deltaPhi(phiM_st1, phiM_st2);
   dPhi_dir_st1_st12 = deltaPhi(phiM_st1, phiM_st12);
   dPhi_dir_st2_st23 = deltaPhi(phiM_st2, phiM_st23);
   dPhi_dir_st12_st23 = deltaPhi(phiM_st12, phiM_st23);
   return true;
}


bool DisplacedMuonTriggerPtassignment::runDirectionbasedCSConly() 
{
   if (not ( npar<4 and npar>=0 and ((meRing==1 and hasGEMPad_st1) or meRing==2))) return false; 
   float xfactor_st1 = 0;
   if (meRing==1){
   	xfactor_st1 = xfactor*fabs(gp_ge11.z() - gp_st1.z());
   	phiM_st1 = phiMomentum_Xfactor(gp_st1.phi(), gp_ge11.phi(), xfactor_st1);//
	std::cout <<"DisplacedMuonTrigger meRing "<< meRing <<" xfactor st1 "<< xfactor_st1 << std::endl;
   }else if (meRing == 2){
   	xfactor_st1 = xfactor*fabs(gp_st1_layer1.z() - gp_st1_layer6.z())/(xfactor*fabs(gp_st1.z() - gp_st1_layer6.z())+1);
   	phiM_st1 = phiMomentum_Xfactor(gp_st1_layer6.phi(), gp_st1_layer1.phi(), xfactor_st1);//
	std::cout <<"DisplacedMuonTrigger meRing "<< meRing <<" xfactor st1 "<< xfactor_st1 << std::endl;
   }
   float xfactor_st2 = xfactor*fabs(gp_st2_layer1.z() - gp_st2_layer6.z())/(xfactor*fabs(gp_st1.z() - gp_st2_layer6.z())+1);
   float xfactor_st12 = xfactor*fabs(gp_st1.z() - gp_st2.z())/(xfactor*fabs(gp_st1.z() - gp_st2.z())+1);
   float xfactor_st23 = xfactor*fabs(gp_st2.z() - gp_st3.z())/(xfactor*fabs(gp_st1.z() - gp_st2.z())+1);
   phiM_st2 = phiMomentum_Xfactor(gp_st2_layer6.phi(), gp_st2_layer1.phi(), xfactor_st2);
   phiM_st12 = phiMomentum_Xfactor(gp_st2.phi(), gp_st1.phi(), xfactor_st12);
   phiM_st23 = phiMomentum_Xfactor(gp_st3.phi(), gp_st2.phi(), xfactor_st23);
   dPhi_dir_st1_st2 = deltaPhi(phiM_st1, phiM_st2);
   dPhi_dir_st1_st12 = deltaPhi(phiM_st1, phiM_st12);
   dPhi_dir_st2_st23 = deltaPhi(phiM_st2, phiM_st23);
   dPhi_dir_st12_st23 = deltaPhi(phiM_st12, phiM_st23);
   return true;
}



float DisplacedMuonTriggerPtassignment::getlocalPhiDirection(int st) const
{
   if (st==1 and hasStub_st1 and hasGEMPad_st1) return phiM_st1;
   else if (st==2 and hasStub_st2 and hasGEMPad_st2) return phiM_st2;
   else if (st == 12 and hasStub_st1 and hasStub_st2) return phiM_st12;
   else if (st == 23 and hasStub_st2 and hasStub_st3) return phiM_st23;
   else{
   	std::cout <<" error in getlocalPhiDirection, st  "<<st <<" not in range or not not have stub or GEMpad" << std::endl;
	return -99; 
   }
}

float DisplacedMuonTriggerPtassignment::getdeltaPhiDirection(int st1, int st2) const
{

   if (((st1 == 1 and st2 == 2) or (st1 == 1 and st2 == 2)) and hasStub_st1 and hasGEMPad_st1 and hasStub_st2 and hasGEMPad_st2) return dPhi_dir_st1_st2;
   else if (((st1 == 1 and st2 == 12) or (st1 == 12 and st2 == 1)) and hasStub_st1 and hasGEMPad_st1 and hasStub_st2) return dPhi_dir_st1_st12;
   else if (((st1 == 2 and st2 == 23) or (st1 == 23 and st2 == 2)) and hasStub_st2 and hasGEMPad_st2 and hasStub_st3) return dPhi_dir_st2_st23;
   else if (((st1 == 12 and st2 == 23) or (st1 == 23 and st2 == 12)) and hasStub_st1 and hasStub_st2 and hasStub_st3) return dPhi_dir_st12_st23;
   else{
   	std::cout <<" error in getlocalPhiDirection, st1 "<<st1 <<" st2 "<< st2 <<" not in range or not not have stub or GEMpad" << std::endl;
	return -99;
   }

}



#endif
