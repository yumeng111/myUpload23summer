import ROOT
from array import array

from helpers.stations import *
from helpers.tools import *


def printCSCSimhitsInSimtrack(tree):
    if tree.has_csc_sh_even[st]:
        print(cscStations[st].label," simhits in even ", tree.has_csc_sh_even[st], " chamber ", tree.chamber_sh_even[st]," eta ", tree.eta_csc_sh_even[st]," phi ", tree.phi_csc_sh_even[st]," strip ", tree.strip_csc_sh_even[st])
    if tree.has_csc_sh_odd[st]:
        print(cscStations[st].label," simhits in odd  ", tree.has_csc_sh_odd[st], " chamber ", tree.chamber_sh_odd[st]," eta ", tree.eta_csc_sh_odd[st]," phi ", tree.phi_csc_sh_odd[st]," strip ", tree.strip_csc_sh_odd[st])

def printCSCCLCTInSimtrack(tree, st):
    """print out stubs for the simtrack"""
    chamberstring = "+%s"%(cscStations[st].label)
    if tree.eta < 0.0:
        chamberstring = "-%s"%(cscStations[st].label)
    if tree.has_clct_even[st]:
        chamberstring += "/%d"%tree.chamber_sh_even[st]
        print(cscStations[st].label, " Stub in ", chamberstring, " hs %3d"%tree.hs_clct_even[st]," es %4d"%tree.es_clct_even[st]," bend %2d"%tree.bend_clct_even[st])
    if tree.has_clct_odd[st]:
        chamberstring += "/%d"%tree.chamber_sh_odd[st]
        print(cscStations[st].label, " Stub in ", chamberstring, " hs %3d"%tree.hs_clct_odd[st]," es %4d"%tree.es_clct_odd[st]," bend %2d"%tree.bend_clct_odd[st])


def printCSCStubInSimtrack(tree, st):
    """print out stubs for the simtrack"""
    chamberstring = "+%s"%(cscStations[st].label)
    if tree.eta < 0.0:
        chamberstring = "-%s"%(cscStations[st].label)
    if tree.has_lct_even[st]:
        chamberstring += "/%d"%tree.chamber_lct_even[st]
        print(cscStations[st].label, " Stub in ", chamberstring, " wg %3d"%tree.wg_lct_even[st], " hs %3d"%tree.hs_lct_even[st]," es %4d"%tree.es_lct_even[st]," bend %2d"%tree.bend_lct_even[st]," eta %.3f"%tree.eta_lct_even[st], " phi %.5f"%tree.phi_lct_even[st])
    if tree.has_lct_odd[st]:
        chamberstring += "/%d"%tree.chamber_lct_odd[st]
        print(cscStations[st].label, " Stub in ", chamberstring, " wg %3d"%tree.wg_lct_odd[st], " hs %3d"%tree.hs_lct_odd[st]," es %4d"%tree.es_lct_odd[st]," bend %2d"%tree.bend_lct_odd[st]," eta %.3f"%tree.eta_lct_odd[st], " phi %.5f"%tree.phi_lct_odd[st])

def printEmtfTrack(tree):
    print("SimTrack pt ", tree.pt, " eta ", tree.eta, " emtf pt ", tree.emtf_pt, " eta ", tree.emtf_eta, " mode ", tree.mode, " dphi12 ",tree.deltaphi12," dphi23 ", tree.deltaphi23, " dphi34 ",tree.deltaphi34," dphi13 ", tree.deltaphi13)
    print("\tst1 emtfhit_phi %.5f"%tree.emtfhit_st1_phi, " hs %3d"%tree.emtfhit_st1_halfstrip, " simStub_phi %.5f"%tree.cscstub_st1_phi," hs %3d"%tree.cscstub_st1_halfstrip," simhits_phi %.5f"%tree.simhits_st1_phi) 
    print("\tst2 emtfhit_phi %.5f"%tree.emtfhit_st2_phi, " hs %3d"%tree.emtfhit_st2_halfstrip, " simStub_phi %.5f"%tree.cscstub_st2_phi," hs %3d"%tree.cscstub_st2_halfstrip," simhits_phi %.5f"%tree.simhits_st2_phi) 
    print("\tst3 emtfhit_phi %.5f"%tree.emtfhit_st3_phi, " hs %3d"%tree.emtfhit_st3_halfstrip, " simStub_phi %.5f"%tree.cscstub_st3_phi," hs %3d"%tree.cscstub_st3_halfstrip," simhits_phi %.5f"%tree.simhits_st3_phi) 
    print("\tst4 emtfhit_phi %.5f"%tree.emtfhit_st4_phi, " hs %3d"%tree.emtfhit_st4_halfstrip, " simStub_phi %.5f"%tree.cscstub_st4_phi," hs %3d"%tree.cscstub_st4_halfstrip," simhits_phi %.5f"%tree.simhits_st4_phi) 

def analyzeTrees(plotterlist):
    #tree = self.dirAna.Get("simTrack")
    totalEntries  = plotterlist[0].getTree().GetEntries()
    for nEv in range(0, 10):
        for plotter in plotterlist:
            tree = plotter.getTree()
            tree.GetEntry(nEv)
            print(plotter.analyzer," ievent ",  tree.ievent," emtf_pt ", tree.emtf_pt, " emtf_eta ", tree.emtf_eta, " genpt ", tree.pt," geneta ", tree.eta)

