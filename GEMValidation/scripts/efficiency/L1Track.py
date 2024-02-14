from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
yTitle = "Efficiency"
subdirectory = "efficiency/L1Mu/"

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12


def L1TrackMuonPt(plotter):

    ## variables for the plot
    topTitle = ""
    xTitle = "Generated muon p_{T} [GeV]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = genpt

    h_bins = "(20,0,100)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(plotter.yMax)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

    denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_l1trackmuon(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_l1trackmuon(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_l1trackmuon(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_l1trackmuon(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_l1trackmuon(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("L1TrackMuon")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");

    c.Print("%sEff_L1TrackMuon_pt_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5


def L1TrackMuonEta(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(%s)"%(geneta)

    h_bins = "(20,1.2,2.4)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(plotter.yMax)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

    denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>20")), ok_l1trackmuon(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>15")), ok_l1trackmuon(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>10")), ok_l1trackmuon(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>5")), ok_l1trackmuon(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>3")), ok_l1trackmuon(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("L1TrackMuon")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");

    c.Print("%sEff_L1TrackMuon_eta_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5


def L1TrackMuonPhi(plotter):

    ## variables for the plot
    xTitle = "Generated muon #phi"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = genphi

    h_bins = "(20,-3.2,3.2)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(plotter.yMax)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

    denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>20")), ok_l1trackmuon(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>15")), ok_l1trackmuon(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>10")), ok_l1trackmuon(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>5")), ok_l1trackmuon(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>3")), ok_l1trackmuon(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("L1TrackMuon")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");

    c.Print("%sEff_L1TrackMuon_phi_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5

def L1Track(plotter):
    L1TrackMuonPt(plotter)
    L1TrackMuonEta(plotter)
    L1TrackMuonPhi(plotter)
