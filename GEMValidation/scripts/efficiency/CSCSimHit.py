from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
yTitle = "Efficiency"
subdirectory = "efficiency/CSCSimHit/"

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def CSCSimHitEta(plotter):

    ## variables for the plot

    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(cscStations[st].eta_min, cscStations[st].eta_max), ok_csc_sh(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "SimHit","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCSimHit_eta_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc


def CSCSimHitPhi(plotter):

    ## variables for the plot

    xTitle = "Generated muon #phi [rad]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "phi"


    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].phi_min,cscStations[st].phi_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(cscStations[st].eta_min, cscStations[st].eta_max), ok_csc_sh(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "SimHit","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCSimHit_phi_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc


def CSCSimHitL(plotter):

    ## variables for the plot

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"


    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(cscStations[st].l_min, cscStations[st].l_max), ok_csc_sh(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "SimHit","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCSimHit_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc
        


def CSCSimHit(plotter):
    CSCSimHitEta(plotter)
    CSCSimHitPhi(plotter)
    CSCSimHitL(plotter)
