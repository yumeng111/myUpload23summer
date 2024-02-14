#ifndef GEMCode_GEMValidation_CSCStubStruct
#define GEMCode_GEMValidation_CSCStubStruct

#include "GEMCode/GEMValidation/interface/Structs/BaseStruct.h"

namespace gem {

  struct CSCStubStruct {

    static const int nStations = 11;

    // bools
    bool has_clct_odd[nStations];
    bool has_alct_odd[nStations];
    bool has_lct_odd[nStations];

    bool has_clct_even[nStations];
    bool has_alct_even[nStations];
    bool has_lct_even[nStations];

    // ints
    int bx_lct_odd[nStations];
    int bx_alct_odd[nStations];
    int bx_clct_odd[nStations];
    int bx_lct_even[nStations];
    int bx_alct_even[nStations];
    int bx_clct_even[nStations];

    int alctbx_lct_odd[nStations];
    int clctbx_lct_odd[nStations];
    int bxdiff_lct_odd[nStations];
    int matchtype_lct_odd[nStations];
    int simtype_lct_odd[nStations];
    int alctbx_lct_even[nStations];
    int clctbx_lct_even[nStations];
    int bxdiff_lct_even[nStations];
    int matchtype_lct_even[nStations];
    int simtype_lct_even[nStations];

    int clctquality_lct_odd[nStations];
    int clctquality_lct_even[nStations];
    int clctpattern_lct_odd[nStations];
    int clctpattern_lct_even[nStations];

    int bend_clct_odd[nStations];
    int bend_clct_even[nStations];

    int endcap_clct_odd[nStations];
    int endcap_clct_even[nStations];

    int hs_clct_odd[nStations];
    int hs_clct_even[nStations];

    int qs_clct_odd[nStations];
    int qs_clct_even[nStations];

    int es_clct_odd[nStations];
    int es_clct_even[nStations];

    int hs_lct_odd[nStations];
    int hs_lct_even[nStations];

    int qs_lct_odd[nStations];
    int qs_lct_even[nStations];

    int es_lct_odd[nStations];
    int es_lct_even[nStations];

    int wg_alct_odd[nStations];
    int wg_alct_even[nStations];

    int pattern_clct_odd[nStations];
    int pattern_clct_even[nStations];
    int run3pattern_clct_odd[nStations];
    int run3pattern_clct_even[nStations];

    int wg_lct_odd[nStations];
    int wg_lct_even[nStations];

    int chamber_lct_odd[nStations];
    int chamber_lct_even[nStations];

    int bend_lct_odd[nStations];
    int bend_lct_even[nStations];
    int run3slope_lct_odd[nStations];
    int run3slope_lct_even[nStations];

    int passdphi_odd[nStations];
    int passdphi_even[nStations];

    int quality_clct_odd[nStations];
    int quality_clct_even[nStations];
    int quality_alct_odd[nStations];
    int quality_alct_even[nStations];
    int quality_lct_odd[nStations];
    int quality_lct_even[nStations];

    // floats
    float phi_lct_odd[nStations];
    float phi_lct_even[nStations];
    float eta_lct_odd[nStations];
    float eta_lct_even[nStations];
    float dphi_lct_odd[nStations];
    float dphi_lct_even[nStations];
    float chi2_lct_odd[nStations];
    float chi2_lct_even[nStations];

    float perp_lct_odd[nStations];
    float perp_lct_even[nStations];

    int run3slope_clct_odd[nStations];
    int run3slope_clct_even[nStations];
    // bending resolution
    float slope_clct_odd[nStations];
    float slope_clct_even[nStations];

    float fslope_clct_odd[nStations];
    float fslope_clct_even[nStations];

    float dslope_clct_odd[nStations];
    float dslope_clct_even[nStations];

    // floating point strip in hs, qs or es precision
    float ffhs_clct_odd[nStations];
    float ffhs_clct_even[nStations];

