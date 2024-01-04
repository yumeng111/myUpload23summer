#include "GEMCode/GEMValidation/interface/SimHitMatcher.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "TF1.h"
#include "TGraph.h"
#include "TFitResult.h"

#include <algorithm>
#include <iomanip>

using namespace std;


SimHitMatcher::SimHitMatcher(const SimTrack& t, const SimVertex& v,
      const edm::ParameterSet& ps, const edm::Event& ev, const edm::EventSetup& es)
: BaseMatcher(t, v, ps, ev, es)
{
  auto gemSimHit_ = conf().getParameter<edm::ParameterSet>("gemSimHit");
  verboseGEM_ = gemSimHit_.getParameter<int>("verbose");
  gemSimHitInput_ = gemSimHit_.getParameter<std::vector<edm::InputTag>>("validInputTags");
  simMuOnlyGEM_ = gemSimHit_.getParameter<bool>("simMuOnly");
  discardEleHitsGEM_ = gemSimHit_.getParameter<bool>("discardEleHits");
  runGEMSimHit_ = gemSimHit_.getParameter<bool>("run");

  auto cscSimHit_= conf().getParameter<edm::ParameterSet>("cscSimHit");
  verboseCSC_ = cscSimHit_.getParameter<int>("verbose");
  cscSimHitInput_ = cscSimHit_.getParameter<std::vector<edm::InputTag>>("validInputTags");
  simMuOnlyCSC_ = cscSimHit_.getParameter<bool>("simMuOnly");
  discardEleHitsCSC_ = cscSimHit_.getParameter<bool>("discardEleHits");
  runCSCSimHit_ = cscSimHit_.getParameter<bool>("run");

  auto me0SimHit_ = conf().getParameter<edm::ParameterSet>("me0SimHit");
  verboseME0_ = me0SimHit_.getParameter<int>("verbose");
  me0SimHitInput_ = me0SimHit_.getParameter<std::vector<edm::InputTag>>("validInputTags");
  simMuOnlyME0_ = me0SimHit_.getParameter<bool>("simMuOnly");
  discardEleHitsME0_ = me0SimHit_.getParameter<bool>("discardEleHits");
  runME0SimHit_ = me0SimHit_.getParameter<bool>("run");

  auto rpcSimHit_ = conf().getParameter<edm::ParameterSet>("rpcSimHit");
  verboseRPC_ = rpcSimHit_.getParameter<int>("verbose");
  rpcSimHitInput_ = rpcSimHit_.getParameter<std::vector<edm::InputTag>>("validInputTags");
  simMuOnlyRPC_ = rpcSimHit_.getParameter<bool>("simMuOnly");
  discardEleHitsRPC_ = rpcSimHit_.getParameter<bool>("discardEleHits");
  runRPCSimHit_ = rpcSimHit_.getParameter<bool>("run");

  auto dtSimHit_ = conf().getParameter<edm::ParameterSet>("dtSimHit");
  verboseDT_ = dtSimHit_.getParameter<int>("verbose");
  dtSimHitInput_ = dtSimHit_.getParameter<std::vector<edm::InputTag>>("validInputTags");
  simMuOnlyDT_ = dtSimHit_.getParameter<bool>("simMuOnly");
  discardEleHitsDT_ = dtSimHit_.getParameter<bool>("discardEleHits");
  runDTSimHit_ = dtSimHit_.getParameter<bool>("run");

  simInputLabel_ = conf().getUntrackedParameter<std::string>("simInputLabel", "g4SimHits");

  init();
}


SimHitMatcher::~SimHitMatcher() {}


