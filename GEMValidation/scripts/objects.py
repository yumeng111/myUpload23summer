import awkward1 as ak
import numpy as np
#import numba as nb

id_to_station = {
    0 : [1,1],
    1 : [1,2],
    2 : [1,3],
    3 : [2,1],
    4 : [2,2],
    5 : [3,1],
    6 : [3,2],
    7 : [4,1],
    8 : [4,2]
}

def embed_crossref(source, idx_name, dest, dest_name):
    """Embed a cross-reference

    Parameters
    ----------
        source : ak.Array
            any array with shape N * var * {record}
        idx_name : str
            A field in the source record
        dest : ak.Array
            any array with shape N * var * {record}, where:
            ``ak.max(source[idx_name], axis=1) < ak.num(dest)`` and
            ``ak.min(source[idx_name], axis=1) >= 0``
    """
    print(ak.max(source[idx_name], axis=1))
    print(ak.num(dest))
    print(ak.all(ak.max(source[idx_name], axis=1) < ak.num(dest)))

    assert ak.all(ak.max(source[idx_name], axis=1) < ak.num(dest))
    assert ak.all(ak.min(source[idx_name], axis=1) >= 0)

    id_global = ak.flatten(
        source[idx_name] + np.asarray(dest.layout.starts), axis=None
    )
    source[dest_name] = ak.Array(
        ak.layout.ListOffsetArray64(
            source.layout.offsets,
            ak.layout.ListOffsetArray64(
                source.layout.content.offsets,
                ak.flatten(dest)[id_global].layout,
            ),
        )
    )


def getObjects(tree):
    sim_muon = ak.zip({
        "charge" : tree["sim_charge"],
        "pdgid" : tree["sim_pdgid"],
        "index" : tree["sim_index"],
        "pt" : tree["sim_pt"],
        "px" : tree["sim_px"],
        "py" : tree["sim_py"],
        "pz" : tree["sim_pz"],
        "eta" : tree["sim_eta"],
        "phi" : tree["sim_phi"],
        "vx" : tree["sim_vx"],
        "vy" : tree["sim_vy"],
        "vz" : tree["sim_vz"],
        "sim_id_gem_cluster" : tree["sim_id_gem_cluster"],
        "id_gem_sh" : tree["sim_id_gem_sh"],
        "id_csc_sh" : tree["sim_id_csc_sh"],
        "id_gen" : tree["sim_id_gen"],
        "id_gem_dg" : tree["sim_id_gem_dg"],
        "id_gem_pad" : tree["sim_id_gem_pad"],
        "id_gem_copad" : tree["sim_id_gem_copad"],
        "id_gem_cluster" : tree["sim_id_gem_cluster"],
        "id_csc_wire" : tree["sim_id_csc_wire"],
        "id_csc_strip" : tree["sim_id_csc_strip"],
        "id_csc_alct" : tree["sim_id_csc_alct"],
        "id_csc_clct" : tree["sim_id_csc_clct"],
        "id_csc_lct" : tree["sim_id_csc_lct"],
        "id_csc_mplct" : tree["sim_id_csc_mplct"],
        "id_emtf_track" : tree["sim_id_emtf_track"],
        "id_emtf_cand" : tree["sim_id_emtf_cand"]
    }, depth_limit=1
    )
    csc_clct = ak.zip( {
        "bx" : tree["csc_clct_bx"],
        "hs" : tree["csc_clct_hs"],
        "qs" : tree["csc_clct_qs"],
        "es" : tree["csc_clct_es"],
        "isodd" : tree["csc_clct_isodd"],
        "region" : tree["csc_clct_region"],
        "station" : tree["csc_clct_station"],
        "ring" : tree["csc_clct_ring"],
        "chamber" : tree["csc_clct_chamber"],
        "quality" : tree["csc_clct_quality"],
        "pattern" : tree["csc_clct_pattern"],
        "pattern_run3" : tree["csc_clct_pattern_run3"],
        "tpid" : tree["csc_clct_tpid"]
    })
    csc_alct = ak.zip( {
        "bx" : tree["csc_alct_bx"],
        "wg" : tree["csc_alct_wg"],
        "isodd" : tree["csc_alct_isodd"],
        "region" : tree["csc_alct_region"],
        "station" : tree["csc_alct_station"],
        "ring" : tree["csc_alct_ring"],
        "chamber" : tree["csc_alct_chamber"],
        "quality" : tree["csc_alct_quality"],
        "tpid" : tree["csc_alct_tpid"]
    })
    csc_lct = ak.zip( {
        "station" : tree["csc_lct_station"],
        "ring" : tree["csc_lct_ring"],
        "bx" : tree["csc_lct_bx"]
        #"hs" : tree["csc_lct_hs"],
        #"qs" : tree["csc_lct_qs"],
        #"es" : tree["csc_lct_es"],
        #"isodd" : tree["csc_lct_isodd"]
    })
    gem_cluster = ak.zip( {
        "bx" : tree["gem_cluster_bx"],
        "pad" : tree["gem_cluster_pad"],
        "isodd" : tree["gem_cluster_isodd"],
        "size" : tree["gem_cluster_size"],
        "region" : tree["gem_cluster_region"],
        "station" : tree["gem_cluster_station"],
        "roll" : tree["gem_cluster_roll"],
        "layer" : tree["gem_cluster_layer"],
        "chamber" : tree["gem_cluster_chamber"]
    })
    emtftrack = ak.zip({
        "charge" : tree["emtftrack_charge"],
        "bx" : tree["emtftrack_bx"],
        "tpid" : tree["emtftrack_tpid"],
        "pt" : tree["emtftrack_pt"],
        "eta" : tree["emtftrack_eta"],
        "phi" : tree["emtftrack_phi"]
    })
    l1mu = ak.zip({
        "charge" : tree["l1mu_charge"],
        "bx" : tree["l1mu_bx"],
        "tpid" : tree["l1mu_tpid"],
        "pt" : tree["l1mu_pt"],
        "eta" : tree["l1mu_eta"],
        "phi" : tree["l1mu_phi"]
    })
    return csc_clct, csc_alct, csc_lct, gem_cluster, emtftrack, l1mu, sim_muon

