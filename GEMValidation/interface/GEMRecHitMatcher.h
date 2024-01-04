#ifndef GEMCode_GEMValidation_GEMRecHitMatcher_h
#define GEMCode_GEMValidation_GEMRecHitMatcher_h

/**\class RecHitMatcher

 Description: Matching of RecHits for SimTrack in GEM

 Original Author:  "Vadim Khotilovich"
*/

#include "GEMCode/GEMValidation/interface/BaseMatcher.h"

#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/GEMRecHit/interface/GEMRecHitCollection.h"
#include "GEMCode/GEMValidation/interface/GenericDigi.h"
#include "GEMCode/GEMValidation/interface/DigiMatcher.h"

#include <vector>
#include <map>
#include <set>

class SimHitMatcher;
class GEMGeometry;

class GEMRecHitMatcher : BaseMatcher
{
public:

  typedef matching::Digi RecHit;
  typedef matching::DigiContainer RecHitContainer;

  typedef std::vector<GEMRecHit> GEMRecHitContainer;

  GEMRecHitMatcher(SimHitMatcher& sh);
  
  ~GEMRecHitMatcher();

  // partition GEM detIds with rechits
  std::set<unsigned int> detIds() const;

  // chamber detIds with rechits
  std::set<unsigned int> chamberIds() const;

  // superchamber detIds with rechits
  std::set<unsigned int> superChamberIds() const;

  // GEM recHits from a particular partition, chamber or superchamber
  const RecHitContainer& recHitsInDetId(unsigned int) const;
  const RecHitContainer& recHitsInChamber(unsigned int) const;
  const RecHitContainer& recHitsInSuperChamber(unsigned int) const;

  const GEMRecHitContainer& gemRecHitsInDetId(unsigned int) const;
  const GEMRecHitContainer& gemRecHitsInChamber(unsigned int) const;
  const GEMRecHitContainer& gemRecHitsInSuperChamber(unsigned int) const;

  const GEMRecHitContainer gemRecHits() const;

  // #layers with recHits from this simtrack
  int nLayersWithRecHitsInSuperChamber(unsigned int) const;

  /// How many recHits in GEM did this simtrack get in total?
  int nRecHits() const;

  /// How many GEM chambers with minimum number of layer with rechits did this simtrack get?
  int nCoincidenceGEMChambers(int st, int min_n_layers = 2) const;

  bool gemRecHitInContainer(const GEMRecHit&, const GEMRecHitContainer&) const;
  bool isGEMRecHitMatched(const GEMRecHit&) const;
  bool areGEMRecHitSame(const GEMRecHit&, const GEMRecHit&) const;

  std::set<int> stripNumbersInDetId(unsigned int) const;

  // what unique partitions numbers with recHits from this simtrack?
  std::set<int> partitionNumbers() const;

  GlobalPoint recHitPosition(const RecHit& rechit) const;
  GlobalPoint recHitMeanPosition(const RecHitContainer& rechits) const;

private:

  void matchRecHitsToSimTrack(const GEMRecHitCollection& recHits);

  std::vector<edm::InputTag> gemRecHitInput_;

  const SimHitMatcher* simhit_matcher_;

  int minBXGEM_, maxBXGEM_;

  int matchDeltaStrip_;

  std::map<unsigned int, RecHitContainer> detid_to_recHits_;
  std::map<unsigned int, RecHitContainer> chamber_to_recHits_;
  std::map<unsigned int, RecHitContainer> superchamber_to_recHits_;

  std::map<unsigned int, GEMRecHitContainer> detid_to_gemRecHits_;
  std::map<unsigned int, GEMRecHitContainer> chamber_to_gemRecHits_;
  std::map<unsigned int, GEMRecHitContainer> superchamber_to_gemRecHits_;

  const RecHitContainer no_recHits_;
  const GEMRecHitContainer no_gemRecHits_;

  bool verboseGEMRecHit_;
  bool runGEMRecHit_;
};

#endif
