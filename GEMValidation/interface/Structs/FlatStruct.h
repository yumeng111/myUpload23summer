#ifndef GEMCode_GEMValidation_FlatStruct
#define GEMCode_GEMValidation_FlatStruct

#include "TTree.h"
#include <vector>
#include <string>

typedef std::vector<float> t_floats;
typedef t_floats* p_floats;
typedef std::vector<int> t_ints;
typedef t_ints* p_ints;
typedef std::vector<t_ints> t_vints;
typedef t_vints* p_vints;
typedef std::vector<unsigned long long> t_longs;
typedef t_longs* p_longs;

namespace my {

  struct EventStruct {

    p_ints event_run;
    p_ints event_lumi;
    p_longs event_event;
    p_ints event_bx;
    p_ints event_orbit;
    p_ints event_time;

    void init() {
      event_run = new t_ints;
      event_lumi = new t_ints;
      event_event = new t_longs;
      event_bx = new t_ints;
      event_orbit = new t_ints;
      event_time = new t_ints;
    }

    void book(TTree* t) {
      t->Branch("event_run", &event_run);
      t->Branch("event_lumi", &event_lumi);
      t->Branch("event_event", &event_event);
      t->Branch("event_bx", &event_bx);
      t->Branch("event_orbit", &event_orbit);
      t->Branch("event_time", &event_time);
    }
  };

  struct GenParticleStruct {

    p_ints gen_charge;
    p_ints gen_pdgid;
    p_ints gen_tpid;
    p_ints gen_llp_in_acceptance;
    p_floats gen_pt;
    p_floats gen_pz;
    p_floats gen_eta;
    p_floats gen_phi;
    p_floats gen_dxy;
    p_floats gen_d0;
    p_floats gen_z0;
    p_floats gen_d0_prod;
    p_floats gen_z0_prod;
    p_floats gen_vx;
    p_floats gen_vy;
    p_floats gen_vz;
    p_floats gen_r;

    void init() {
      gen_charge = new t_ints;
      gen_pdgid = new t_ints;
      gen_tpid = new t_ints;
      gen_llp_in_acceptance = new t_ints;
      gen_pt = new t_floats;
      gen_pz = new t_floats;
      gen_eta = new t_floats;
      gen_phi = new t_floats;
      gen_dxy = new t_floats;
      gen_d0 = new t_floats;
      gen_z0 = new t_floats;
      gen_d0_prod = new t_floats;
      gen_z0_prod = new t_floats;
      gen_vx = new t_floats;
      gen_vy = new t_floats;
      gen_vz = new t_floats;
      gen_r = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("gen_charge", &gen_charge);
      t->Branch("gen_pdgid", &gen_pdgid);
      t->Branch("gen_tpid", &gen_tpid);
      t->Branch("gen_llp_in_acceptance", &gen_llp_in_acceptance);
      t->Branch("gen_pt", &gen_pt);
      t->Branch("gen_pz", &gen_pz);
      t->Branch("gen_eta", &gen_eta);
      t->Branch("gen_phi", &gen_phi);
      t->Branch("gen_dxy", &gen_dxy);
      t->Branch("gen_d0", &gen_d0);
      t->Branch("gen_z0", &gen_z0);
      t->Branch("gen_d0_prod", &gen_d0_prod);
      t->Branch("gen_z0_prod", &gen_z0_prod);
      t->Branch("gen_vx", &gen_vx);
      t->Branch("gen_vy", &gen_vy);
      t->Branch("gen_vz", &gen_vz);
      t->Branch("gen_r", &gen_r);
    }
  };

  struct SimTrackStruct {

    p_ints sim_charge;
    p_ints sim_pdgid;
    p_ints sim_index;
    p_ints sim_id_gen;
    p_floats sim_pt;
    p_floats sim_px;
    p_floats sim_py;
    p_floats sim_pz;
    p_floats sim_eta;
    p_floats sim_phi;
    p_floats sim_vx;
    p_floats sim_vy;
    p_floats sim_vz;
    p_floats sim_d0;
    p_floats sim_z0;
    p_floats sim_d0_prod;
    p_floats sim_z0_prod;
    p_vints sim_id_gem_sh;
    p_vints sim_id_gem_dg;
    p_vints sim_id_gem_pad;
    p_vints sim_id_gem_copad;
    p_vints sim_id_gem_cluster;
    p_vints sim_id_csc_sh;
    p_vints sim_id_csc_wire;
    p_vints sim_id_csc_strip;
    p_vints sim_id_csc_preclct;
    p_vints sim_id_csc_clct;
    p_vints sim_id_csc_alct;
    p_vints sim_id_csc_lct;
    p_vints sim_id_csc_mplct;
    p_vints sim_id_csc_shower;
    p_vints sim_id_emtf_track;
    p_vints sim_id_emtf_cand;
    p_vints sim_id_emtf_shower;
    p_vints sim_id_l1mu;
    p_vints sim_id_l1mu_shower;
    p_vints sim_id_l1track;
    p_vints sim_id_l1trackmu;

