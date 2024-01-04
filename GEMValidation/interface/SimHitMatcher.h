#ifndef GEMCode_GEMValidation_SimHitMatcher_h
#define GEMCode_GEMValidation_SimHitMatcher_h

/**\class SimHitMatcher

 Description: Matching of SimHit for SimTrack in CSC & GEM

 Original Author:  "Vadim Khotilovich"
*/

#include "GEMCode/GEMValidation/interface/BaseMatcher.h"

#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

#include <vector>
#include <map>
#include <set>

class CSCGeometry;
class GEMGeometry;
class ME0Geometry;
class RPCGeometry;
class DTGeometry;

class SimHitMatcher : public BaseMatcher
{
public:
  
  SimHitMatcher(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es);
  
  ~SimHitMatcher();

  /// access to all the Muon SimHits (use MuonSubdetId::SubSystem)
  const edm::PSimHitContainer& simHits(enum MuonType) const;
  /// access to all the GEM SimHits
  const edm::PSimHitContainer& simHitsGEM() const {return gem_hits_;}
  /// access to all the CSC SimHits
  const edm::PSimHitContainer& simHitsCSC() const {return csc_hits_;}
  /// access to all the ME0 SimHits
  const edm::PSimHitContainer& simHitsME0() const {return me0_hits_;}
  /// access to all the RPC SimHits
  const edm::PSimHitContainer& simHitsRPC() const {return rpc_hits_;}
  /// access to all the DT SimHits
  const edm::PSimHitContainer& simHitsDT() const {return dt_hits_;}

  /// GEM partitions' detIds with SimHits
  std::set<unsigned int> detIdsGEM(int gem_type = GEM_ALL) const;
  /// ME0 partitions' detIds with SimHits
  std::set<unsigned int> detIdsME0() const;
  /// RPC partitions' detIds with SimHits
  std::set<unsigned int> detIdsRPC(int rpc_type = RPC_ALL) const;
  /// CSC layers' detIds with SimHits
  /// by default, only returns those from ME1b
  std::set<unsigned int> detIdsCSC(int csc_type = CSC_ALL) const;
  /// DT partitions' detIds with SimHits
  std::set<unsigned int> detIdsDT(int dt_type = DT_ALL) const;

  /// GEM detid's with hits in 2 layers of coincidence pads
  /// those are layer==1 only detid's
  std::set<unsigned int> detIdsGEMCoincidences() const;
  /// RPC detid's with hits in 2 layers of coincidence pads
  /// those are layer==1 only detid's
  std::set<unsigned int> detIdsME0Coincidences(int min_n_layers = 2) const;

  /// GEM chamber detIds with SimHits
  std::set<unsigned int> chamberIdsGEM(int gem_type = GEM_ALL) const;
  /// ME0 chamber detIds with SimHits
  std::set<unsigned int> chamberIdsME0() const;
  /// RPC chamber detIds with SimHits
  std::set<unsigned int> chamberIdsRPC(int rpc_type = RPC_ALL) const;
  /// CSC chamber detIds with SimHits
  std::set<unsigned int> chamberIdsCSC(int csc_type = CSC_ALL) const;
  /// DT chamber detIds with SimHits
  std::set<unsigned int> chamberIdsDT(int dt_type = DT_ALL) const;

  /// DT station detIds with SimHits
  std::set<unsigned int> chamberIdsCSCStation(int station) const;
  /// DT station detIds with SimHits
  std::set<unsigned int> chamberIdsDTStation(int station) const;


  /// GEM superchamber detIds with SimHits
  std::set<unsigned int> superChamberIdsGEM() const;
  /// GEM superchamber detIds with SimHits 2 layers of coincidence pads
  std::set<unsigned int> superChamberIdsGEMCoincidences() const;

  /// ME0 superchamber detIds with SimHits
  std::set<unsigned int> superChamberIdsME0() const;
  /// ME0 superchamber detIds with SimHits >=2 layers of coincidence pads
  std::set<unsigned int> superChamberIdsME0Coincidences(int min_n_layers = 2) const;

  /// DT layer detIds with SimHits
  std::set<unsigned int> layerIdsDT() const;
  /// DT super layer detIds with SimHits
  std::set<unsigned int> superlayerIdsDT() const;

  /// simhits from a particular partition (GEM)/layer (CSC), chamber or superchamber
  const edm::PSimHitContainer& hitsInDetId(unsigned int) const;
  const edm::PSimHitContainer& hitsInChamber(unsigned int) const;
  const edm::PSimHitContainer& hitsInSuperChamber(unsigned int) const;

  // was there a hit in a particular DT/CSC station?
  bool hitStationCSC(int, int) const;
  bool hitStationDT(int, int, int) const;
  bool hitStationGEM(int, int) const;
  bool hitStationRPC(int) const;

  // number of stations with hits in at least X layers
  int nStationsCSC(int nl=4) const;
  int nStationsDT(int nsl=1, int nl=3) const;
  int nStationsRPC() const;
  int nStationsGEM(int nl=2) const;

  int nCellsWithHitsInLayerDT(unsigned int) const;
  int nLayersWithHitsInSuperLayerDT(unsigned int) const;
  int nSuperLayersWithHitsInChamberDT(unsigned int) const;
  int nLayersWithHitsInChamberDT(unsigned int) const;
  const edm::PSimHitContainer& hitsInLayerDT(unsigned int) const;
  const edm::PSimHitContainer& hitsInSuperLayerDT(unsigned int) const;
  const edm::PSimHitContainer& hitsInChamberDT(unsigned int) const;

  /// #layers with hits
  /// for CSC: "super-chamber" means chamber
  int nLayersWithHitsInSuperChamber(unsigned int) const;

