#ifndef GEMCode_GEMValidation_L1MuStruct
#define GEMCode_GEMValidation_L1MuStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct L1MuStruct {

    // Track properties
    bool has_emtfTrack;
    bool has_gmtRegCand;
    bool has_gmtCand;

    //csctf
    float emtf_pt, emtf_eta, emtf_phi;
    UInt_t chargesign;
    UInt_t rank;
    UInt_t nstubs;
    //UInt_t deltaphi12, deltaphi23;
    bool hasME1, hasME2, hasME3, hasME4;
    //Char_t chamberME1, chamberME2;  //bit1: odd, bit2: even
    //Int_t ME1_ring, ME2_ring;
    //Int_t ME1_hs, ME2_hs, ME1_wg, ME2_wg;
    float dphiGE11, dphiGE21;
    //bool passGE11, passGE21;
    //bool passGE11_pt5, passGE11_pt7, passGE11_pt10, passGE11_pt15, passGE11_pt20, passGE11_pt30, passGE11_pt40;
    //bool passGE21_pt5, passGE21_pt7, passGE21_pt10, passGE21_pt15, passGE21_pt20, passGE21_pt30, passGE21_pt40;
    //bool passGE11_simpt, passGE21_simpt;
    float deltaR;
    float lctdphi12;

    bool allstubs_matched_TF;
    int nstubs_matched_TF;
    bool allstubs_bestmatched_TF;
    int nstubs_bestmatched_TF;

    int mode;
    int quality;
    int deltaphi12;
    int deltaphi13;
    int deltaphi14;
    int deltaphi23;
    int deltaphi24;
    int deltaphi34;
    int deltatheta12;
    int deltatheta13;
    int deltatheta14;
    int deltatheta23;
    int deltatheta24;
    int deltatheta34;
    int lctslope1;
    int lctslope2;
    int lctslope3;
    int lctslope4;
    int lctpattern1;
    int lctpattern2;
    int lctpattern3;
    int lctpattern4;

    bool  cscstub_st1_matched;
    bool  cscstub_st1_bestmatched;
    bool  cscstub_st1_found;
    int   cscstub_st1_wire;
    float cscstub_st1_halfstrip;
    int   cscstub_st1_pattern;
    int   cscstub_st1_ring;
    float cscstub_st1_phi;
    float emtfhit_st1_phi;
    float simhits_st1_phi;
    int   emtfhit_st1_wire;
    float emtfhit_st1_halfstrip;
    int   emtfhit_st1_pattern;
    int   emtfhit_st1_ring;
    int   emtfhit_st1_nwiredigis;
    int   emtfhit_st1_nstripdigis;
    int   emtfhit_st1_ncomparatordigis;
    int   emtfhit_st1_anodeHMT;
    int   emtfhit_st1_cathodeHMT;
    bool emtfhit_st1_digiChmatch;

    bool  cscstub_st2_matched;
    bool  cscstub_st2_bestmatched;
    bool  cscstub_st2_found;
    int   cscstub_st2_wire;
    float cscstub_st2_halfstrip;
    int   cscstub_st2_pattern;
    int   cscstub_st2_ring;
    float cscstub_st2_phi;
    float emtfhit_st2_phi;
    float simhits_st2_phi;
    int   emtfhit_st2_wire;
    float emtfhit_st2_halfstrip;
    int   emtfhit_st2_pattern;
    int   emtfhit_st2_ring;
    int   emtfhit_st2_nwiredigis;
    int   emtfhit_st2_nstripdigis;
    int   emtfhit_st2_ncomparatordigis;
    int   emtfhit_st2_anodeHMT;
    int   emtfhit_st2_cathodeHMT;
    bool  emtfhit_st2_digiChmatch;

    bool  cscstub_st3_matched;
    bool  cscstub_st3_bestmatched;
    bool  cscstub_st3_found;
    int   cscstub_st3_wire;
    float cscstub_st3_halfstrip;
    int   cscstub_st3_pattern;
    int   cscstub_st3_ring;
    float cscstub_st3_phi;
    float emtfhit_st3_phi;
    float simhits_st3_phi;
    int   emtfhit_st3_wire;
    float emtfhit_st3_halfstrip;
    int   emtfhit_st3_pattern;
    int   emtfhit_st3_ring;
    int   emtfhit_st3_nwiredigis;
    int   emtfhit_st3_nstripdigis;
    int   emtfhit_st3_ncomparatordigis;
    int   emtfhit_st3_anodeHMT;
    int   emtfhit_st3_cathodeHMT;
    bool  emtfhit_st3_digiChmatch;

    bool  cscstub_st4_matched;
    bool  cscstub_st4_bestmatched;
    bool  cscstub_st4_found;
    int   cscstub_st4_wire;
    float cscstub_st4_halfstrip;
    int   cscstub_st4_pattern;
    int   cscstub_st4_ring;
    float cscstub_st4_phi;
    float emtfhit_st4_phi;
    float simhits_st4_phi;
    int   emtfhit_st4_wire;
    float emtfhit_st4_halfstrip;
    int   emtfhit_st4_pattern;
    int   emtfhit_st4_ring;
    int   emtfhit_st4_nwiredigis;
    int   emtfhit_st4_nstripdigis;
    int   emtfhit_st4_ncomparatordigis;
    int   emtfhit_st4_anodeHMT;
    int   emtfhit_st4_cathodeHMT;
    bool  emtfhit_st4_digiChmatch;

    //check whether there is EMTF shower around
    bool emtfShower_valid;
    int  emtfShower_tftype;
    int  emtfShower_processor;
    bool emtfShower_nominal;
    bool emtfShower_tight;
    bool emtfShower_twoloose;

    //L1Mu
    float bestdRGmtCand;
    float L1Mu_pt, L1Mu_eta, L1Mu_phi, L1Mu_quality, L1Mu_bx;
    Int_t L1Mu_charge;

    void init() {
      // Track properties
      has_emtfTrack = 0;
      has_gmtCand = 0;
      has_gmtCand = 0;

      //csctf
      emtf_pt = -1;
      emtf_eta = -9;
      emtf_phi = -9;
      //ME1_hs = -1;
      //ME1_wg = -1;
      //ME2_hs = -1;
      //ME2_wg = -1;
      chargesign =99;
      rank = 0;
      deltaphi12 = 0;
      deltaphi23 = 0;;
      hasME1 = false;
      hasME2 = false;
      hasME3 = false;
      hasME4 = false;
      //ME1_ring = -1;
      //ME2_ring = -1;
      //chamberME1 = 0;
      //chamberME2 = 0;
      dphiGE11 = -99.0;
      dphiGE21 = -99.0;
      //passGE11 = false;
      //passGE11_pt5 = false;
      //passGE11_pt7 = false;
      //passGE11_pt10 = false;
      //passGE11_pt15 = false;
      //passGE11_pt20 = false;
      //passGE11_pt30 = false;
      //passGE11_pt40 = false;
      //passGE21 = false;
      //passGE21_pt5 = false;
      //passGE21_pt7 = false;
      //passGE21_pt10 = false;
      //passGE21_pt15 = false;
      //passGE21_pt20 = false;
      //passGE21_pt30 = false;
      //passGE21_pt40 = false;
      //passGE11_simpt = false;
      //passGE21_simpt = false;//to debug dphi cut eff
      nstubs = 0;
      deltaR = 10;
      lctdphi12 = -99;
      mode = 0;
      deltaphi12 = -99999;
      deltaphi13 = -99999;
      deltaphi14 = -99999;
      deltaphi23 = -99999;
      deltaphi24 = -99999;
      deltaphi34 = -99999;
      deltatheta12 = -99999;
      deltatheta13 = -99999;
      deltatheta14 = -99999;
      deltatheta23 = -99999;
      deltatheta24 = -99999;
      deltatheta34 = -99999;
      lctslope1 = -99999;
      lctslope2 = -99999;
      lctslope3 = -99999;
      lctslope4 = -99999;
      lctpattern1 = -99999;
      lctpattern2 = -99999;
      lctpattern3 = -99999;
      lctpattern4 = -99999;


      cscstub_st1_bestmatched  = false;
      cscstub_st1_matched  = false;
      cscstub_st1_found    = false;
      cscstub_st1_wire     = -9999;
      cscstub_st1_halfstrip= -9999.0;
      cscstub_st1_pattern  = -9999;
      cscstub_st1_ring     = -1;
      cscstub_st1_phi      = -9.0;
      emtfhit_st1_wire     = -9999;
      emtfhit_st1_halfstrip= -9999.0;
      emtfhit_st1_pattern  = -9999;
      emtfhit_st1_ring     = -1;
      emtfhit_st1_phi      = -9.0;
      simhits_st1_phi      = -9.0;
      emtfhit_st1_nwiredigis      =0;
      emtfhit_st1_nstripdigis     =0;
      emtfhit_st1_ncomparatordigis=0;
      emtfhit_st1_anodeHMT =0;
      emtfhit_st1_cathodeHMT =0;
      emtfhit_st1_digiChmatch = false;

      cscstub_st2_bestmatched  = false;
      cscstub_st2_matched  = false;
      cscstub_st2_found    = false;
      cscstub_st2_wire     = -9999;
      cscstub_st2_halfstrip= -9999.0;
      cscstub_st2_pattern  = -9999;
      cscstub_st2_ring     = -1;
      cscstub_st2_phi      = -9.0;
      emtfhit_st2_wire     = -9999;
      emtfhit_st2_halfstrip= -9999.0;
      emtfhit_st2_pattern  = -9999;
      emtfhit_st2_ring     = -1;
      emtfhit_st2_phi      = -9.0;
      simhits_st2_phi      = -9.0;
      emtfhit_st2_nwiredigis      =0;
      emtfhit_st2_nstripdigis     =0;
      emtfhit_st2_ncomparatordigis=0;
      emtfhit_st2_anodeHMT =0;
      emtfhit_st2_cathodeHMT =0;
      emtfhit_st2_digiChmatch = false;

      cscstub_st3_bestmatched  = false;
      cscstub_st3_matched  = false;
      cscstub_st3_found    = false;
      cscstub_st3_wire     = -9999;
      cscstub_st3_halfstrip= -9999.0;
      cscstub_st3_pattern  = -9999;
      cscstub_st3_ring     = -1;
      cscstub_st3_phi      = -9.0;
      emtfhit_st3_wire     = -9999;
      emtfhit_st3_halfstrip= -9999.0;
      emtfhit_st3_pattern  = -9999;
      emtfhit_st3_ring     = -1;
      emtfhit_st3_phi      = -9.0;
      simhits_st3_phi      = -9.0;
      emtfhit_st3_nwiredigis      =0;
      emtfhit_st3_nstripdigis     =0;
      emtfhit_st3_ncomparatordigis=0;
      emtfhit_st3_anodeHMT =0;
      emtfhit_st3_cathodeHMT =0;
      emtfhit_st3_digiChmatch = false;

      cscstub_st4_bestmatched  = false;
      cscstub_st4_matched  = false;
      cscstub_st4_found    = false;
      cscstub_st4_wire     = -9999;
      cscstub_st4_halfstrip= -9999.0;
      cscstub_st4_pattern  = -9999;
      cscstub_st4_ring     = -1;
      cscstub_st4_phi      = -9.0;
      emtfhit_st4_wire     = -9999;
      emtfhit_st4_halfstrip= -9999.0;
      emtfhit_st4_pattern  = -9999;
      emtfhit_st4_ring     = -1;
      emtfhit_st4_phi      = -9.0;
      simhits_st4_phi      = -9.0;
      emtfhit_st4_nwiredigis      =0;
      emtfhit_st4_nstripdigis     =0;
      emtfhit_st4_ncomparatordigis=0;
      emtfhit_st4_anodeHMT =0;
      emtfhit_st4_cathodeHMT =0;
      emtfhit_st4_digiChmatch = false;

      allstubs_matched_TF = false;
      nstubs_matched_TF = -1;
      allstubs_bestmatched_TF = false;
      nstubs_bestmatched_TF = -1;

      emtfShower_valid = false;
      emtfShower_tftype = -1;
      emtfShower_processor = 0;
      emtfShower_nominal = false;
      emtfShower_tight = false;
      emtfShower_twoloose = false;

      bestdRGmtCand = 99;
      L1Mu_pt = -99;
      L1Mu_eta = -99;
      L1Mu_phi = -99;
      L1Mu_quality = -99;
      L1Mu_bx = -99;
      L1Mu_charge = -99;
    };

    void book(TTree* t) {

      t->Branch("has_emtfTrack", &has_emtfTrack);
      t->Branch("has_gmtCand", &has_gmtCand);
      t->Branch("has_gmtCand", &has_gmtCand);

      t->Branch("emtf_pt", &emtf_pt);
      t->Branch("emtf_eta", &emtf_eta);
      t->Branch("emtf_phi", &emtf_phi);
      t->Branch("rank",&rank);
      t->Branch("chargesign",&chargesign);
      //t->Branch("deltaphi12",&deltaphi12);
      //t->Branch("deltaphi23",&deltaphi23);
      t->Branch("hasME1",&hasME1);
      t->Branch("hasME2",&hasME2);
      t->Branch("hasME3",&hasME3);
      t->Branch("hasME4",&hasME4);
      //t->Branch("ME1_ring",&ME1_ring);
      //t->Branch("ME2_ring",&ME2_ring);
      //t->Branch("chamberME1",&chamberME1);
      //t->Branch("chamberME2",&chamberME2);
      //t->Branch("ME1_hs",&ME1_hs);
      //t->Branch("ME1_wg",&ME1_wg);
      //t->Branch("ME2_hs",&ME2_hs);
      //t->Branch("ME2_wg",&ME2_wg);
      t->Branch("dphiGE11",&dphiGE11);
      t->Branch("dphiGE21",&dphiGE21);
      //t->Branch("passGE11",&passGE11);
      //t->Branch("passGE11_pt5",&passGE11_pt5);
      //t->Branch("passGE11_pt7",&passGE11_pt7);
      //t->Branch("passGE11_pt10",&passGE11_pt10);
      //t->Branch("passGE11_pt15",&passGE11_pt15);
      //t->Branch("passGE11_pt20",&passGE11_pt20);
      //t->Branch("passGE11_pt30",&passGE11_pt30);
      //t->Branch("passGE11_pt40",&passGE11_pt40);
      //t->Branch("passGE21",&passGE21);
      //t->Branch("passGE21_pt5",&passGE21_pt5);
      //t->Branch("passGE21_pt7",&passGE21_pt7);
      //t->Branch("passGE21_pt10",&passGE21_pt10);
      //t->Branch("passGE21_pt15",&passGE21_pt15);
      //t->Branch("passGE21_pt20",&passGE21_pt20);
      //t->Branch("passGE21_pt30",&passGE21_pt30);
      //t->Branch("passGE21_pt40",&passGE21_pt40);
      //t->Branch("passGE11_simpt",&passGE11_simpt);
      //t->Branch("passGE21_simpt",&passGE21_simpt);
      t->Branch("nstubs",&nstubs);
      t->Branch("deltaR",&deltaR);
      t->Branch("lctdphi12",&lctdphi12);

      t->Branch("mode",&mode);
      t->Branch("quality",&quality);
      t->Branch("deltaphi12",&deltaphi12);
      t->Branch("deltaphi13",&deltaphi13);
      t->Branch("deltaphi14",&deltaphi14);
      t->Branch("deltaphi23",&deltaphi23);
      t->Branch("deltaphi24",&deltaphi24);
      t->Branch("deltaphi34",&deltaphi34);
      t->Branch("deltatheta12",&deltatheta12);
      t->Branch("deltatheta13",&deltatheta13);
      t->Branch("deltatheta14",&deltatheta14);
      t->Branch("deltatheta23",&deltatheta23);
      t->Branch("deltatheta24",&deltatheta24);
      t->Branch("deltatheta34",&deltatheta34);
      t->Branch("lctslope1",&lctslope1);
      t->Branch("lctslope2",&lctslope2);
      t->Branch("lctslope3",&lctslope3);
      t->Branch("lctslope4",&lctslope4);
      t->Branch("lctpattern1",&lctpattern1);
      t->Branch("lctpattern2",&lctpattern2);
      t->Branch("lctpattern3",&lctpattern3);
      t->Branch("lctpattern4",&lctpattern4);

      t->Branch("cscstub_st1_bestmatched",   &cscstub_st1_bestmatched);
      t->Branch("cscstub_st1_matched",   &cscstub_st1_matched);
      t->Branch("cscstub_st1_found",     &cscstub_st1_found);
      t->Branch("cscstub_st1_wire",      &cscstub_st1_wire);
      t->Branch("cscstub_st1_halfstrip", &cscstub_st1_halfstrip);
      t->Branch("cscstub_st1_pattern",   &cscstub_st1_pattern);
      t->Branch("cscstub_st1_ring",      &cscstub_st1_ring);
      t->Branch("cscstub_st1_phi",       &cscstub_st1_phi);
      t->Branch("emtfhit_st1_wire",      &emtfhit_st1_wire);
      t->Branch("emtfhit_st1_halfstrip", &emtfhit_st1_halfstrip);
      t->Branch("emtfhit_st1_pattern",   &emtfhit_st1_pattern);
      t->Branch("emtfhit_st1_ring",      &emtfhit_st1_ring);
      t->Branch("emtfhit_st1_phi",       &emtfhit_st1_phi);
      t->Branch("simhits_st1_phi",       &simhits_st1_phi);
      t->Branch("emtfhit_st1_nwiredigis",       &emtfhit_st1_nwiredigis);
      t->Branch("emtfhit_st1_nstripdigis",      &emtfhit_st1_nstripdigis);
      t->Branch("emtfhit_st1_ncomparatordigis", &emtfhit_st1_ncomparatordigis);
      t->Branch("emtfhit_st1_anodeHMT",   &emtfhit_st1_anodeHMT);
      t->Branch("emtfhit_st1_cathodeHMT", &emtfhit_st1_cathodeHMT);
      t->Branch("emtfhit_st1_digiChmatch",&emtfhit_st1_digiChmatch);

      t->Branch("cscstub_st2_bestmatched",   &cscstub_st2_bestmatched);
      t->Branch("cscstub_st2_matched",   &cscstub_st2_matched);
      t->Branch("cscstub_st2_found",     &cscstub_st2_found);
      t->Branch("cscstub_st2_wire",      &cscstub_st2_wire);
      t->Branch("cscstub_st2_halfstrip", &cscstub_st2_halfstrip);
      t->Branch("cscstub_st2_pattern",   &cscstub_st2_pattern);
      t->Branch("cscstub_st2_ring",      &cscstub_st2_ring);
      t->Branch("cscstub_st2_phi",       &cscstub_st2_phi);
      t->Branch("emtfhit_st2_wire",      &emtfhit_st2_wire);
      t->Branch("emtfhit_st2_halfstrip", &emtfhit_st2_halfstrip);
      t->Branch("emtfhit_st2_pattern",   &emtfhit_st2_pattern);
      t->Branch("emtfhit_st2_ring",      &emtfhit_st2_ring);
      t->Branch("emtfhit_st2_phi",       &emtfhit_st2_phi);
      t->Branch("simhits_st2_phi",       &simhits_st2_phi);
      t->Branch("emtfhit_st2_nwiredigis",       &emtfhit_st2_nwiredigis);
      t->Branch("emtfhit_st2_nstripdigis",      &emtfhit_st2_nstripdigis);
      t->Branch("emtfhit_st2_ncomparatordigis", &emtfhit_st2_ncomparatordigis);
      t->Branch("emtfhit_st2_anodeHMT",   &emtfhit_st2_anodeHMT);
      t->Branch("emtfhit_st2_cathodeHMT", &emtfhit_st2_cathodeHMT);
      t->Branch("emtfhit_st2_digiChmatch",&emtfhit_st2_digiChmatch);

      t->Branch("cscstub_st3_bestmatched",   &cscstub_st3_bestmatched);
      t->Branch("cscstub_st3_matched",   &cscstub_st3_matched);
      t->Branch("cscstub_st3_found",     &cscstub_st3_found);
      t->Branch("cscstub_st3_wire",      &cscstub_st3_wire);
      t->Branch("cscstub_st3_halfstrip", &cscstub_st3_halfstrip);
      t->Branch("cscstub_st3_pattern",   &cscstub_st3_pattern);
      t->Branch("cscstub_st3_ring",      &cscstub_st3_ring);
      t->Branch("cscstub_st3_phi",       &cscstub_st3_phi);
      t->Branch("emtfhit_st3_wire",      &emtfhit_st3_wire);
      t->Branch("emtfhit_st3_halfstrip", &emtfhit_st3_halfstrip);
      t->Branch("emtfhit_st3_pattern",   &emtfhit_st3_pattern);
      t->Branch("emtfhit_st3_ring",      &emtfhit_st3_ring);
      t->Branch("emtfhit_st3_phi",       &emtfhit_st3_phi);
      t->Branch("simhits_st3_phi",       &simhits_st3_phi);
      t->Branch("emtfhit_st3_nwiredigis",       &emtfhit_st3_nwiredigis);
      t->Branch("emtfhit_st3_nstripdigis",      &emtfhit_st3_nstripdigis);
      t->Branch("emtfhit_st3_ncomparatordigis", &emtfhit_st3_ncomparatordigis);
      t->Branch("emtfhit_st3_anodeHMT",   &emtfhit_st3_anodeHMT);
      t->Branch("emtfhit_st3_cathodeHMT", &emtfhit_st3_cathodeHMT);
      t->Branch("emtfhit_st3_digiChmatch",&emtfhit_st3_digiChmatch);

      t->Branch("cscstub_st4_bestmatched",   &cscstub_st4_bestmatched);
      t->Branch("cscstub_st4_matched",   &cscstub_st4_matched);
      t->Branch("cscstub_st4_found",     &cscstub_st4_found);
      t->Branch("cscstub_st4_wire",      &cscstub_st4_wire);
      t->Branch("cscstub_st4_halfstrip", &cscstub_st4_halfstrip);
      t->Branch("cscstub_st4_pattern",   &cscstub_st4_pattern);
      t->Branch("cscstub_st4_ring",      &cscstub_st4_ring);
      t->Branch("cscstub_st4_phi",       &cscstub_st4_phi);
      t->Branch("emtfhit_st4_wire",      &emtfhit_st4_wire);
      t->Branch("emtfhit_st4_halfstrip", &emtfhit_st4_halfstrip);
      t->Branch("emtfhit_st4_pattern",   &emtfhit_st4_pattern);
      t->Branch("emtfhit_st4_ring",      &emtfhit_st4_ring);
      t->Branch("emtfhit_st4_phi",       &emtfhit_st4_phi);
      t->Branch("simhits_st4_phi",       &simhits_st4_phi);
      t->Branch("emtfhit_st4_nwiredigis",       &emtfhit_st4_nwiredigis);
      t->Branch("emtfhit_st4_nstripdigis",      &emtfhit_st4_nstripdigis);
      t->Branch("emtfhit_st4_ncomparatordigis", &emtfhit_st4_ncomparatordigis);
      t->Branch("emtfhit_st4_anodeHMT",   &emtfhit_st4_anodeHMT);
      t->Branch("emtfhit_st4_cathodeHMT", &emtfhit_st4_cathodeHMT);
      t->Branch("emtfhit_st4_digiChmatch",&emtfhit_st4_digiChmatch);

      t->Branch("nstubs_matched_TF",  &nstubs_matched_TF);
      t->Branch("allstubs_matched_TF",&allstubs_matched_TF);
      t->Branch("nstubs_bestmatched_TF",  &nstubs_bestmatched_TF);
      t->Branch("allstubs_bestmatched_TF",&allstubs_bestmatched_TF);

      t->Branch("emtfShower_valid",     &emtfShower_valid);
      t->Branch("emtfShower_tftype",    &emtfShower_tftype);
      t->Branch("emtfShower_processor", &emtfShower_processor);
      t->Branch("emtfShower_nominal",   &emtfShower_nominal);
      t->Branch("emtfShower_tight",     &emtfShower_tight);
      t->Branch("emtfShower_twoloose",  &emtfShower_twoloose);


      t->Branch("bestdRGmtCand", &bestdRGmtCand);
      t->Branch("L1Mu_pt", &L1Mu_pt);
      t->Branch("L1Mu_eta", &L1Mu_eta);
      t->Branch("L1Mu_phi", &L1Mu_phi);
      t->Branch("L1Mu_quality", &L1Mu_quality);
      t->Branch("L1Mu_bx", &L1Mu_bx);
      t->Branch("L1Mu_charge", &L1Mu_charge);

      t->Branch("L1Mu_charge", &L1Mu_charge);
    }
  };
}  // namespace

#endif