    void init() {
      sim_charge = new t_ints;
      sim_pdgid = new t_ints;
      sim_index = new t_ints;
      sim_id_gen = new t_ints;
      sim_pt = new t_floats;
      sim_px = new t_floats;
      sim_py = new t_floats;
      sim_pz = new t_floats;
      sim_eta = new t_floats;
      sim_phi = new t_floats;
      sim_vx = new t_floats;
      sim_vy = new t_floats;
      sim_vz = new t_floats;
      sim_d0 = new t_floats;
      sim_z0 = new t_floats;
      sim_d0_prod = new t_floats;
      sim_z0_prod = new t_floats;
      sim_id_gem_sh = new t_vints;
      sim_id_gem_dg = new t_vints;
      sim_id_gem_pad = new t_vints;
      sim_id_gem_copad = new t_vints;
      sim_id_gem_cluster = new t_vints;
      sim_id_csc_sh = new t_vints;
      sim_id_csc_wire = new t_vints;
      sim_id_csc_strip = new t_vints;
      sim_id_csc_preclct = new t_vints;
      sim_id_csc_clct = new t_vints;
      sim_id_csc_alct = new t_vints;
      sim_id_csc_lct = new t_vints;
      sim_id_csc_mplct = new t_vints;
      sim_id_csc_shower = new t_vints;
      sim_id_emtf_track = new t_vints;
      sim_id_emtf_cand = new t_vints;
      sim_id_emtf_shower = new t_vints;
      sim_id_l1mu = new t_vints;
      sim_id_l1mu_shower = new t_vints;
      sim_id_l1track = new t_vints;
      sim_id_l1trackmu = new t_vints;
    }

    void book(TTree* t) {
      t->Branch("sim_charge", &sim_charge);
      t->Branch("sim_pdgid", &sim_pdgid);
      t->Branch("sim_index", &sim_index);
      t->Branch("sim_id_gen", &sim_id_gen);
      t->Branch("sim_pt", &sim_pt);
      t->Branch("sim_px", &sim_px);
      t->Branch("sim_py", &sim_py);
      t->Branch("sim_pz", &sim_pz);
      t->Branch("sim_eta", &sim_eta);
      t->Branch("sim_phi", &sim_phi);
      t->Branch("sim_vx", &sim_vx);
      t->Branch("sim_vy", &sim_vy);
      t->Branch("sim_vz", &sim_vz);
      t->Branch("sim_d0", &sim_d0);
      t->Branch("sim_z0", &sim_z0);
      t->Branch("sim_d0_prod", &sim_d0_prod);
      t->Branch("sim_z0_prod", &sim_z0_prod);
      t->Branch("sim_id_gem_sh", &sim_id_gem_sh);
      t->Branch("sim_id_gem_dg", &sim_id_gem_dg);
      t->Branch("sim_id_gem_pad", &sim_id_gem_pad);
      t->Branch("sim_id_gem_copad", &sim_id_gem_copad);
      t->Branch("sim_id_gem_cluster", &sim_id_gem_cluster);
      t->Branch("sim_id_csc_sh", &sim_id_csc_sh);
      t->Branch("sim_id_csc_wire", &sim_id_csc_wire);
      t->Branch("sim_id_csc_strip", &sim_id_csc_strip);
      t->Branch("sim_id_csc_preclct", &sim_id_csc_preclct);
      t->Branch("sim_id_csc_clct", &sim_id_csc_clct);
      t->Branch("sim_id_csc_alct", &sim_id_csc_alct);
      t->Branch("sim_id_csc_lct", &sim_id_csc_lct);
      t->Branch("sim_id_csc_mplct", &sim_id_csc_mplct);
      t->Branch("sim_id_csc_shower", &sim_id_csc_shower);
      t->Branch("sim_id_emtf_track", &sim_id_emtf_track);
      t->Branch("sim_id_emtf_cand", &sim_id_emtf_cand);
      t->Branch("sim_id_emtf_shower", &sim_id_emtf_shower);
      t->Branch("sim_id_l1mu", &sim_id_l1mu);
      t->Branch("sim_id_l1mu_shower", &sim_id_l1mu_shower);
      t->Branch("sim_id_l1track", &sim_id_l1track);
      t->Branch("sim_id_l1trackmu", &sim_id_l1trackmu);
    }
  };

  struct GEMSimHitStruct {

    p_ints gem_sh_bx;
    p_ints gem_sh_strip_l1;
    p_ints gem_sh_strip_l2;
    p_ints gem_sh_isodd;
    p_ints gem_sh_region;
    p_ints gem_sh_station;
    p_ints gem_sh_ring;
    p_ints gem_sh_chamber;
    p_ints gem_sh_tpid;
    p_ints gem_sh_nlayers;
    p_floats gem_sh_dphi;
    p_floats gem_sh_bend;
    p_floats gem_sh_phi;
    p_floats gem_sh_eta;
    p_floats gem_sh_perp;

    void init() {
      gem_sh_bx = new t_ints;
      gem_sh_strip_l1 = new t_ints;
      gem_sh_strip_l2 = new t_ints;
      gem_sh_isodd = new t_ints;
      gem_sh_region = new t_ints;
      gem_sh_station = new t_ints;
      gem_sh_ring = new t_ints;
      gem_sh_chamber = new t_ints;
      gem_sh_tpid = new t_ints;
      gem_sh_nlayers = new t_ints;
      gem_sh_dphi = new t_floats;
      gem_sh_bend = new t_floats;
      gem_sh_phi = new t_floats;
      gem_sh_eta = new t_floats;
      gem_sh_perp = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("gem_sh_bx", &gem_sh_bx);
      t->Branch("gem_sh_strip_l1", &gem_sh_strip_l1);
      t->Branch("gem_sh_strip_l2", &gem_sh_strip_l2);
      t->Branch("gem_sh_isodd", &gem_sh_isodd);
      t->Branch("gem_sh_region", &gem_sh_region);
      t->Branch("gem_sh_station", &gem_sh_station);
      t->Branch("gem_sh_ring", &gem_sh_ring);
      t->Branch("gem_sh_chamber", &gem_sh_chamber);
      t->Branch("gem_sh_tpid", &gem_sh_tpid);
      t->Branch("gem_sh_nlayers", &gem_sh_nlayers);
      t->Branch("gem_sh_dphi", &gem_sh_dphi);
      t->Branch("gem_sh_bend", &gem_sh_bend);
      t->Branch("gem_sh_phi", &gem_sh_phi);
      t->Branch("gem_sh_eta", &gem_sh_eta);
      t->Branch("gem_sh_perp", &gem_sh_perp);
    }
  };