    float fhs_clct_odd[nStations];
    float fhs_clct_even[nStations];

    float fqs_clct_odd[nStations];
    float fqs_clct_even[nStations];

    float fes_clct_odd[nStations];
    float fes_clct_even[nStations];

    float delta_ffhs_clct_odd[nStations];
    float delta_ffhs_clct_even[nStations];

    float delta_fhs_clct_odd[nStations];
    float delta_fhs_clct_even[nStations];

    float delta_fqs_clct_odd[nStations];
    float delta_fqs_clct_even[nStations];

    float delta_fes_clct_odd[nStations];
    float delta_fes_clct_even[nStations];

    void init() {
      for (unsigned i = 0 ; i < nStations; i++) {

        has_alct_even[i] = 0;
        has_clct_even[i] = 0;
        has_lct_even[i] = 0;
        has_alct_odd[i] = 0;
        has_clct_odd[i] = 0;
        has_lct_odd[i] = 0;

        chamber_lct_odd[i] = -1;
        chamber_lct_even[i] = -1;

        bend_lct_odd[i] = -9;
        bend_lct_even[i] = -9;
        run3slope_lct_odd[i] = -9;
        run3slope_lct_even[i] = -9;
        dphi_lct_odd[i] = -9;
        dphi_lct_even[i] = -9;

        bx_lct_odd[i] = -9;
        bx_lct_even[i] = -9;


        alctbx_lct_odd[i] = -9;
        clctbx_lct_odd[i] = -9;
        bxdiff_lct_odd[i] = -9;
        matchtype_lct_odd[i] = -1;
        simtype_lct_odd[i] = -1;
        alctbx_lct_even[i] = -9;
        clctbx_lct_even[i] = -9;
        bxdiff_lct_even[i] = -9;
        matchtype_lct_even[i] = -1;
        simtype_lct_even[i] = -1;
        clctquality_lct_odd [i] = -9;
        clctquality_lct_odd [i] = -9;
        clctpattern_lct_even[i] = -9;
        clctpattern_lct_even[i] = -9;

        hs_lct_odd[i] = 0;
        hs_lct_even[i] = 0;

        qs_lct_odd[i] = 0;
        qs_lct_even[i] = 0;

        es_lct_odd[i] = 0;
        es_lct_even[i] = 0;

        hs_clct_odd[i] = 0;
        hs_clct_even[i] = 0;

        qs_clct_odd[i] = 0;
        qs_clct_even[i] = 0;

        es_clct_odd[i] = 0;
        es_clct_even[i] = 0;

        ffhs_clct_odd[i] = -9;
        ffhs_clct_even[i] = -9;

        fhs_clct_odd[i] = -9;
        fhs_clct_even[i] = -9;

        fqs_clct_odd[i] = -9;
        fqs_clct_even[i] = -9;

        fes_clct_odd[i] = -9;
        fes_clct_even[i] = -9;

        delta_ffhs_clct_odd[i] = -9.;
        delta_ffhs_clct_even[i] = -9.;

        delta_fhs_clct_odd[i] = -9.;
        delta_fhs_clct_even[i] = -9.;

        delta_fqs_clct_odd[i] = -9.;
        delta_fqs_clct_even[i] = -9.;

        delta_fes_clct_odd[i] = -9.;
        delta_fes_clct_even[i] = -9.;

        wg_alct_odd[i] = -1;
        wg_alct_even[i] = -1;

        wg_lct_odd[i] = -1;
        wg_lct_even[i] = -1;

        eta_lct_odd[i] = -9.;
        eta_lct_even[i] = -9.;

        phi_lct_odd[i] = -9.;
        phi_lct_even[i] = -9.;

        chi2_lct_odd[i] = -99999;
        chi2_lct_even[i] = -99999;

        passdphi_odd[i] = 0;
        passdphi_even[i] = 0;

        perp_lct_odd[i] = -1;
        perp_lct_even[i] = -1;

        quality_clct_odd[i] = -1;
        quality_clct_even[i] = -1;
        quality_alct_odd[i] = -1;
        quality_alct_even[i] = -1;
        quality_lct_odd[i] = -1;
        quality_lct_even[i] = -1;

        bx_clct_odd[i] = -9;
        bx_clct_even[i] = -9;

        endcap_clct_odd[i] = -9;
        endcap_clct_even[i] = -9;

        bend_clct_odd[i] = -9;
        bend_clct_even[i] = -9;
        bx_alct_odd[i] = -9;
        bx_alct_even[i] = -9;

        run3slope_clct_odd[i] = -99;
        run3slope_clct_even[i] = -99;

        dslope_clct_odd[i] = -9;
        dslope_clct_even[i] = -9;

        slope_clct_odd[i] = -9;
        slope_clct_even[i] = -9;

        fslope_clct_odd[i] = -9;
        fslope_clct_even[i] = -9;

        pattern_clct_odd[i] = -9;
        pattern_clct_even[i] = -9;

        run3pattern_clct_odd[i] = -9;
        run3pattern_clct_even[i] = -9;
      }
    };

