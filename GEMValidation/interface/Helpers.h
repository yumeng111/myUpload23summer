#ifndef GEMCode_GEMValidation_Helpers_h
#define GEMCode_GEMValidation_Helpers_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/CSCTriggerNumbering.h"
#include "DataFormats/MuonDetId/interface/GEMDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "DataFormats/MuonDetId/interface/ME0DetId.h"
#include "DataFormats/MuonDetId/interface/DTWireId.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"


static const float AVERAGE_GEM_Z(568.6); // [cm]

static const float AVERAGE_GE11_ODD_Z(568.6); // [cm]
static const float AVERAGE_GE11_EVEN_Z(568.6); // [cm]

static const float AVERAGE_GE21_LONG_Z(568.6); // [cm]
static const float AVERAGE_GE21_SHORT_Z(568.6); // [cm]

static const float AVERAGE_ME11_EVEN_Z(585); // [cm]
static const float AVERAGE_ME11_ODD_Z(615); // [cm]

static const float AVERAGE_ME21_EVEN_Z(820); // [cm]
static const float AVERAGE_ME21_ODD_Z(835); // [cm]

static const float AVERAGE_ME0_Z(568.6); // [cm]

/// Muon Subsystem
enum MuonType {DT= 1, CSC=2, RPC=3, GEM=4, ME0=5};

/// CSC chamber types, according to CSCDetId::iChamberType()
enum CSCType {CSC_ALL = 0, CSC_ME11,
	      CSC_ME1a, CSC_ME1b, CSC_ME12, CSC_ME13, CSC_ME21, 
	      CSC_ME22, CSC_ME31, CSC_ME32, CSC_ME41, CSC_ME42,
	      CSC_ME1, CSC_ME2, CSC_ME3, CSC_ME4};

/// GEM chamber types
enum GEMType {GEM_ALL = 0, 
	      GEM_ME11, GEM_ME21};

/// RPC endcap chamber types -- FIXME
enum RPCType {RPC_ALL = 0, 
	      RPC_ME12, RPC_ME13, RPC_ME22, RPC_ME23, RPC_ME31, 
	      RPC_ME32, RPC_ME33, RPC_ME41, RPC_ME42, RPC_ME43,
	      RPC_MB01,  RPC_MB02,  RPC_MB03,  RPC_MB04, 
	      RPC_MB11p, RPC_MB12p, RPC_MB13p, RPC_MB14p, 
	      RPC_MB21p, RPC_MB22p, RPC_MB23p, RPC_MB24p, 
	      RPC_MB11n, RPC_MB12n, RPC_MB13n, RPC_MB14n, 
	      RPC_MB21n, RPC_MB22n, RPC_MB23n, RPC_MB24n,
	      RPC_ME1, RPC_ME2, RPC_ME3, RPC_ME4,
	      RPC_MB1, RPC_MB2, RPC_MB3, RPC_MB4};

/// DT chamber types -- FIXME
enum DTType {DT_ALL = 0, 
	     DT_MB01,  DT_MB02,  DT_MB03,  DT_MB04, 
	     DT_MB11p, DT_MB12p, DT_MB13p, DT_MB14p, 
	     DT_MB21p, DT_MB22p, DT_MB23p, DT_MB24p, 
	     DT_MB11n, DT_MB12n, DT_MB13n, DT_MB14n, 
	     DT_MB21n, DT_MB22n, DT_MB23n, DT_MB24n,
	     DT_MB1, DT_MB2, DT_MB3, DT_MB4};

const double ME11GEMdPhi[8][3] = {
		{-2, 1.0, 1.0},
		{5.0,  0.02131422,  0.00907379 },
		{7.0,  0.01480166,  0.00658598 },
		{10.0,  0.01019511,  0.00467867 },
		{15.0,  0.00685720,  0.00336636 },
		{20.0,  0.00528981,  0.00279064 },
		{30.0,  0.00381797,  0.00231837 },
		{40.0,  0.00313074,  0.00213513 },
    };
const double ME21GEMdPhi[8][3] = {
		{-2, 1.0, 1.0},
		{5.0,  0.00884066,  0.00479478 },
		{7.0,  0.00660301,  0.00403733 },
		{10.0,  0.00503144,  0.00369953 },
		{15.0,  0.00409270,  0.00358023 },
		{20.0,  0.00378257,  0.00358023 },
		{30.0,  0.00369842,  0.00358023 },
		{40.0,  0.00369842,  0.00358023 },
    };

