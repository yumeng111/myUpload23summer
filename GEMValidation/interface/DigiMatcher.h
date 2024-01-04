#ifndef GEMCode_GEMValidation_DigiMatcher_h
#define GEMCode_GEMValidation_DigiMatcher_h

/**\class DigiMatcher

 Description: Base class for matching of CSC or GEM Digis to SimTrack

 Original Author:  "Vadim Khotilovich"
*/

#include "GEMCode/GEMValidation/interface/BaseMatcher.h"
#include "GEMCode/GEMValidation/interface/SimHitMatcher.h"
#include "GEMCode/GEMValidation/interface/GenericDigi.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

class DigiMatcher : public BaseMatcher
{
public:

  typedef matching::Digi Digi;
  typedef matching::DigiContainer DigiContainer;
  typedef std::map<unsigned int, Digi> Id2Digi;

  typedef std::map<unsigned int, DigiContainer> Id2DigiContainer;

  DigiMatcher(SimHitMatcher& sh);
  
  ~DigiMatcher();

  /// calculate Global position for a digi
  /// works for GEM and CSC strip digis
  GlobalPoint digiPosition(const Digi& digi) const;

  /// calculate Global average position for a provided collection of digis
  /// works for GEM and CSC strip digis
  GlobalPoint digisMeanPosition(const DigiContainer& digis) const;

  /// for CSC strip and wire:
  /// first calculate median half-strip and widegroup
  /// then use CSCLayerGeometry::intersectionOfStripAndWire to calculate the intersection
  GlobalPoint digisCSCMedianPosition(const DigiContainer& strip_digis, const DigiContainer& wire_digis) const;

  /// calculate median strip (or wiregroup for wire digis) in a set
  /// assume that the set of digis was from layers of a single chamber
  int median(const DigiContainer& digis) const;

  /// for GEM:
  /// find a GEM digi with its position that is the closest in deltaR to the provided CSC global position
  std::pair<Digi, GlobalPoint>
  digiInGEMClosestToCSC(const DigiContainer& gem_digis, const GlobalPoint& csc_gp) const;

  /// for RPC:
  /// find a RPC digi with its position that is the closest in deltaR to the provided CSC global position
  std::pair<Digi, GlobalPoint>
  digiInRPCClosestToCSC(const DigiContainer& rpc_digis, const GlobalPoint& csc_gp) const;

  const SimHitMatcher* simHitMatcher() const {return simhit_matcher_;}

protected:

  const SimHitMatcher* simhit_matcher_;

  const DigiContainer no_digis_;
};

#endif
