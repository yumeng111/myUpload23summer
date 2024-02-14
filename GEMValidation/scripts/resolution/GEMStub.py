## position resolution for GEM pads

from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
xTitle = "#Phi_{L1T} - #Phi_{SIM} [rad]"
yTitle = "Entries"
subdirectory = "resolution/GEMStub/"
title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def GEMPadDigi(plotter):

    for st in range(0,len(gemStations)):

        h_bins = "(50,-0.0015,0.0015)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.06)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_1D(plotter.tree, title, h_bins, dphi_pad1(st), "", "same", kBlue)
        h1.Scale(1./h1.GetEntries())
        h1.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "GEM pad","pl")
        leg.Draw("same");

        gem = drawCSCLabel(gemStations[st].label, 0.85,0.85,0.05)

        c.Print("%sRes_GEMPadDigi_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del base, leg, gem, h1, c


def GEMStub(plotter):
    GEMPadDigi(plotter)