'''
        "d0" : tree["sim_d0"],
        "z0" : tree["sim_z0"]

  #        "d0_prod" : tree["sim_d0_prod"],
#        "z0_prod" : tree["sim_z0_prod"]



#"isodd" : tree["csc_lct_isodd"],
#"region" : tree["csc_lct_region"]
#"station" : tree["csc_lct_station"]
#"ring" : tree["csc_lct_ring"],
#"chamber" : tree["csc_lct_chamber"]
#"quality" : tree["csc_lct_quality"]
#"pattern" : tree["csc_lct_pattern"]
#"pattern_run3" : tree["csc_lct_pattern_run3"],
#"tpid" : tree["csc_lct_tpid"]

def getObjects(tree):
    """
    gen = ak.zip({
        "charge" : tree["gen_charge"],
        "pdgid" : tree["gen_pdgid"],
        "tpid" : tree["gen_tpid"],
        "pt" : tree["gen_pt"],
        #"pz" : tree["gen_pz"],
        "eta" : tree["gen_eta"],
        "phi" : tree["gen_phi"]
        #"dxy" : tree["gen_dxy"],
        #"d0" : tree["gen_d0"],
        #"z0" : tree["gen_z0"],
        #"d0_prod" : tree["gen_d0_prod"],
        #"z0_prod" : tree["gen_z0_prod"],
    })
    """



    """
    gem_sh = ak.zip({
        "bx" : tree["gem_sh_bx"],
        "strip_l1" : tree["gem_sh_strip_l1"],
        "strip_l2" : tree["gem_sh_strip_l2"],
        "isodd" : tree["gem_sh_isodd"],
        "region" : tree["gem_sh_region"],
        "station" : tree["gem_sh_station"],
        "ring" : tree["gem_sh_ring"],
        "chamber" : tree["gem_sh_chamber"],
        "tpid" : tree["gem_sh_tpid"],
        "nlayers" : tree["gem_sh_nlayers"],
        "dphi" : tree["gem_sh_dphi"],
        "bend" : tree["gem_sh_bend"],
        "phi" : tree["gem_sh_phi"],
        "eta" : tree["gem_sh_eta"],
        "perp" : tree["gem_sh_perp"]
    })
    csc_sh = ak.zip({
        "bx" : tree["csc_sh_bx"],
        "keyhs" : tree["csc_sh_keyhs"],
        "isodd" : tree["csc_sh_isodd"],
        "region" : tree["csc_sh_region"],
        "station" : tree["csc_sh_station"],
        "ring" : tree["csc_sh_ring"],
        "chamber" : tree["csc_sh_chamber"],
        "tpid" : tree["csc_sh_tpid"],
        "nlayers" : tree["csc_sh_nlayers"],
        "strip_l1" : tree["csc_sh_strip_l1"],
        "strip_l6" : tree["csc_sh_strip_l6"],
        "dphi" : tree["csc_sh_dphi"],
        "bend" : tree["csc_sh_bend"],
        "phi" : tree["csc_sh_phi"],
        "phi_l1" : tree["csc_sh_phi_l1"],
        "phi_l6" : tree["csc_sh_phi_l6"],
        "eta" : tree["csc_sh_eta"],
        "perp" : tree["csc_sh_perp"]
    })
    gem_digi = ak.zip({
        "bx" : tree["gem_digi_bx"],
        "strip" : tree["gem_digi_strip"],
        "isodd" : tree["gem_digi_isodd"],
        "region" : tree["gem_digi_region"],
        "station" : tree["gem_digi_station"],
        "roll" : tree["gem_digi_roll"],
        "layer" : tree["gem_digi_layer"],
        "chamber" : tree["gem_digi_chamber"],
        "tpid" : tree["gem_digi_tpid"],
        "phi" : tree["gem_digi_phi"],
        "eta" : tree["gem_digi_eta"]
    })
    csc_strip = ak.zip({
        "bx" : tree["csc_strip_bx"],
        "keyhs" : tree["csc_strip_keyhs"],
        "isodd" : tree["csc_strip_isodd"],
        "region" : tree["csc_strip_region"],
        "station" : tree["csc_strip_station"],
        "ring" : tree["csc_strip_ring"],
        "quality" : tree["csc_strip_quality"],
        "chamber" : tree["csc_strip_chamber"],
        "tpid" : tree["csc_strip_tpid"]
    })
    csc_wire = ak.zip({
        "bx" : tree["csc_wire_bx"],
        "keywg" : tree["csc_wire_keywg"],
        "isodd" : tree["csc_wire_isodd"],
        "region" : tree["csc_wire_region"],
        "station" : tree["csc_wire_station"],
        "ring" : tree["csc_wire_ring"],
        "quality" : tree["csc_wire_quality"],
        "chamber" : tree["csc_wire_chamber"],
        "tpid" : tree["csc_wire_tpid"]
    })
    gem_pad = ak.zip({
        "bx" : tree["gem_pad_bx"],
        "pad" : tree["gem_pad_pad"],
        "isodd" : tree["gem_pad_isodd"],
        "region" : tree["gem_pad_region"],
        "station" : tree["gem_pad_station"],
        "roll" : tree["gem_pad_roll"],
        "layer" : tree["gem_pad_layer"],
        "chamber" : tree["gem_pad_chamber"],
        "tpid" : tree["gem_pad_tpid"],
        "phi" : tree["gem_pad_phi"],
        "eta" : tree["gem_pad_eta"]
    })
    gem_copad = ak.zip({
        "bx" : tree["gem_copad_bx"],
        "pad" : tree["gem_copad_pad"],
        "isodd" : tree["gem_copad_isodd"],
        "region" : tree["gem_copad_region"],
        "station" : tree["gem_copad_station"],
        "roll" : tree["gem_copad_roll"],
        "layer" : tree["gem_copad_layer"],
        "chamber" : tree["gem_copad_chamber"],
        "tpid" : tree["gem_copad_tpid"],
        "phi" : tree["gem_copad_phi"],
        "eta" : tree["gem_copad_eta"]
    })
    """
    csc_mplct = ak.zip({
        "bx" : tree["csc_mplct_bx"],
        "wg" : tree["csc_mplct_wg"],
        "hs" : tree["csc_mplct_hs"],
        "qs" : tree["csc_mplct_qs"],
        "es" : tree["csc_mplct_es"],
        "isodd" : tree["csc_mplct_isodd"],
        "region" : tree["csc_mplct_region"],
        "station" : tree["csc_mplct_station"],
        "ring" : tree["csc_mplct_ring"],
        "chamber" : tree["csc_mplct_chamber"],
        "quality" : tree["csc_mplct_quality"],
        "pattern" : tree["csc_mplct_pattern"],
        "pattern_run3" : tree["csc_mplct_pattern_run3"]
        "tpid" : tree["csc_mplct_tpid"]
        depth_limit=1
    })
    emtfcand = ak.zip({
        "charge" : tree["emtfcand_charge"],
        "bx" : tree["emtfcand_bx"],
        "tpid" : tree["emtfcand_tpid"],
        "pt" : tree["emtfcand_pt"],
        "eta" : tree["emtfcand_eta"],
        "phi" : tree["emtfcand_phi"]
    })
    return sim, csc_clct, csc_alct, csc_lct
'''
