#include "GEMCode/GEMValidation/interface/Matchers/ME0DigiMatcher.h"

using namespace std;

ME0DigiMatcher::ME0DigiMatcher(const edm::ParameterSet& pset, edm::ConsumesCollector&& iC) {
  const auto& me0Digi = pset.getParameterSet("me0StripDigi");
  minBXDigi_ = me0Digi.getParameter<int>("minBX");
  maxBXDigi_ = me0Digi.getParameter<int>("maxBX");
  matchDeltaStrip_ = me0Digi.getParameter<int>("matchDeltaStrip");
  verboseDigi_ = me0Digi.getParameter<int>("verbose");
  runDigi_ = me0Digi.getParameter<bool>("run");

  const auto& me0Pad = pset.getParameterSet("me0PadDigi");
  minBXPad_ = me0Pad.getParameter<int>("minBX");
  maxBXPad_ = me0Pad.getParameter<int>("maxBX");
  verbosePad_ = me0Pad.getParameter<int>("verbose");
  runPad_ = me0Pad.getParameter<bool>("run");

  // make a new simhits matcher
  muonSimHitMatcher_.reset(new ME0SimHitMatcher(pset, std::move(iC)));

  me0DigiToken_ = iC.consumes<ME0DigiCollection>(me0Digi.getParameter<edm::InputTag>("inputTag"));
  me0PadToken_ = iC.consumes<ME0PadDigiCollection>(me0Pad.getParameter<edm::InputTag>("inputTag"));

  me0Token_ = iC.esConsumes<ME0Geometry, MuonGeometryRecord>();
}

void ME0DigiMatcher::init(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  muonSimHitMatcher_->init(iEvent, iSetup);

  iEvent.getByToken(me0DigiToken_, me0DigisH_);
  iEvent.getByToken(me0PadToken_, me0PadsH_);

  //iSetup.get<MuonGeometryRecord>().get(me0_geom_);
  //if (me0_geom_.isValid()) {
  //  me0Geometry_ = &*me0_geom_;
  //} else {
  //  std::cout << "+++ Info: ME0 geometry is unavailable. +++\n";
  //}
  me0Geometry_ = &iSetup.getData(me0Token_); 
}

/// do the matching
void ME0DigiMatcher::match(const SimTrack& t, const SimVertex& v) {
  // match simhits first
  muonSimHitMatcher_->match(t, v);

  // get the digi collections
  const ME0DigiCollection& me0Digis = *me0DigisH_.product();
  const ME0PadDigiCollection& me0Pads = *me0PadsH_.product();

  // now match the digis
  if (runDigi_)
    matchDigisToSimTrack(me0Digis);

  if (runPad_)
    matchPadsToSimTrack(me0Pads);
}

void ME0DigiMatcher::matchDigisToSimTrack(const ME0DigiCollection& digis) {
  if (verboseDigi_)
    cout << "Matching simtrack to ME0 digis" << endl;
  const auto& det_ids = muonSimHitMatcher_->detIds();
  for (const auto& id : det_ids) {
    ME0DetId p_id(id);
    const auto& hit_strips = muonSimHitMatcher_->hitStripsInDetId(id, matchDeltaStrip_);
    if (verboseDigi_) {
      cout << "hit_strips_fat ";
      copy(hit_strips.begin(), hit_strips.end(), ostream_iterator<int>(cout, " "));
      cout << endl;
    }

    const auto& digis_in_det = digis.get(ME0DetId(id));

    for (auto d = digis_in_det.first; d != digis_in_det.second; ++d) {
      if (verboseDigi_)
        cout << "ME0Digi " << p_id << " " << *d << endl;
      // check that the digi is within BX range
      if (d->bx() < minBXDigi_ || d->bx() > maxBXDigi_)
        continue;
      // check that it matches a strip that was hit by SimHits from our track
      if (hit_strips.find(d->strip()) == hit_strips.end())
        continue;
      if (verboseDigi_)
        cout << "...was matched!" << endl;

      detid_to_digis_[id].push_back(*d);
      chamber_to_digis_[p_id.layerId().rawId()].push_back(*d);
      superchamber_to_digis_[p_id.chamberId().rawId()].push_back(*d);
    }
  }
}

