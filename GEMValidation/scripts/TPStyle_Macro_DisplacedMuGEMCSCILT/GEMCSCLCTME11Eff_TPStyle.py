import ROOT
import array
#verts=array.array('i',(0,)*1000)
import numpy as np
from samples import *
#myarray = np.asarray(mylist)
ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

ROOT.gStyle.SetOptStat(0)
ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.04)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)

c1 = ROOT.TCanvas()
c1.SetGridx()
c1.SetGridy()
c1.SetTickx()
c1.SetTicky()

def getEff(file,dir,den,num):
    f = ROOT.TFile(file)
    t = f.Get(dir)
    xbins=[0.0 for x in range(32)]
    xbins[0] = float(1.650)
    xbins[1] = float(1.725)
    for x in range(1,31):
	xbins[x+1] = float(xbins[x]+0.025)
	print "x ",x, " xbins  ",xbins[x]
    xbins[31] = 2.45
    print "x  32  xbins", xbins[31]
    mybins = np.asarray(xbins)
    print "mybins ", mybins, mybins.dtype
    h1 = ROOT.TH1F("h1","h1",len(mybins)-1,mybins)
    t.Draw("abs(-eta)>> h1",den)
    h2 = ROOT.TH1F("h2","h2",len(mybins)-1,mybins)
    t.Draw("abs(-eta)>> h2",num)
    e = ROOT.TEfficiency(h2,h1)
    return e

b1 = ROOT.TH1F("b1","b1",36,1.6,2.5)
b1.GetYaxis().SetRangeUser(0.50,1.06)
b1.GetYaxis().SetNdivisions(520)
b1.GetYaxis().SetTitle("Efficiency")
b1.GetYaxis().SetTitleSize(0.05)
b1.GetXaxis().SetTitle("Simulated muon #eta")
b1.GetXaxis().SetTitleSize(0.05)
b1.SetTitle(" "*10 +"YE1/1 stub reconstruction"+" "*30 + "PU = 0, 14 TeV")
b1.SetStats(0)

treename = "GEMCSCAnalyzer/trk_eff_ME11"
treenames = ["GEMCSCAnalyzer/trk_eff_ME11","GEMCSCAnalyzer/trk_eff_ME21","GEMCSCAnalyzer/trk_eff_ME31","GEMCSCAnalyzer/trk_eff_ME41"]
den = "has_csc_sh>0 && pt >10"
num = "has_csc_sh>0 && has_lct>0 && pt>10"

e1 = getEff(loc+mGammaD_0400_ctau_00,treename,den,num)
e2 = getEff(loc+mGammaD_0400_ctau_5,treename,den,num)
e3 = getEff(loc+mGammaD_0400_ctau_10,treename,den,num)
e4 = getEff(loc+mGammaD_0400_ctau_20,treename,den,num)
e5 = getEff(loc+mGammaD_0400_ctau_50,treename,den,num)
e6 = getEff(loc+mGammaD_0400_ctau_100,treename,den,num)

e7 = getEff(loc+mGammaD_20000_ctau_00,treename,den,num)
e8 = getEff(loc+mGammaD_20000_ctau_10,treename,den,num)
e9 = getEff(loc+mGammaD_20000_ctau_100,treename,den,num)
e10 = getEff(loc+mGammaD_20000_ctau_1000,treename,den,num)

e1_PU140 = getEff(loc+mGammaD_0400_ctau_00_PU140,treename,den,num)
e2_PU140 = getEff(loc+mGammaD_0400_ctau_5_PU140,treename,den,num)
e3_PU140 = getEff(loc+mGammaD_0400_ctau_10_PU140,treename,den,num)
e4_PU140 = getEff(loc+mGammaD_0400_ctau_20_PU140,treename,den,num)
e5_PU140 = getEff(loc+mGammaD_0400_ctau_50_PU140,treename,den,num)
e6_PU140 = getEff(loc+mGammaD_0400_ctau_100_PU140,treename,den,num)

e7_PU140 = getEff(loc+mGammaD_20000_ctau_00_PU140,treename,den,num)
e8_PU140 = getEff(loc+mGammaD_20000_ctau_10_PU140,treename,den,num)
e9_PU140 = getEff(loc+mGammaD_20000_ctau_100_PU140,treename,den,num)
e10_PU140 = getEff(loc+mGammaD_20000_ctau_1000_PU140,treename,den,num)

e1.SetLineColor(ROOT.kRed)
e2.SetLineColor(ROOT.kBlue-9)
e3.SetLineColor(ROOT.kBlue-8)
e4.SetLineColor(ROOT.kBlue-7)
e5.SetLineColor(ROOT.kBlue-6)
e6.SetLineColor(ROOT.kBlue)

e7.SetLineColor(ROOT.kOrange)
e8.SetLineColor(ROOT.kOrange+1)
e9.SetLineColor(ROOT.kOrange+2)
e10.SetLineColor(ROOT.kOrange+3)

b1.Draw("")
e7.Draw("same")
"""
e2.Draw("same")
e3.Draw("same")
e4.Draw("same")
e5.Draw("same")
"""
e10.Draw("same")
#e1.Draw("e3same")
#e6.Draw("same")
#e7.Draw("same")

legend = ROOT.TLegend(0.20,0.15,.89,0.32, "", "brNDC")
legend.SetBorderSize(0)
#legend.SetFillStyle(0)
legend.SetFillColor(ROOT.kWhite)
#legend.SetTextSize(0.05)
#legend.SetHeader(" "*5+"PU140 @ 25 nsec")
#legend.AddEntry(e1,"CSC SLHC Algorithm (4 hits)","l")
#legend.AddEntry(e2,"CSC only SLHC Algorithm in 2019","l")
legend.AddEntry(e1,"0 mm","L")
#legend.AddEntry(e3,"GEM-CSC Algorithm in 2023 Jason","l")
"""
legend.AddEntry(e2,"5 mm","L")
legend.AddEntry(e3,"10 mm","L")
legend.AddEntry(e4,"20 mm","L")
legend.AddEntry(e5,"50 mm","L")
"""
legend.AddEntry(e6,"100 mm","L")

#legend.AddEntry(e1,"Phase II detector:potentially achievable","f")
#legend.AddEntry(e6,"GEM-CSC Algorithm (step 3)","l")
#legend.AddEntry(e7,"GEM-CSC Algorithm (step 4)","l")
legend.Draw("same")

tex = ROOT.TLatex(0.7,0.6,"p_{T}>10GeV")
tex.SetTextSize(0.05)
tex.SetNDC()
tex.Draw("same")

t = ROOT.gPad.GetTopMargin()
cmsTextFont = 61
cmsTextSize = 0.75
tex1 = ROOT.TLatex(0.60,0.88,"CMS PhaseII Simulation")
tex1.SetTextSize(cmsTextSize*t)
tex1.SetTextFont(cmsTextFont)
tex1.SetNDC()
tex1.Draw("same")

c1.SaveAs("LCT_80k_ME11_reco_eff_com_TP_tdr2.png")
