#ifndef GEMCode_GEMValidation_ME0DigiMatcher_h
#define GEMCode_GEMValidation_ME0DigiMatcher_h

/**\class DigiMatcher

 Description: Matching of Digis for SimTrack in ME0

 Author: Vadim Khotilovich, Sven Dildick (TAMU), Tao Huang (TAMU)
*/

#include "Validation/MuonHits/interface/ME0SimHitMatcher.h"
#include "Geometry/GEMGeometry/interface/ME0Geometry.h"
#include "DataFormats/GEMDigi/interface/ME0DigiCollection.h"
#include "DataFormats/GEMDigi/interface/ME0PadDigiCollection.h"

#include <vector>
#include <map>
#include <set>
#include <unordered_set>

typedef std::vector<ME0Digi> ME0DigiContainer;
typedef std::vector<ME0PadDigi> ME0PadDigiContainer;

class ME0DigiMatcher {
public:
  // constructor
  ME0DigiMatcher(edm::ParameterSet const& iPS, edm::ConsumesCollector&& iC);

  // destructor
  ~ME0DigiMatcher() {}

  // initialize the event
  void init(const edm::Event& e, const edm::EventSetup& eventSetup);

  // do the matching
  void match(const SimTrack& t, const SimVertex& v);

  // partition ME0 detIds with digis
  std::set<unsigned int> detIdsDigi() const;
  std::set<unsigned int> detIdsPad() const;

  // chamber detIds with digis
  std::set<unsigned int> chamberIdsDigi() const;
  std::set<unsigned int> chamberIdsPad() const;

  // superchamber detIds with digis
  std::set<unsigned int> superChamberIdsDigi() const;
  std::set<unsigned int> superChamberIdsPad() const;

  // ME0 digis from a particular partition, chamber or superchamber
  const ME0DigiContainer& digisInDetId(unsigned int) const;
  const ME0DigiContainer& digisInChamber(unsigned int) const;
  const ME0DigiContainer& digisInSuperChamber(unsigned int) const;

  // ME0 pads from a particular partition, chamber or superchamber
  const ME0PadDigiContainer& padsInDetId(unsigned int) const;
  const ME0PadDigiContainer& padsInChamber(unsigned int) const;
  const ME0PadDigiContainer& padsInSuperChamber(unsigned int) const;

  const std::map<unsigned int, ME0PadDigiContainer> allPads() const { return detid_to_pads_; }

  // #layers with digis from this simtrack
  int nLayersWithDigisInSuperChamber(unsigned int) const;
  int nLayersWithPadsInSuperChamber(unsigned int) const;

  /// How many pads in ME0 did this simtrack get in total?
  int nPads() const;

  /// How many coincidence pads in ME0 did this simtrack get in total?
  int nCoPads() const;

  std::set<int> stripNumbersInDetId(unsigned int) const;
  std::set<int> padNumbersInDetId(unsigned int) const;

  int extrapolateHsfromME0Pad(unsigned int, int) const;
  int extrapolateHsfromME0Strip(unsigned int, int) const;

  // what unique partitions numbers with digis from this simtrack?
  std::set<int> partitionNumbers() const;

  GlobalPoint getGlobalPointDigi(unsigned int rawId, const ME0Digi& d) const;
  GlobalPoint getGlobalPointPad(unsigned int rawId, const ME0PadDigi& tp) const;

  // get the ME0SimHitMatcher
  std::shared_ptr<ME0SimHitMatcher> muonSimHitMatcher() { return muonSimHitMatcher_; }

private:
  void matchDigisToSimTrack(const ME0DigiCollection&);
  void matchPadsToSimTrack(const ME0PadDigiCollection&);

  edm::EDGetTokenT<ME0DigiCollection> me0DigiToken_;
  edm::EDGetTokenT<ME0PadDigiCollection> me0PadToken_;

  edm::Handle<ME0DigiCollection> me0DigisH_;
  edm::Handle<ME0PadDigiCollection> me0PadsH_;

  std::shared_ptr<ME0SimHitMatcher> muonSimHitMatcher_;

  //edm::ESHandle<ME0Geometry> me0_geom_;
  edm::ESGetToken<ME0Geometry, MuonGeometryRecord> me0Token_;
  const ME0Geometry* me0Geometry_;

  template <class T>
  std::set<unsigned int> selectDetIds(const T&, int) const;

  int minBXDigi_, maxBXDigi_;
  int minBXPad_, maxBXPad_;

  int matchDeltaStrip_;

  bool verboseDigi_;
  bool verbosePad_;
  bool runDigi_;
  bool runPad_;

  std::map<unsigned int, ME0DigiContainer> detid_to_digis_;
  std::map<unsigned int, ME0DigiContainer> chamber_to_digis_;
  std::map<unsigned int, ME0DigiContainer> superchamber_to_digis_;

  std::map<unsigned int, ME0PadDigiContainer> detid_to_pads_;
  std::map<unsigned int, ME0PadDigiContainer> chamber_to_pads_;
  std::map<unsigned int, ME0PadDigiContainer> superchamber_to_pads_;

  ME0DigiContainer no_me0_digis_;
  ME0PadDigiContainer no_me0_pads_;
};

#endif