  /// How many pads with simhits in GEM did this simtrack get?
  int nPadsWithHits() const;
  /// How many coincidence pads with simhits in GEM did this simtrack get?
  int nCoincidencePadsWithHits() const;

  /// How many ME0 chambers with minimum number of layer with simhits did this simtrack get?
  int nCoincidenceME0Chambers(int min_n_layers = 2) const;

  /// How many CSC chambers with minimum number of layer with simhits did this simtrack get?
  int nCoincidenceCSCChambers(int min_n_layers = 4) const;

  /// calculate Global average position for a provided collection of simhits
  GlobalPoint simHitsMeanPosition(const edm::PSimHitContainer& sim_hits) const;

  /// calculated the fitted position in a given layer for CSC simhits in a chamber
  GlobalPoint simHitPositionKeyLayer(unsigned int chamberid) const;
   
  /// calculate Global average momentum for a provided collection of simhits in CSC
  GlobalVector simHitsMeanMomentum(const edm::PSimHitContainer& sim_hits) const;

  float	simHitsGEMCentralPosition(const edm::PSimHitContainer& sim_hits) const;
  /// 
  float LocalBendingInChamber(unsigned int detid) const;

  /// calculate average strip (strip for GEM/ME0, half-strip for CSC) number for a provided collection of simhits
  float simHitsMeanStrip(const edm::PSimHitContainer& sim_hits) const;

  /// calculate average wg number for a provided collection of simhits (for CSC)
  float simHitsMeanWG(const edm::PSimHitContainer& sim_hits) const;

  /// calculate average wg number for a provided collection of simhits (for DT)
  float simHitsMeanWire(const edm::PSimHitContainer& sim_hits) const;
   


  std::set<int> hitStripsInDetId(unsigned int, int margin_n_strips = 0) const;  // GEM/ME0 or CSC
  std::set<int> hitWiregroupsInDetId(unsigned int, int margin_n_wg = 0) const; // CSC
  std::set<int> hitPadsInDetId(unsigned int) const; // GEM
  std::set<int> hitCoPadsInDetId(unsigned int) const; // GEM coincidence pads with hits
  std::set<unsigned int> hitWiresInDTLayerId(unsigned int, int margin_n_wires = 0) const;  // DT
  std::set<unsigned int> hitWiresInDTSuperLayerId(unsigned int, int margin_n_wires = 0) const;  // DT
  std::set<unsigned int> hitWiresInDTChamberId(unsigned int, int margin_n_wires = 0) const;  // DT

  // what unique partitions numbers were hit by this simtrack?
  std::set<int> hitPartitions() const; // GEM

  void cscChamberIdsToString(const std::set<unsigned int>&) const;
  void dtChamberIdsToString(const std::set<unsigned int>&) const;

private:

  void init();

  std::vector<unsigned int> getIdsOfSimTrackShower(unsigned  trk_id,
      const edm::SimTrackContainer& simTracks, const edm::SimVertexContainer& simVertices);

  void matchCSCSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& csc_hits);
  void matchRPCSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& rpc_hits);
  void matchGEMSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& gem_hits);
  void matchME0SimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& me0_hits);
  void matchDTSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& dt_hits);

  bool simMuOnlyCSC_;
  bool simMuOnlyGEM_;
  bool simMuOnlyRPC_;
  bool simMuOnlyME0_;
  bool simMuOnlyDT_;

  bool discardEleHitsCSC_;
  bool discardEleHitsGEM_;
  bool discardEleHitsRPC_;
  bool discardEleHitsME0_;
  bool discardEleHitsDT_;

  bool runCSCSimHit_;
  bool runGEMSimHit_;
  bool runRPCSimHit_;
  bool runME0SimHit_;
  bool runDTSimHit_;

  std::string simInputLabel_;
  
  std::map<unsigned int, unsigned int> trkid_to_index_;

  edm::PSimHitContainer no_hits_;

  edm::PSimHitContainer csc_hits_;
  std::map<unsigned int, edm::PSimHitContainer > csc_detid_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > csc_chamber_to_hits_;

  edm::PSimHitContainer gem_hits_;
  std::map<unsigned int, edm::PSimHitContainer > gem_detid_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > gem_chamber_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > gem_superchamber_to_hits_;

  edm::PSimHitContainer me0_hits_;
  std::map<unsigned int, edm::PSimHitContainer > me0_detid_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > me0_chamber_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > me0_superchamber_to_hits_;

  edm::PSimHitContainer rpc_hits_;
  std::map<unsigned int, edm::PSimHitContainer > rpc_detid_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > rpc_chamber_to_hits_;

  edm::PSimHitContainer dt_hits_;
  std::map<unsigned int, edm::PSimHitContainer > dt_detid_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > dt_layer_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > dt_superlayer_to_hits_;
  std::map<unsigned int, edm::PSimHitContainer > dt_chamber_to_hits_;

  // detids with hits in pads
  std::map<unsigned int, std::set<int> > gem_detids_to_pads_;
  // detids with hits in 2-layer pad coincidences
  std::map<unsigned int, std::set<int> > gem_detids_to_copads_;

  bool verboseGEM_;
  bool verboseCSC_;
  bool verboseRPC_;
  bool verboseME0_;
  bool verboseDT_;

  std::vector<edm::InputTag> gemSimHitInput_;
  std::vector<edm::InputTag> cscSimHitInput_;
  std::vector<edm::InputTag> rpcSimHitInput_;
  std::vector<edm::InputTag> me0SimHitInput_;
  std::vector<edm::InputTag> dtSimHitInput_;

  edm::Handle<edm::SimTrackContainer> sim_tracks;
  edm::Handle<edm::SimVertexContainer> sim_vertices;
};

#endif