  struct CSCSimHitStruct {

    p_ints csc_sh_bx;
    p_ints csc_sh_keyhs;
    p_ints csc_sh_isodd;
    p_ints csc_sh_region;
    p_ints csc_sh_station;
    p_ints csc_sh_ring;
    p_ints csc_sh_chamber;
    p_ints csc_sh_tpid;
    p_ints csc_sh_nlayers;
    p_ints csc_sh_strip_l1;
    p_ints csc_sh_strip_l6;
    p_floats csc_sh_dphi;
    p_floats csc_sh_bend;
    p_floats csc_sh_phi;
    p_floats csc_sh_phi_l1;
    p_floats csc_sh_phi_l6;
    p_floats csc_sh_eta;
    p_floats csc_sh_perp;

    void init() {
      csc_sh_bx = new t_ints;
      csc_sh_keyhs = new t_ints;
      csc_sh_isodd = new t_ints;
      csc_sh_region = new t_ints;
      csc_sh_station = new t_ints;
      csc_sh_ring = new t_ints;
      csc_sh_chamber = new t_ints;
      csc_sh_tpid = new t_ints;
      csc_sh_nlayers = new t_ints;
      csc_sh_strip_l1 = new t_ints;
      csc_sh_strip_l6 = new t_ints;
      csc_sh_dphi = new t_floats;
      csc_sh_bend = new t_floats;
      csc_sh_phi = new t_floats;
      csc_sh_phi_l1 = new t_floats;
      csc_sh_phi_l6 = new t_floats;
      csc_sh_eta = new t_floats;
      csc_sh_perp = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("csc_sh_bx", &csc_sh_bx);
      t->Branch("csc_sh_keyhs", &csc_sh_keyhs);
      t->Branch("csc_sh_isodd", &csc_sh_isodd);
      t->Branch("csc_sh_region", &csc_sh_region);
      t->Branch("csc_sh_station", &csc_sh_station);
      t->Branch("csc_sh_ring", &csc_sh_ring);
      t->Branch("csc_sh_chamber", &csc_sh_chamber);
      t->Branch("csc_sh_tpid", &csc_sh_tpid);
      t->Branch("csc_sh_nlayers", &csc_sh_nlayers);
      t->Branch("csc_sh_strip_l1", &csc_sh_strip_l1);
      t->Branch("csc_sh_strip_l6", &csc_sh_strip_l6);
      t->Branch("csc_sh_dphi", &csc_sh_dphi);
      t->Branch("csc_sh_bend", &csc_sh_bend);
      t->Branch("csc_sh_phi", &csc_sh_phi);
      t->Branch("csc_sh_phi_l1", &csc_sh_phi_l1);
      t->Branch("csc_sh_phi_l6", &csc_sh_phi_l6);
      t->Branch("csc_sh_eta", &csc_sh_eta);
      t->Branch("csc_sh_perp", &csc_sh_perp);
    }
  };

  struct GEMDigiStruct {

    p_ints gem_digi_bx;
    p_ints gem_digi_strip;
    p_ints gem_digi_isodd;
    p_ints gem_digi_region;
    p_ints gem_digi_station;
    p_ints gem_digi_roll;
    p_ints gem_digi_layer;
    p_ints gem_digi_chamber;
    p_ints gem_digi_tpid;
    p_floats gem_digi_phi;
    p_floats gem_digi_eta;

    void init() {
      gem_digi_bx = new t_ints;
      gem_digi_strip = new t_ints;
      gem_digi_isodd = new t_ints;
      gem_digi_region = new t_ints;
      gem_digi_station = new t_ints;
      gem_digi_roll = new t_ints;
      gem_digi_layer = new t_ints;
      gem_digi_chamber = new t_ints;
      gem_digi_tpid = new t_ints;
      gem_digi_phi = new t_floats;
      gem_digi_eta = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("gem_digi_bx", &gem_digi_bx);
      t->Branch("gem_digi_strip", &gem_digi_strip);
      t->Branch("gem_digi_isodd", &gem_digi_isodd);
      t->Branch("gem_digi_region", &gem_digi_region);
      t->Branch("gem_digi_station", &gem_digi_station);
      t->Branch("gem_digi_roll", &gem_digi_roll);
      t->Branch("gem_digi_layer", &gem_digi_layer);
      t->Branch("gem_digi_chamber", &gem_digi_chamber);
      t->Branch("gem_digi_tpid", &gem_digi_tpid);
      t->Branch("gem_digi_phi", &gem_digi_phi);
      t->Branch("gem_digi_eta", &gem_digi_eta);
    }
  };

  struct CSCDigiStruct {

    p_ints csc_strip_bx;
    p_ints csc_strip_strip;
    p_ints csc_strip_isodd;
    p_ints csc_strip_region;
    p_ints csc_strip_station;
    p_ints csc_strip_ring;
    p_ints csc_strip_quality;
    p_ints csc_strip_chamber;
    p_ints csc_strip_tpid;
    p_ints csc_comp_time;
    p_ints csc_comp_hs;
    p_ints csc_comp_isodd;
    p_ints csc_comp_region;
    p_ints csc_comp_station;
    p_ints csc_comp_layer;
    p_ints csc_comp_ring;
    p_ints csc_comp_quality;
    p_ints csc_comp_chamber;
    p_ints csc_comp_tpid;
    p_ints csc_wire_time;
    p_ints csc_wire_keywg;
    p_ints csc_wire_isodd;
    p_ints csc_wire_region;
    p_ints csc_wire_station;
    p_ints csc_wire_ring;
    p_ints csc_wire_layer;
    p_ints csc_wire_chamber;
    p_ints csc_wire_tpid;