void ME0DigiMatcher::matchPadsToSimTrack(const ME0PadDigiCollection& pads) {
  const auto& det_ids = muonSimHitMatcher_->detIds();
  for (const auto& id : det_ids) {
    ME0DetId p_id(id);

    const auto& hit_pads = muonSimHitMatcher_->hitPadsInDetId(id);
    const auto& pads_in_det = pads.get(p_id);

    if (verbosePad_) {
      cout << "checkpads " << hit_pads.size() << " " << std::distance(pads_in_det.first, pads_in_det.second)
           << " hit_pads: ";
      copy(hit_pads.begin(), hit_pads.end(), ostream_iterator<int>(cout, " "));
      cout << endl;
    }

    for (auto pad = pads_in_det.first; pad != pads_in_det.second; ++pad) {
      if (verbosePad_)
        cout << "chp " << *pad << endl;
      // check that the pad BX is within the range
      if (pad->bx() < minBXPad_ || pad->bx() > maxBXPad_)
        continue;
      if (verbosePad_)
        cout << "chp1" << endl;
      // check that it matches a pad that was hit by SimHits from our track
      if (hit_pads.find(pad->pad()) == hit_pads.end())
        continue;
      if (verbosePad_)
        cout << "chp2" << endl;

      detid_to_pads_[id].push_back(*pad);
      chamber_to_pads_[p_id.layerId().rawId()].push_back(*pad);
      superchamber_to_pads_[p_id.chamberId()].push_back(*pad);
    }
  }
}

std::set<unsigned int> ME0DigiMatcher::detIdsDigi() const {
  std::set<unsigned int> result;
  for (const auto& p : detid_to_digis_) {
    result.insert(p.first);
  }
  return result;
}

std::set<unsigned int> ME0DigiMatcher::detIdsPad() const {
  std::set<unsigned int> result;
  for (const auto& p : detid_to_pads_) {
    result.insert(p.first);
  }
  return result;
}

std::set<unsigned int> ME0DigiMatcher::chamberIdsDigi() const {
  std::set<unsigned int> result;
  for (const auto& p : chamber_to_digis_) {
    result.insert(p.first);
  }
  return result;
}

std::set<unsigned int> ME0DigiMatcher::chamberIdsPad() const {
  std::set<unsigned int> result;
  for (const auto& p : chamber_to_pads_) {
    result.insert(p.first);
  }
  return result;
}

std::set<unsigned int> ME0DigiMatcher::superChamberIdsDigi() const {
  std::set<unsigned int> result;
  for (const auto& p : superchamber_to_digis_) {
    result.insert(p.first);
  }
  return result;
}

std::set<unsigned int> ME0DigiMatcher::superChamberIdsPad() const {
  std::set<unsigned int> result;
  for (const auto& p : superchamber_to_pads_) {
    result.insert(p.first);
  }
  return result;
}

const ME0DigiContainer& ME0DigiMatcher::digisInDetId(unsigned int detid) const {
  if (detid_to_digis_.find(detid) == detid_to_digis_.end())
    return no_me0_digis_;
  return detid_to_digis_.at(detid);
}

const ME0DigiContainer& ME0DigiMatcher::digisInChamber(unsigned int detid) const {
  if (chamber_to_digis_.find(detid) == chamber_to_digis_.end())
    return no_me0_digis_;
  return chamber_to_digis_.at(detid);
}

const ME0DigiContainer& ME0DigiMatcher::digisInSuperChamber(unsigned int detid) const {
  if (superchamber_to_digis_.find(detid) == superchamber_to_digis_.end())
    return no_me0_digis_;
  return superchamber_to_digis_.at(detid);
}