namespace gemvalidation
{
inline bool 
is_dt(unsigned int detId) 
{
  return (DetId(detId)).subdetId() == MuonSubdetId::DT;
}

inline bool 
is_gem(unsigned int detId) 
{
  return (DetId(detId)).subdetId() == MuonSubdetId::GEM;
}

inline bool 
is_csc(unsigned int detId) 
{
  return (DetId(detId)).subdetId() == MuonSubdetId::CSC;
}

inline bool 
is_rpc(unsigned int detId) 
{
  return (DetId(detId)).subdetId() == MuonSubdetId::RPC;
}

inline bool 
is_me0(unsigned int detId) 
{
  return (DetId(detId)).subdetId() == MuonSubdetId::ME0;
}

int 
chamber(const DetId& id);

unsigned int
gemDetFromCSCDet(unsigned int id,int layer);

float 
cscHalfStripWidth(CSCDetId id);

std::pair<unsigned int, unsigned int> 
gemDetsFromCSCDet(unsigned int id);

// return MuonType for a particular DetId
int toGEMType(int st, int ri);
int toRPCType(int re, int st, int ri);
int toDTType(int wh, int st);
int toCSCType(int st, int ri);

std::string toGEMTypeString(int st, int ri);
std::string toRPCTypeString(int re, int st, int ri);
std::string toDTTypeString(int wh, int st);
std::string toCSCTypeString(int st, int ri);

bool PtOrder(const reco::GenParticle* p1, const reco::GenParticle* p2);

template<typename PROD>
bool
getByLabel(std::vector<edm::InputTag> const& tags, edm::Handle<PROD>& result, const edm::Event& iEvent)
{
  const bool verbose(false);
  bool inputTagIsNotValid(true);
  for (unsigned i=0; i<tags.size(); ++i){
    iEvent.getByLabel(tags[i], result);
    if (result.isValid()) {
      if (verbose) std::cout << tags[i] << " is a valid inputTag " << i << std::endl;
      inputTagIsNotValid = false;
      break;
    } else {
      if (verbose) std::cout << tags[i] << " is an invalid inputTag " << i << std::endl;
    }
  }
  return (!inputTagIsNotValid);
}
}

/* std::vector<std::vector<int> > pat0 {  */
/*   { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*     { 999, 999, 999, 999, 999}, */
/*       {999},            // pid=0: no pattern found */
/*         {999, 999, 999, 999, 999}, */
/*           {999, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*             {999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat1 {  */
/*   {   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}, */
/*     {1,   1,   1,   1,   1}, */
/*       {2},             // pid=1: layer-OR trigger */
/*         {3,   3,   3,   3,   3}, */
/*           {4,   4,   4,   4,   4,   4,   4,   4,   4}, */
/*             {5,   5,   5,   5,   5,   5,   5,   5,   5,   5,   5} */
/* }; */

/* std::vector<std::vector<int> > pat2 {  */
/*   { 999, 999, 999, 999, 999, 999, 999, 999,   0,   0,   0}, */
/*     {999, 999, 999,   1,   1}, */
/*       {2},             // pid=2: right-bending (large) */
/*         {3,   3,   3, 999, 999}, */
/*           {4,   4,   4, 999, 999, 999, 999, 999, 999}, */
/*             {5,   5,   5, 999, 999, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat3 { */
/*   {   0,   0,   0, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*     {1,   1, 999, 999, 999}, */
/*       {2},             // pid=3: left-bending (large) */
/*         {999, 999,   3,   3,   3}, */
/*           {999, 999, 999, 999, 999, 999,   4,   4,   4}, */
/*             {999, 999, 999, 999, 999, 999, 999, 999,   5,   5,   5} */
/* };  */

/* std::vector<std::vector<int> > pat4 { */
/*   { 999, 999, 999, 999, 999, 999, 999,   0,   0,   0, 999}, */
/*     {999, 999, 999,   1,   1}, */
/*       {2},             // pid=4: right-bending (medium) */
/*         {3,   3, 999, 999, 999}, */
/*           {4,   4,   4, 999, 999, 999, 999, 999, 999}, */
/*             {999,   5,   5,   5, 999, 999, 999, 999, 999, 999, 999} */
            
