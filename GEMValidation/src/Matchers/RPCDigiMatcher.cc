#include "GEMCode/GEMValidation/interface/Matchers/RPCDigiMatcher.h"

using namespace std;

RPCDigiMatcher::RPCDigiMatcher(const edm::ParameterSet& pset, edm::ConsumesCollector && iC)
{
  const auto& rpcDigi = pset.getParameterSet("rpcStripDigi");
  minRPCDigi_ = rpcDigi.getParameter<int>("minBX");
  maxRPCDigi_ = rpcDigi.getParameter<int>("maxBX");
  matchDeltaStrip_ = rpcDigi.getParameter<int>("matchDeltaStrip");
  verboseDigi_ = rpcDigi.getParameter<int>("verbose");
  run_ = rpcDigi.getParameter<bool>("run");
  // make a new simhits matcher
  muonSimHitMatcher_.reset(new RPCSimHitMatcher(pset, std::move(iC)));

  if (run_)
    rpcDigiToken_ = iC.consumes<RPCDigiCollection>(rpcDigi.getParameter<edm::InputTag>("inputTag"));
  rpcToken_ = iC.esConsumes<RPCGeometry, MuonGeometryRecord>();
}

void RPCDigiMatcher::init(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  muonSimHitMatcher_->init(iEvent, iSetup);

  if (run_)
    iEvent.getByToken(rpcDigiToken_, rpcDigisH_);

  //iSetup.get<MuonGeometryRecord>().get(rpc_geom_);
  //if (rpc_geom_.isValid()) {
  //  rpcGeometry_ = &*rpc_geom_;
  //} else {
  //  edm::LogWarning("RPCDigiMatcher")
  //      << "+++ Info: RPC geometry is unavailable. +++\n";
  //}
  rpcGeometry_ = &iSetup.getData(rpcToken_);
}

/// do the matching
void RPCDigiMatcher::match(const SimTrack& t, const SimVertex& v)
{
  if (run_) {
    // match simhits first
    muonSimHitMatcher_->match(t,v);

    // get the digi collections
    const RPCDigiCollection& rpcDigis = *rpcDigisH_.product();

    // now match the digis
    matchDigisToSimTrack(rpcDigis);
  }
}


void
RPCDigiMatcher::matchDigisToSimTrack(const RPCDigiCollection& digis)
{
  if (verboseDigi_) cout << "Matching simtrack to RPC digis" << endl;
  const auto& det_ids = muonSimHitMatcher_->detIds();
  for (const auto& id: det_ids)
  {
    RPCDetId p_id(id);
    const auto& hit_strips = muonSimHitMatcher_->hitStripsInDetId(id, matchDeltaStrip_);
    if (verboseDigi_)
    {
      cout<<"hit_strips_fat ";
      copy(hit_strips.begin(), hit_strips.end(), ostream_iterator<int>(cout, " "));
      cout<<endl;
    }

    const auto& digis_in_det = digis.get(RPCDetId(id));

    for (auto d = digis_in_det.first; d != digis_in_det.second; ++d)
    {
      if (verboseDigi_) cout<<"RPCDigi "<<p_id<<" "<<*d<<endl;
      // check that the digi is within BX range
      if (d->bx() < minRPCDigi_ || d->bx() > maxRPCDigi_) continue;
      // check that it matches a strip that was hit by SimHits from our track
      if (hit_strips.find(d->strip()) == hit_strips.end()) continue;
      if (verboseDigi_) cout<<"...was matched!"<<endl;

      detid_to_digis_[id].push_back(*d);
      chamber_to_digis_[ p_id.chamberId().rawId() ].push_back(*d);
    }
  }
}


std::set<unsigned int>
RPCDigiMatcher::detIds(int rpc_type) const
{
  return selectDetIds(detid_to_digis_, rpc_type);
}


std::set<unsigned int>
RPCDigiMatcher::chamberIds(int rpc_type) const
{
  return selectDetIds(chamber_to_digis_, rpc_type);
}


const RPCDigiContainer&
RPCDigiMatcher::digisInDetId(unsigned int detid) const
{
  if (detid_to_digis_.find(detid) == detid_to_digis_.end()) return no_rpc_digis_;
  return detid_to_digis_.at(detid);
}


const RPCDigiContainer&
RPCDigiMatcher::digisInChamber(unsigned int detid) const
{
  if (chamber_to_digis_.find(detid) == chamber_to_digis_.end()) return no_rpc_digis_;
  return chamber_to_digis_.at(detid);
}

int
RPCDigiMatcher::nDigis() const
{
  int n = 0;
  for (const auto& id: detIds())
  {
    n += digisInDetId(id).size();
  }
  return n;
}


std::set<int>
RPCDigiMatcher::stripNumbersInDetId(unsigned int detid) const
{
  set<int> result;
  const auto& digis = digisInDetId(detid);
  for (const auto& d: digis)
  {
    result.insert( d.strip() );
  }
  return result;
}


std::set<int>
RPCDigiMatcher::partitionNumbers() const
{
  std::set<int> result;

  const auto& detids = detIds();
  for (const auto& id: detids)
  {
    const RPCDetId& idd(id);
    result.insert( idd.roll() );
  }
  return result;
}


GlobalPoint
RPCDigiMatcher::getGlobalPointDigi(unsigned int rawId, const RPCDigi& d) const
{
  RPCDetId rpc_id(rawId);
  const LocalPoint& rpc_lp = rpcGeometry_->roll(rpc_id)->centreOfStrip(d.strip());
  const GlobalPoint& rpc_gp = rpcGeometry_->idToDet(rpc_id)->surface().toGlobal(rpc_lp);
  return rpc_gp;
}
