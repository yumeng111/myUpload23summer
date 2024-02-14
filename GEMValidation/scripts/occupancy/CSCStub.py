from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, TH2F, gPad

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
from array import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas, newCanvas2D
import numpy

topTitle = ""
xTitle = "Strip_{L1T} - Strip_{SIM}"
yTitle = "Entries"
subdirectory = "occupancy/CSCStub/"
title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def CLCTPattern(plotter, text):

    for st in range(0,len(cscStations)):

        h_bins = "(100,0,1.5)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        toPlot1 = delta_fhs_clct(st)
        toPlot2 = delta_fqs_clct(st)
        toPlot3 = delta_fes_clct(st)

        h1 = draw_1D(plotter.tree, title, h_bins, toPlot1, "", "same", kBlue)
        h2 = draw_1D(plotter.tree, title, h_bins, toPlot2, "", "same", kGreen+2)
        h3 = draw_1D(plotter.tree, title, h_bins, toPlot3, "", "same", kRed+1)

        h1.Scale(1./h1.GetEntries())
        h2.Scale(1./h2.GetEntries())
        h3.Scale(1./h3.GetEntries())
        h1.Draw("histsame")
        h2.Draw("histsame")
        h3.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "1/2 strip","pl")
        leg.AddEntry(h2, "1/4 strip","pl")
        leg.AddEntry(h3, "1/8 strip","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        c.Print("%sOcc_CSCCLCT_pos_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h2, leg, csc, h1, h3

def CSCFloatSlope(plotter, text, even):

    xTitle = "True muon p_{T} * charge [GeV]"
    yTitle = "CLCT Slope * endcap [Half-strips/layer]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        h_bins = "(100,-50,50)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        h_binsY = "(100,-2.5,2.5)"
        nBinsY = int(h_binsY[1:-1].split(',')[0])
        minBinY = float(h_binsY[1:-1].split(',')[1])
        maxBinY = float(h_binsY[1:-1].split(',')[2])

        toPlot1 = "%s : (simTrack.pt * simTrack.charge)"%(slope_clct_bend(st, even))

        c = newCanvas2D()
        gPad.SetLogz()
        base = draw_2D(plotter.tree, title, h_bins, h_binsY, toPlot1, "", "COLZ")
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        if even:
            c.Print("%sOcc_CSCCLCT_floatslope_even_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        else:
            c.Print("%sOcc_CSCCLCT_floatslope_odd_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, csc

def CSCFloatSlopeV2(plotter, text, even):

    xTitle = "True muon p_{T} * charge [GeV]"
    yTitle = "CLCT Slope * endcap [Half-strips/layer]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        h_bins = "(100,-50,50)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        '''
        bins with size 1/16-strip / layer
        1 bin with size 1/8-strip / layer
        1 (overflow) bin with size 1/4-strip / layer
        '''
        h_binsY = [-2.5, -2.0, -1.75, -1.625, -1.5,
                   -1.375, -1.25, -1.125, -1.0,
                   -0.875, -0.75, -0.625, -0.5,
                    -0.375, -0.25, -0.125,
                   0.0,
                   0.125, 0.25, 0.375,
                   0.5, 0.625, 0.75, 0.875,
                   1.0, 1.125, 1.25, 1.375,
                   1.5, 1.625, 1.75, 2.0, 2.5]

        toPlot1 = "%s : (simTrack.pt *  simTrack.charge)"%(slope_clct_bend(st, even))

        c = newCanvas2D()
        gPad.SetLogz()
        base = draw_2Dbis(plotter.tree, title, h_bins, 32, array('d',h_binsY), toPlot1, "", "COLZ")
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        if even:
            c.Print("%sOcc_CSCCLCT_floatslope_even_final_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        else:
            c.Print("%sOcc_CSCCLCT_floatslope_odd_final_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, csc



def CSCStub(plotter, text):
    #CLCTPattern(plotter)
    CSCFloatSlope(plotter, text, True)
    CSCFloatSlope(plotter, text, False)
    CSCFloatSlopeV2(plotter, text, True)
    CSCFloatSlopeV2(plotter, text, False)
