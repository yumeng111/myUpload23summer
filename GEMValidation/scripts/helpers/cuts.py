from ROOT import TCut
from helpers.logic import ANDtwo, ORtwo, AND, OR

def ANDtwo(cut1,cut2):
    """AND of two TCuts in PyROOT"""
    if cut1.GetTitle() == "":
        return cut2
    if cut2.GetTitle() == "":
        return cut1
    return TCut("(%s) && (%s)"%(cut1.GetTitle(),cut2.GetTitle()))


def ORtwo(cut1,cut2):
    """OR of two TCuts in PyROOT"""
    if cut1.GetTitle() == "":
        return cut2
    if cut2.GetTitle() == "":
        return cut1
    return TCut("(%s) || (%s)"%(cut1.GetTitle(),cut2.GetTitle()))


def AND(*arg):
    """AND of any number of TCuts in PyROOT"""
    length = len(arg)
    if length == 0:
        print("ERROR: invalid number of arguments")
        return
    if length == 1:
        return arg[0]
    if length==2:
        return ANDtwo(arg[0],arg[1])
    if length>2:
        result = arg[0]
        for i in range(1,len(arg)):
            result = ANDtwo(result,arg[i])
        return result


def OR(*arg):
    """OR of any number of TCuts in PyROOT"""
    length = len(arg)
    if length == 0:
        print("ERROR: invalid number of arguments")
        return
    if length == 1:
        return arg[0]
    if length==2:
        return ORtwo(arg[0],arg[1])
    if length>2:
        result = arg[0]
        for i in range(1,len(arg)):
            result = ORtwo(result,arg[i])
        return result


nocut = TCut("")

genpt = "genParticle.pt"
geneta = "genParticle.eta"
genphi = "genParticle.phi"

## cut on the generator pT
def ok_pt(pt_min):
    return TCut("genParticle.pt >= %f"%(pt_min))

## eta for a station
def ok_eta(eta_min, eta_max):
    ok_eta_min = TCut("TMath::Abs(genParticle.eta) > %f"%(eta_min))
    ok_eta_max = TCut("TMath::Abs(genParticle.eta) < %f"%(eta_max))
    ok_eta = AND(ok_eta_min,ok_eta_max)
    return ok_eta

def ok_phi(phi_min, phi_max):
    ok_phi_min = TCut("genParticle.phi > %f"%(phi_min))
    ok_phi_max = TCut("genParticle.phi < %f"%(phi_max))
    ok_phi = AND(ok_phi_min,ok_phi_max)
    return ok_phi

## csc simhits & digis
def ok_csc_sh(st):
    return TCut("cscSimHit.has_csc_sh_even[%d] || cscSimHit.has_csc_sh_odd[%d]"%(st,st))

def ok_csc_strip(st):
    return TCut("cscDigi.has_csc_strips_even[%d] || cscDigi.has_csc_strips_odd[%d]"%(st,st))

def ok_csc_wire(st):
    return TCut("cscDigi.has_csc_wires_even[%d] || cscDigi.has_csc_wires_odd[%d]"%(st,st))

def ok_csc_digi(st):
    return AND(ok_csc_strip(st), ok_csc_wire(st))

## csc stub
def ok_csc_lct(st):
    return TCut("(cscStub.has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==3) || (cscStub.has_lct_odd[%d] && cscStub.matchtype_lct_odd[%d]==3)"%(st,st,st,st))

def ok_csc_lct_loose(st):
    return TCut("cscStub.has_lct_even[%d] || cscStub.has_lct_odd[%d]"%(st,st))

def ok_csc_alct(st):
    return TCut("cscStub.has_alct_even[%d] || cscStub.has_alct_odd[%d]"%(st,st))

def ok_csc_clct(st):
    return TCut("cscStub.has_clct_even[%d] || cscStub.has_clct_odd[%d]"%(st,st))

def delta_ffhs_clct(st):
    return "max(cscStub.delta_ffhs_clct_even[%d], cscStub.delta_ffhs_clct_odd[%d])"%(st,st)

def delta_fhs_clct(st):
    return "max(cscStub.delta_fhs_clct_even[%d], cscStub.delta_fhs_clct_odd[%d])"%(st,st)