    void init() {
      csc_strip_bx = new t_ints;
      csc_strip_strip = new t_ints;
      csc_strip_isodd = new t_ints;
      csc_strip_region = new t_ints;
      csc_strip_station = new t_ints;
      csc_strip_ring = new t_ints;
      csc_strip_quality = new t_ints;
      csc_strip_chamber = new t_ints;
      csc_strip_tpid = new t_ints;
      csc_comp_time = new t_ints;
      csc_comp_hs = new t_ints;
      csc_comp_isodd = new t_ints;
      csc_comp_region = new t_ints;
      csc_comp_station = new t_ints;
      csc_comp_layer = new t_ints;
      csc_comp_ring = new t_ints;
      csc_comp_quality = new t_ints;
      csc_comp_chamber = new t_ints;
      csc_comp_tpid = new t_ints;
      csc_wire_time = new t_ints;
      csc_wire_keywg = new t_ints;
      csc_wire_isodd = new t_ints;
      csc_wire_region = new t_ints;
      csc_wire_station = new t_ints;
      csc_wire_ring = new t_ints;
      csc_wire_layer = new t_ints;
      csc_wire_chamber = new t_ints;
      csc_wire_tpid = new t_ints;
    }

    void book(TTree* t) {
      t->Branch("csc_strip_bx", &csc_strip_bx);
      t->Branch("csc_strip_strip", &csc_strip_strip);
      t->Branch("csc_strip_isodd", &csc_strip_isodd);
      t->Branch("csc_strip_region", &csc_strip_region);
      t->Branch("csc_strip_station", &csc_strip_station);
      t->Branch("csc_strip_ring", &csc_strip_ring);
      t->Branch("csc_strip_quality", &csc_strip_quality);
      t->Branch("csc_strip_chamber", &csc_strip_chamber);
      t->Branch("csc_strip_tpid", &csc_strip_tpid);
      t->Branch("csc_comp_time", &csc_comp_time);
      t->Branch("csc_comp_hs", &csc_comp_hs);
      t->Branch("csc_comp_isodd", &csc_comp_isodd);
      t->Branch("csc_comp_region", &csc_comp_region);
      t->Branch("csc_comp_station", &csc_comp_station);
      t->Branch("csc_comp_layer", &csc_comp_layer);
      t->Branch("csc_comp_ring", &csc_comp_ring);
      t->Branch("csc_comp_quality", &csc_comp_quality);
      t->Branch("csc_comp_chamber", &csc_comp_chamber);
      t->Branch("csc_comp_tpid", &csc_comp_tpid);
      t->Branch("csc_wire_time", &csc_wire_time);
      t->Branch("csc_wire_keywg", &csc_wire_keywg);
      t->Branch("csc_wire_isodd", &csc_wire_isodd);
      t->Branch("csc_wire_region", &csc_wire_region);
      t->Branch("csc_wire_station", &csc_wire_station);
      t->Branch("csc_wire_ring", &csc_wire_ring);
      t->Branch("csc_wire_layer", &csc_wire_layer);
      t->Branch("csc_wire_chamber", &csc_wire_chamber);
      t->Branch("csc_wire_tpid", &csc_wire_tpid);
    }
  };

  struct GEMStubStruct {

    p_ints gem_pad_bx;
    p_ints gem_pad_pad;
    p_ints gem_pad_isodd;
    p_ints gem_pad_region;
    p_ints gem_pad_station;
    p_ints gem_pad_roll;
    p_ints gem_pad_layer;
    p_ints gem_pad_chamber;
    p_ints gem_pad_tpid;
    p_floats gem_pad_phi;
    p_floats gem_pad_eta;
    p_ints gem_copad_bx;
    p_ints gem_copad_pad;
    p_ints gem_copad_isodd;
    p_ints gem_copad_region;
    p_ints gem_copad_station;
    p_ints gem_copad_roll;
    p_ints gem_copad_layer;
    p_ints gem_copad_chamber;
    p_ints gem_copad_tpid;
    p_floats gem_copad_phi;
    p_floats gem_copad_eta;
    p_ints gem_cluster_bx;
    p_ints gem_cluster_pad;
    p_ints gem_cluster_isodd;
    p_ints gem_cluster_size;
    p_ints gem_cluster_region;
    p_ints gem_cluster_station;
    p_ints gem_cluster_roll;
    p_ints gem_cluster_layer;
    p_ints gem_cluster_chamber;
    p_ints gem_cluster_tpid;
    p_floats gem_cluster_phi;
    p_floats gem_cluster_eta;