const ME0PadDigiContainer& ME0DigiMatcher::padsInDetId(unsigned int detid) const {
  if (detid_to_pads_.find(detid) == detid_to_pads_.end())
    return no_me0_pads_;
  return detid_to_pads_.at(detid);
}

const ME0PadDigiContainer& ME0DigiMatcher::padsInChamber(unsigned int detid) const {
  if (chamber_to_pads_.find(detid) == chamber_to_pads_.end())
    return no_me0_pads_;
  return chamber_to_pads_.at(detid);
}

const ME0PadDigiContainer& ME0DigiMatcher::padsInSuperChamber(unsigned int detid) const {
  if (superchamber_to_pads_.find(detid) == superchamber_to_pads_.end())
    return no_me0_pads_;
  return superchamber_to_pads_.at(detid);
}

int ME0DigiMatcher::nLayersWithDigisInSuperChamber(unsigned int detid) const {
  set<int> layers;
  ME0DetId sch_id(detid);
  for (int iLayer = 1; iLayer <= 6; iLayer++) {
    ME0DetId ch_id(sch_id.region(), iLayer, sch_id.chamber(), 0);
    // get the digis in this chamber
    const auto& digis = digisInChamber(ch_id.rawId());
    // at least one digi in this layer!
    if (!digis.empty()) {
      layers.insert(iLayer);
    }
  }
  return layers.size();
}

int ME0DigiMatcher::nLayersWithPadsInSuperChamber(unsigned int detid) const {
  set<int> layers;
  ME0DetId sch_id(detid);
  for (int iLayer = 1; iLayer <= 6; iLayer++) {
    ME0DetId ch_id(sch_id.region(), iLayer, sch_id.chamber(), 0);
    // get the pads in this chamber
    const auto& pads = padsInChamber(ch_id.rawId());
    // at least one digi in this layer!
    if (!pads.empty()) {
      layers.insert(iLayer);
    }
  }
  return layers.size();
}

int ME0DigiMatcher::nPads() const {
  int n = 0;
  const auto& ids = superChamberIdsPad();
  for (const auto& id : ids) {
    n += padsInSuperChamber(id).size();
  }
  return n;
}

std::set<int> ME0DigiMatcher::stripNumbersInDetId(unsigned int detid) const {
  set<int> result;
  const auto& digis = digisInDetId(detid);
  for (const auto& d : digis) {
    result.insert(d.strip());
  }
  return result;
}

std::set<int> ME0DigiMatcher::padNumbersInDetId(unsigned int detid) const {
  set<int> result;
  const auto& digis = padsInDetId(detid);
  for (const auto& d : digis) {
    result.insert(d.pad());
  }
  return result;
}

std::set<int> ME0DigiMatcher::partitionNumbers() const {
  std::set<int> result;

  const auto& detids = detIdsDigi();
  for (const auto& id : detids) {
    const ME0DetId& idd(id);
    result.insert(idd.roll());
  }
  return result;
}

GlobalPoint ME0DigiMatcher::getGlobalPointDigi(unsigned int rawId, const ME0Digi& d) const {
  ME0DetId me0_id(rawId);
  const LocalPoint& me0_lp = me0Geometry_->etaPartition(me0_id)->centreOfStrip(d.strip());
  const GlobalPoint& me0_gp = me0Geometry_->idToDet(me0_id)->surface().toGlobal(me0_lp);
  return me0_gp;
}

GlobalPoint ME0DigiMatcher::getGlobalPointPad(unsigned int rawId, const ME0PadDigi& tp) const {
  ME0DetId me0_id(rawId);
  const LocalPoint& me0_lp = me0Geometry_->etaPartition(me0_id)->centreOfPad(tp.pad());
  const GlobalPoint& me0_gp = me0Geometry_->idToDet(me0_id)->surface().toGlobal(me0_lp);
  return me0_gp;
}