/* }; */

/* std::vector<std::vector<int> > pat5 { */
/*   { 999,   0,   0,   0, 999, 999, 999, 999, 999, 999, 999}, */
/*     {1,   1, 999, 999, 999}, */
/*       {2},             // pid=5: left-bending (medium) */
/*         {999, 999, 999,   3,   3}, */
/*           {999, 999, 999, 999, 999, 999,   4,   4,   4}, */
/*             {999, 999, 999, 999, 999, 999, 999,   5,   5,   5, 999} */
            
/* }; */

/* std::vector<std::vector<int> > pat6 { */
/*   { 999, 999, 999, 999, 999, 999,   0,   0,   0, 999, 999}, */
/*     {999, 999,   1,   1, 999}, */
/*       {2},             // pid=6: right-bending (medium) */
/*         {999,   3,   3, 999, 999}, */
/*           {999, 999,   4,   4, 999, 999, 999, 999, 999}, */
/*             {999, 999,   5,   5,   5, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat7 { */
/*   { 999, 999,   0,   0,   0, 999, 999, 999, 999, 999, 999}, */
/*     {999,   1,   1, 999, 999}, */
/*       {2},             // pid=7: left-bending (medium) */
/*         {999, 999,   3,   3, 999}, */
/*           {999, 999, 999, 999, 999,   4,   4, 999, 999}, */
/*             {999, 999, 999, 999, 999, 999,   5,   5,   5, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat8 { */
/*   { 999, 999, 999, 999, 999,   0,   0,   0, 999, 999, 999}, */
/*     {999, 999,   1,   1, 999}, */
/*       {2},             // pid=8: right-bending (small) */
/*         {999,   3,   3, 999, 999}, */
/*           {999, 999,   4,   4,   4, 999, 999, 999, 999}, */
/*             {999, 999, 999,   5,   5,   5, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat9 { */
/*   { 999, 999, 999,   0,   0,   0, 999, 999, 999, 999, 999}, */
/*     {999,   1,   1, 999, 999}, */
/*       {2},             // pid=9: left-bending (small) */
/*         {999, 999,   3,   3, 999}, */
/*           {999, 999, 999, 999,   4,   4,   4, 999, 999}, */
/*             {999, 999, 999, 999, 999,   5,   5,   5, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > patA { */
/*   { 999, 999, 999, 999,   0,   0,   0, 999, 999, 999, 999}, */
/*     {999, 999,   1, 999, 999}, */
/*       {2},             // pid=A: straight-through */
/*         {999, 999,   3, 999, 999}, */
/*           {999, 999, 999,   4,   4,   4, 999, 999, 999}, */
/*             {999, 999, 999, 999,   5,   5,   5, 999, 999, 999, 999} */
/* }; */

/* std::vector< std::vector<std::vector<int> > > patIndexToPattern { */
/*   pat0, pat1, pat2, pat3, pat4, pat5, pat6, pat7, pat8, pat9, patA */
/* }; */

/* std::vector<std::vector<int> > pat0delta {  */
/*   { 999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*     { 999, 999, 999, 999, 999}, */
/*       {999},            // pid=999: no pattern found */
/*         {999, 999, 999, 999, 999}, */
/*           {999, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*             {999, 999, 999, 999, 999, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat1delta {  */
/*   { -5,   -4,   -3,   -2,   -1,   1,   1,   2,   3,   4,   5}, */
/*     {-2,   -1,   0,   1,   2}, */
/*       {0},             // pid=1: layer-OR trigger */
/*         {-2,   -1,   0,   1,   2}, */
/*           { -4,   -3,   -2,   -1,   0,   1,   2,   3,   4}, */
/*             { -5,   -4,   -3,   -2,   -1,   0,   1,   2,   3,   4,   5}, */
/* }; */