    void init() {
      gem_pad_bx = new t_ints;
      gem_pad_pad = new t_ints;
      gem_pad_isodd = new t_ints;
      gem_pad_region = new t_ints;
      gem_pad_station = new t_ints;
      gem_pad_roll = new t_ints;
      gem_pad_layer = new t_ints;
      gem_pad_chamber = new t_ints;
      gem_pad_tpid = new t_ints;
      gem_pad_phi = new t_floats;
      gem_pad_eta = new t_floats;
      gem_copad_bx = new t_ints;
      gem_copad_pad = new t_ints;
      gem_copad_isodd = new t_ints;
      gem_copad_region = new t_ints;
      gem_copad_station = new t_ints;
      gem_copad_roll = new t_ints;
      gem_copad_layer = new t_ints;
      gem_copad_chamber = new t_ints;
      gem_copad_tpid = new t_ints;
      gem_copad_phi = new t_floats;
      gem_copad_eta = new t_floats;
      gem_cluster_bx = new t_ints;
      gem_cluster_pad = new t_ints;
      gem_cluster_isodd = new t_ints;
      gem_cluster_size = new t_ints;
      gem_cluster_region = new t_ints;
      gem_cluster_station = new t_ints;
      gem_cluster_roll = new t_ints;
      gem_cluster_layer = new t_ints;
      gem_cluster_chamber = new t_ints;
      gem_cluster_tpid = new t_ints;
      gem_cluster_phi = new t_floats;
      gem_cluster_eta = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("gem_pad_bx", &gem_pad_bx);
      t->Branch("gem_pad_pad", &gem_pad_pad);
      t->Branch("gem_pad_isodd", &gem_pad_isodd);
      t->Branch("gem_pad_region", &gem_pad_region);
      t->Branch("gem_pad_station", &gem_pad_station);
      t->Branch("gem_pad_roll", &gem_pad_roll);
      t->Branch("gem_pad_layer", &gem_pad_layer);
      t->Branch("gem_pad_chamber", &gem_pad_chamber);
      t->Branch("gem_pad_tpid", &gem_pad_tpid);
      t->Branch("gem_pad_phi", &gem_pad_phi);
      t->Branch("gem_pad_eta", &gem_pad_eta);
      t->Branch("gem_copad_bx", &gem_copad_bx);
      t->Branch("gem_copad_pad", &gem_copad_pad);
      t->Branch("gem_copad_isodd", &gem_copad_isodd);
      t->Branch("gem_copad_region", &gem_copad_region);
      t->Branch("gem_copad_station", &gem_copad_station);
      t->Branch("gem_copad_roll", &gem_copad_roll);
      t->Branch("gem_copad_layer", &gem_copad_layer);
      t->Branch("gem_copad_chamber", &gem_copad_chamber);
      t->Branch("gem_copad_tpid", &gem_copad_tpid);
      t->Branch("gem_copad_phi", &gem_copad_phi);
      t->Branch("gem_copad_eta", &gem_copad_eta);
      t->Branch("gem_cluster_bx", &gem_cluster_bx);
      t->Branch("gem_cluster_pad", &gem_cluster_pad);
      t->Branch("gem_cluster_isodd", &gem_cluster_isodd);
      t->Branch("gem_cluster_size", &gem_cluster_size);
      t->Branch("gem_cluster_region", &gem_cluster_region);
      t->Branch("gem_cluster_station", &gem_cluster_station);
      t->Branch("gem_cluster_roll", &gem_cluster_roll);
      t->Branch("gem_cluster_layer", &gem_cluster_layer);
      t->Branch("gem_cluster_chamber", &gem_cluster_chamber);
      t->Branch("gem_cluster_tpid", &gem_cluster_tpid);
      t->Branch("gem_cluster_phi", &gem_cluster_phi);
      t->Branch("gem_cluster_eta", &gem_cluster_eta);
    }
  };

  struct CSCStubStruct {

    p_ints csc_alct_bx;
    p_ints csc_alct_wg;
    p_ints csc_alct_isodd;
    p_ints csc_alct_region;
    p_ints csc_alct_station;
    p_ints csc_alct_ring;
    p_ints csc_alct_sector;
    p_ints csc_alct_chamber;
    p_ints csc_alct_quality;
    p_ints csc_alct_tpid;
    p_ints csc_preclct_bx;
    p_ints csc_preclct_hs;
    p_ints csc_preclct_qs;
    p_ints csc_preclct_es;
    p_ints csc_preclct_isodd;
    p_ints csc_preclct_region;
    p_ints csc_preclct_station;
    p_ints csc_preclct_ring;
    p_ints csc_preclct_sector;
    p_ints csc_preclct_chamber;
    p_ints csc_preclct_quality;
    p_ints csc_preclct_pattern;
    p_ints csc_preclct_pattern_run3;
    p_ints csc_preclct_tpid;
    p_ints csc_clct_bx;
    p_ints csc_clct_hs;
    p_ints csc_clct_qs;
    p_ints csc_clct_es;
    p_ints csc_clct_isodd;
    p_ints csc_clct_region;
    p_ints csc_clct_station;
    p_ints csc_clct_ring;
    p_ints csc_clct_sector;
    p_ints csc_clct_chamber;
    p_ints csc_clct_quality;
    p_ints csc_clct_pattern;
    p_ints csc_clct_pattern_run3;
    p_ints csc_clct_tpid;
    p_ints csc_lct_bx;
    p_ints csc_lct_wg;
    p_ints csc_lct_hs;
    p_ints csc_lct_qs;
    p_ints csc_lct_es;
    p_ints csc_lct_isodd;
    p_ints csc_lct_region;
    p_ints csc_lct_station;
    p_ints csc_lct_ring;
    p_ints csc_lct_sector;
    p_ints csc_lct_chamber;
    p_ints csc_lct_quality;
    p_ints csc_lct_pattern;
    p_ints csc_lct_pattern_run3;
    p_ints csc_lct_tpid;
    p_ints csc_mplct_bx;
    p_ints csc_mplct_wg;
    p_ints csc_mplct_hs;
    p_ints csc_mplct_qs;
    p_ints csc_mplct_es;
    p_ints csc_mplct_isodd;
    p_ints csc_mplct_region;
    p_ints csc_mplct_station;
    p_ints csc_mplct_ring;
    p_ints csc_mplct_sector;
    p_ints csc_mplct_chamber;
    p_ints csc_mplct_quality;
    p_ints csc_mplct_pattern;
    p_ints csc_mplct_pattern_run3;
    p_ints csc_mplct_tpid;
    p_ints csc_shower_bx;
    p_ints csc_shower_region;
    p_ints csc_shower_station;
    p_ints csc_shower_ring;
    p_ints csc_shower_chamber;
    p_ints csc_shower_sector;
    p_ints csc_shower_anodehits;
    p_ints csc_shower_cathodehits;
    p_ints csc_shower_isLooseInTime;
    p_ints csc_shower_isNominalInTime;
    p_ints csc_shower_isTightInTime;
    p_ints csc_shower_isLooseOutOfTime;
    p_ints csc_shower_isNominalOutOfTime;
    p_ints csc_shower_isTightOutOfTime;