def delta_fqs_clct(st):
    return "max(cscStub.delta_fqs_clct_even[%d], cscStub.delta_fqs_clct_odd[%d])"%(st,st)

def delta_fes_clct(st):
    return "max(cscStub.delta_fes_clct_even[%d], cscStub.delta_fes_clct_odd[%d])"%(st,st)

def delta_bend_clct(st):
    return "max(cscStub.dslope_clct_odd[%d], cscStub.dslope_clct_even[%d])"%(st,st)

def slope_clct(st, even):
    if even:
        return "cscStub.slope_clct_even[%d]"%(st)
    else:
        return "cscStub.slope_clct_odd[%d]"%(st)

def slope_clct_bend(st, even):
    if even:
        return "(cscStub.slope_clct_even[%d] * cscStub.endcap_clct_even[%d])"%(st,st)
    else:
        return "(cscStub.slope_clct_odd[%d] * cscStub.endcap_clct_odd[%d])"%(st,st)

def ok_pattern(st, ipat):
    c1 = TCut("cscStub.pattern_clct_even[%d] >= %d"%(st,ipat))
    c2 = TCut("cscStub.pattern_clct_odd[%d] >= %d"%(st,ipat))
    return OR(c1,c2)

ok_me11 = ok_csc_lct(0)
ok_me12 = ok_csc_lct(3)
ok_me13 = ok_csc_lct(4)

ok_me21 = ok_csc_lct(5)
ok_me22 = ok_csc_lct(6)
ok_me31 = ok_csc_lct(7)

ok_me32 = ok_csc_lct(8)
ok_me41 = ok_csc_lct(9)
ok_me42 = ok_csc_lct(10)

ok_me1 = OR(ok_me11, ok_me12, ok_me13)
ok_me2 = OR(ok_me21, ok_me22)
ok_me3 = OR(ok_me31, ok_me32)
ok_me4 = OR(ok_me41, ok_me42)

def ok_2_csc_lcts():
    ok_me1_me2 = AND(ok_me1, ok_me2)
    ok_me1_me3 = AND(ok_me1, ok_me3)
    ok_me1_me4 = AND(ok_me1, ok_me4)
    ok_me2_me3 = AND(ok_me2, ok_me3)
    ok_me2_me4 = AND(ok_me2, ok_me4)
    ok_me3_me4 = AND(ok_me3, ok_me4)

    return OR(ok_me1_me2, ok_me1_me3, ok_me1_me4, ok_me2_me3, ok_me2_me4, ok_me3_me4)

def ok_3_csc_lcts():
    ok_me1_me2_me3 = AND(ok_me1, ok_me2, ok_me3)
    ok_me1_me2_me4 = AND(ok_me1, ok_me2, ok_me4)
    ok_me1_me3_me4 = AND(ok_me1, ok_me3, ok_me4)
    ok_me2_me3_me4 = AND(ok_me2, ok_me3, ok_me4)

    return OR(ok_me1_me2_me3, ok_me1_me2_me4, ok_me1_me3_me4, ok_me2_me3_me4)

def ok_4_csc_lcts():
    return AND(ok_me1, ok_me2, ok_me3, ok_me4)

def ok_csc_lcts(n_min = 2):
    if n_min == 2:
        return ok_2_csc_lcts()
    elif n_min == 3:
        return ok_3_csc_lcts()
    elif n_min == 4:
        return ok_4_csc_lcts()
    else:
        return ok_2_csc_lcts()


ok_me11_sh = ok_csc_sh(0)
ok_me12_sh = ok_csc_sh(3)
ok_me13_sh = ok_csc_sh(4)

ok_me21_sh = ok_csc_sh(5)
ok_me22_sh = ok_csc_sh(6)
ok_me31_sh = ok_csc_sh(7)

ok_me32_sh = ok_csc_sh(8)
ok_me41_sh = ok_csc_sh(9)
ok_me42_sh = ok_csc_sh(10)