void 
SimHitMatcher::init()
{
  event().getByLabel(simInputLabel_, sim_tracks);
  event().getByLabel(simInputLabel_, sim_vertices);

  // fill trkId2Index associoation:
  int no = 0;
  trkid_to_index_.clear();
  for (auto& t: *sim_tracks.product())
  {
    trkid_to_index_[t.trackId()] = no;
    no++;
  }
  vector<unsigned> track_ids = getIdsOfSimTrackShower(trk().trackId(), *sim_tracks.product(), *sim_vertices.product());
  if (verboseSimTrack_) {
    std::cout << "Printing track_ids" << std::endl;
    for (auto id: track_ids) std::cout << "id: " << id << std::endl;
  }
  
  if (hasCSCGeometry_) {
    edm::Handle<edm::PSimHitContainer> csc_hits;  
    if (gemvalidation::getByLabel(cscSimHitInput_, csc_hits, event())) {
      
      // select CSC simhits
      edm::PSimHitContainer csc_hits_select;
      for (auto& h: *csc_hits.product()) {
        CSCDetId id(h.detUnitId());
        if (useCSCChamberType(gemvalidation::toCSCType(id.station(), id.ring()))) csc_hits_select.push_back(h);
      }
      
      if(runCSCSimHit_) {
        matchCSCSimHitsToSimTrack(track_ids, csc_hits_select);
      
	if (verboseCSC_) {
	  cout<<"nSimHits "<<no<<" nTrackIds "<<track_ids.size()<<" nSelectedCSCSimHits "<<csc_hits_select.size()<<endl;
	  cout<<"detids CSC " << detIdsCSC(0).size()<<endl;
	  
	  for (auto id: detIdsCSC(0)) {
	    auto csc_simhits = hitsInDetId(id);
	    auto csc_simhits_gp = simHitsMeanPosition(csc_simhits);
	    auto strips = hitStripsInDetId(id);
	    cout<<"detid "<<CSCDetId(id)<<": "<<csc_simhits.size()<<" "<<csc_simhits_gp.phi()<<" "<< csc_detid_to_hits_[id].size()<<endl;
	    cout<<"nStrip "<<strips.size()<<endl;
	    cout<<"strips : "; std::copy(strips.begin(), strips.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
	  }
	}
      }
    }
  }
  
  if (hasGEMGeometry_) {
    edm::Handle<edm::PSimHitContainer> gem_hits;
    if (gemvalidation::getByLabel(gemSimHitInput_, gem_hits, event())) {      

      // select GEM simhits
      edm::PSimHitContainer gem_hits_select;
      for (auto& h: *gem_hits.product()) {
        GEMDetId id(h.detUnitId());
        if (id.station()==2) continue;
        if (useGEMChamberType(gemvalidation::toGEMType(id.station(), id.ring()))) gem_hits_select.push_back(h);
      }

      if(runGEMSimHit_) {
        matchGEMSimHitsToSimTrack(track_ids, gem_hits_select);
        
        if (verboseGEM_) {
          cout<<"nSimHits "<<no<<" nTrackIds "<<track_ids.size()<<" nSelectedGEMSimHits "<<(*gem_hits.product()).size()<<endl;
          cout << "detids GEM " << detIdsGEM().size() << endl;
          
          auto gem_ch_ids = chamberIdsGEM();
          for (auto id: gem_ch_ids) {
            auto& gem_simhits = hitsInChamber(id);
            auto gem_simhits_gp = simHitsMeanPosition(gem_simhits);
            cout<<"cchid "<<GEMDetId(id)<<": nHits "<<gem_simhits.size()<<" phi "<<gem_simhits_gp.phi()<<" nCh "<< gem_chamber_to_hits_[id].size()<<endl;
            // auto strips = hitStripsInDetId(id);
            // cout<<"nStrip "<<strips.size()<<endl;
            // cout<<"strips : "; std::copy(strips.begin(), strips.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
          }
          auto gem_sch_ids = superChamberIdsGEM();
          for (auto id: gem_sch_ids) {
            auto& gem_simhits = hitsInSuperChamber(id);
            auto gem_simhits_gp = simHitsMeanPosition(gem_simhits);
            cout<<"schid "<<GEMDetId(id)<<": "<<nCoincidencePadsWithHits() <<" | "<<gem_simhits.size()<<" "<<gem_simhits_gp.phi()<<" "<< gem_superchamber_to_hits_[id].size()<<endl;
          }
        }    
      }
    }
  }
  
  if (hasME0Geometry_) {
    edm::Handle<edm::PSimHitContainer> me0_hits;
    if (gemvalidation::getByLabel(me0SimHitInput_, me0_hits, event())) {

      if (runME0SimHit_) {
        matchME0SimHitsToSimTrack(track_ids, *me0_hits.product());
	
        if (verboseME0_) {
          cout<<"nSimHits "<<no<<" nTrackIds "<<track_ids.size()<<" nSelectedME0SimHits "<<(*me0_hits.product()).size()<<endl;
          cout << "detids ME0 " << detIdsME0().size() << endl;
          
          auto me0_ch_ids = chamberIdsME0();
          for (auto id: me0_ch_ids) {
            auto& me0_simhits = hitsInChamber(id);
            auto me0_simhits_gp = simHitsMeanPosition(me0_simhits);
            cout<<"cchid "<<ME0DetId(id)<<": nHits "<<me0_simhits.size()<<" phi "<<me0_simhits_gp.phi()<<" nCh "<< me0_chamber_to_hits_[id].size()<<endl;
            // auto strips = hitStripsInDetId(id);
            // cout<<"nStrip "<<strips.size()<<endl;
            // cout<<"strips : "; std::copy(strips.begin(), strips.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
          }
        }    
      }
    }
  }

  if (hasRPCGeometry_) {
    edm::Handle<edm::PSimHitContainer> rpc_hits;
    if (gemvalidation::getByLabel(rpcSimHitInput_, rpc_hits, event())) {

      // select RPC simhits
      edm::PSimHitContainer rpc_hits_select;
      for (auto& h: *rpc_hits.product()) {
        RPCDetId id(h.detUnitId());
        if (useRPCChamberType(gemvalidation::toRPCType(id.region(), id.station(), id.ring()))) rpc_hits_select.push_back(h);
      }

      if (runRPCSimHit_) {
        matchRPCSimHitsToSimTrack(track_ids, rpc_hits_select);
	
        if (verboseRPC_) {
          cout<<"nSimHits "<<no<<" nTrackIds "<<track_ids.size()<<" nSelectedRPCSimHits "<<(*rpc_hits.product()).size()<<endl;
          cout << "detids RPC " << detIdsRPC().size() << endl;
          
          auto rpc_ch_ids = chamberIdsRPC();
          for (auto id: rpc_ch_ids) {
            auto& rpc_simhits = hitsInChamber(id);
            auto rpc_simhits_gp = simHitsMeanPosition(rpc_simhits);
            cout<<"RPCDetId "<<RPCDetId(id)<<": nHits "<<rpc_simhits.size()<<" eta "<<rpc_simhits_gp.eta()<<" phi "<<rpc_simhits_gp.phi()<<" nCh "<< rpc_chamber_to_hits_[id].size()<<endl;
            auto strips = hitStripsInDetId(id);
            cout<<"nStrips "<<strips.size()<<endl;
            cout<<"strips : "; std::copy(strips.begin(), strips.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
          }
        }
      }
    }
  }
  
  if (hasDTGeometry_) {
    edm::Handle<edm::PSimHitContainer> dt_hits;
    if (gemvalidation::getByLabel(dtSimHitInput_, dt_hits, event())) {

      // select DT simhits
      edm::PSimHitContainer dt_hits_select;
      for (auto& h: *dt_hits.product()) {
        DTWireId id(h.detUnitId());
        if (useDTChamberType(gemvalidation::toDTType(id.wheel(), id.station()))) dt_hits_select.push_back(h);
      }

      if (runDTSimHit_) {
        matchDTSimHitsToSimTrack(track_ids, dt_hits_select);    
        
        if (verboseDT_) {
          cout<<"nSimHits "<<no<<" nTrackIds "<<track_ids.size()<<" nSelectedDTSimHits "<<(*dt_hits.product()).size()<<endl;
          cout<<"detids DT " << detIdsDT().size()<<endl;
          
          auto dt_det_ids = detIdsDT();
          for (auto id: dt_det_ids) {
            auto dt_simhits = hitsInDetId(id);
            auto dt_simhits_gp = simHitsMeanPosition(dt_simhits);
            cout<<"DTWireId "<<DTWireId(id)<<": nHits "<<dt_simhits.size()<<" eta "<<dt_simhits_gp.eta()<<" phi "<<dt_simhits_gp.phi()<<" nCh "<< dt_chamber_to_hits_[id].size()<<endl;
            // only 1 wire per DT cell
            // auto wires = hitWiresInDTLayerId(id);
            // cout<<"nWires "<<wires.size()<<endl;
            // cout<<"wires : "; std::copy(wires.begin(), wires.end(), ostream_iterator<int>(cout, " ")); cout<<endl;
          }
        }
      }
    }
  }
}


std::vector<unsigned int>
SimHitMatcher::getIdsOfSimTrackShower(unsigned int initial_trk_id,
    const edm::SimTrackContainer & sim_tracks, const edm::SimVertexContainer & sim_vertices)
{
  vector<unsigned int> result;
  result.push_back(initial_trk_id);

  if (! (simMuOnlyGEM_ || simMuOnlyCSC_ || simMuOnlyDT_ || simMuOnlyME0_ || simMuOnlyRPC_) ) return result;

  for (auto& t: sim_tracks)
  {
    SimTrack last_trk = t;
    bool is_child = 0;
    while (1)
    {
      if ( last_trk.noVertex() ) break;
      if ( sim_vertices[last_trk.vertIndex()].noParent() ) break;
      
      unsigned parentId = sim_vertices[last_trk.vertIndex()].parentIndex();
      if ( parentId == initial_trk_id )
      {
        is_child = 1;
        break;
      }
      
      auto association = trkid_to_index_.find( parentId );
      if ( association == trkid_to_index_.end() ) break;

      last_trk = sim_tracks[ association->second ];
    }
    if (is_child)
    {
      result.push_back(t.trackId());
    }
  }
  return result;
}


void 
SimHitMatcher::matchCSCSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& csc_hits)
{
  for (auto& track_id: track_ids)
  {
    for (auto& h: csc_hits)
    {
      if (h.trackId() != track_id) continue;
      int pdgid = h.particleType();
      if (simMuOnlyCSC_ && std::abs(pdgid) != 13) continue;
      // discard electron hits in the CSC chambers
      if (discardEleHitsCSC_ && pdgid == 11) continue;
      LocalPoint lp = h.entryPoint();
      GlobalPoint gp;
      if (gemvalidation::is_csc(h.detUnitId()))
       {
     	 gp = getCSCGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
	 if (verboseCSC_) std::cout <<" csc id "<< CSCDetId(h.detUnitId()) <<" x "<< gp.x()<<" y "<< gp.y() <<" z "<< gp.z()<< std::endl;
       }
	
      csc_detid_to_hits_[ h.detUnitId() ].push_back(h);
      csc_hits_.push_back(h);
      CSCDetId layer_id( h.detUnitId() );
      csc_chamber_to_hits_[ layer_id.chamberId().rawId() ].push_back(h);
    }
  }
}


void
SimHitMatcher::matchRPCSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& rpc_hits)
{
  for (auto& track_id: track_ids)
  {
    for (auto& h: rpc_hits)
    {
      if (h.trackId() != track_id) continue;
      int pdgid = h.particleType();
      if (simMuOnlyRPC_ && std::abs(pdgid) != 13) continue;
      // discard electron hits in the RPC chambers
      if (discardEleHitsRPC_ && pdgid == 11) continue;
      rpc_detid_to_hits_[ h.detUnitId() ].push_back(h);
      rpc_hits_.push_back(h);
      RPCDetId layer_id( h.detUnitId() );
      rpc_chamber_to_hits_[ layer_id.chamberId().rawId() ].push_back(h);
    }
  }
}