    void init() {
      csc_alct_bx = new t_ints;
      csc_alct_wg = new t_ints;
      csc_alct_isodd = new t_ints;
      csc_alct_region = new t_ints;
      csc_alct_station = new t_ints;
      csc_alct_ring = new t_ints;
      csc_alct_sector = new t_ints;
      csc_alct_chamber = new t_ints;
      csc_alct_quality = new t_ints;
      csc_alct_tpid = new t_ints;
      csc_preclct_bx = new t_ints;
      csc_preclct_hs = new t_ints;
      csc_preclct_qs = new t_ints;
      csc_preclct_es = new t_ints;
      csc_preclct_isodd = new t_ints;
      csc_preclct_region = new t_ints;
      csc_preclct_station = new t_ints;
      csc_preclct_ring = new t_ints;
      csc_preclct_sector = new t_ints;
      csc_preclct_chamber = new t_ints;
      csc_preclct_quality = new t_ints;
      csc_preclct_pattern = new t_ints;
      csc_preclct_pattern_run3 = new t_ints;
      csc_preclct_tpid = new t_ints;
      csc_clct_bx = new t_ints;
      csc_clct_hs = new t_ints;
      csc_clct_qs = new t_ints;
      csc_clct_es = new t_ints;
      csc_clct_isodd = new t_ints;
      csc_clct_region = new t_ints;
      csc_clct_station = new t_ints;
      csc_clct_ring = new t_ints;
      csc_clct_sector = new t_ints;
      csc_clct_chamber = new t_ints;
      csc_clct_quality = new t_ints;
      csc_clct_pattern = new t_ints;
      csc_clct_pattern_run3 = new t_ints;
      csc_clct_tpid = new t_ints;
      csc_lct_bx = new t_ints;
      csc_lct_wg = new t_ints;
      csc_lct_hs = new t_ints;
      csc_lct_qs = new t_ints;
      csc_lct_es = new t_ints;
      csc_lct_isodd = new t_ints;
      csc_lct_region = new t_ints;
      csc_lct_station = new t_ints;
      csc_lct_ring = new t_ints;
      csc_lct_sector = new t_ints;
      csc_lct_chamber = new t_ints;
      csc_lct_quality = new t_ints;
      csc_lct_pattern = new t_ints;
      csc_lct_pattern_run3 = new t_ints;
      csc_lct_tpid = new t_ints;
      csc_mplct_bx = new t_ints;
      csc_mplct_wg = new t_ints;
      csc_mplct_hs = new t_ints;
      csc_mplct_qs = new t_ints;
      csc_mplct_es = new t_ints;
      csc_mplct_isodd = new t_ints;
      csc_mplct_region = new t_ints;
      csc_mplct_station = new t_ints;
      csc_mplct_ring = new t_ints;
      csc_mplct_sector = new t_ints;
      csc_mplct_chamber = new t_ints;
      csc_mplct_quality = new t_ints;
      csc_mplct_pattern = new t_ints;
      csc_mplct_pattern_run3 = new t_ints;
      csc_mplct_tpid = new t_ints;
      csc_shower_bx = new t_ints;
      csc_shower_region = new t_ints;
      csc_shower_station = new t_ints;
      csc_shower_ring = new t_ints;
      csc_shower_chamber = new t_ints;
      csc_shower_sector = new t_ints;
      csc_shower_anodehits = new t_ints;
      csc_shower_cathodehits = new t_ints;
      csc_shower_isLooseInTime = new t_ints;
      csc_shower_isNominalInTime = new t_ints;
      csc_shower_isTightInTime = new t_ints;
      csc_shower_isLooseOutOfTime = new t_ints;
      csc_shower_isNominalOutOfTime = new t_ints;
      csc_shower_isTightOutOfTime = new t_ints;
    }