ok_me1_sh = OR(ok_me11_sh, ok_me12_sh, ok_me13_sh)
ok_me2_sh = OR(ok_me21_sh, ok_me22_sh)
ok_me3_sh = OR(ok_me31_sh, ok_me32_sh)
ok_me4_sh = OR(ok_me41_sh, ok_me42_sh)

def ok_2_csc_sh():
    ok_me1_me2 = AND(ok_me1_sh, ok_me2_sh)
    ok_me1_me3 = AND(ok_me1_sh, ok_me3_sh)
    ok_me1_me4 = AND(ok_me1_sh, ok_me4_sh)
    ok_me2_me3 = AND(ok_me2_sh, ok_me3_sh)
    ok_me2_me4 = AND(ok_me2_sh, ok_me4_sh)
    ok_me3_me4 = AND(ok_me3_sh, ok_me4_sh)

    return OR(ok_me1_me2, ok_me1_me3, ok_me1_me4, ok_me2_me3, ok_me2_me4, ok_me3_me4)

def ok_3_csc_sh():
    ok_me1_me2_me3 = AND(ok_me1_sh, ok_me2_sh, ok_me3_sh)
    ok_me1_me2_me4 = AND(ok_me1_sh, ok_me2_sh, ok_me4_sh)
    ok_me1_me3_me4 = AND(ok_me1_sh, ok_me3_sh, ok_me4_sh)
    ok_me2_me3_me4 = AND(ok_me2_sh, ok_me3_sh, ok_me4_sh)

    return OR(ok_me1_me2_me3, ok_me1_me2_me4, ok_me1_me3_me4, ok_me2_me3_me4)

def ok_4_csc_sh():
    return AND(ok_me1_sh, ok_me2_sh, ok_me3_sh, ok_me4_sh)

def ok_csc_sh_track(n_min = 2):
    if n_min == 2:
        return ok_2_csc_sh()
    elif n_min == 3:
        return ok_3_csc_sh()
    elif n_min == 4:
        return ok_4_csc_sh()
    else:
        return ok_2_csc_sh()


## GEM simhit
def ok_gem_sh(st):
    return TCut("gemSimHit.has_gem_sh_even[%d] || gemSimHit.has_gem_sh_odd[%d]"%(st,st))

def ok_gem_sh2(st):
    return TCut("gemSimHit.has_gem_sh2_even[%d] || gemSimHit.has_gem_sh2_odd[%d]"%(st,st))

## gem digi
def ok_gem_dg(st):
    return TCut("gemDigi.has_gem_dg_even[%d] || gemDigi.has_gem_dg_odd[%d]"%(st,st))

def ok_gem_dg2(st):
    return TCut("gemDigi.has_gem_dg2_even[%d] || gemDigi.has_gem_dg2_odd[%d]"%(st,st))

## gem stub
def ok_gem_pad(st):
    return TCut("gemStub.has_gem_pad_even[%d] || gemStub.has_gem_pad_odd[%d]"%(st,st))

def ok_gem_pad2(st):
    return TCut("gemStub.has_gem_pad2_even[%d] || gemStub.has_gem_pad2_odd[%d]"%(st,st))

def ok_gem_copad(st):
    return TCut("gemStub.has_gem_copad_even[%d] || gemStub.has_gem_copad_odd[%d]"%(st,st))

def ok_gem_cluster(st):
    return TCut("gemStub.has_gem_cluster_even[%d] || gemStub.has_gem_cluster_odd[%d]"%(st,st))

def dphi_pad1(st):
    return "max(gemStub.dphi_pad1_even[%d], gemStub.dphi_pad1_odd[%d])"%(st,st)

## l1Mu
def ok_emtf(pt):
    return TCut("l1Mu.has_emtfTrack && l1Mu.emtf_pt >= %f"%(pt))

def ok_emtfcand():
    return TCut("l1Mu.has_emtfRegCand")

def ok_l1mu():
    return TCut("l1Mu.has_gmtCand")

## l1Track
def ok_l1track(pt):
    return TCut("l1Track.has_l1Track && l1Track.l1Track_pt >= %f"%(pt))

def ok_l1trackmuon(pt):
    return TCut("l1Track.has_l1TrackMuon && l1Track.l1TrackMuon_pt >= %f"%(pt))