void 
SimHitMatcher::matchGEMSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& gem_hits)
{
  for (auto& track_id: track_ids)
  {
    for (auto& h: gem_hits)
    {
      if (h.trackId() != track_id) continue;
      int pdgid = h.particleType();
      if (simMuOnlyGEM_ && std::abs(pdgid) != 13) continue;
      // discard electron hits in the GEM chambers
      if (discardEleHitsGEM_ && pdgid == 11) continue;
      
      GEMDetId p_id( h.detUnitId() );
      // ignore hits in the short GE21
      if (p_id.station()==2) continue;

      gem_detid_to_hits_[ h.detUnitId() ].push_back(h);
      gem_hits_.push_back(h);
      gem_chamber_to_hits_[ p_id.chamberId().rawId() ].push_back(h);
      GEMDetId superch_id(p_id.region(), p_id.ring(), p_id.station(), 1, p_id.chamber(), 0);
      gem_superchamber_to_hits_[ superch_id() ].push_back(h);
    }
  }

  // find pads with hits
  auto detids = detIdsGEM();
  // find 2-layer coincidence pads with hits
  for (auto d: detids) {
    GEMDetId id(d);
    auto hits = hitsInDetId(d);
    auto roll = getGEMGeometry()->etaPartition(id);
    //int max_npads = roll->npads();
    set<int> pads;
    for (auto& h: hits) {
      LocalPoint lp = h.entryPoint();
      pads.insert( 1 + static_cast<int>(roll->padTopology().channel(lp)) );
    }
    gem_detids_to_pads_[d] = pads;
  }
  
  // find 2-layer coincidence pads with hits
  for (auto d: detids) {
    GEMDetId id1(d);
    if (id1.layer() != 1) continue;
    GEMDetId id2(id1.region(), id1.ring(), id1.station(), 2, id1.chamber(), id1.roll());
    // does layer 2 has simhits?
    if (detids.find(id2()) == detids.end()) continue;
    
    // find pads with hits in layer1
    auto hits1 = hitsInDetId(d);
    auto roll1 = getGEMGeometry()->etaPartition(id1);
    set<int> pads1;
    for (auto& h: hits1) {
      LocalPoint lp = h.entryPoint();
      pads1.insert( 1 + static_cast<int>(roll1->padTopology().channel(lp)) );
    }
    
    // find pads with hits in layer2
    auto hits2 = hitsInDetId(id2());
    auto roll2 = getGEMGeometry()->etaPartition(id2);
    set<int> pads2;
    for (auto& h: hits2) {
      LocalPoint lp = h.entryPoint();
      pads2.insert( 1 + static_cast<int>(roll2->padTopology().channel(lp)) );
    }
    
    set<int> copads;
    std::set_intersection(pads1.begin(), pads1.end(), pads2.begin(), pads2.end(), std::inserter(copads, copads.begin()));
    if (copads.empty()) continue;
    gem_detids_to_copads_[d] = copads;
  }
}


void 
SimHitMatcher::matchME0SimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& me0_hits)
{
  for (auto& track_id: track_ids)
  {
    for (auto& h: me0_hits)
    {
      if (h.trackId() != track_id) continue;
      int pdgid = h.particleType();
      if (simMuOnlyME0_ && std::abs(pdgid) != 13) continue;
      // discard electron hits in the ME0 chambers
      if (discardEleHitsME0_ && pdgid == 11) continue;

      me0_detid_to_hits_[ h.detUnitId() ].push_back(h);
      me0_hits_.push_back(h);
      ME0DetId layer_id( h.detUnitId() );
      me0_chamber_to_hits_[ layer_id.chamberId().rawId() ].push_back(h);
    }
  }
}


void 
SimHitMatcher::matchDTSimHitsToSimTrack(std::vector<unsigned int> track_ids, const edm::PSimHitContainer& dt_hits)
{
  for (auto& track_id: track_ids)
  {
    for (auto& h: dt_hits)
    {
      if (h.trackId() != track_id) continue;
      int pdgid = h.particleType();
      if (simMuOnlyDT_ && std::abs(pdgid) != 13) continue; 
      // discard electron hits in the DT chambers
      if (discardEleHitsDT_ && pdgid == 11) continue; 

      dt_detid_to_hits_[ h.detUnitId() ].push_back(h);
      DTWireId layer_id( h.detUnitId() );
      dt_hits_.push_back(h);
      dt_layer_to_hits_ [ layer_id.layerId().rawId() ].push_back(h);
      dt_superlayer_to_hits_ [ layer_id.superlayerId().rawId() ].push_back(h);
      dt_chamber_to_hits_[ layer_id.chamberId().rawId() ].push_back(h);
    }
  }
}


const edm::PSimHitContainer& 
SimHitMatcher::simHits(enum MuonType sub) const
{
  switch(sub) {
  case MuonSubdetId::GEM: 
    return gem_hits_;
  case MuonSubdetId::CSC: 
    return csc_hits_;
  case MuonSubdetId::ME0: 
    return me0_hits_;
  case MuonSubdetId::RPC: 
    return rpc_hits_;
  case MuonSubdetId::DT: 
    return dt_hits_;
  }
  return no_hits_;
}