    void book(TTree* t) {
      t->Branch("csc_alct_bx", &csc_alct_bx);
      t->Branch("csc_alct_wg", &csc_alct_wg);
      t->Branch("csc_alct_isodd", &csc_alct_isodd);
      t->Branch("csc_alct_region", &csc_alct_region);
      t->Branch("csc_alct_station", &csc_alct_station);
      t->Branch("csc_alct_ring", &csc_alct_ring);
      t->Branch("csc_alct_sector", &csc_alct_sector);
      t->Branch("csc_alct_chamber", &csc_alct_chamber);
      t->Branch("csc_alct_quality", &csc_alct_quality);
      t->Branch("csc_alct_tpid", &csc_alct_tpid);
      t->Branch("csc_preclct_bx", &csc_preclct_bx);
      t->Branch("csc_preclct_hs", &csc_preclct_hs);
      t->Branch("csc_preclct_qs", &csc_preclct_qs);
      t->Branch("csc_preclct_es", &csc_preclct_es);
      t->Branch("csc_preclct_isodd", &csc_preclct_isodd);
      t->Branch("csc_preclct_region", &csc_preclct_region);
      t->Branch("csc_preclct_station", &csc_preclct_station);
      t->Branch("csc_preclct_ring", &csc_preclct_ring);
      t->Branch("csc_preclct_sector", &csc_preclct_sector);
      t->Branch("csc_preclct_chamber", &csc_preclct_chamber);
      t->Branch("csc_preclct_quality", &csc_preclct_quality);
      t->Branch("csc_preclct_pattern", &csc_preclct_pattern);
      t->Branch("csc_preclct_pattern_run3", &csc_preclct_pattern_run3);
      t->Branch("csc_preclct_tpid", &csc_preclct_tpid);
      t->Branch("csc_clct_bx", &csc_clct_bx);
      t->Branch("csc_clct_hs", &csc_clct_hs);
      t->Branch("csc_clct_qs", &csc_clct_qs);
      t->Branch("csc_clct_es", &csc_clct_es);
      t->Branch("csc_clct_isodd", &csc_clct_isodd);
      t->Branch("csc_clct_region", &csc_clct_region);
      t->Branch("csc_clct_station", &csc_clct_station);
      t->Branch("csc_clct_ring", &csc_clct_ring);
      t->Branch("csc_clct_sector", &csc_clct_sector);
      t->Branch("csc_clct_chamber", &csc_clct_chamber);
      t->Branch("csc_clct_quality", &csc_clct_quality);
      t->Branch("csc_clct_pattern", &csc_clct_pattern);
      t->Branch("csc_clct_pattern_run3", &csc_clct_pattern_run3);
      t->Branch("csc_clct_tpid", &csc_clct_tpid);
      t->Branch("csc_lct_bx", &csc_lct_bx);
      t->Branch("csc_lct_wg", &csc_lct_wg);
      t->Branch("csc_lct_hs", &csc_lct_hs);
      t->Branch("csc_lct_qs", &csc_lct_qs);
      t->Branch("csc_lct_es", &csc_lct_es);
      t->Branch("csc_lct_isodd", &csc_lct_isodd);
      t->Branch("csc_lct_region", &csc_lct_region);
      t->Branch("csc_lct_station", &csc_lct_station);
      t->Branch("csc_lct_ring", &csc_lct_ring);
      t->Branch("csc_lct_sector", &csc_lct_sector);
      t->Branch("csc_lct_chamber", &csc_lct_chamber);
      t->Branch("csc_lct_quality", &csc_lct_quality);
      t->Branch("csc_lct_pattern", &csc_lct_pattern);
      t->Branch("csc_lct_pattern_run3", &csc_lct_pattern_run3);
      t->Branch("csc_lct_tpid", &csc_lct_tpid);
      t->Branch("csc_mplct_bx", &csc_mplct_bx);
      t->Branch("csc_mplct_wg", &csc_mplct_wg);
      t->Branch("csc_mplct_hs", &csc_mplct_hs);
      t->Branch("csc_mplct_qs", &csc_mplct_qs);
      t->Branch("csc_mplct_es", &csc_mplct_es);
      t->Branch("csc_mplct_isodd", &csc_mplct_isodd);
      t->Branch("csc_mplct_region", &csc_mplct_region);
      t->Branch("csc_mplct_station", &csc_mplct_station);
      t->Branch("csc_mplct_ring", &csc_mplct_ring);
      t->Branch("csc_mplct_sector", &csc_mplct_sector);
      t->Branch("csc_mplct_chamber", &csc_mplct_chamber);
      t->Branch("csc_mplct_quality", &csc_mplct_quality);
      t->Branch("csc_mplct_pattern", &csc_mplct_pattern);
      t->Branch("csc_mplct_pattern_run3", &csc_mplct_pattern_run3);
      t->Branch("csc_mplct_tpid", &csc_mplct_tpid);
      t->Branch("csc_shower_bx", &csc_shower_bx);
      t->Branch("csc_shower_region", &csc_shower_region);
      t->Branch("csc_shower_station", &csc_shower_station);
      t->Branch("csc_shower_ring", &csc_shower_ring);
      t->Branch("csc_shower_chamber", &csc_shower_chamber);
      t->Branch("csc_shower_sector", &csc_shower_sector);
      t->Branch("csc_shower_anodehits", &csc_shower_anodehits);
      t->Branch("csc_shower_cathodehits", &csc_shower_cathodehits);
      t->Branch("csc_shower_isLooseInTime", &csc_shower_isLooseInTime);
      t->Branch("csc_shower_isNominalInTime", &csc_shower_isNominalInTime);
      t->Branch("csc_shower_isTightInTime", &csc_shower_isTightInTime);
      t->Branch("csc_shower_isLooseOutOfTime", &csc_shower_isLooseOutOfTime);
      t->Branch("csc_shower_isNominalOutOfTime", &csc_shower_isNominalOutOfTime);
      t->Branch("csc_shower_isTightOutOfTime", &csc_shower_isTightOutOfTime);
    }
  };

  struct L1MuStruct {

    p_ints emtftrack_charge;
    p_ints emtftrack_bx;
    p_ints emtftrack_tpid;
    p_floats emtftrack_pt;
    p_floats emtftrack_eta;
    p_floats emtftrack_phi;
    p_ints emtfcand_charge;
    p_ints emtfcand_bx;
    p_ints emtfcand_tpid;
    p_floats emtfcand_pt;
    p_floats emtfcand_eta;
    p_floats emtfcand_phi;
    p_ints emtfshower_bx;
    p_ints emtfshower_region;
    p_ints emtfshower_processor;
    p_ints emtfshower_isTwoLooseInTime;
    p_ints emtfshower_isOneNominalInTime;
    p_ints emtfshower_isTwoLooseOutOfTime;
    p_ints emtfshower_isOneNominalOutOfTime;
    p_ints l1mu_charge;
    p_ints l1mu_bx;
    p_ints l1mu_tpid;
    p_floats l1mu_pt;
    p_floats l1mu_eta;
    p_floats l1mu_phi;
    p_ints l1mushower_bits;
    p_ints l1mushower_bx;
    p_ints l1mushower_isTwoLooseInTime;
    p_ints l1mushower_isOneNominalInTime;
    p_ints l1mushower_isTwoLooseOutOfTime;
    p_ints l1mushower_isOneNominalOutOfTime;