def analyzeTwoTrees(plotter0, plotter1, treename, filename):
    f = ROOT.TFile(filename, "recreate"); f.cd()
    newTree = ROOT.TTree(treename, treename)
    totalEntries0 = plotter0.getTree().GetEntries()
    totalEntries1 = plotter1.getTree().GetEntries()
    print("plotter0 ", plotter0.analyzer, " entries ", totalEntries0, " plotter1 ", plotter1.analyzer, " entries ", totalEntries1)
    tree0 = plotter0.getTree()
    tree1 = plotter1.getTree()
    tree0.GetEntry(0)
    tree1.GetEntry(0)

    maxn= 1
    tree0_ievent   = array( 'f', maxn*[ 0 ] )
    tree0_eta      = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree0_phi      = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree0_pt       = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree0_emtf_eta = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree0_emtf_phi = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree0_emtf_pt  = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_ievent   = array( 'f', maxn*[ 0 ] )
    tree1_eta      = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_phi      = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_pt       = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_emtf_eta = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_emtf_phi = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)
    tree1_emtf_pt  = array( 'f', maxn*[ 0. ] ) #np.zeros(1, dtype=float)

    tree0_hasME1  = array('f', maxn*[0])
    tree0_hasME2  = array('f', maxn*[0])
    tree0_hasME3  = array('f', maxn*[0])
    tree0_hasME4  = array('f', maxn*[0])
    tree0_nstubs  = array('f', maxn*[0])
    tree0_mode    = array('f', maxn*[0])
    tree1_hasME1  = array('f', maxn*[0])
    tree1_hasME2  = array('f', maxn*[0])
    tree1_hasME3  = array('f', maxn*[0])
    tree1_hasME4  = array('f', maxn*[0])
    tree1_nstubs  = array('f', maxn*[0])
    tree1_mode    = array('f', maxn*[0])
    
    tree0_simhits_phi_all = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    tree0_lct_phi_all     = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    tree1_simhits_phi_all = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    tree1_lct_phi_all     = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    simhits_dphi_all      = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    lct_dphi_all          = []#[array( 'f', maxn*[ -9.0 ] )] * len(cscStations)
    tree0_lct_pattern_all = []#[array( 'f', maxn*[ 0 ] )] * len(cscStations)
    tree1_lct_pattern_all = []#[array( 'f', maxn*[ 0 ] )] * len(cscStations)

    tree0_alct_st_bx               = []
    tree0_clct_st_bx               = []
    tree1_alct_st_bx               = []
    tree1_clct_st_bx               = []
    tree0_lct_st_alctbx            = []
    tree0_lct_st_clctbx            = []
    tree1_lct_st_alctbx            = []
    tree1_lct_st_clctbx            = []
    tree0_lct_st_matchtype         = []
    tree1_lct_st_matchtype         = []
    nwires_dg_st                   = []
    ncomps_dg_st                   = []
    nstrips_dg_st                  = []
    totalwires_dg_st                   = []
    totalcomps_dg_st                   = []
    totalstrips_dg_st                  = []
    
    for st in range(len(cscStations)):
        tree0_simhits_phi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        tree0_lct_phi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        tree1_simhits_phi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        tree1_lct_phi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        simhits_dphi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        lct_dphi_all.append(array( 'f', maxn*[ -9.0 ] ))#] * len(cscStations)
        tree0_lct_pattern_all.append(array( 'f', maxn*[ 0 ] )   )#] * len(cscStations)
        tree1_lct_pattern_all.append(array( 'f', maxn*[ 0 ] )   )#] * len(cscStations)
        tree0_alct_st_bx.append(array( 'f', maxn*[ 0 ]))
        tree0_clct_st_bx.append(array( 'f', maxn*[ 0 ]))
        tree1_alct_st_bx.append(array( 'f', maxn*[ 0 ]))
        tree1_clct_st_bx.append(array( 'f', maxn*[ 0 ]))
        tree0_lct_st_alctbx.append(array( 'f', maxn*[ 0 ]))
        tree0_lct_st_clctbx.append(array( 'f', maxn*[ 0 ]))
        tree1_lct_st_alctbx.append(array( 'f', maxn*[ 0 ]))
        tree1_lct_st_clctbx.append(array( 'f', maxn*[ 0 ]))
        tree0_lct_st_matchtype.append(array( 'f', maxn*[ 0 ]))
        tree1_lct_st_matchtype.append(array( 'f', maxn*[ 0 ]))
        nwires_dg_st.append(array( 'f', maxn*[ 0 ]))
        ncomps_dg_st.append(array( 'f', maxn*[ 0 ]))
        nstrips_dg_st.append(array( 'f', maxn*[ 0 ]))
        totalwires_dg_st.append(array( 'f', maxn*[ 0 ]))
        totalcomps_dg_st.append(array( 'f', maxn*[ 0 ]))
        totalstrips_dg_st.append(array( 'f', maxn*[ 0 ]))

    
    tree0_emtfhit_st_halfstrip     = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree0_cscstub_st_halfstrip     = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree1_emtfhit_st_halfstrip     = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree1_cscstub_st_halfstrip     = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree0_emtfhit_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree0_cscstub_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree0_simhits_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree1_emtfhit_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree1_cscstub_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4
    tree0_emtfhit_st_pattern       = []#array( 'f', maxn*[  0 ] )] * 4
    tree0_cscstub_st_pattern       = []#array( 'f', maxn*[  0 ] )] * 4
    tree1_emtfhit_st_pattern       = []#array( 'f', maxn*[  0 ] )] * 4
    tree1_cscstub_st_pattern       = []#array( 'f', maxn*[  0 ] )] * 4
    tree1_simhits_st_phi           = []#array( 'f', maxn*[ -9.0 ] )] * 4

    emtfhit_st_phidiff = []#array( 'f', maxn*[ -9.0 ] )] * 4
    cscstub_st_phidiff = []#array( 'f', maxn*[ -9.0 ] )] * 4
    simhits_st_phidiff = []#array( 'f', maxn*[ -9.0 ] )] * 4
    emtfhit_st_match   = []#array('f', maxn*[0])] * 4
    cscstub_st_match   = []#array('f', maxn*[0])] * 4
    simhits_st_match   = []#array('f', maxn*[0])] * 4
    for st in range(0,4):
        tree0_emtfhit_st_halfstrip.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        tree0_cscstub_st_halfstrip.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        tree1_emtfhit_st_halfstrip.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        tree1_cscstub_st_halfstrip.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        tree0_emtfhit_st_phi.append(array( 'f', maxn*[ -9.0 ] )      )#] * 4
        tree0_cscstub_st_phi.append(array( 'f', maxn*[ -9.0 ] )      )#] * 4
        tree0_simhits_st_phi.append(array( 'f', maxn*[ -9.0 ] )      )#] * 4
        tree1_emtfhit_st_phi.append(array( 'f', maxn*[ -9.0 ] )      )#] * 4
        tree1_cscstub_st_phi.append(array( 'f', maxn*[ -9.0 ] )      )#] * 4
        tree0_emtfhit_st_pattern.append(array( 'f', maxn*[  0 ] ))#] * 4
        tree0_cscstub_st_pattern.append(array( 'f', maxn*[  0 ] ))#] * 4
        tree1_emtfhit_st_pattern.append(array( 'f', maxn*[  0 ] ))#] * 4
        tree1_cscstub_st_pattern.append(array( 'f', maxn*[  0 ] ))#] * 4
        tree1_simhits_st_phi.append(array( 'f', maxn*[ -9.0 ] )  )#] * 4

        emtfhit_st_phidiff.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        cscstub_st_phidiff.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        simhits_st_phidiff.append(array( 'f', maxn*[ -9.0 ] ))#] * 4
        emtfhit_st_match.append(array('f', maxn*[0]))#] * 4
        cscstub_st_match.append(array('f', maxn*[0]))#] * 4
        simhits_st_match.append(array('f', maxn*[0]))#] * 4

    emtfhit_nmismatch = array('f', maxn*[0])
    cscstub_nmismatch = array('f', maxn*[0])
    simhits_nmismatch = array('f', maxn*[0])

    def extractTree0():
        tree0_ievent[0] = tree0.ievent
        tree0_eta[0] = tree0.eta
        tree0_phi[0] = tree0.phi
        tree0_pt[0]  = tree0.pt
        tree0_emtf_eta[0] = tree0.emtf_eta
        tree0_emtf_phi[0] = tree0.emtf_phi
        tree0_emtf_pt[0]  = tree0.emtf_pt
        tree0_hasME1[0]   = int(tree0.hasME1)
        tree0_hasME2[0]   = int(tree0.hasME2)
        tree0_hasME3[0]   = int(tree0.hasME3)
        tree0_hasME4[0]   = int(tree0.hasME4)
        tree0_nstubs[0]   = tree0.nstubs
        tree0_mode[0]     = tree0.mode
        for st in range(0, len(cscStations)):
            tree0_simhits_phi_all[st][0] = max(tree0.phi_csc_sh_odd[st], tree0.phi_csc_sh_even[st])
            tree0_lct_phi_all[st][0]     = max(tree0.phi_lct_odd[st], tree0.phi_lct_even[st])
            tree0_lct_pattern_all[st][0] = max(tree0.bend_lct_odd[st], tree0.bend_lct_even[st])
            tree0_alct_st_bx[st][0]      = max(tree0.bx_alct_odd[st], tree0.bx_alct_even[st])
            tree0_clct_st_bx[st][0]      = max(tree0.bx_clct_odd[st], tree0.bx_clct_even[st])
            if tree0.has_lct_even[st]:
                tree0_lct_st_alctbx[st][0]      = tree0.alctbx_lct_even[st]
                tree0_lct_st_clctbx[st][0]      = tree0.clctbx_lct_even[st]
                tree0_lct_st_matchtype[st][0]   = tree0.matchtype_lct_even[st]
            else:
                tree0_lct_st_alctbx[st][0]      = tree0.alctbx_lct_odd[st]
                tree0_lct_st_clctbx[st][0]      = tree0.clctbx_lct_odd[st]
                tree0_lct_st_matchtype[st][0]   = tree0.matchtype_lct_odd[st]
            nwires_dg_st[st][0]          = max(tree0.nwires_dg_even[st], tree0.nwires_dg_odd[st])
            ncomps_dg_st[st][0]          = max(tree0.ncomparators_dg_even[st], tree0.ncomparators_dg_odd[st])
            nstrips_dg_st[st][0]         = max(tree0.nstrips_dg_even[st], tree0.nstrips_dg_odd[st])
            totalwires_dg_st[st][0]      = max(tree0.totalwires_dg_even[st],       tree0.totalwires_dg_odd[st])
            totalcomps_dg_st[st][0]      = max(tree0.totalcomparators_dg_even[st], tree0.totalcomparators_dg_odd[st])
            totalstrips_dg_st[st][0]     = max(tree0.totalstrips_dg_even[st],      tree0.totalstrips_dg_odd[st])

        for st in range(0, 4):
            tree0_emtfhit_st_phi[st][0]      = getattr(tree0, "emtfhit_st%d_phi"%(st+1))
            tree0_emtfhit_st_halfstrip[st][0]= getattr(tree0, "emtfhit_st%d_halfstrip"%(st+1))
            tree0_emtfhit_st_pattern[st][0]  = getattr(tree0, "emtfhit_st%d_pattern"%(st+1))
            tree0_cscstub_st_phi[st][0]      = getattr(tree0, "cscstub_st%d_phi"%(st+1))
            tree0_cscstub_st_halfstrip[st][0]= getattr(tree0, "cscstub_st%d_halfstrip"%(st+1))
            tree0_cscstub_st_pattern[st][0]  = getattr(tree0, "cscstub_st%d_pattern"%(st+1))
            tree0_simhits_st_phi[st][0]      = getattr(tree0, "simhits_st%d_phi"%(st+1))


    def extractTree1():
        tree1_ievent[0] = tree1.ievent
        tree1_eta[0] = tree1.eta
        tree1_phi[0] = tree1.phi
        tree1_pt[0]  = tree1.pt
        tree1_emtf_eta[0] = tree1.emtf_eta
        tree1_emtf_phi[0] = tree1.emtf_phi
        tree1_emtf_pt[0]  = tree1.emtf_pt
        tree1_hasME1[0]   = int(tree1.hasME1)
        tree1_hasME2[0]   = int(tree1.hasME2)
        tree1_hasME3[0]   = int(tree1.hasME3)
        tree1_hasME4[0]   = int(tree1.hasME4)
        tree1_nstubs[0]   = tree1.nstubs
        tree1_mode[0]     = tree1.mode
        for st in range(0, len(cscStations)):
            tree1_simhits_phi_all[st][0] = max(tree1.phi_csc_sh_odd[st], tree1.phi_csc_sh_even[st])
            tree1_lct_phi_all[st][0]     = max(tree1.phi_lct_odd[st],  tree1.phi_lct_even[st])
            tree1_lct_pattern_all[st][0] = max(tree1.bend_lct_odd[st], tree1.bend_lct_even[st])
            tree1_alct_st_bx[st][0]      = max(tree1.bx_alct_odd[st], tree1.bx_alct_even[st])
            tree1_clct_st_bx[st][0]      = max(tree1.bx_clct_odd[st], tree1.bx_clct_even[st])
            if tree1.has_lct_even[st]:
                tree1_lct_st_alctbx[st][0]      = tree1.alctbx_lct_even[st]
                tree1_lct_st_clctbx[st][0]      = tree1.clctbx_lct_even[st]
                tree1_lct_st_matchtype[st][0]   = tree1.matchtype_lct_even[st]
            else:
                tree1_lct_st_alctbx[st][0]      = tree1.alctbx_lct_odd[st]
                tree1_lct_st_clctbx[st][0]      = tree1.clctbx_lct_odd[st]
                tree1_lct_st_matchtype[st][0]   = tree1.matchtype_lct_odd[st]

        for st in range(0, 4):
            tree1_emtfhit_st_phi[st][0]      = getattr(tree1, "emtfhit_st%d_phi"%(st+1))
            tree1_emtfhit_st_halfstrip[st][0]= getattr(tree1, "emtfhit_st%d_halfstrip"%(st+1))
            tree1_emtfhit_st_pattern[st][0]  = getattr(tree1, "emtfhit_st%d_pattern"%(st+1))
            tree1_cscstub_st_phi[st][0]      = getattr(tree1, "cscstub_st%d_phi"%(st+1))
            tree1_cscstub_st_halfstrip[st][0]= getattr(tree1, "cscstub_st%d_halfstrip"%(st+1))
            tree1_cscstub_st_pattern[st][0]  = getattr(tree1, "cscstub_st%d_pattern"%(st+1))
            tree1_simhits_st_phi[st][0]      = getattr(tree1, "simhits_st%d_phi"%(st+1))



    def initBranches():
        emtfhit_nmismatch[0] = 0
        simhits_nmismatch[0] = 0
        cscstub_nmismatch[0] = 0
        for st in range(0, 4):
            emtfhit_st_match[st][0] = 0  
            simhits_st_match[st][0] = 0 
            cscstub_st_match[st][0] = 0 
            emtfhit_st_phidiff[st][0] = -9.0
            simhits_st_phidiff[st][0] = -9.0
            cscstub_st_phidiff[st][0] = -9.0

        for st in range(0, len(cscStations)):
            if tree0_lct_phi_all[st][0] > -9.0 and tree1_lct_phi_all[st][0] > -9.0:
                lct_dphi_all[st][0] = deltaPhi(tree0_lct_phi_all[st][0], tree1_lct_phi_all[st][0])
            else:
                lct_dphi_all[st][0] =  -9.0
            if tree0_simhits_phi_all[st][0] > -9.0 and tree1_simhits_phi_all[st][0] > -9.0:
                simhits_dphi_all[st][0] = deltaPhi(tree0_simhits_phi_all[st][0], tree1_simhits_phi_all[st][0])
            else:
                simhits_dphi_all[st][0] =  -9.0


    
    #newTree.Branch("deltatheta12",          deltatheta12);
    #newTree.Branch("deltatheta13",          deltatheta13);
    #newTree.Branch("deltatheta14",          deltatheta14);
    #newTree.Branch("deltatheta23",          deltatheta23);
    #newTree.Branch("deltatheta24",          deltatheta24);
    #newTree.Branch("deltatheta34",          deltatheta34);
    #newTree.Branch("lctslope1",             lctslope1);
    #newTree.Branch("lctslope2",             lctslope2);
    #newTree.Branch("lctslope3",             lctslope3);
    #newTree.Branch("lctslope4",             lctslope4);
    #newTree.Branch("lctpattern1",           lctpattern1);
    #newTree.Branch("lctpattern2",           lctpattern2);
    #newTree.Branch("lctpattern3",           lctpattern3);
    #newTree.Branch("lctpattern4",           lctpattern4);
    #newTree.Branch("quality",               quality);
    #newTree.Branch("tree0_mode",                  tree0_mode, "I");
    #newTree.Branch("tree0_deltaphi12",            tree0_deltaphi12, "I");
    #newTree.Branch("tree0_deltaphi13",            tree0_deltaphi13, "I");
    #newTree.Branch("tree0_deltaphi14",            tree0_deltaphi14, "I");
    #newTree.Branch("tree0_deltaphi23",            tree0_deltaphi23, "I");
    #newTree.Branch("tree0_deltaphi24",            tree0_deltaphi24, "I");
    #newTree.Branch("tree0_deltaphi34",            tree0_deltaphi34, "I");

    #newTree.Branch("tree0_cscstub_st1_matched",   tree0_cscstub_st1_matched, "I");
    #newTree.Branch("tree0_cscstub_st1_found",     tree0_cscstub_st1_found, "I");
    #newTree.Branch("tree0_cscstub_st1_wire",      tree0_cscstub_st1_wire);
    #newTree.Branch("tree0_cscstub_st1_halfstrip", tree0_cscstub_st1_halfstrip);
    #newTree.Branch("tree0_cscstub_st1_pattern",   tree0_cscstub_st1_pattern);
    #newTree.Branch("tree0_cscstub_st1_ring",      tree0_cscstub_st1_ring);
    #newTree.Branch("tree0_emtfhit_st1_wire",      tree0_emtfhit_st1_wire);
    #newTree.Branch("tree0_emtfhit_st1_halfstrip", tree0_emtfhit_st1_halfstrip);
    #newTree.Branch("tree0_emtfhit_st1_pattern",   tree0_emtfhit_st1_pattern);
    #newTree.Branch("tree0_emtfhit_st1_ring",      tree0_emtfhit_st1_ring);

    #newTree.Branch("tree0_cscstub_st2_matched",   tree0_cscstub_st2_matched);
    #newTree.Branch("tree0_cscstub_st2_found",     tree0_cscstub_st2_found);
    #newTree.Branch("tree0_cscstub_st2_wire",      tree0_cscstub_st2_wire);
    #newTree.Branch("tree0_cscstub_st2_halfstrip", tree0_cscstub_st2_halfstrip);
    #newTree.Branch("tree0_cscstub_st2_pattern",   tree0_cscstub_st2_pattern);
    #newTree.Branch("tree0_cscstub_st2_ring",      tree0_cscstub_st2_ring);
    #newTree.Branch("tree0_emtfhit_st2_wire",      tree0_emtfhit_st2_wire);
    #newTree.Branch("tree0_emtfhit_st2_halfstrip", tree0_emtfhit_st2_halfstrip);
    #newTree.Branch("tree0_emtfhit_st2_pattern",   tree0_emtfhit_st2_pattern);
    #newTree.Branch("tree0_emtfhit_st2_ring",      tree0_emtfhit_st2_ring);

    #newTree.Branch("tree0_cscstub_st3_matched",   tree0_cscstub_st3_matched);
    #newTree.Branch("tree0_cscstub_st3_found",     tree0_cscstub_st3_found);
    #newTree.Branch("tree0_cscstub_st3_wire",      tree0_cscstub_st3_wire);
    #newTree.Branch("tree0_cscstub_st3_halfstrip", tree0_cscstub_st3_halfstrip);
    #newTree.Branch("tree0_cscstub_st3_pattern",   tree0_cscstub_st3_pattern);
    #newTree.Branch("tree0_cscstub_st3_matched",   tree0_cscstub_st3_matched);
    #newTree.Branch("tree0_cscstub_st3_found",     tree0_cscstub_st3_found);
    #newTree.Branch("tree0_cscstub_st3_wire",      tree0_cscstub_st3_wire);
    #newTree.Branch("tree0_cscstub_st3_halfstrip", tree0_cscstub_st3_halfstrip);
    #newTree.Branch("tree0_cscstub_st3_pattern",   tree0_cscstub_st3_pattern);
    #newTree.Branch("tree0_cscstub_st3_ring",      tree0_cscstub_st3_ring);
    #newTree.Branch("tree0_emtfhit_st3_wire",      tree0_emtfhit_st3_wire);
    #newTree.Branch("tree0_emtfhit_st3_halfstrip", tree0_emtfhit_st3_halfstrip);
    #newTree.Branch("tree0_emtfhit_st3_pattern",   tree0_emtfhit_st3_pattern);
    #newTree.Branch("tree0_emtfhit_st3_ring",      tree0_emtfhit_st3_ring);

    #newTree.Branch("tree0_cscstub_st4_matched",   tree0_cscstub_st4_matched);
    #newTree.Branch("tree0_cscstub_st4_found",     tree0_cscstub_st4_found);
    #newTree.Branch("tree0_cscstub_st4_wire",      tree0_cscstub_st4_wire);
    #newTree.Branch("tree0_cscstub_st4_halfstrip", tree0_cscstub_st4_halfstrip);
    #newTree.Branch("tree0_cscstub_st4_pattern",   tree0_cscstub_st4_pattern);
    #newTree.Branch("tree0_cscstub_st4_ring",      tree0_cscstub_st4_ring);
    #newTree.Branch("tree0_emtfhit_st4_wire",      tree0_emtfhit_st4_wire);
    #newTree.Branch("tree0_emtfhit_st4_halfstrip", tree0_emtfhit_st4_halfstrip);
    #newTree.Branch("tree0_emtfhit_st4_pattern",   tree0_emtfhit_st4_pattern);
    #newTree.Branch("tree0_emtfhit_st4_ring",      tree0_emtfhit_st4_ring);

    #newTree.Branch("tree0_nstubs_matched_TF",     tree0_nstubs_matched_TF);
    #newTree.Branch("tree0_allstubs_matched_TF",   tree0_allstubs_matched_TF);

    newTree.Branch("tree0_ievent",   tree0_ievent, "I")
    newTree.Branch("tree0_gen_eta",  tree0_eta, "F")
    newTree.Branch("tree0_gen_phi",  tree0_phi, "F")
    newTree.Branch("tree0_gen_pt",   tree0_pt, "F")
    newTree.Branch("tree0_emtf_eta", tree0_emtf_eta, "F")
    newTree.Branch("tree0_emtf_phi", tree0_emtf_phi, "F")
    newTree.Branch("tree0_emtf_pt",  tree0_emtf_pt, "F")
    newTree.Branch("tree1_ievent",   tree1_ievent, "I")
    newTree.Branch("tree1_gen_eta",  tree1_eta, "F")
    newTree.Branch("tree1_gen_phi",  tree1_phi, "F")
    newTree.Branch("tree1_gen_pt",   tree1_pt, "F")
    newTree.Branch("tree1_emtf_eta", tree1_emtf_eta, "F")
    newTree.Branch("tree1_emtf_phi", tree1_emtf_phi, "F")
    newTree.Branch("tree1_emtf_pt",  tree1_emtf_pt, "F")

    newTree.Branch("tree0_emtf_hasME1",tree0_hasME1, "I");
    newTree.Branch("tree0_emtf_hasME2",tree0_hasME2, "I");
    newTree.Branch("tree0_emtf_hasME3",tree0_hasME3, "I");
    newTree.Branch("tree0_emtf_hasME4",tree0_hasME4, "I");
    newTree.Branch("tree0_emtf_nstubs",tree0_nstubs, "I");
    newTree.Branch("tree0_emtf_mode",  tree0_mode, "I");
    newTree.Branch("tree1_emtf_hasME1",tree1_hasME1, "I");
    newTree.Branch("tree1_emtf_hasME2",tree1_hasME2, "I");
    newTree.Branch("tree1_emtf_hasME3",tree1_hasME3, "I");
    newTree.Branch("tree1_emtf_hasME4",tree1_hasME4, "I");
    newTree.Branch("tree1_emtf_nstubs",tree1_nstubs, "I");
    newTree.Branch("tree1_emtf_mode",  tree1_mode, "I");

    newTree.Branch("emtfhit_nmismatch", emtfhit_nmismatch,'I')
    newTree.Branch("cscstub_nmismatch", cscstub_nmismatch,'I')
    newTree.Branch("simhits_nmismatch", simhits_nmismatch,'I')
    for st in range(0, 4):
        newTree.Branch("emtfhit_st%d_phidiff"%(st+1), emtfhit_st_phidiff[st],'F')
        newTree.Branch("cscstub_st%d_phidiff"%(st+1), cscstub_st_phidiff[st],'F')
        newTree.Branch("simhits_st%d_phidiff"%(st+1), simhits_st_phidiff[st],'F')
        newTree.Branch("emtfhit_st%d_match"%(st+1), emtfhit_st_match[st],'I')
        newTree.Branch("cscstub_st%d_match"%(st+1), cscstub_st_match[st],'I')
        newTree.Branch("simhits_st%d_match"%(st+1), simhits_st_match[st],'I')

        newTree.Branch("tree0_emtfhit_st%d_phi"%(st+1),       tree0_emtfhit_st_phi[st],'F')
        newTree.Branch("tree0_emtfhit_st%d_pattern"%(st+1),   tree0_emtfhit_st_pattern[st],'I')
        newTree.Branch("tree0_emtfhit_st%d_halfstrip"%(st+1), tree0_emtfhit_st_halfstrip[st],'F')
        newTree.Branch("tree0_cscstub_st%d_phi"%(st+1),       tree0_cscstub_st_phi[st],'F')
        newTree.Branch("tree0_cscstub_st%d_pattern"%(st+1),   tree0_cscstub_st_pattern[st],'I')
        newTree.Branch("tree0_cscstub_st%d_halfstrip"%(st+1), tree0_cscstub_st_halfstrip[st],'F')
        newTree.Branch("tree0_simhits_st%d_phi"%(st+1),       tree0_simhits_st_phi[st],'F')
        newTree.Branch("tree1_emtfhit_st%d_phi"%(st+1),       tree1_emtfhit_st_phi[st],'F')
        newTree.Branch("tree1_emtfhit_st%d_pattern"%(st+1),   tree1_emtfhit_st_pattern[st],'I')
        newTree.Branch("tree1_emtfhit_st%d_halfstrip"%(st+1), tree1_emtfhit_st_halfstrip[st],'F')
        newTree.Branch("tree1_cscstub_st%d_phi"%(st+1),       tree1_cscstub_st_phi[st],'F')
        newTree.Branch("tree1_cscstub_st%d_pattern"%(st+1),   tree1_cscstub_st_pattern[st],'I')
        newTree.Branch("tree1_cscstub_st%d_halfstrip"%(st+1), tree1_cscstub_st_halfstrip[st],'F')
        newTree.Branch("tree1_simhits_st%d_phi"%(st+1),       tree1_simhits_st_phi[st],'F')


    for st in range(0, len(cscStations)):
        newTree.Branch("tree0_simhits_phi_%s"%cscStations[st].labelc, tree0_simhits_phi_all[st],'F')
        newTree.Branch("tree0_lct_phi_%s"%cscStations[st].labelc,     tree0_lct_phi_all[st],'F')
        newTree.Branch("tree0_lct_pattern_%s"%cscStations[st].labelc, tree0_lct_pattern_all[st],'I')
        newTree.Branch("tree1_simhits_phi_%s"%cscStations[st].labelc, tree1_simhits_phi_all[st],'F')
        newTree.Branch("tree1_lct_phi_%s"%cscStations[st].labelc,     tree1_lct_phi_all[st],'F')
        newTree.Branch("tree1_lct_pattern_%s"%cscStations[st].labelc, tree1_lct_pattern_all[st],'I')
        newTree.Branch("lct_dphi_%s"%cscStations[st].labelc,     lct_dphi_all[st],'F')
        newTree.Branch("simhits_dphi_%s"%cscStations[st].labelc,     simhits_dphi_all[st],'F')
        newTree.Branch("tree0_alct_bx_%s"%cscStations[st].labelc,       tree0_alct_st_bx[st],'I')
        newTree.Branch("tree0_clct_bx_%s"%cscStations[st].labelc,       tree0_clct_st_bx[st],'I')
        newTree.Branch("tree1_alct_bx_%s"%cscStations[st].labelc,       tree1_alct_st_bx[st],'I')
        newTree.Branch("tree1_clct_bx_%s"%cscStations[st].labelc,       tree1_clct_st_bx[st],'I')
        newTree.Branch("tree0_lct_alctbx_%s"%cscStations[st].labelc,    tree0_lct_st_alctbx[st],'I')
        newTree.Branch("tree0_lct_clctbx_%s"%cscStations[st].labelc,    tree0_lct_st_clctbx[st],'I')
        newTree.Branch("tree1_lct_alctbx_%s"%cscStations[st].labelc,    tree1_lct_st_alctbx[st],'I')
        newTree.Branch("tree1_lct_clctbx_%s"%cscStations[st].labelc,    tree1_lct_st_clctbx[st],'I')
        newTree.Branch("tree0_lct_matchtype_%s"%cscStations[st].labelc, tree0_lct_st_matchtype[st],'I')
        newTree.Branch("tree1_lct_matchtype_%s"%cscStations[st].labelc, tree1_lct_st_matchtype[st],'I')
        newTree.Branch("ncomps_dg_%s"%cscStations[st].labelc,       ncomps_dg_st[st], 'I')
        newTree.Branch("nwires_dg_%s"%cscStations[st].labelc,       nwires_dg_st[st], 'I')
        newTree.Branch("nstrips_dg_%s"%cscStations[st].labelc,      nstrips_dg_st[st], 'I')
        newTree.Branch("totalcomps_dg_%s"%cscStations[st].labelc,       totalcomps_dg_st[st], 'I')
        newTree.Branch("totalwires_dg_%s"%cscStations[st].labelc,       totalwires_dg_st[st], 'I')
        newTree.Branch("totalstrips_dg_%s"%cscStations[st].labelc,      totalstrips_dg_st[st], 'I')

    phidiff_cut = 0.002
    debuglist = {}
    debugevents = False
    nstart = 0
    #if debugevents: nstart=8385
    nevent = 0
    Totevents = 0; Badevents = 0
    evlist_display = open("eventlist_display%s.txt"%filename, "w+")
    #for nEv in range(8385, totalEntries0):
    #for nEv in range(8385, 8500):
    for nEv in range(nstart, totalEntries0):
        nevent += 1
        tree0.GetEntry(nEv)
        tree1.GetEntry(nEv)
        if debugevents and not (tree0.ievent >= 2000 and tree0.ievent<=3000 ): continue
        print("nevent ", nevent, " ievent from tree ", tree0.ievent)

        
        extractTree0()
        extractTree1()

        initBranches()
        
        for st in range(1,5):
          #if getattr(tree0, "emtfhit_st%d_ring"%st) == getattr(tree1, "emtfhit_st%d_ring"%st) and getattr(tree0, "emtfhit_st%d_wire"%st) == getattr(tree1, "emtfhit_st%d_wire"%st) and abs(getattr(tree0, "emtfhit_st%d_halfstrip"%st) - getattr(tree1, "emtfhit_st%d_halfstrip"%st) <= 1.0) and :
          if abs(deltaPhi(getattr(tree0, "emtfhit_st%d_phi"%st), getattr(tree1, "emtfhit_st%d_phi"%st))) <= phidiff_cut:
              emtfhit_st_match[st-1][0] = 1
          if getattr(tree0, "emtfhit_st%d_phi"%st)  > -9 and  getattr(tree1, "emtfhit_st%d_phi"%st) > -9:
              emtfhit_st_phidiff[st-1][0] = deltaPhi(getattr(tree0, "emtfhit_st%d_phi"%st), getattr(tree1, "emtfhit_st%d_phi"%st))
          #if getattr(tree0, "cscstub_st%d_ring"%st) == getattr(tree1, "cscstub_st%d_ring"%st) and getattr(tree0, "cscstub_st%d_wire"%st) == getattr(tree1, "cscstub_st%d_wire"%st) and abs(getattr(tree0, "cscstub_st%d_halfstrip"%st) - getattr(tree1, "cscstub_st%d_halfstrip"%st) <= 1.0) :
          if abs(deltaPhi(getattr(tree0, "cscstub_st%d_phi"%st), getattr(tree1, "cscstub_st%d_phi"%st))) <= phidiff_cut:
              cscstub_st_match[st-1][0] = 1
          if getattr(tree0, "cscstub_st%d_phi"%st)  > -9 and  getattr(tree1, "cscstub_st%d_phi"%st) > -9:
              cscstub_st_phidiff[st-1][0] = deltaPhi(getattr(tree0, "cscstub_st%d_phi"%st), getattr(tree1, "cscstub_st%d_phi"%st))
          if abs(getattr(tree0, "simhits_st%d_phi"%st) - getattr(tree1, "simhits_st%d_phi"%st)) <= phidiff_cut:
              simhits_st_match[st-1][0] = 1
          if getattr(tree0, "simhits_st%d_phi"%st)  > -9 and  getattr(tree1, "simhits_st%d_phi"%st) > -9:
              simhits_st_phidiff[st-1][0] = deltaPhi(getattr(tree0, "simhits_st%d_phi"%st), getattr(tree1, "simhits_st%d_phi"%st))

          emtfhit_nmismatch[0]  =emtfhit_nmismatch[0] + 1-emtfhit_st_match[st-1][0] 
          cscstub_nmismatch[0]  =cscstub_nmismatch[0] + 1-cscstub_st_match[st-1][0] 
          simhits_nmismatch[0]  =simhits_nmismatch[0] + 1-simhits_st_match[st-1][0] 

        newTree.Fill()

        st1_dphi_stub_sim0 = -9; st1_dphi_emtfhit_sim0 = -9
        st1_dphi_stub_sim1 = -9; st1_dphi_emtfhit_sim1 = -9
        if tree0.emtfhit_st1_phi > -9 and tree0.simhits_st1_phi > -9:
            st1_dphi_stub_sim0 = tree0.cscstub_st1_phi  - tree0.simhits_st1_phi 
            st1_dphi_emtfhit_sim0 = tree0.emtfhit_st1_phi  - tree0.simhits_st1_phi 
        if tree1.emtfhit_st1_phi > -9 and tree1.simhits_st1_phi > -9:
            st1_dphi_stub_sim1 = tree1.cscstub_st1_phi  - tree1.simhits_st1_phi 
            st1_dphi_emtfhit_sim1 = tree1.emtfhit_st1_phi  - tree1.simhits_st1_phi 

        #print("st1_dphi_emtfhit_sim0 ", st1_dphi_emtfhit_sim0, " st1_dphi_emtfhit_sim1 ", st1_dphi_emtfhit_sim1," diff ", abs(st1_dphi_emtfhit_sim1-st1_dphi_emtfhit_sim0))
        #if st1_dphi_emtfhit_sim1 > -9 and st1_dphi_emtfhit_sim0 > -9 and abs(st1_dphi_emtfhit_sim1-st1_dphi_emtfhit_sim0)>0.001:
        if abs(tree0.eta)>1.65: Totevents += 1 
        chamberlist = ""; endcap = "+"
        if tree0.eta < 0.0: endcap = "-"

        ch_st1 = None; ch_st2 = None
        ch_display = ""
        dodisplay = False
        #if (tree0.matchtype_lct_odd[5]==2 and tree1.matchtype_lct_odd[5]==3) or (tree0.matchtype_lct_even[5]==2 and tree1.matchtype_lct_even[5]==3) and debugevents:
        if max(tree0.totalwires_dg_even[5],       tree0.totalwires_dg_odd[5]) > 60 and  debugevents and False:
            print("-------------------------- Analyzing the ievent ",tree0.ievent," nEv ", nevent," with tree0 too many wire digis -------------------")
            Badevents += 1
            print("total wire digis ",  max(tree0.totalwires_dg_even[5],       tree0.totalwires_dg_odd[5]))
            if abs(tree0.eta)>1.65 and (tree0.has_csc_sh_odd[5] or tree0.has_csc_sh_even[5]):
                chstr = "/%d"%tree0.chamber_sh_odd[5]
                if tree0.has_csc_sh_even[5]: chstr = "/%d"%tree0.chamber_sh_even[5]
                ch_st1 = int(chstr[1:])
                chamberlist = cscStations[5].label[:2]+endcap+cscStations[5].label[2:]+chstr+","
                ##format: run event endcap station ring chamber
                if tree0.eta < 0.0: ch_display += "1 %d 2 2 1 %d,"%(tree0.ievent, ch_st1)
                else: ch_display += "1 %d 1 2 1 %d,"%(tree0.ievent, ch_st1)
                #debuglist[tree0.ievent] = chamberlist[:-1]
                dodisplay = True
                print("bad event with chamber ", chamberlist[:-1], " display format ", ch_display)

        if (tree0.has_lct_odd[0] or tree0.has_lct_even[0]) and not(tree1.has_lct_odd[0] or tree1.has_lct_even[0]) and debugevents:
            print("-------------------------- Analyzing the ievent ",tree0.ievent," nEv ", nevent," with tree0 has LCT but not tree1 ----------------------------")
            if (tree1.has_clct_odd[0] or tree1.has_clct_even[0]): print("CLCT in Tree1")
            if (tree1.has_alct_odd[0] or tree1.has_alct_even[0]): print("ALCT in Tree1")
            #printEmtfTrack(tree0)
            #printCSCCLCTInSimtrack(tree0, 0)
            printCSCStubInSimtrack(tree0, 0)
            printCSCStubInSimtrack(tree1, 0)
            Badevents += 1
            if abs(tree0.eta)>1.65 and (tree0.has_csc_sh_odd[0] or tree0.has_csc_sh_even[0]):
                chstr = "/%d"%tree0.chamber_sh_odd[0]
                if tree0.has_csc_sh_even[0]: chstr = "/%d"%tree0.chamber_sh_even[0]
                ch_st1 = int(chstr[1:])
                chamberlist = cscStations[0].label[:2]+endcap+cscStations[0].label[2:]+chstr+","
                if tree0.eta < 0.0: ch_display += "1 %d 2 1 1 %d,"%(tree0.ievent, ch_st1)
                else: ch_display += "1 %d 1 1 1 %d,"%(tree0.ievent, ch_st1)
                #debuglist[tree0.ievent] = chamberlist[:-1]
                dodisplay = True
                print("bad event with chamber ", chamberlist[:-1], " display format ", ch_display)

        if tree0.emtf_pt >= 20 and tree1.emtf_pt < 20 and abs(tree0.eta)>1.65 and debugevents and False:
            print("-------------------------- Analyzing the ievent ",tree0.ievent," nEv ", nevent," with different phi from CSC stub ----------------------------")
            print("\t ========= Analyzer ", plotter0.analyzer, " =========")
            printEmtfTrack(tree0)
            Badevents += 1

            if abs(tree0.eta)>1.65 and (tree0.has_lct_odd[0] or tree0.has_lct_even[0]):
                printCSCStubInSimtrack(tree0, 0)
                chstr = "/%d"%tree0.chamber_lct_odd[0]
                if tree0.has_lct_even[0]: chstr = "/%d"%tree0.chamber_lct_even[0]
                chamberlist += cscStations[0].label[:2]+endcap+cscStations[0].label[2:]+chstr+","
                ch_st1 = int(chstr[1:])

            if abs(tree0.eta)>1.65 and (tree0.has_lct_odd[5] or tree0.has_lct_even[5]):
                printCSCStubInSimtrack(tree0, 5)
                chstr = "/%d"%tree0.chamber_lct_odd[5]
                if tree0.has_lct_even[5]: chstr = "/%d"%tree0.chamber_lct_even[5]
                chamberlist += cscStations[5].label[:2]+endcap+cscStations[5].label[2:]+chstr+","
                ch_st2 = int(chstr[1:])
            elif  abs(tree0.eta)>1.65 and ch_st1:
                ch_st2 = (ch_st1+1)/2
                chstr = "/%d"%ch_st2
                chamberlist += cscStations[5].label[:2]+endcap+cscStations[5].label[2:]+chstr+","

            if abs(tree0.eta)>1.65 and (tree0.has_lct_odd[7] or tree0.has_lct_even[7]):
                printCSCStubInSimtrack(tree0, 7)
                chstr = "/%d"%tree0.chamber_lct_odd[7]
                if tree0.has_lct_even[7]: chstr = "/%d"%tree0.chamber_lct_even[7]
                chamberlist += cscStations[7].label[:2]+endcap+cscStations[7].label[2:]+chstr+","
            elif  abs(tree0.eta)>1.65 and ch_st2:
                chstr = "/%d"%ch_st2
                chamberlist += cscStations[7].label[:2]+endcap+cscStations[7].label[2:]+chstr+","

            if abs(tree0.eta)>1.65 and (tree0.has_lct_odd[9] or tree0.has_lct_even[9]):
                printCSCStubInSimtrack(tree0, 9)
                chstr = "/%d"%tree0.chamber_lct_odd[9]
                if tree0.has_lct_even[9]: chstr = "/%d"%tree0.chamber_lct_even[9]
                chamberlist += cscStations[9].label[:2]+endcap+cscStations[9].label[2:]+chstr+","
            elif  abs(tree0.eta)>1.65 and ch_st2:
                chstr = "/%d"%ch_st2
                chamberlist += cscStations[9].label[:2]+endcap+cscStations[9].label[2:]+chstr+","

            if abs(tree0.eta)>1.65 and not ch_st1 and ch_st2:
                ch_st1 = ch_st2*2
                chstr = "/%d"%ch_st1
                chamberlist += cscStations[0].label[:2]+endcap+cscStations[0].label[2:]+chstr+","
                ch_st1 = ch_st2*2-1
                chstr = "/%d"%ch_st1
                chamberlist += cscStations[0].label[:2]+endcap+cscStations[0].label[2:]+chstr+","
            elif abs(tree0.eta)>1.65 and not ch_st1 and not ch_st2:
                print("error!! EMTF has no hit in st1 and st2, should be ignore!!")

                
            print("\t ========= Analyzer ", plotter1.analyzer, " =========")
            printEmtfTrack(tree1)
            if abs(tree1.eta)>1.65 and (tree1.has_lct_odd[0] or tree1.has_lct_even[0]):
                printCSCStubInSimtrack(tree1, 0)
            if abs(tree1.eta)>1.65 and (tree1.has_lct_odd[5] or tree1.has_lct_even[5]):
                printCSCStubInSimtrack(tree1, 5)
            if abs(tree1.eta)>1.65 and (tree1.has_lct_odd[7] or tree1.has_lct_even[7]):
                printCSCStubInSimtrack(tree1, 7)
            if abs(tree1.eta)>1.65 and (tree1.has_lct_odd[9] or tree1.has_lct_even[9]):
                printCSCStubInSimtrack(tree1, 9)
            if ch_st1 and tree0.emtfhit_st1_phi > -9 and abs(deltaPhi(tree0.emtfhit_st1_phi, tree1.emtfhit_st1_phi)) > 0.003:
                ##format: run event endcap station ring chamber
                if tree0.eta < 0.0: ch_display += "1 %d 2 1 1 %d,"%(tree0.ievent, ch_st1)
                else: ch_display += "1 %d 1 1 1 %d,"%(tree0.ievent, ch_st1)
            if ch_st2 and tree0.emtfhit_st2_phi > -9 and abs(deltaPhi(tree0.emtfhit_st2_phi, tree1.emtfhit_st2_phi)) > 0.003:
                if tree0.eta < 0.0: ch_display += "1 %d 2 2 1 %d,"%(tree0.ievent, ch_st2)
                else: ch_display += "1 %d 1 2 1 %d,"%(tree0.ievent, ch_st2)
            if ch_st2 and tree0.emtfhit_st3_phi > -9 and abs(deltaPhi(tree0.emtfhit_st3_phi, tree1.emtfhit_st3_phi)) > 0.003:
                if tree0.eta < 0.0: ch_display += "1 %d 2 3 1 %d,"%(tree0.ievent, ch_st2)
                else: ch_display += "1 %d 1 3 1 %d,"%(tree0.ievent, ch_st2)
            if ch_st2 and tree0.emtfhit_st4_phi > -9 and abs(deltaPhi(tree0.emtfhit_st4_phi, tree1.emtfhit_st4_phi)) > 0.003:
                if tree0.eta < 0.0: ch_display += "1 %d 2 4 1 %d,"%(tree0.ievent, ch_st2)
                else: ch_display += "1 %d 1 4 1 %d,"%(tree0.ievent, ch_st2)
            dodisplay = True

        if dodisplay:
            print("list of chambers ", chamberlist[:-1], "\n")
            print("chambers in display format", ch_display)
            if ch_display != "":
                for ch in ch_display.split(",")[:-1]:
                    evlist_display.write(ch+"\n")
            if tree0.ievent not in debuglist.keys():
                debuglist[tree0.ievent] = chamberlist[:-1]
            else:
                debuglist[tree0.ievent] += ","+chamberlist[:-1]
    newTree.Write()
    f.Close()
    evlist_display.close()
    print(debuglist)
    print("Total simtracks in ring1 : ", Totevents, " badevents ", Badevents," len of debuglist ", len(debuglist))
    print("Done, analyzeTwoTrees")
