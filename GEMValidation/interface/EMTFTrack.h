#ifndef GEMCode_GEMValidation_EMTFTrack_h
#define GEMCode_GEMValidation_EMTFTrack_h

// system include files
#include <memory>
#include <cmath>
#include <vector>
#include <tuple>
#include <iostream>

// user include files
#include "L1Trigger/L1TMuonEndCap/interface/TrackTools.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonCand.h"
#include "DataFormats/L1TMuon/interface/RegionalMuonShower.h"
#include "DataFormats/L1TMuon/interface/EMTFTrack.h"
#include "DataFormats/L1TMuon/interface/EMTFHit.h"
#include "DataFormats/L1Trigger/interface/Muon.h"
#include "DataFormats/Math/interface/normalizedPhi.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

namespace gem {

class EMTFTrack
{
 public:
  /// constructor
  EMTFTrack(const l1t::EMTFTrack& t);
  /// copy constructor
  EMTFTrack(const gem::EMTFTrack& t);
  /// destructor
  ~EMTFTrack();

  bool operator==(const gem::EMTFTrack& rhs) const;

  /// collection of hits
  const l1t::EMTFTrack* emtfTrack() const { return &emtfTrack_; }
  const l1t::EMTFHitCollection* emtfHits() const {return &emtfHits_;}

  unsigned int nStubs() const;
  bool hasStub(int station) const;
  bool hasStub(int station, int ring) const;
  l1t::EMTFHit stub(int station, int ring) const;

  double pt() const {return pt_;}
  double eta() const {return eta_;}
  double phi() const {return phi_;}
  int charge() const {return charge_;}
  double phi_local() const {return phi_local_;}
  double dR() const {return dr_;}
  int quality() const {return quality_;}
  int bx() const {return bx_;}

  int deltaphi(unsigned int index) {return deltaphi_[index]; }
  int deltatheta(unsigned int index) {return deltatheta_[index]; }
  int lctslope(unsigned int index ) {return lctslope_[index]; }
  int lctpattern(unsigned int index) {return lctpattern_[index]; }
  int mode() {return mode_; }

  void setDR(double dr) { dr_ = dr; }

 private:
  //https://github.com/cms-sw/cmssw/blob/master/DataFormats/L1TMuon/interface/EMTFTrack.h#L24
  l1t::EMTFTrack emtfTrack_;
  l1t::EMTFHitCollection emtfHits_;
  int charge_;
  double phi_;
  double phi_local_;
  double eta_;
  double pt_;
  double dr_;
  int quality_;
  int bx_;
  int mode_;
  int deltaphi_[6];
  int deltatheta_[6];
  int lctslope_[4];
  int lctpattern_[4];
};


class EMTFCand
{
 public:
  /// constructor
  EMTFCand(const l1t::RegionalMuonCand& t);
  /// copy constructor
  EMTFCand(const l1t::Muon& t);
  /// destructor
  ~EMTFCand();

  bool operator==(const gem::EMTFCand& rhs) const;

  double pt() const {return pt_;}
  double eta() const {return eta_;}
  double phi() const {return phi_;}
  int charge() const {return charge_;}
  double phi_local() const {return phi_local_;}
  double dR() const {return dr_;}
  int quality() const {return quality_;}
  int bx() const {return bx_;}

  void setDR(double dr) { dr_ = dr; }

 private:
  int charge_;
  double phi_;
  double phi_local_;
  double eta_;
  double pt_;
  double dr_;
  int quality_;
  int bx_;
};

}

std::ostream& operator<<(std::ostream& o, const gem::EMTFTrack& trk);
std::ostream& operator<<(std::ostream& o, const gem::EMTFCand& trk);

#endif