    void init() {
      emtftrack_charge = new t_ints;
      emtftrack_bx = new t_ints;
      emtftrack_tpid = new t_ints;
      emtftrack_pt = new t_floats;
      emtftrack_eta = new t_floats;
      emtftrack_phi = new t_floats;
      emtfcand_charge = new t_ints;
      emtfcand_bx = new t_ints;
      emtfcand_tpid = new t_ints;
      emtfcand_pt = new t_floats;
      emtfcand_eta = new t_floats;
      emtfcand_phi = new t_floats;
      emtfshower_bx = new t_ints;
      emtfshower_region = new t_ints;
      emtfshower_processor = new t_ints;
      emtfshower_isTwoLooseInTime = new t_ints;
      emtfshower_isOneNominalInTime = new t_ints;
      emtfshower_isTwoLooseOutOfTime = new t_ints;
      emtfshower_isOneNominalOutOfTime = new t_ints;
      l1mu_charge = new t_ints;
      l1mu_bx = new t_ints;
      l1mu_tpid = new t_ints;
      l1mu_pt = new t_floats;
      l1mu_eta = new t_floats;
      l1mu_phi = new t_floats;
      l1mushower_bits = new t_ints;
      l1mushower_bx = new t_ints;
      l1mushower_isTwoLooseInTime = new t_ints;
      l1mushower_isOneNominalInTime = new t_ints;
      l1mushower_isTwoLooseOutOfTime = new t_ints;
      l1mushower_isOneNominalOutOfTime = new t_ints;
    }

    void book(TTree* t) {
      t->Branch("emtftrack_charge", &emtftrack_charge);
      t->Branch("emtftrack_bx", &emtftrack_bx);
      t->Branch("emtftrack_tpid", &emtftrack_tpid);
      t->Branch("emtftrack_pt", &emtftrack_pt);
      t->Branch("emtftrack_eta", &emtftrack_eta);
      t->Branch("emtftrack_phi", &emtftrack_phi);
      t->Branch("emtfcand_charge", &emtfcand_charge);
      t->Branch("emtfcand_bx", &emtfcand_bx);
      t->Branch("emtfcand_tpid", &emtfcand_tpid);
      t->Branch("emtfcand_pt", &emtfcand_pt);
      t->Branch("emtfcand_eta", &emtfcand_eta);
      t->Branch("emtfcand_phi", &emtfcand_phi);
      t->Branch("emtfshower_bx", &emtfshower_bx);
      t->Branch("emtfshower_region", &emtfshower_region);
      t->Branch("emtfshower_processor", &emtfshower_processor);
      t->Branch("emtfshower_isTwoLooseInTime", &emtfshower_isTwoLooseInTime);
      t->Branch("emtfshower_isOneNominalInTime", &emtfshower_isOneNominalInTime);
      t->Branch("emtfshower_isTwoLooseOutOfTime", &emtfshower_isTwoLooseOutOfTime);
      t->Branch("emtfshower_isOneNominalOutOfTime", &emtfshower_isOneNominalOutOfTime);
      t->Branch("l1mu_charge", &l1mu_charge);
      t->Branch("l1mu_bx", &l1mu_bx);
      t->Branch("l1mu_tpid", &l1mu_tpid);
      t->Branch("l1mu_pt", &l1mu_pt);
      t->Branch("l1mu_eta", &l1mu_eta);
      t->Branch("l1mu_phi", &l1mu_phi);
      t->Branch("l1mushower_bits", &l1mushower_bits);
      t->Branch("l1mushower_bx", &l1mushower_bx);
      t->Branch("l1mushower_isTwoLooseInTime", &l1mushower_isTwoLooseInTime);
      t->Branch("l1mushower_isOneNominalInTime", &l1mushower_isOneNominalInTime);
      t->Branch("l1mushower_isTwoLooseOutOfTime", &l1mushower_isTwoLooseOutOfTime);
      t->Branch("l1mushower_isOneNominalOutOfTime", &l1mushower_isOneNominalOutOfTime);
    }
  };

  struct L1TrackStruct {

    p_ints l1track_charge;
    p_ints l1track_bx;
    p_ints l1track_tpid;
    p_floats l1track_pt;
    p_floats l1track_eta;
    p_floats l1track_phi;
    p_ints l1trackmuon_charge;
    p_ints l1trackmuon_bx;
    p_ints l1trackmuon_tpid;
    p_floats l1trackmuon_pt;
    p_floats l1trackmuon_eta;
    p_floats l1trackmuon_phi;

    void init() {
      l1track_charge = new t_ints;
      l1track_bx = new t_ints;
      l1track_tpid = new t_ints;
      l1track_pt = new t_floats;
      l1track_eta = new t_floats;
      l1track_phi = new t_floats;
      l1trackmuon_charge = new t_ints;
      l1trackmuon_bx = new t_ints;
      l1trackmuon_tpid = new t_ints;
      l1trackmuon_pt = new t_floats;
      l1trackmuon_eta = new t_floats;
      l1trackmuon_phi = new t_floats;
    }

    void book(TTree* t) {
      t->Branch("l1track_charge", &l1track_charge);
      t->Branch("l1track_bx", &l1track_bx);
      t->Branch("l1track_tpid", &l1track_tpid);
      t->Branch("l1track_pt", &l1track_pt);
      t->Branch("l1track_eta", &l1track_eta);
      t->Branch("l1track_phi", &l1track_phi);
      t->Branch("l1trackmuon_charge", &l1trackmuon_charge);
      t->Branch("l1trackmuon_bx", &l1trackmuon_bx);
      t->Branch("l1trackmuon_tpid", &l1trackmuon_tpid);
      t->Branch("l1trackmuon_pt", &l1trackmuon_pt);
      t->Branch("l1trackmuon_eta", &l1trackmuon_eta);
      t->Branch("l1trackmuon_phi", &l1trackmuon_phi);
    }
  };
};

#endif