std::set<unsigned int> 
SimHitMatcher::detIdsGEM(int gem_type) const
{
  std::set<unsigned int> result;
  for (auto& p: gem_detid_to_hits_)
  {
    auto id = p.first;
    if (gem_type > 0)
    {
      GEMDetId detId(id);
      if (gemvalidation::toGEMType(detId.station(), detId.ring()) != gem_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsRPC(int rpc_type) const
{
  std::set<unsigned int> result;
  for (auto& p: rpc_detid_to_hits_)
  {
    auto id = p.first;
    if (rpc_type > 0)
    {
      RPCDetId detId(id);
      if (gemvalidation::toRPCType(detId.region(), detId.station(), detId.ring()) != rpc_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsME0() const
{
  std::set<unsigned int> result;
  for (auto& p: me0_detid_to_hits_) result.insert(p.first);
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsCSC(int csc_type) const
{
  std::set<unsigned int> result;
  for (auto& p: csc_detid_to_hits_)
  {
    auto id = p.first;
    if (csc_type > 0)
    {
      CSCDetId detId(id);
      if (gemvalidation::toCSCType(detId.station(), detId.ring()) != csc_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsDT(int dt_type) const
{
  std::set<unsigned int> result;
  for (auto& p: dt_detid_to_hits_)
  {
    auto id = p.first;
    if (dt_type > 0)
    {
      DTWireId detId(id);
      if (gemvalidation::toDTType(detId.wheel(), detId.station()) != dt_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsGEMCoincidences() const
{
  std::set<unsigned int> result;
  for (auto& p: gem_detids_to_copads_) result.insert(p.first);
  return result;
}


std::set<unsigned int> 
SimHitMatcher::detIdsME0Coincidences(int min_n_layers) const
{
  std::set<unsigned int> result;

  //   int result = 0;
  //   auto chamber_ids = chamberIdsCSC();
  //   for (auto id: chamber_ids)
  //   {
  //     if (nLayersWithHitsInSuperChamber(id) >= min_n_layers) result += 1;
  //   }
  //   return result;

  //   for (auto& p: me0_detids_to_copads_) result.insert(p.first);
  return result;
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsGEM(int gem_type) const
{
  std::set<unsigned int> result;
  for (auto& p: gem_chamber_to_hits_)
  {
    auto id = p.first;
    if (gem_type > 0)
    {
      GEMDetId detId(id);
      if (gemvalidation::toGEMType(detId.station(), detId.ring()) != gem_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsRPC(int rpc_type) const
{
  std::set<unsigned int> result;
  for (auto& p: rpc_chamber_to_hits_)
  {
    auto id = p.first;
    if (rpc_type > 0)
    {
      RPCDetId detId(id);
      if (gemvalidation::toRPCType(detId.region(), detId.station(), detId.ring()) != rpc_type) continue;
    }
    result.insert(id);
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsME0() const
{
  std::set<unsigned int> result;
  for (auto& p: me0_chamber_to_hits_) result.insert(p.first);
  return result;
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsCSC(int csc_type) const
{
  std::set<unsigned int> result;
  for (auto& p: csc_chamber_to_hits_)
  {
    auto id = p.first;
    if (csc_type > 0)
    {
      CSCDetId detId(id);
      if (gemvalidation::toCSCType(detId.station(), detId.ring()) != csc_type) continue;
    }
    result.insert(id);
  }
  return result;
}

std::set<unsigned int>
SimHitMatcher::chamberIdsDT(int dt_type) const
{
  std::set<unsigned int> result;
  for (auto& p: dt_chamber_to_hits_)
  {
    auto id = p.first;
    if (dt_type > 0)
    {
      DTChamberId detId(id);
      if (gemvalidation::toDTType(detId.wheel(), detId.station()) != dt_type) continue;
    }
    result.insert(id);
  }
  return result;
}

std::set<unsigned int>
SimHitMatcher::superChamberIdsGEM() const
{
  std::set<unsigned int> result;
  for (auto& p: gem_superchamber_to_hits_) result.insert(p.first);
  return result;
}


std::set<unsigned int>
SimHitMatcher::superChamberIdsME0() const
{
  std::set<unsigned int> result;
  for (auto& p: me0_superchamber_to_hits_) result.insert(p.first);
  return result;
}


std::set<unsigned int>
SimHitMatcher::superChamberIdsGEMCoincidences() const
{
  std::set<unsigned int> result;
  for (auto& p: gem_detids_to_copads_)
  {
    GEMDetId id(p.first);
    result.insert(id.chamberId().rawId());
  }
  return result;
}

std::set<unsigned int>
SimHitMatcher::layerIdsDT() const
{
  std::set<unsigned int> result;
  for (auto& p: dt_layer_to_hits_) result.insert(p.first);
  return result;
}

std::set<unsigned int>
SimHitMatcher::superlayerIdsDT() const
{
  std::set<unsigned int> result;
  for (auto& p: dt_superlayer_to_hits_) result.insert(p.first);
  return result;
}

const edm::PSimHitContainer&
SimHitMatcher::hitsInDetId(unsigned int detid) const
{
  if (gemvalidation::is_gem(detid))
  {
    if (gem_detid_to_hits_.find(detid) == gem_detid_to_hits_.end()) return no_hits_;
    return gem_detid_to_hits_.at(detid);
  }
  if (gemvalidation::is_me0(detid))
  {
    if (me0_detid_to_hits_.find(detid) == me0_detid_to_hits_.end()) return no_hits_;
    return me0_detid_to_hits_.at(detid);
  }
  if (gemvalidation::is_csc(detid))
  {
    if (csc_detid_to_hits_.find(detid) == csc_detid_to_hits_.end()) return no_hits_;
    return csc_detid_to_hits_.at(detid);
  }
  if (gemvalidation::is_rpc(detid))
  {
    if (rpc_detid_to_hits_.find(detid) == rpc_detid_to_hits_.end()) return no_hits_;
    return rpc_detid_to_hits_.at(detid);
  }
  if(gemvalidation::is_dt(detid))
  {
    if (dt_detid_to_hits_.find(detid) == dt_detid_to_hits_.end()) return no_hits_;
    return dt_detid_to_hits_.at(detid);
  }
  return no_hits_;
}


const edm::PSimHitContainer&
SimHitMatcher::hitsInChamber(unsigned int detid) const
{
  if (gemvalidation::is_gem(detid)) // make sure we use chamber id
  {
    const GEMDetId id(detid);
    if (gem_chamber_to_hits_.find(id.chamberId().rawId()) == gem_chamber_to_hits_.end()) return no_hits_;
    return gem_chamber_to_hits_.at(id.chamberId().rawId());
  }
  if (gemvalidation::is_me0(detid)) // make sure we use chamber id
  {
    const ME0DetId id(detid);
    if (me0_chamber_to_hits_.find(id.chamberId().rawId()) == me0_chamber_to_hits_.end()) return no_hits_;
    return me0_chamber_to_hits_.at(id.chamberId().rawId());
  }
  if (gemvalidation::is_csc(detid))
  {
    const CSCDetId id(detid);
    if (csc_chamber_to_hits_.find(id.chamberId().rawId()) == csc_chamber_to_hits_.end()) return no_hits_;
    return csc_chamber_to_hits_.at(id.chamberId().rawId());
  }
  if (gemvalidation::is_rpc(detid))
  {
    const RPCDetId id(detid);
    if (rpc_chamber_to_hits_.find(id.chamberId().rawId()) == rpc_chamber_to_hits_.end()) return no_hits_;
    return rpc_chamber_to_hits_.at(id.chamberId().rawId());
  }
  if(gemvalidation::is_dt(detid)) 
  {
    const DTWireId id(detid);
    if(dt_chamber_to_hits_.find(id.chamberId().rawId()) == dt_chamber_to_hits_.end()) return no_hits_;
    return dt_chamber_to_hits_.at(id.chamberId().rawId());
  }
  return no_hits_;
}


const edm::PSimHitContainer&
SimHitMatcher::hitsInSuperChamber(unsigned int detid) const
{
  if (gemvalidation::is_gem(detid))
  {
    const GEMDetId id(detid);
    if (gem_superchamber_to_hits_.find(id.chamberId().rawId()) == gem_superchamber_to_hits_.end()) return no_hits_;
    return gem_superchamber_to_hits_.at(id.chamberId().rawId());
  }
  if (gemvalidation::is_csc(detid)) return hitsInChamber(detid);

  return no_hits_;
}

const edm::PSimHitContainer&
SimHitMatcher::hitsInLayerDT(unsigned int detid) const
{
  if (!gemvalidation::is_dt(detid)) return no_hits_;

  const DTWireId id(detid);
  if (dt_layer_to_hits_.find(id.layerId().rawId()) == dt_layer_to_hits_.end()) return no_hits_;
  return dt_layer_to_hits_.at(id.layerId().rawId());
}

const edm::PSimHitContainer&
SimHitMatcher::hitsInSuperLayerDT(unsigned int detid) const
{
  if (!gemvalidation::is_dt(detid)) return no_hits_;

  const DTWireId id(detid);
  if (dt_superlayer_to_hits_.find(id.superlayerId().rawId()) == dt_superlayer_to_hits_.end()) return no_hits_;
  return dt_superlayer_to_hits_.at(id.superlayerId().rawId());
}

const edm::PSimHitContainer&
SimHitMatcher::hitsInChamberDT(unsigned int detid) const
{
  if (!gemvalidation::is_dt(detid)) return no_hits_;

  const DTWireId id(detid);
  if (dt_chamber_to_hits_.find(id.chamberId().rawId()) == dt_chamber_to_hits_.end()) return no_hits_;
  return dt_chamber_to_hits_.at(id.chamberId().rawId());
}

int
SimHitMatcher::nLayersWithHitsInSuperChamber(unsigned int detid) const
{
  set<int> layers_with_hits;
  const auto hits = hitsInSuperChamber(detid);
  for (auto& h: hits)
  {
    if (gemvalidation::is_gem(detid))
    {
      const GEMDetId idd(h.detUnitId());
      layers_with_hits.insert(idd.layer());
    }
    if (gemvalidation::is_me0(detid))
    {
      const ME0DetId idd(h.detUnitId());
      layers_with_hits.insert(idd.layer());
    }
    if (gemvalidation::is_csc(detid))
    {
      const CSCDetId idd(h.detUnitId());
      layers_with_hits.insert(idd.layer());
    }
    if (gemvalidation::is_rpc(detid))
    {
      const RPCDetId idd(h.detUnitId());
      layers_with_hits.insert(idd.layer());
    }
  }
  return layers_with_hits.size();
}


bool 
SimHitMatcher::hitStationGEM(int st, int nlayers) const
{
  int nst=0;
  for(auto ddt: chamberIdsGEM()) {

    const GEMDetId id(ddt);
    if (id.station()!=st) continue;
        
    const int nl(nLayersWithHitsInSuperChamber(id.rawId()));
    if (nl < nlayers) continue; 
    ++nst;    
  }
  return nst;
}


bool 
SimHitMatcher::hitStationRPC(int st) const
{
  int nst=0;
  for(auto ddt: chamberIdsRPC()) {
    const RPCDetId id(ddt);
    if (id.station()!=st) continue;    
    ++nst;    
  }
  return nst;
}


bool 
SimHitMatcher::hitStationCSC(int st, int nlayers) const
{
  int nst=0;
  for(auto ddt: chamberIdsCSC()) {

    const CSCDetId id(ddt);
    int ri(id.ring());
    if (id.station()!=st) continue;
        
    // ME1/a case - check the ME1/b chamber
    if (st==1 and ri==4) {
      CSCDetId idME1b(id.endcap(), id.station(), 1, id.chamber(), 0);
      const int nl1a(nLayersWithHitsInSuperChamber(id.rawId()));
      const int nl1b(nLayersWithHitsInSuperChamber(idME1b.rawId()));
      if (nl1a + nl1b < nlayers) continue; 
      ++nst;      
    }
    // ME1/b case - check the ME1/a chamber
    else if (st==1 and ri==1) {
      CSCDetId idME1a(id.endcap(), id.station(), 4, id.chamber(), 0);
      const int nl1a(nLayersWithHitsInSuperChamber(idME1a.rawId()));
      const int nl1b(nLayersWithHitsInSuperChamber(id.rawId()));
      if (nl1a + nl1b < nlayers) continue; 
      ++nst;
    }
    // default case
    else {
      const int nl(nLayersWithHitsInSuperChamber(id.rawId()));
      if (nl < nlayers) continue; 
      ++nst;    
    }
  }
  return nst;
}

bool 
SimHitMatcher::hitStationDT(int st, int nsuperlayers, int nlayers) const
{
  int nst=0;
  for(auto ddt: chamberIdsDT()) {

    const DTChamberId id(ddt);
    if (id.station()!=st) continue;
    
    // require at least 1 superlayer
    const int nsl(nSuperLayersWithHitsInChamberDT(id.rawId()));
    if (nsl < nsuperlayers) continue; 

    // require at least 3 layers hit per chamber
    const int nl(nLayersWithHitsInChamberDT(id.rawId()));
    if (nl < nlayers) continue;    
    ++nst;
  }
  return nst;
}

int 
SimHitMatcher::nStationsCSC(int nlayers) const
{
  return (hitStationCSC(1, nlayers) + hitStationCSC(2, nlayers) + 
	  hitStationCSC(3, nlayers) + hitStationCSC(4, nlayers));
}

int 
SimHitMatcher::nStationsDT(int nsuperlayers, int nlayers) const
{
  return (hitStationDT(1, nsuperlayers, nlayers) + hitStationDT(2, nsuperlayers, nlayers) + 
	  hitStationDT(3, nsuperlayers, nlayers) + hitStationDT(4,nsuperlayers, nlayers));
}


int 
SimHitMatcher::nStationsRPC() const
{
  return (hitStationRPC(1) + hitStationRPC(2) + hitStationRPC(3) + hitStationRPC(4));
}


int 
SimHitMatcher::nStationsGEM(int nlayers) const
{
  return (hitStationGEM(1, nlayers) + hitStationGEM(3, nlayers));
}

int 
SimHitMatcher::nCellsWithHitsInLayerDT(unsigned int detid) const
{
  set<int> layers_with_hits;
  const auto hits = hitsInLayerDT(detid);
  for (auto& h: hits) {
    if (gemvalidation::is_dt(detid)) {
      const DTWireId idd(h.detUnitId());
      layers_with_hits.insert(idd.wire());
    }
  }
  return layers_with_hits.size();
}

int 
SimHitMatcher::nLayersWithHitsInSuperLayerDT(unsigned int detid) const
{
  set<int> layers_with_hits;
  const auto hits = hitsInSuperLayerDT(detid);
  for (auto& h: hits) {
    if (gemvalidation::is_dt(detid)) {
      const DTLayerId idd(h.detUnitId());
      layers_with_hits.insert(idd.layer());
    }
  }
  return layers_with_hits.size();
}

int
SimHitMatcher::nSuperLayersWithHitsInChamberDT(unsigned int detid) const
{
  set<int> sl_with_hits;
  const auto hits = hitsInChamber(detid);
  for (auto& h: hits) {
    if (gemvalidation::is_dt(detid)) {
      const DTSuperLayerId idd(h.detUnitId());
      sl_with_hits.insert(idd.superLayer());
    }
  }
  return sl_with_hits.size();
}

int 
SimHitMatcher::nLayersWithHitsInChamberDT(unsigned int detid) const
{
  int nLayers = 0;
  auto superLayers(getDTGeometry()->chamber(DTChamberId(detid))->superLayers());
  for (auto& sl: superLayers) {
    nLayers += nLayersWithHitsInSuperLayerDT(sl->id().rawId());
  }
  return nLayers;
}

GlobalPoint
SimHitMatcher::simHitsMeanPosition(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return GlobalPoint(); // point "zero"

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.f;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalPoint lp = h.entryPoint();
    GlobalPoint gp;
    if ( gemvalidation::is_gem(h.detUnitId()) )
    {
      gp = getGEMGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else if ( gemvalidation::is_me0(h.detUnitId()) )
    {
      gp = getME0Geometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else if (gemvalidation::is_csc(h.detUnitId()))
    {
      gp = getCSCGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
      //std::cout <<"In simHitsMeanPosition localPoint "<< lp<<" gloablPoint "<< gp  << std::endl;
    }
    else if (gemvalidation::is_rpc(h.detUnitId()))
    {
      gp = getRPCGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else if (gemvalidation::is_dt(h.detUnitId()))
    {
      gp = getDTGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else continue;
    sumx += gp.x();
    sumy += gp.y();
    sumz += gp.z();
    ++n;
  }
  if (n == 0) return GlobalPoint();
  return GlobalPoint(sumx/n, sumy/n, sumz/n);
}

GlobalPoint
SimHitMatcher::simHitPositionKeyLayer(unsigned int chid) const
{
  /*
    1. Return the (average) position of the keylayer simhit if available
    2. Return the fitted position of the simhit in the keylayer if fit good
    3. Return average position of the simhits in the chamber if all else fails
  */
  const CSCDetId chamberId(chid);
  const CSCDetId keyLayerId(chamberId.endcap(), chamberId.station(),
                            chamberId.ring(), chamberId.chamber(), 3);
  GlobalPoint returnValue;
  
  const edm::PSimHitContainer keyLayerIdHits(hitsInDetId(keyLayerId.rawId()));
  if (keyLayerIdHits.size()!=0) returnValue = simHitsMeanPosition(keyLayerIdHits);
  else{
    // check if the chamber has hits at all
    if (hitsInChamber(chid).size()==0) returnValue = GlobalPoint();
    else if (hitsInChamber(chid).size()==1) returnValue = simHitsMeanPosition(hitsInChamber(chid));
    else {
      std::vector<float> zs; 
      std::vector<float> xs;
      std::vector<float> ys;

      // add the positions of all hits in a chamber
      for (int l=1; l<=6; l++){
        CSCDetId l_id(chamberId.endcap(), chamberId.station(), 
                      chamberId.ring(), chamberId.chamber(), l);
        for (auto p: hitsInDetId(l_id.rawId())) {
          LocalPoint lp = p.entryPoint();
          GlobalPoint gp = getCSCGeometry()->idToDet(p.detUnitId())->surface().toGlobal(lp);
          zs.push_back(gp.z());
          xs.push_back(gp.x());
          ys.push_back(gp.y());
        }
      }
      // fit a straight line through the hits (bending is negligible
      float zmin;
      float zmax;
      if (zs.front() < zs.back())  { zmin = zs.front(); zmax = zs.back();  }
      else                         { zmin = zs.back();  zmax = zs.front(); }
      
      TF1* fit = new TF1("fit","pol1",zmin,zmax); 
      TGraph* gr = new TGraph(zs.size(),&(zs[0]),&(xs[0]));
      TFitResultPtr r = gr->Fit(fit,"RQ"); 

      TF1* fit2 = new TF1("fit2","pol1",zmin,zmax); 
      TGraph* grr = new TGraph(zs.size(),&(zs[0]),&(ys[0]));
      TFitResultPtr rr = grr->Fit(fit2,"RQ"); 

      if (r->Status()==0 and rr->Status()==0){    
        float z_pos_L3 = getCSCGeometry()->layer(keyLayerId)->centerOfStrip(20).z();
        float x_pos_L3 = fit->GetParameter(0) + fit->GetParameter(1) * z_pos_L3;
        float y_pos_L3 = fit2->GetParameter(0) + fit2->GetParameter(1) * z_pos_L3;
        returnValue =  GlobalPoint(x_pos_L3, y_pos_L3, z_pos_L3);
      }
      else{
        returnValue = simHitsMeanPosition(hitsInChamber(chid));
      }
      delete fit;
      delete fit2;
      delete gr;
      delete grr;
    } 
  }
  return returnValue;
}
   

GlobalVector
SimHitMatcher::simHitsMeanMomentum(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return GlobalVector(); // point "zero"

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.f;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalVector lv = h.momentumAtEntry();
    GlobalVector gv;
    if ( gemvalidation::is_gem(h.detUnitId()) )
    {
      gv = getGEMGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lv);
    }
    else if ( gemvalidation::is_me0(h.detUnitId()) )
    {
      gv = getME0Geometry()->idToDet(h.detUnitId())->surface().toGlobal(lv);
    }
    else if (gemvalidation::is_csc(h.detUnitId()))
    {
      gv = getCSCGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lv);
    }
    else if (gemvalidation::is_rpc(h.detUnitId()))
    {
      gv = getRPCGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lv);
    }
    else if (gemvalidation::is_dt(h.detUnitId()))
    {
      gv = getDTGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lv);
    }
    else continue;
    sumx += gv.x();
    sumy += gv.y();
    sumz += gv.z();
    ++n;
  }
  if (n == 0) return GlobalVector();
  return GlobalVector(sumx/n, sumy/n, sumz/n);
}


float
SimHitMatcher::simHitsGEMCentralPosition(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return -0.0; // point "zero"

  float central = -0.0;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalPoint lp( 0., 0., 0. );//local central
    GlobalPoint gp;
    if ( gemvalidation::is_gem(h.detUnitId()) )
    {
      gp = getGEMGeometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else if ( gemvalidation::is_me0(h.detUnitId()) )
    {
      gp = getME0Geometry()->idToDet(h.detUnitId())->surface().toGlobal(lp);
    }
    else continue;
    central = gp.perp();
    //std::cout <<" GEMid "<< GEMDetId(h.detUnitId()) <<"cenral gp perp "<< gp.perp()<<" x "<< gp.x()<<" y "<< gp.y() <<" z "<< gp.z() <<std::endl;
    if (n>=1) std::cout <<"warning! find more than one simhits in GEM chamber " << std::endl;
    ++n;
  }

  return central;
}




float 
SimHitMatcher::LocalBendingInChamber(unsigned int detid) const
{
  const CSCDetId cscid(detid);
  if (nLayersWithHitsInSuperChamber(detid)<6) return -100;
  float phi_layer1=-10;
  float phi_layer6=10;

  if (cscid.station()==1 and (cscid.ring()==1 or cscid.ring()==4)){
  	const CSCDetId cscid1a(cscid.endcap(), cscid.station(), 4, cscid.chamber(), 1);
  	const CSCDetId cscid1b(cscid.endcap(), cscid.station(), 1, cscid.chamber(), 1);
	const edm::PSimHitContainer hits1a = hitsInDetId(cscid1a.rawId());
	const edm::PSimHitContainer hits1b = hitsInDetId(cscid1b.rawId());
	GlobalPoint gp1a = simHitsMeanPosition(hitsInDetId(cscid1a.rawId()));	
	GlobalPoint gp1b = simHitsMeanPosition(hitsInDetId(cscid1b.rawId()));	
	if (hits1a.size()>0 and hits1b.size()>0) 
	    //phi_layer1 = (gp1a.phi()*hits1a.size()+gp1b.phi()*hits1b.size())/(hits1a.size()+hits1b.size());
	    phi_layer1 = (gp1a.phi()+gp1b.phi())/2.0;
	else if (hits1a.size()>0) phi_layer1 = gp1a.phi();
	else if (hits1b.size()>0) phi_layer1 = gp1b.phi();
	else std::cerr <<" no hits in layer1, cant not find global phi of hits " << std::endl;

  	const CSCDetId cscid6a(cscid.endcap(), cscid.station(), 4, cscid.chamber(), 6);
  	const CSCDetId cscid6b(cscid.endcap(), cscid.station(), 1, cscid.chamber(), 6);
	const edm::PSimHitContainer hits6a = hitsInDetId(cscid6a.rawId());
	const edm::PSimHitContainer hits6b = hitsInDetId(cscid6b.rawId());
	GlobalPoint gp6a = simHitsMeanPosition(hitsInDetId(cscid6a.rawId()));	
	GlobalPoint gp6b = simHitsMeanPosition(hitsInDetId(cscid6b.rawId()));	
	if (hits6a.size()>0 and hits6b.size()>0) 
	    //phi_layer6 = (gp6a.phi()*hits6a.size()+gp6b.phi()*hits6b.size())/(hits6a.size()+hits6b.size());
	    phi_layer6 = (gp6a.phi()+gp6b.phi())/2.0;
	else if (hits6a.size()>0) phi_layer6 = gp6a.phi();
	else if (hits6b.size()>0) phi_layer6 = gp6b.phi();
	else std::cerr <<" no hits in layer6, cant not find global phi of hits " << std::endl;
   

  }
  else {
  	const CSCDetId cscid1(cscid.endcap(), cscid.station(), cscid.ring(), cscid.chamber(), 1);
	const edm::PSimHitContainer hits1 = hitsInDetId(cscid1.rawId());
	if (hits1.size()==0) std::cerr <<" no hits in layer1, cant not find global phi of hits " << std::endl;
	GlobalPoint gp1 = simHitsMeanPosition(hitsInDetId(cscid1.rawId()));	
	phi_layer1 = gp1.phi();

  	const CSCDetId cscid6(cscid.endcap(), cscid.station(), cscid.ring(), cscid.chamber(), 6);
	const edm::PSimHitContainer hits6 = hitsInDetId(cscid6.rawId());
	if (hits6.size()==0) std::cerr <<" no hits in layer6, cant not find global phi of hits " << std::endl;
	GlobalPoint gp6 = simHitsMeanPosition(hitsInDetId(cscid6.rawId()));	
	phi_layer6 = gp6.phi();

  }
	//std::cout <<" phi1 "<< phi_layer1 <<" phi6 " << phi_layer6 << std::endl;
	return deltaPhi(phi_layer6,phi_layer1);
}


float 
SimHitMatcher::simHitsMeanStrip(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return -1.f;

  float sums = 0.f;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalPoint lp = h.entryPoint();
    float s;
    auto d = h.detUnitId();
    if ( gemvalidation::is_gem(d) )
    {
      s = getGEMGeometry()->etaPartition(d)->strip(lp);
    }
    else if ( gemvalidation::is_me0(d) )
    {
      s = getME0Geometry()->etaPartition(d)->strip(lp);
    }
    else if (gemvalidation::is_csc(d))
    {
      s = getCSCGeometry()->layer(d)->geometry()->strip(lp);
      // convert to half-strip:
      s *= 2.;
    }
    else if (gemvalidation::is_rpc(d))
    {
      s = getRPCGeometry()->roll(d)->strip(lp);
    }
    else continue;
    sums += s;
    ++n;
  }
  if (n == 0) return -1.f;
  return sums/n;
}


float 
SimHitMatcher::simHitsMeanWG(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return -1.f;

  float sums = 0.f;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalPoint lp = h.entryPoint();
    float s;
    auto d = h.detUnitId();
    if (gemvalidation::is_csc(d))
    {
      // find nearest wire
      int nearestWire(getCSCGeometry()->layer(d)->geometry()->nearestWire(lp));
      // then find the corresponding wire group
      s = getCSCGeometry()->layer(d)->geometry()->wireGroup(nearestWire);
    }
    else continue;
    sums += s;
    ++n;
  }
  if (n == 0) return -1.f;
  return sums/n;
}


float 
SimHitMatcher::simHitsMeanWire(const edm::PSimHitContainer& sim_hits) const
{
  if (sim_hits.empty()) return -1.f;

  float sums = 0.f;
  size_t n = 0;
  for (auto& h: sim_hits)
  {
    LocalPoint lp = h.entryPoint();
    float s;
    auto d = h.detUnitId();
    if (gemvalidation::is_dt(d))
    {
      // find nearest wire
      s  = getDTGeometry()->layer(DTLayerId(d))->specificTopology().channel(lp);
    }
    else continue;
    sums += s;
    ++n;
  }
  if (n == 0) return -1.f;
  return sums/n;
}


std::set<int> 
SimHitMatcher::hitStripsInDetId(unsigned int detid, int margin_n_strips) const
{
  set<int> result;
  auto simhits = hitsInDetId(detid);
  if ( gemvalidation::is_gem(detid) )
  {
    GEMDetId id(detid);
    int max_nstrips = getGEMGeometry()->etaPartition(id)->nstrips();
    for (auto& h: simhits)
    {
      LocalPoint lp = h.entryPoint();
      int central_strip = 1 + static_cast<int>(getGEMGeometry()->etaPartition(id)->topology().channel(lp));
      int smin = central_strip - margin_n_strips;
      smin = (smin > 0) ? smin : 1;
      int smax = central_strip + margin_n_strips;
      smax = (smax <= max_nstrips) ? smax : max_nstrips;
      for (int ss = smin; ss <= smax; ++ss) result.insert(ss);
    }
  }
  else if ( gemvalidation::is_me0(detid) )
  {
    ME0DetId id(detid);
    int max_nstrips = getME0Geometry()->etaPartition(id)->nstrips();
    for (auto& h: simhits)
    {
      LocalPoint lp = h.entryPoint();
      int central_strip = 1 + static_cast<int>(getME0Geometry()->etaPartition(id)->topology().channel(lp));
      int smin = central_strip - margin_n_strips;
      smin = (smin > 0) ? smin : 1;
      int smax = central_strip + margin_n_strips;
      smax = (smax <= max_nstrips) ? smax : max_nstrips;
      for (int ss = smin; ss <= smax; ++ss) result.insert(ss);
    }
  }
  else if ( gemvalidation::is_csc(detid) )
  {
    CSCDetId id(detid);
    int max_nstrips = getCSCGeometry()->layer(id)->geometry()->numberOfStrips();
    for (auto& h: simhits)
    {
      LocalPoint lp = h.entryPoint();
      int central_strip = getCSCGeometry()->layer(id)->geometry()->nearestStrip(lp);
      int smin = central_strip - margin_n_strips;
      smin = (smin > 0) ? smin : 1;
      int smax = central_strip + margin_n_strips;
      smax = (smax <= max_nstrips) ? smax : max_nstrips;
      for (int ss = smin; ss <= smax; ++ss) result.insert(ss);
    }
  }
  else if ( gemvalidation::is_rpc(detid) )
  {
    RPCDetId id(detid); 
    for (auto roll: getRPCGeometry()->chamber(id)->rolls()) {
      int max_nstrips = roll->nstrips();
      for (auto& h: hitsInDetId(roll->id().rawId())) {
        LocalPoint lp = h.entryPoint();
        // check how the RPC strip numbers start counting - Ask Piet!!!
        int central_strip = static_cast<int>(roll->topology().channel(lp));
        // int central_strip2 = 1 + static_cast<int>(getRPCGeometry()->roll(id)->strip(lp));
        // std::cout <<"strip from topology"<< central_strip <<" strip from roll" << central_strip2 <<std::endl; 
        int smin = central_strip - margin_n_strips;
        smin = (smin > 0) ? smin : 1;
        int smax = central_strip + margin_n_strips;
        smax = (smax <= max_nstrips) ? smax : max_nstrips;
        for (int ss = smin; ss <= smax; ++ss) result.insert(ss);
      }
    }
  }
  return result;
}


std::set<int> 
SimHitMatcher::hitWiregroupsInDetId(unsigned int detid, int margin_n_wg) const
{
  set<int> result;
  if ( gemvalidation::is_csc(detid) ) {
    auto simhits = hitsInDetId(detid);
    CSCDetId id(detid);
    int max_n_wg = getCSCGeometry()->layer(id)->geometry()->numberOfWireGroups();
    for (auto& h: simhits) {
      LocalPoint lp = h.entryPoint();
      auto layer_geo = getCSCGeometry()->layer(id)->geometry();
      int central_wg = layer_geo->wireGroup(layer_geo->nearestWire(lp));
      int wg_min = central_wg - margin_n_wg;
      wg_min = (wg_min > 0) ? wg_min : 1;
      int wg_max = central_wg + margin_n_wg;
      wg_max = (wg_max <= max_n_wg) ? wg_max : max_n_wg;
      for (int wg = wg_min; wg <= wg_max; ++wg) result.insert(wg);
    }
  }
  return result;
}


std::set<int> 
SimHitMatcher::hitPadsInDetId(unsigned int detid) const
{
  set<int> none;
  if (gem_detids_to_pads_.find(detid) == gem_detids_to_pads_.end()) return none;
  return gem_detids_to_pads_.at(detid);
}


std::set<int>
SimHitMatcher::hitCoPadsInDetId(unsigned int detid) const
{
  set<int> none;
  if (gem_detids_to_copads_.find(detid) == gem_detids_to_copads_.end()) return none;
  return gem_detids_to_copads_.at(detid);
}


std::set<unsigned int> 
SimHitMatcher::hitWiresInDTLayerId(unsigned int detid, int margin_n_wires) const
{
  set<unsigned int> result;
  if ( gemvalidation::is_dt(detid) ) {
    DTLayerId id(detid);
    int max_nwires = getDTGeometry()->layer(id)->specificTopology().channels();
    for (int wn = 0; wn <= max_nwires; ++wn) {
      DTWireId wid(id,wn);
      for (auto& h: hitsInDetId(wid.rawId())) {
	if (verboseDT_) cout << "central DTWireId "<< wid << " simhit " <<h<< endl;
	int smin = wn - margin_n_wires;
	smin = (smin > 0) ? smin : 1;
	int smax = wn + margin_n_wires;
	smax = (smax <= max_nwires) ? smax : max_nwires;
	for (int ss = smin; ss <= smax; ++ss) {
	  DTWireId widd(id, ss);
	  if (verboseDT_) cout << "\tadding DTWireId to collection "<< widd << endl;
	  result.insert(widd.rawId());
	}
      }
    }
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::hitWiresInDTSuperLayerId(unsigned int detid, int margin_n_wires) const
{
  set<unsigned int> result;
  auto layers(getDTGeometry()->superLayer(DTSuperLayerId(detid))->layers());
  for (auto& l: layers) {
    if (verboseDT_)cout << "hitWiresInDTSuperLayerId::l id "<<l->id() << endl;
    auto p(hitWiresInDTLayerId(l->id().rawId(), margin_n_wires));
    result.insert(p.begin(),p.end());
  }
  return result;
}


std::set<unsigned int> 
SimHitMatcher::hitWiresInDTChamberId(unsigned int detid, int margin_n_wires) const
{
  set<unsigned int> result;
  auto superLayers(getDTGeometry()->chamber(DTChamberId(detid))->superLayers());
  for (auto& sl: superLayers) {
    if (verboseDT_)cout << "hitWiresInDTChamberId::sl id "<<sl->id() << endl;
    auto p(hitWiresInDTSuperLayerId(sl->id().rawId(), margin_n_wires));
    result.insert(p.begin(),p.end());
  }
  return result;
}


std::set<int> 
SimHitMatcher::hitPartitions() const
{
  std::set<int> result;

  auto detids = detIdsGEM();
  for (auto id: detids)
  {
    GEMDetId idd(id);
    result.insert( idd.roll() );
  }
  return result;
}


int
SimHitMatcher::nPadsWithHits() const
{
  int result = 0;
  auto pad_ids = detIdsGEM();
  for (auto id: pad_ids)
  {
    result += hitPadsInDetId(id).size();
  }
  return result;
}


int
SimHitMatcher::nCoincidencePadsWithHits() const
{
  int result = 0;
  auto copad_ids = detIdsGEMCoincidences();
  for (auto id: copad_ids)
  {
    result += hitCoPadsInDetId(id).size();
  }
  return result;
}


int
SimHitMatcher::nCoincidenceCSCChambers(int min_n_layers) const
{
  int result = 0;
  auto chamber_ids = chamberIdsCSC();
  for (auto id: chamber_ids)
  {
    if (nLayersWithHitsInSuperChamber(id) >= min_n_layers) result += 1;
  }
  return result;
}

void
SimHitMatcher::cscChamberIdsToString(const std::set<unsigned int>& set) const
{
  for (auto p: set) {
    CSCDetId detId(p);
    std::cout << " " << detId << "\n";
  }
}


void
SimHitMatcher::dtChamberIdsToString(const std::set<unsigned int>& set) const
{
  for (auto p: set) {
    DTChamberId detId(p);
    std::cout << " " << detId << "\n";
  }
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsCSCStation(int station) const
{
  set<unsigned int> result;
  switch(station){
  case 1: {
    auto p1(chamberIdsCSC(CSC_ME1a));  
    auto p2(chamberIdsCSC(CSC_ME1b));  
      auto p3(chamberIdsCSC(CSC_ME12));  
      auto p4(chamberIdsCSC(CSC_ME13));  
      result.insert(p1.begin(),p1.end());
      result.insert(p2.begin(),p2.end());
      result.insert(p3.begin(),p3.end());
      result.insert(p4.begin(),p4.end());
  }
    break;
  case 2: {
    auto p1(chamberIdsCSC(CSC_ME21));  
    auto p2(chamberIdsCSC(CSC_ME22));  
    result.insert(p1.begin(),p1.end());
      result.insert(p2.begin(),p2.end());
  }
    break;
  case 3: {
      auto p1(chamberIdsCSC(CSC_ME31));  
      auto p2(chamberIdsCSC(CSC_ME32));  
      result.insert(p1.begin(),p1.end());
      result.insert(p2.begin(),p2.end());
  }
    break;
  case 4: {
    auto p1(chamberIdsCSC(CSC_ME41));  
      auto p2(chamberIdsCSC(CSC_ME42));  
      result.insert(p1.begin(),p1.end());
      result.insert(p2.begin(),p2.end());
  }
      break;
  };
  return result;
}


std::set<unsigned int> 
SimHitMatcher::chamberIdsDTStation(int station) const
{
  set<unsigned int> result;
  switch(station){
  case 1: {
    auto p1(chamberIdsDT(DT_MB21p));  
    auto p2(chamberIdsDT(DT_MB11p));  
    auto p3(chamberIdsDT(DT_MB01));  
    auto p4(chamberIdsDT(DT_MB11n));  
    auto p5(chamberIdsDT(DT_MB21n));  
    result.insert(p1.begin(),p1.end());
    result.insert(p2.begin(),p2.end());
    result.insert(p3.begin(),p3.end());
    result.insert(p4.begin(),p4.end());
    result.insert(p5.begin(),p5.end());
  }
    break;
  case 2: {
    auto p1(chamberIdsDT(DT_MB22p));  
    auto p2(chamberIdsDT(DT_MB12p));  
    auto p3(chamberIdsDT(DT_MB02));  
    auto p4(chamberIdsDT(DT_MB12n));  
    auto p5(chamberIdsDT(DT_MB22n));  
    result.insert(p1.begin(),p1.end());
    result.insert(p2.begin(),p2.end());
    result.insert(p3.begin(),p3.end());
    result.insert(p4.begin(),p4.end());
    result.insert(p5.begin(),p5.end());
  }
    break;
  case 3: {
    auto p1(chamberIdsDT(DT_MB23p));  
    auto p2(chamberIdsDT(DT_MB13p));  
    auto p3(chamberIdsDT(DT_MB03));  
    auto p4(chamberIdsDT(DT_MB13n));  
    auto p5(chamberIdsDT(DT_MB23n));  
    result.insert(p1.begin(),p1.end());
    result.insert(p2.begin(),p2.end());
    result.insert(p3.begin(),p3.end());
    result.insert(p4.begin(),p4.end());
    result.insert(p5.begin(),p5.end());
  }
    break;
  case 4: {
    auto p1(chamberIdsDT(DT_MB24p));  
    auto p2(chamberIdsDT(DT_MB14p));  
    auto p3(chamberIdsDT(DT_MB04));  
    auto p4(chamberIdsDT(DT_MB14n));  
    auto p5(chamberIdsDT(DT_MB24n));  
    result.insert(p1.begin(),p1.end());
    result.insert(p2.begin(),p2.end());
    result.insert(p3.begin(),p3.end());
    result.insert(p4.begin(),p4.end());
    result.insert(p5.begin(),p5.end());
  }
    break;
  };
  return result;
}
