#include "GEMCode/GEMValidation/interface/GenericDigi.h"

using namespace matching;

int& matching::digi_wg(matching::Digi& d)
{
  if (std::get<3>(d) == CSC_LCT) return std::get<6>(d);
  return std::get<1>(d);
}

int matching::digi_wg(const matching::Digi& d)
{
  if (std::get<3>(d) == CSC_LCT) return std::get<6>(d);
  return std::get<1>(d);
}

std::ostream & operator<<(std::ostream & o, const matching::Digi& d)
{
  unsigned int id = digi_id(d);
  int ch = digi_channel(d);
  int bx = digi_bx(d);
  DigiType t = digi_type(d);
  int q = digi_quality(d);
  int p = digi_pattern(d);
  int wg = digi_wg(d);
  float dphi = digi_dphi(d);
  

  if (t == CSC_CLCT) o<<id<<CSCDetId(id)<<" t: CLCT hs "<<ch<<" bx "<<bx<<" q "<<q<<" p "<<p;
  else if (t == CSC_ALCT) o<<id<<CSCDetId(id)<<" t: ALCT wg "<<ch<<" bx "<<bx<<" q "<<q;
  else if (t == CSC_LCT) o<<id<<CSCDetId(id)<<" t: LCT hs "<<ch<<" wg "<<wg<<" bx "<<bx<<" q "<<q<<" p "<<p<<" dphi "<<dphi;
  else if (t == GEM_PAD) o<<id<<GEMDetId(id)<<" t: GEM Pad  pad "<<ch <<" bx "<<bx;
  else if (t == GEM_COPAD) o<<id<<GEMDetId(id)<<" t: GEM CoPad  pad "<<ch <<" bx "<<bx;
  else if (t == CSC_STRIP) o<<id<<CSCDetId(id)<<" t: Strip Digi  hs "<<ch <<" bx "<<bx;
  else if (t == CSC_WIRE) o<<id<<CSCDetId(id)<<" t: Wire Digi  wg "<<ch <<" bx "<<bx;
  else if (t == RPC_STRIP) o<<id<<RPCDetId(id)<<" t: RPC Digi strip "<<ch <<" bx "<<bx;
  else o<<id<<" t: "<<t<<" ch "<<ch<<" bx "<<bx;

  return o;
}