/* std::vector<std::vector<int> > pat2delta {  */
/*   { 999, 999, 999, 999, 999, 999, 999, 999,   3,   4,   5}, */
/*     {999, 999, 999,   1,   2}, */
/*       {0},             // pid=2: right-bending (large) */
/*         {-2,   -1,   0, 999, 999}, */
/*           {-4,   -3,   -2, 999, 999, 999, 999, 999, 999}, */
/*             {-5,   -4,   -3, 999, 999, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat3delta { */
/*   {   -5,   -4,   -3, 999, 999, 999, 999, 999, 999, 999, 999}, */
/*     {-2,   -1, 999, 999, 999}, */
/*       {0},             // pid=3: left-bending (large) */
/*         {999, 999,   0,   1,   2}, */
/*           {999, 999, 999, 999, 999, 999,   2,   3,   4}, */
/*             {999, 999, 999, 999, 999, 999, 999, 999,   3,   4,   5} */
/* };  */

/* std::vector<std::vector<int> > pat4delta { */
/*   { 999, 999, 999, 999, 999, 999, 999,   2,   3,   4, 999}, */
/*     {999, 999, 999,   1,   2}, */
/*       {0},             // pid=4: right-bending (medium) */
/*         {-2,   -1, 999, 999, 999}, */
/*           {-4,   -3,   -2, 999, 999, 999, 999, 999, 999}, */
/*             {999,   -4,   -3,   -2, 999, 999, 999, 999, 999, 999, 999} */
            
/* }; */

/* std::vector<std::vector<int> > pat5delta { */
/*   { 999,   -4,   -3,   -2, 999, 999, 999, 999, 999, 999, 999}, */
/*     {-2,   -1, 999, 999, 999}, */
/*       {0},             // pid=5: left-bending (medium) */
/*         {999, 999, 999,   1,   2}, */
/*           {999, 999, 999, 999, 999, 999,   2,   3,   4}, */
/*             {999, 999, 999, 999, 999, 999, 999,   2,   3,   4, 999} */
            
/* }; */

/* std::vector<std::vector<int> > pat6delta { */
/*   { 999, 999, 999, 999, 999, 999,   1,   2,   3, 999, 999}, */
/*     {999, 999,   0,   1, 999}, */
/*       {0},             // pid=6: right-bending (medium) */
/*         {999,   -1,   0, 999, 999}, */
/*           {999, 999,   -2,   -1, 999, 999, 999, 999, 999}, */
/*             {999, 999,   -3,   -2,   -1, 999, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat7delta { */
/*   { 999, 999,   -3,   -2,   -1, 999, 999, 999, 999, 999, 999}, */
/*     {999,   -1,   0, 999, 999}, */
/*       {0},             // pid=7: left-bending (medium) */
/*         {999, 999,   0,   1, 999}, */
/*           {999, 999, 999, 999, 999,   1,   2, 999, 999}, */
/*             {999, 999, 999, 999, 999, 999,  1,  2,   3, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat8delta { */
/*   { 999, 999, 999, 999, 999,   0,   1,   2, 999, 999, 999}, */
/*     {999, 999,   0,   1, 999}, */
/*       {0},             // pid=8: right-bending (small) */
/*         {999,   -1,   0, 999, 999}, */
/*           {999, 999,   -2,   -1,   0, 999, 999, 999, 999}, */
/*             {999, 999, 999,   -2,   -1,   0, 999, 999, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > pat9delta { */
/*   { 999, 999, 999,   -2,   -1,   0, 999, 999, 999, 999, 999}, */
/*     {999,   -1,   0, 999, 999}, */
/*       {0},             // pid=9: left-bending (small) */
/*         {999, 999,   0,   1, 999}, */
/*           {999, 999, 999, 999,   0,   1,   2, 999, 999}, */
/*             {999, 999, 999, 999, 999,   0,   1,   2, 999, 999, 999} */
/* }; */

/* std::vector<std::vector<int> > patAdelta { */
/*   { 999, 999, 999, 999,   -1,   0,   1, 999, 999, 999, 999}, */
/*     {999, 999,   0, 999, 999}, */
/*       {0},             // pid=A: straight-through */
/*         {999, 999,   0, 999, 999}, */
/*           {999, 999, 999,   -1,   0,   1, 999, 999, 999}, */
/*             {999, 999, 999, 999,   -1,   0,   1, 999, 999, 999, 999} */
/* }; */

/* std::vector< std::vector<std::vector<int> > > patIndexToPatternDelta { */
/*   pat0delta, pat1delta, pat2delta, pat3delta, pat4delta, pat5delta, pat6delta, pat7delta, pat8delta, pat9delta, patAdelta */
/* }; */

#endif