    void book(TTree* t) {
      t->Branch("has_clct_odd", has_clct_odd, "has_clct_odd[11]/O");
      t->Branch("has_alct_odd", has_alct_odd, "has_alct_odd[11]/O");
      t->Branch("has_lct_odd", has_lct_odd, "has_lct_odd[11]/O");

      t->Branch("has_clct_even", has_clct_even, "has_clct_even[11]/O");
      t->Branch("has_alct_even", has_alct_even, "has_alct_even[11]/O");
      t->Branch("has_lct_even", has_lct_even, "has_lct_even[11]/O");

      t->Branch("bx_clct_odd", bx_clct_odd, "bx_clct_odd[11]/I");
      t->Branch("bx_clct_even", bx_clct_even, "bx_clct_even[11]/I");

      t->Branch("endcap_clct_odd", endcap_clct_odd, "endcap_clct_odd[11]/I");
      t->Branch("endcap_clct_even", endcap_clct_even, "endcap_clct_even[11]/I");

      t->Branch("bend_clct_odd", bend_clct_odd, "bend_clct_odd[11]/I");
      t->Branch("bend_clct_even", bend_clct_even, "bend_clct_even[11]/I");

      t->Branch("pattern_clct_odd", pattern_clct_odd, "pattern_clct_odd[11]/I");
      t->Branch("pattern_clct_even", pattern_clct_even, "pattern_clct_even[11]/I");
      t->Branch("run3pattern_clct_odd",  run3pattern_clct_odd,  "run3pattern_clct_odd[11]/I");
      t->Branch("run3pattern_clct_even", run3pattern_clct_even, "run3pattern_clct_even[11]/I");


      t->Branch("quality_clct_odd", quality_clct_odd, "quality_clct_odd[11]/I");
      t->Branch("quality_clct_even", quality_clct_even, "quality_clct_even[11]/I");
      t->Branch("quality_alct_odd", quality_alct_odd, "quality_alct_odd[11]/I");
      t->Branch("quality_alct_even", quality_alct_even, "quality_alct_even[11]/I");
      t->Branch("quality_lct_odd", quality_lct_odd, "quality_lct_odd[11]/I");
      t->Branch("quality_lct_even", quality_lct_even, "quality_lct_even[11]/I");

      t->Branch("bx_alct_odd", bx_alct_odd, "bx_alct_odd[11]/I");
      t->Branch("bx_alct_even", bx_alct_even, "bx_alct_even[11]/I");

      t->Branch("chamber_lct_odd", chamber_lct_odd, "chamber_lct_odd[11]/I");
      t->Branch("chamber_lct_even", chamber_lct_even, "chamber_lct_even[11]/I");

      t->Branch("bend_lct_odd", bend_lct_odd, "bend_lct_odd[11]/I");
      t->Branch("bend_lct_even", bend_lct_even, "bend_lct_even[11]/I");
      t->Branch("run3slope_lct_odd",  run3slope_lct_odd,  "run3slope_lct_odd[11]/I");
      t->Branch("run3slope_lct_even", run3slope_lct_even, "run3slope_lct_even[11]/I");

      t->Branch("bx_lct_odd", bx_lct_odd, "bx_lct_odd[11]/I");
      t->Branch("bx_lct_even", bx_lct_even, "bx_lct_even[11]/I");
      t->Branch("alctbx_lct_odd",  alctbx_lct_odd,  "alctbx_lct_odd[11]/I");
      t->Branch("alctbx_lct_even", alctbx_lct_even, "alctbx_lct_even[11]/I");
      t->Branch("clctbx_lct_odd",  clctbx_lct_odd,  "clctbx_lct_odd[11]/I");
      t->Branch("clctbx_lct_even", clctbx_lct_even, "clctbx_lct_even[11]/I");
      t->Branch("clctpattern_lct_odd",  clctpattern_lct_odd,  "clctpattern_lct_odd[11]/I");
      t->Branch("clctpattern_lct_even", clctpattern_lct_even, "clctpattern_lct_even[11]/I");
      t->Branch("clctquality_lct_odd",  clctquality_lct_odd,  "clctquality_lct_odd[11]/I");
      t->Branch("clctquality_lct_even", clctquality_lct_even, "clctquality_lct_even[11]/I");
      t->Branch("bxdiff_lct_odd",  bxdiff_lct_odd,  "bxdiff_lct_odd[11]/I");
      t->Branch("bxdiff_lct_even", bxdiff_lct_even, "bxdiff_lct_even[11]/I");
      t->Branch("matchtype_lct_odd",  matchtype_lct_odd,  "matchtype_lct_odd[11]/I");
      t->Branch("matchtype_lct_even", matchtype_lct_even, "matchtype_lct_even[11]/I");
      t->Branch("simtype_lct_odd",  simtype_lct_odd,  "simtype_lct_odd[11]/I");
      t->Branch("simtype_lct_even", simtype_lct_even, "simtype_lct_even[11]/I");

      t->Branch("hs_lct_odd", hs_lct_odd, "hs_lct_odd[11]/I");
      t->Branch("hs_lct_even", hs_lct_even, "hs_lct_even[11]/I");

      t->Branch("qs_lct_odd", qs_lct_odd, "qs_lct_odd[11]/I");
      t->Branch("qs_lct_even", qs_lct_even, "qs_lct_even[11]/I");

      t->Branch("es_lct_odd", es_lct_odd, "es_lct_odd[11]/I");
      t->Branch("es_lct_even", es_lct_even, "es_lct_even[11]/I");

      t->Branch("hs_clct_odd", hs_clct_odd, "hs_clct_odd[11]/I");
      t->Branch("hs_clct_even", hs_clct_even, "hs_clct_even[11]/I");

      t->Branch("qs_clct_odd", qs_clct_odd, "qs_clct_odd[11]/I");
      t->Branch("qs_clct_even", qs_clct_even, "qs_clct_even[11]/I");

      t->Branch("es_clct_odd", es_clct_odd, "es_clct_odd[11]/I");
      t->Branch("es_clct_even", es_clct_even, "es_clct_even[11]/I");

      t->Branch("ffhs_clct_odd", ffhs_clct_odd, "ffhs_clct_odd[11]/F");
      t->Branch("ffhs_clct_even", ffhs_clct_even, "ffhs_clct_even[11]/F");

      t->Branch("fhs_clct_odd", fhs_clct_odd, "fhs_clct_odd[11]/F");
      t->Branch("fhs_clct_even", fhs_clct_even, "fhs_clct_even[11]/F");

      t->Branch("fqs_clct_odd", fqs_clct_odd, "fqs_clct_odd[11]/F");
      t->Branch("fqs_clct_even", fqs_clct_even, "fqs_clct_even[11]/F");

      t->Branch("fes_clct_odd", fes_clct_odd, "fes_clct_odd[11]/F");
      t->Branch("fes_clct_even", fes_clct_even, "fes_clct_even[11]/F");

      t->Branch("delta_ffhs_clct_odd", delta_ffhs_clct_odd, "delta_ffhs_clct_odd[11]/F");
      t->Branch("delta_ffhs_clct_even", delta_ffhs_clct_even, "delta_ffhs_clct_even[11]/F");

      t->Branch("delta_fhs_clct_odd", delta_fhs_clct_odd, "delta_fhs_clct_odd[11]/F");
      t->Branch("delta_fhs_clct_even", delta_fhs_clct_even, "delta_fhs_clct_even[11]/F");

      t->Branch("delta_fqs_clct_odd", delta_fqs_clct_odd, "delta_fqs_clct_odd[11]/F");
      t->Branch("delta_fqs_clct_even", delta_fqs_clct_even, "delta_fqs_clct_even[11]/F");

      t->Branch("delta_fes_clct_odd", delta_fes_clct_odd, "delta_fes_clct_odd[11]/F");
      t->Branch("delta_fes_clct_even", delta_fes_clct_even, "delta_fes_clct_even[11]/F");

      t->Branch("wg_alct_odd", wg_alct_odd, "wg_alct_odd[11]/I");
      t->Branch("wg_alct_even", wg_alct_even, "wg_alct_even[11]/I");

      t->Branch("wg_lct_odd", wg_lct_odd, "wg_lct_odd[11]/I");
      t->Branch("wg_lct_even", wg_lct_even, "wg_lct_even[11]/I");

      t->Branch("perp_lct_odd", perp_lct_odd, "perp_lct_odd[11]/F");
      t->Branch("perp_lct_even", perp_lct_even, "perp_lct_even[11]/F");

      t->Branch("eta_lct_odd", eta_lct_odd, "eta_lct_odd[11]/F");
      t->Branch("eta_lct_even", eta_lct_even, "eta_lct_even[11]/F");

      t->Branch("phi_lct_odd", phi_lct_odd, "phi_lct_odd[11]/F");
      t->Branch("phi_lct_even", phi_lct_even, "phi_lct_even[11]/F");

      t->Branch("dphi_lct_odd", dphi_lct_odd, "dphi_lct_odd[11]/F");
      t->Branch("dphi_lct_even", dphi_lct_even, "dphi_lct_even[11]/F");

      t->Branch("chi2_lct_odd", chi2_lct_odd, "chi2_lct_odd[11]/F");
      t->Branch("chi2_lct_even", chi2_lct_even, "chi2_lct_even[11]/F");

      t->Branch("passdphi_odd", passdphi_odd, "passdphi_odd[11]/F");
      t->Branch("passdphi_even", passdphi_even, "passdphi_even[11]/F");

      t->Branch("run3slope_clct_odd",  run3slope_clct_odd,  "run3slope_clct_odd[11]/I");
      t->Branch("run3slope_clct_even", run3slope_clct_even, "run3slope_clct_even[11]/I");
      t->Branch("dslope_clct_odd", dslope_clct_odd, "dslope_clct_odd[11]/F");
      t->Branch("dslope_clct_even", dslope_clct_even, "dslope_clct_even[11]/F");

      t->Branch("slope_clct_odd", slope_clct_odd, "slope_clct_odd[11]/F");
      t->Branch("slope_clct_even", slope_clct_even, "slope_clct_even[11]/F");

      t->Branch("fslope_clct_odd", fslope_clct_odd, "fslope_clct_odd[11]/F");
      t->Branch("fslope_clct_even", fslope_clct_even, "fslope_clct_even[11]/F");
    }
  };
}  // namespace

#endif
