## position resolution

## bending resolution CSC-only

## bending resolution GEM-CSC

from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack, kMagenta, gPad,THStack

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
xTitle = "Strip_{L1T} - Strip_{SIM}"
yTitle = "Normalized"
subdirectory = "resolution/CSCStub/"
title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

setTDRStyle()

iPeriod = 0# use -1 to hide PU
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def CSCCNDigis(plotter, st, toPlot1, h_bins, xtitle, cuts, suffix, text):


        #h_bins = "(100,-1,1)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        base  = TH1F("base","base",nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.GetXaxis().SetTitle(xtitle)
        base.GetYaxis().SetTitle("Events")
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        #toPlot1 = delta_fhs_clct(st)
        h1 = draw_1D(plotter.tree, title, h_bins, toPlot1, cuts, "same", kBlue)

        #h1.Scale(1./h1.GetEntries())
        base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        h1.Draw("histsame")

        #leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        #leg.SetBorderSize(0)
        #leg.SetFillStyle(0)
        #leg.SetTextSize(0.05)
        #leg.AddEntry(h1, "","pl")
        #leg.Draw("same");

        txt = drawCSCLabel(text, 0.15,0.25,0.035)
        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sCSC_NDigis_%s_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  suffix, plotter.ext))

        del base, csc, h1, c, txt


def makeNDigis(plotter, text):
    ndigisbins = "(50, 0, 50)"
    xtitle_wire = "nwiredigis"
    xtitle_comp = "ncomparatordigis"
    xtitle_strip = "nstripdigis"
    for st in range(0,len(cscStations)):
        nwiredigi = "max(cscDigi.nwires_dg_even[%d], cscDigi.nwires_dg_odd[%d])"%(st,st)
        ncompdigi = "max(cscDigi.ncomparators_dg_even[%d], cscDigi.ncomparators_dg_odd[%d])"%(st,st)
        nstripdigi = "max(cscDigi.nstrips_dg_even[%d], cscDigi.nstrips_dg_odd[%d])"%(st,st)
        CSCCNDigis(plotter, st, nwiredigi, ndigisbins, xtitle_wire, nwiredigi+">0.5", xtitle_wire, text)
        CSCCNDigis(plotter, st, ncompdigi, ndigisbins, xtitle_comp, ncompdigi+">0.5", xtitle_comp, text)
        CSCCNDigis(plotter, st, nstripdigi, ndigisbins, xtitle_strip, nstripdigi+">0.5", xtitle_strip, text)

def CSCCLCTPos1(plotter):

    for st in range(0,len(cscStations)):

        h_bins = "(100,-1,1)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
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
        h1 = draw_1D(plotter.tree, title, h_bins, toPlot1, "", "same", kBlue)

        h1.Scale(1./h1.GetEntries())
        base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.5)
        h1.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "1/2 strip","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sRes_CSCCLCT_pos1_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del base, leg, csc, h1, c


def CSCCLCTPos(plotter):

    for st in range(0,len(cscStations)):

        h_bins = "(100,-1,1)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
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
        base.SetMaximum(h3.GetBinContent(h3.GetMaximumBin()) * 1.5)
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

        c.Print("%sRes_CSCCLCT_pos_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del base, h2, leg, csc, h1, h3, c


def CSCCLCTBend(plotter):

    xTitle = "Slope_{L1T} - Slope_{SIM} [Strips/layer]"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        h_bins = "(50,-0.5,0.5)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,2)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        toPlot1 = delta_bend_clct(st)

        h1 = draw_1D(plotter.tree, title, h_bins, toPlot1, "", "same", kBlue)

        h1.Scale(1./h1.GetEntries())
        base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.5)
        h1.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "CLCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sRes_CSCCLCT_bend_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1

def CSCDigiAveragedBX(plotter, text):

    xTitle = "Averaged BX"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        h_bins = "(32,3.5,11.5)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        #toPlot1 = delta_bend_clct(st)
        compbx = "max(cscDigi.ncompsbx_dg_even[%d],cscDigi.ncompsbx_dg_odd[%d])+1"%(st, st)
        wirebx = "max(cscDigi.nwiresbx_dg_even[%d],cscDigi.nwiresbx_dg_odd[%d])"%(st, st)


        h1 = draw_1D(plotter.tree, title, h_bins, wirebx, ok_csc_wire(st), "same", kBlue)
        h2 = draw_1D(plotter.tree, title, h_bins, compbx, ok_csc_strip(st), "same", kRed)

        h1.Scale(1./h1.GetEntries())
        h2.Scale(1./h2.GetEntries())
        #base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        base.SetMaximum(0.8)
        h1.Draw("histsame")
        h2.Draw("histsame")

        leg = TLegend(0.15,0.7,.45,0.8, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        leg.AddEntry(h1, "Matched Wire digis","pl")
        leg.AddEntry(h2, "Matched Comparator digis","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCDigi_AveragedBX_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1, txt

def CSCDigiAveragedBXDiff(plotter, text):

    xTitle = "Averaged BX Difference"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    h_bins = "(14, -3.5, 3.5)"
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        gStyle.SetPaintTextFormat("1.3f");
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        bxdiff_even = "cscDigi.nwiresbx_dg_even[%d]-cscDigi.ncompsbx_dg_even[%d]-1"%(st, st)
        bxdiff_odd  = "cscDigi.nwiresbx_dg_odd[%d] -cscDigi.ncompsbx_dg_odd[%d]-1"%(st,st)
        todraw = "(cscDigi.has_csc_strips_even[%d] && cscDigi.has_csc_wires_even[%d] ? (%s) : (%s))"%(st, st,bxdiff_even,bxdiff_odd)
        cuts = "(cscDigi.has_csc_strips_even[%d] && cscDigi.has_csc_wires_even[%d]) || (cscDigi.has_csc_strips_odd[%d] && cscDigi.has_csc_wires_odd[%d])"%(st,st,st,st)

        h1 = draw_1D(plotter.tree, title, h_bins, todraw, cuts, "same", kGreen+2)

        h1.Scale(1./h1.GetEntries())
        h1.SetMarkerSize(1.8)
        #base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        base.SetMaximum(0.8)
        h1.Draw("histtextsame")

        leg = TLegend(0.15,0.7,.50,0.8, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.035)
        #leg.SetTextFont(42)
        leg.AddEntry(h1, "Matched Wire digis - Matched Comparator digis","l")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.04)

        c.Print("%sRes_CSCDigi_Diff_AveragedBX_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1, txt

def CSCDigiAveragedBXNHits2D(plotter, text):
    xTitle = "Averaged BX of matched comparator digis"
    yTitle = "Total comparator digis"
    xTitlea = "Averaged BX of matched wire digis"
    yTitlea = "Total wire digis"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    titlea = "%s;%s;%s"%(topTitle,xTitlea,yTitlea)
    xbins = "(7, 4.5, 11.5)"
    ybins  = "(7, 4, 32)"
    xnBins  = int(xbins[1:-1].split(',')[0])
    ynBins  = int(ybins[1:-1].split(',')[0])

    gStyle.SetPaintTextFormat("1.0f");
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)

        compsbx = "(cscDigi.ncompsbx_dg_even[%d] > cscDigi.ncompsbx_dg_odd[%d] ? cscDigi.ncompsbx_dg_even[%d]+1 : cscDigi.ncompsbx_dg_odd[%d]+1)"%(st,st,st,st)
        wiresbx = "(cscDigi.nwiresbx_dg_even[%d] > cscDigi.nwiresbx_dg_odd[%d] ? cscDigi.nwiresbx_dg_even[%d] : cscDigi.nwiresbx_dg_odd[%d])"%(st,st,st,st)
        ytodraw = "(cscDigi.ncompsbx_dg_even[%d] > cscDigi.ncompsbx_dg_odd[%d] ? cscDigi.totalcomparators_dg_even[%d] : cscDigi.totalcomparators_dg_odd[%d])"%(st,st,st,st)
        ytodrawa = "(cscDigi.nwiresbx_dg_even[%d] > cscDigi.nwiresbx_dg_odd[%d] ? cscDigi.totalwires_dg_even[%d] : cscDigi.totalwires_dg_odd[%d])"%(st,st,st,st)
        cuts = ok_csc_digi(st)


        hist = draw_2D(plotter.tree, title, xbins, ybins, "%s:%s"%(ytodraw, compsbx), ok_csc_strip(st), "")
        histA = draw_2D(plotter.tree, titlea, xbins, ybins, "%s:%s"%(ytodrawa, wiresbx), ok_csc_wire(st), "")
        for i in range(xnBins):
            total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
            hist.SetBinContent(i+1, ynBins, total)
            totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
            histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        if max(xnBins, ynBins) <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCComparatorDigi_AveragedBX_vs_ncomparators_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))


        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        histA.SetMarkerColor(kRed)
        histA.SetMarkerSize(1.8)
        histA.Draw(drawopt)
        histA.GetXaxis().SetLabelSize(0.04)
        histA.GetYaxis().SetLabelSize(0.04)
        histA.GetXaxis().SetTitleSize(0.04)
        histA.GetYaxis().SetTitleSize(0.04)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCWireDigi_AveragedBX_vs_nwires_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt



def CSCDigiAveragedBXDiffNHits2D(plotter, text):
    xTitle = "Averaged_Wiredigis_BX- Averaged_Comparatordigis_BX"
    yTitle = "Total comparator digis"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    xbins = "(7, -3.5, 3.5)"
    ybins  = "(7, 4, 32)"
    xnBins  = int(xbins[1:-1].split(',')[0])
    ynBins  = int(ybins[1:-1].split(',')[0])

    gStyle.SetPaintTextFormat("1.0f");
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)

        bxdiff_even = "cscDigi.nwiresbx_dg_even[%d]-cscDigi.ncompsbx_dg_even[%d]-1"%(st, st)
        bxdiff_odd  = "cscDigi.nwiresbx_dg_odd[%d] -cscDigi.ncompsbx_dg_odd[%d]-1"%(st,st)
        xtodraw = "(cscDigi.has_csc_strips_even[%d] && cscDigi.has_csc_wires_even[%d] ? (%s) : (%s))"%(st, st,bxdiff_even,bxdiff_odd)
        ytodraw = "(cscDigi.has_csc_strips_even[%d] ? cscDigi.totalcomparators_dg_even[%d] : cscDigi.totalcomparators_dg_odd[%d])"%(st,st,st)
        cuts = "(cscDigi.has_csc_strips_even[%d] && cscDigi.has_csc_wires_even[%d]) || (cscDigi.has_csc_strips_odd[%d] && cscDigi.has_csc_wires_odd[%d])"%(st,st,st,st)

        hist = draw_2D(plotter.tree, title, xbins, ybins, "%s:%s"%(ytodraw, xtodraw), cuts, "")
        for i in range(xnBins):
            total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
            hist.SetBinContent(i+1, ynBins, total)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        if max(xnBins, ynBins) <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCDigi_wiredigi_comparatordigi_Diff_AveragedBX_vs_ncomparators_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


def CSCLCTMatchType(plotter, text):

    xTitle = "LCT Match Type"
    #yTitle = "Normalized"
    yTitle = "Events"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    h_bins = "(6,-1,5)"
    #h_bins = "(5,0,5)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        matchtype = "max(cscStub.matchtype_lct_even[%d],cscStub.matchtype_lct_odd[%d])"%(st, st)
        matchtype = "(has_lct_even[%d] ? cscStub.matchtype_lct_even[%d] : cscStub.matchtype_lct_odd[%d])"%(st, st,st)
        cscshcut = "has_csc_sh_even[%d] || has_csc_sh_odd[%d]"%(st, st)
        lctcut = "has_lct_even[%d] || (has_lct_odd[%d] && !has_lct_even[%d])"%(st,st,st)
        

        #h1 = draw_1D(plotter.tree, title, h_bins, matchtype, cscshcut, "same", kGreen+2)
        #h1 = draw_1D(plotter.tree, title, h_bins, matchtype, ok_csc_lct_loose(st), "same", kGreen+2)
        h1 = draw_1D(plotter.tree, title, h_bins, matchtype, lctcut, "same", kGreen+2)
        h2 = draw_1D(plotter.tree, title, h_bins, matchtype, AND(ok_csc_sh(st), TCut("!(cscStub.has_lct_even[%d] || cscStub.has_lct_odd[%d])"%(st,st))), "same", kGreen+2)
        #h2.Print("ALL")
        
        h1.SetBinContent(1, h2.GetEntries())##get underflow: aka no LCT match events

        #h1.Scale(1./h1.GetEntries())
        base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        #base.SetMaximum(1.1)
        h1.SetMarkerSize(1.8)
        h1.Draw("histsametext")

        leg = TLegend(0.15,0.7,.45,0.8, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "LCT matched to simtrack","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCLCT_Matchtype_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, h1, txt

def CSCLCTBXComparison(plotter, text):

    #yTitle = "Normalized"
    yTitleclct = "BX of CLCT used in LCT"
    xTitleclct = "BX of CLCT matched simtrack"
    yTitlealct = "BX of ALCT used in LCT"
    xTitlealct = "BX of ALCT matched simtrack"
    titlec = "%s;%s;%s"%(topTitle,xTitleclct,yTitleclct)
    titlea = "%s;%s;%s"%(topTitle,xTitlealct,yTitlealct)

    xbins = "(7, 4.5, 11.5)"
    #xbins = "(20, -9.5, 10.5)"
    xnBins  = int(xbins[1:-1].split(',')[0])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        gStyle.SetOptStat(110010);
        iPeriod = -1

        #toPlot1 = delta_bend_clct(st)


        clctbx = "(has_lct_even[%d] ? cscStub.bx_clct_even[%d]+1 : cscStub.bx_clct_odd[%d]+1)"%(st, st,st)
        alctbx = "(has_lct_even[%d] ? cscStub.bx_alct_even[%d]+5 : cscStub.bx_alct_odd[%d]+5)"%(st, st,st)
        ytodrawc = "(has_lct_even[%d] ?  cscStub.clctbx_lct_even[%d]+1 : cscStub.clctbx_lct_odd[%d]+1)"%(st,st,st)
        ytodrawa = "(has_lct_even[%d] ?  cscStub.alctbx_lct_even[%d]+5 : cscStub.alctbx_lct_odd[%d]+5)"%(st,st,st)
        hist  = draw_2D(plotter.tree, titlec, xbins, xbins, "%s:%s"%(ytodrawc, clctbx), ok_csc_lct_loose(st), "")
        histA = draw_2D(plotter.tree, titlea, xbins, xbins, "%s:%s"%(ytodrawa, alctbx), ok_csc_lct_loose(st), "")
        #for i in range(xnBins):
        #    total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
        #    hist.SetBinContent(i+1, ynBins, total)
        #    totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
        #    histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        #if max(xnBins, ynBins) <= 20:
        if xnBins <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.75,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCLCT_CLCTBXComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        histA.SetMarkerColor(kRed)
        histA.SetMarkerSize(1.8)
        histA.Draw(drawopt)
        histA.GetXaxis().SetLabelSize(0.04)
        histA.GetYaxis().SetLabelSize(0.04)
        histA.GetXaxis().SetTitleSize(0.04)
        histA.GetYaxis().SetTitleSize(0.04)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.75,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCLCT_ALCTBXComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, histA, txt
        gStyle.SetOptStat(0);

def CSCLCTBXComparisonBadMatch(plotter, text):

    #yTitle = "Normalized"
    yTitleclct = "BX of CLCT used in LCT"
    xTitleclct = "BX of CLCT matched simtrack"
    yTitlealct = "BX of ALCT used in LCT"
    xTitlealct = "BX of ALCT matched simtrack"
    titlec = "%s;%s;%s"%(topTitle,xTitleclct,yTitleclct)
    titlea = "%s;%s;%s"%(topTitle,xTitlealct,yTitlealct)

    xbins = "(7, 3.5, 10.5)"
    #xbins = "(20, -9.5, 10.5)"
    xnBins  = int(xbins[1:-1].split(',')[0])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        gStyle.SetOptStat(110010);

        #toPlot1 = delta_bend_clct(st)


        cscshcutc = TCut("(has_lct_even[%d] ? cscStub.matchtype_lct_even[%d]==1 : cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st))
        cscshcuta = TCut("(has_lct_even[%d] ? cscStub.matchtype_lct_even[%d]==2 : cscStub.matchtype_lct_odd[%d]==2)"%(st,st,st))
        clctbx = "(has_lct_even[%d] ? cscStub.bx_clct_even[%d]+1 : cscStub.bx_clct_odd[%d]+1)"%(st, st,st)
        alctbx = "(has_lct_even[%d] ? cscStub.bx_alct_even[%d]+5 : cscStub.bx_alct_odd[%d]+5)"%(st, st,st)
        ytodrawc = "(has_lct_even[%d] ?  cscStub.clctbx_lct_even[%d]+1 : cscStub.clctbx_lct_odd[%d]+1)"%(st,st,st)
        ytodrawa = "(has_lct_even[%d] ?  cscStub.alctbx_lct_even[%d]+5 : cscStub.alctbx_lct_odd[%d]+5)"%(st,st,st)

        ok_csc_lct_loose = TCut("has_lct_even[%d] || (has_lct_odd[%d] && !has_lct_even[%d])"%(st,st,st))
        hist  = draw_2D(plotter.tree, titlec, xbins, xbins, "%s:%s"%(ytodrawc, clctbx), AND(ok_csc_lct_loose, cscshcutc), "")
        histA = draw_2D(plotter.tree, titlea, xbins, xbins, "%s:%s"%(ytodrawa, alctbx), AND(ok_csc_lct_loose, cscshcuta), "")
        #for i in range(xnBins):
        #    total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
        #    hist.SetBinContent(i+1, ynBins, total)
        #    totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
        #    histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        #if max(xnBins, ynBins) <= 20:
        if xnBins <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.75,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTBXComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        histA.SetMarkerColor(kRed)
        histA.SetMarkerSize(1.8)
        histA.Draw(drawopt)
        histA.GetXaxis().SetLabelSize(0.04)
        histA.GetYaxis().SetLabelSize(0.04)
        histA.GetXaxis().SetTitleSize(0.04)
        histA.GetYaxis().SetTitleSize(0.04)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.15,0.75,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCLCTBadMatch_ALCTBXComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, histA, txt
        gStyle.SetOptStat(0);

def CSCCLCTQualityComparisonBadMatch(plotter, text):

    #yTitle = "Normalized"
    yTitleclct = "Quality of CLCT used in LCT"
    xTitleclct = "Quality of CLCT matched simtrack"
    titlec = "%s;%s;%s"%(topTitle,xTitleclct,yTitleclct)

    xbins = "(7, -0.5, 6.5)"
    xnBins  = int(xbins[1:-1].split(',')[0])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        #gStyle.SetOptStat(110010);

        #toPlot1 = delta_bend_clct(st)


        cscshcutc = TCut("(has_lct_even[%d] ? cscStub.matchtype_lct_even[%d]==1 : cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st))
        clctbx = "(has_lct_even[%d] ? cscStub.quality_clct_even[%d] : cscStub.quality_clct_odd[%d])"%(st, st,st)
        ytodrawc = "(has_lct_even[%d] ?  cscStub.clctquality_lct_even[%d] : cscStub.clctquality_lct_odd[%d])"%(st,st,st)

        hist  = draw_2D(plotter.tree, titlec, xbins, xbins, "%s:%s"%(ytodrawc, clctbx), AND(ok_csc_lct_loose(st), cscshcutc), "")
        #for i in range(xnBins):
        #    total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
        #    hist.SetBinContent(i+1, ynBins, total)
        #    totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
        #    histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        #if max(xnBins, ynBins) <= 20:
        if xnBins <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.15,0.75,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTQualityComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


def CSCCLCTPatternComparisonBadMatch(plotter, text):

    #yTitle = "Normalized"
    yTitleclct = "Run2 pattern of CLCT used in LCT"
    xTitleclct = "Run2 pattern of CLCT matched simtrack"
    titlec = "%s;%s;%s"%(topTitle,xTitleclct,yTitleclct)

    xbins = "(11, 0.5, 11.5)"
    xnBins  = int(xbins[1:-1].split(',')[0])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)

        #toPlot1 = delta_bend_clct(st)


        cscshcutc = TCut("(has_lct_even[%d] ? cscStub.matchtype_lct_even[%d]==1 : cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st))
        clctpid   = "(has_lct_even[%d] ? cscStub.pattern_clct_even[%d] : cscStub.pattern_clct_odd[%d])"%(st, st,st)
        ytodrawc = "(has_lct_even[%d] ? cscStub.clctpattern_lct_even[%d] : cscStub.clctpattern_lct_odd[%d])"%(st,st,st)

        hist  = draw_2D(plotter.tree, titlec, xbins, xbins, "%s:%s"%(ytodrawc, clctpid), AND(ok_csc_lct_loose(st), cscshcutc), "")
        #for i in range(xnBins):
        #    total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
        #    hist.SetBinContent(i+1, ynBins, total)
        #    totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
        #    histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        #if max(xnBins, ynBins) <= 20:
        if xnBins <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.15,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTPatternComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


def CSCCLCTHSComparisonBadMatch(plotter, text):

    #yTitle = "Normalized"
    xTitle = "Halfstrip difference of CLCT used in LCT and CLCT matched to simtrack"
    yTitle = "Entries"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    yTitleclct = "Haflstrip of CLCT used in LCT"
    xTitleclct = "Haflstrip of CLCT matched simtrack"
    title2d = "%s;%s;%s"%(topTitle,xTitleclct,yTitleclct)

    xbins = "(80, -40.0, 40.0)"
    nBins  = int(xbins[1:-1].split(',')[0])
    minBin = float(xbins[1:-1].split(',')[1])
    maxBin = float(xbins[1:-1].split(',')[2])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        hsbins = "(80, 0.0, 160.0)"
        if st == 0: hsbins = "(112, 0.0, 224.0)"

        c = newCanvas()
        gPad.SetGrid(1,1)
        #gPad.SetRightMargin(0.15)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        #base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.04)
        base.GetYaxis().SetLabelSize(0.04)
        base.GetXaxis().SetTitleSize(0.04)
        base.GetYaxis().SetTitleSize(0.04)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)


        #toPlot1 = delta_bend_clct(st)


        cscshcutc = TCut("(has_lct_even[%d] && has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1) || (has_lct_odd[%d] && has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st,st,st,st))
        todraw = "((has_clct_even[%d] && has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) ? cscStub.hs_lct_even[%d]-cscStub.hs_clct_even[%d] : cscStub.hs_lct_odd[%d]-cscStub.hs_clct_odd[%d])"%(st,st,st,st,st,st,st)
        #hscut = TCut("abs("+todraw+")>=4")

        #hist  = draw_1D(plotter.tree, title, xbins, todraw, AND(hscut, cscshcutc), "", kGreen+2)
        hist  = draw_1D(plotter.tree, title, xbins, todraw, cscshcutc, "", kGreen+2)
        bin1 = hist.GetBinContent(0)+hist.GetBinContent(1)
        binn = hist.GetBinContent(nBins)+hist.GetBinContent(nBins+1)
        hist.SetBinContent(1, bin1)
        hist.SetBinContent(nBins, binn)
        base.SetMaximum(hist.GetBinContent(hist.GetMaximumBin()) * 1.2)
        hist.Draw("histsame")

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTHalfstripComparison_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        gStyle.SetOptStat(110010);
         
        xtodraw = "((has_clct_even[%d] && has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) ? cscStub.hs_clct_even[%d]: cscStub.hs_clct_odd[%d])"%(st,st,st,st,st)
        ytodraw = "((has_clct_even[%d] && has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) ? cscStub.hs_lct_even[%d]: cscStub.hs_lct_odd[%d])"%(st,st,st,st,st)

        hist2d  = draw_2D(plotter.tree, title2d, hsbins, hsbins, "%s:%s"%(ytodraw, xtodraw), cscshcutc, "")
        hist2d.SetMarkerColor(kRed)
        hist2d.SetMarkerSize(1.8)
        drawopt = "colz"
        hist2d.Draw(drawopt)
        hist2d.GetXaxis().SetLabelSize(0.04)
        hist2d.GetYaxis().SetLabelSize(0.04)
        hist2d.GetXaxis().SetTitleSize(0.04)
        hist2d.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, -1, iPos)

        csc = drawCSCLabel(cscStations[st].label, 0.15,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTHalfstripComparison2D_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist2d, txt
        gStyle.SetOptStat(0);


def CSCCLCTPatternQualityBadMatch(plotter, text):

    #yTitle = "Normalized"
    xTitle = "BX difference of CLCT used in LCT and CLCT matched to simtrack"
    yTitle = "Entries"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    xbins = "(8, -3.5, 4.5)"
    nBins  = int(xbins[1:-1].split(',')[0])
    minBin = float(xbins[1:-1].split(',')[1])
    maxBin = float(xbins[1:-1].split(',')[2])

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue

        c = newCanvas()
        gPad.SetGrid(1,1)
        #gPad.SetRightMargin(0.15)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        #base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.04)
        base.GetYaxis().SetLabelSize(0.04)
        base.GetXaxis().SetTitleSize(0.04)
        base.GetYaxis().SetTitleSize(0.04)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)


        #toPlot1 = delta_bend_clct(st)


        ## case1: good CLCT quality > bad CLCT quality
        ## case2: good CLCT quality==badCLCT quality, run3slope smaller
        ## case4: rest
        case1even = "cscStub.quality_clct_even[%d]>cscStub.clctquality_lct_even[%d]"%(st,st)
        case1odd  = "cscStub.quality_clct_odd[%d]>cscStub.clctquality_lct_odd[%d]"%(st,st)
        case1cut= TCut("(has_lct_even[%d] && has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1 && %s) || (has_lct_odd[%d] && has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1 && %s)"%(st,st,st,case1even, st,st,st, case1odd))
        case2even = "cscStub.quality_clct_even[%d]==cscStub.clctquality_lct_even[%d] && cscStub.run3slope_clct_even[%d]<cscStub.run3slope_lct_even[%d]"%(st,st,st,st)
        case2odd  = "cscStub.quality_clct_odd[%d]==cscStub.clctquality_lct_odd[%d] && cscStub.run3slope_clct_odd[%d]<cscStub.run3slope_lct_odd[%d]"%(st,st,st,st)
        case2cut= TCut("(has_lct_even[%d] && has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1 && %s) || (has_lct_odd[%d] && has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1 && %s)"%(st,st,st,case2even, st,st,st, case2odd))
        case3even = "((cscStub.quality_clct_even[%d]<cscStub.clctquality_lct_even[%d])|| (cscStub.quality_clct_even[%d]==cscStub.clctquality_lct_even[%d] && cscStub.run3slope_clct_even[%d]>=cscStub.run3slope_lct_even[%d]))"%(st,st,st,st,st,st)
        case3odd  = "((cscStub.quality_clct_odd[%d]<cscStub.clctquality_lct_odd[%d])|| (cscStub.quality_clct_odd[%d]==cscStub.clctquality_lct_odd[%d] && cscStub.run3slope_clct_odd[%d]>=cscStub.run3slope_lct_odd[%d]))"%(st,st,st,st,st,st)
        case3cut= TCut("(has_lct_even[%d] && has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1 && %s) || (has_lct_odd[%d] && has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1 && %s)"%(st,st,st,case3even, st,st,st, case3odd))
        todraw = "((has_clct_even[%d] && has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) ? cscStub.clctbx_lct_even[%d]-cscStub.bx_clct_even[%d] : cscStub.clctbx_lct_odd[%d]-cscStub.bx_clct_odd[%d])"%(st,st,st,st,st,st,st)
        todraw1 = "(( has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) ? cscStub.clctbx_lct_even[%d]-cscStub.bx_clct_even[%d] : cscStub.clctbx_lct_odd[%d]-cscStub.bx_clct_odd[%d])"%(st,st,st,st,st,st)
        #hscut = TCut("abs("+todraw+")>=4")
        totcut= TCut("(has_lct_even[%d] && has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1) || (has_lct_odd[%d] && has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st,st,st,st))
        totcut2= TCut("(has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==1) || (has_lct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st,st))
        clctmissingcut= TCut("(has_lct_even[%d] && !has_clct_even[%d] && cscStub.matchtype_lct_even[%d]==1) || (has_lct_odd[%d] && !has_clct_odd[%d] && cscStub.matchtype_lct_odd[%d]==1)"%(st,st,st,st,st,st))
        cscshcutc = TCut("(has_lct_even[%d] ? (cscStub.matchtype_lct_even[%d]==1 && has_clct_even[%d]) : (cscStub.matchtype_lct_odd[%d]==1 && has_clct_odd[%d]))"%(st,st,st,st,st))
        

        hist   = draw_1D(plotter.tree, title, xbins, todraw, case1cut, "", kGreen+2)
        hist2  = draw_1D(plotter.tree, title, xbins, todraw, case2cut, "", kRed)
        #hist3  = draw_1D(plotter.tree, title, xbins, todraw, case3cut, "", kBlue)
        hist4  = draw_1D(plotter.tree, title, xbins, todraw1,cscshcutc, "", kBlack)
        #print("case2 cut ", case2cut)
        #print("case3 cut ", case3cut)
        #print(cscStations[st].label, " Total entrie from hist4 ", hist4.GetEntries(), " missing Good CLCT ", hist5.GetEntries())
        #bin1 = hist4.GetBinContent(0)+hist4.GetBinContent(1)
        #binn = hist4.GetBinContent(nBins)+hist4.GetBinContent(nBins+1)
        #hist4.SetBinContent(1, bin1)
        #hist4.SetBinContent(nBins, binn)
        print("hist ", hist.GetEntries(), " hist2 ", hist2.GetEntries()," hist4 ", hist4.GetEntries())
        hist3 = hist4-hist2-hist
        hist3.SetLineColor(kBlue)

        hist.SetFillColor(kGreen+2)
        hist2.SetFillColor(kRed)
        hist3.SetFillColor(kBlue)

        hs = THStack("all_matchtype1", title)
        hs.Add(hist)
        hs.Add(hist2)
        hs.Add(hist3)
        leg = TLegend(0.15,0.72,.6,0.88, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.035)
        leg.SetTextFont(42)
        totevts = hist4.GetEntries()
        leg.SetHeader("Total events: %d"%(totevts))
        leg.AddEntry(hist,  "Good CLCT with better quality:%d events"%hist.GetEntries(),"pl")
        leg.AddEntry(hist2, "Equal quality but Good CLCT with smaller bending:%d events"%hist2.GetEntries(),"pl")
        leg.AddEntry(hist3, "Rest events in the category:%d events"%hist3.GetEntries(),"pl")


        base.SetMaximum(hs.GetMaximum() * 1.5)
        hs.Draw("same")
        hist4.SetMarkerSize(1.3)
        hist4.Draw("sametext")
        #hist6.Draw("sametext")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCLCTBadMatch_CLCTComparisonForSorting_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        del c, csc, hs, txt
        


def CSCCLCTALCTBX(plotter, text):

    xTitle = "BX"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        h_bins = "(16,-0.5,15.5)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        #toPlot1 = delta_bend_clct(st)
        clctbx = "max(cscStub.bx_clct_even[%d],cscStub.bx_clct_odd[%d])+1"%(st, st)
        alctbx = "max(cscStub.bx_alct_even[%d],cscStub.bx_alct_odd[%d])+5"%(st, st)


        h1 = draw_1D(plotter.tree, title, h_bins, alctbx, ok_csc_alct(st), "same", kBlue)
        h2 = draw_1D(plotter.tree, title, h_bins, clctbx, ok_csc_clct(st), "same", kRed)

        h1.Scale(1./h1.GetEntries())
        h2.Scale(1./h2.GetEntries())
        #base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        base.SetMaximum(1.1)
        h1.Draw("histsame")
        h2.Draw("histsame")

        leg = TLegend(0.15,0.7,.45,0.8, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT","pl")
        leg.AddEntry(h2, "CLCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.2,0.04)

        c.Print("%sRes_CSCALCTCLCT_BX_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1, txt


def CSCCLCTALCTBXDiff(plotter, text):

    xTitle = "BX"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    h_bins = "(7, -3.5, 3.5)"
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        gStyle.SetPaintTextFormat("1.3f");
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        bxdiff_even = "cscStub.bx_alct_even[%d]+4-cscStub.bx_clct_even[%d]"%(st,st)
        bxdiff_odd  = "cscStub.bx_alct_odd[%d]+4-cscStub.bx_clct_odd[%d]"%(st,st)
        todraw = "(cscStub.has_clct_even[%d] && cscStub.has_alct_even[%d] ? (%s) : (%s))"%(st, st,bxdiff_even,bxdiff_odd)
        cuts = "(cscStub.has_clct_even[%d] && cscStub.has_alct_even[%d]) || (cscStub.has_clct_odd[%d] && cscStub.has_alct_odd[%d])"%(st,st,st,st)


        h1 = draw_1D(plotter.tree, title, h_bins, todraw, cuts, "same", kGreen+2)

        h1.Scale(1./h1.GetEntries())
        h1.SetMarkerSize(1.8)
        #base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.2)
        base.SetMaximum(1.1)
        h1.Draw("histtextsame")

        leg = TLegend(0.15,0.7,.45,0.8, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT-CLCT","l")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.04)

        c.Print("%sRes_CSCALCTCLCTDiff_BX_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1, txt

def CSCALCTCLCTBXNHits2D(plotter, text):
    xTitle = "CLCT BX"
    yTitle = "Total Comparator digis"
    xTitlea = "ALCT BX"
    yTitlea = "Total Wire digis"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    titlea = "%s;%s;%s"%(topTitle,xTitlea,yTitlea)
    xbins = "(7, 4.5, 11.5)"
    ybins  = "(7, 4, 32)"
    xnBins  = int(xbins[1:-1].split(',')[0])
    ynBins  = int(ybins[1:-1].split(',')[0])

    gStyle.SetPaintTextFormat("1.0f");
    #gStyle.SetOptStat(110010);
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)

        clctbx = "max(cscStub.bx_clct_even[%d],cscStub.bx_clct_odd[%d])+1"%(st, st)
        alctbx = "max(cscStub.bx_alct_even[%d],cscStub.bx_alct_odd[%d])+5"%(st, st)
        ytodraw = "(cscStub.bx_clct_even[%d] > cscStub.bx_clct_odd[%d] ? cscDigi.totalcomparators_dg_even[%d] : cscDigi.totalcomparators_dg_odd[%d])"%(st,st,st,st)
        ytodrawa = "(cscStub.bx_alct_even[%d] > cscStub.bx_alct_odd[%d] ? cscDigi.totalwires_dg_even[%d] : cscDigi.totalwires_dg_odd[%d])"%(st,st,st,st)
        cuts = AND(ok_csc_clct(st), ok_csc_alct(st))


        hist = draw_2D(plotter.tree, title, xbins, ybins, "%s:%s"%(ytodraw, clctbx), ok_csc_clct(st), "")
        histA = draw_2D(plotter.tree, titlea, xbins, ybins, "%s:%s"%(ytodrawa, alctbx), ok_csc_alct(st), "")
        for i in range(xnBins):
            total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
            hist.SetBinContent(i+1, ynBins, total)
            totala = histA.GetBinContent(i+1, ynBins) + histA.GetBinContent(i+1, ynBins+1)
            histA.SetBinContent(i+1, ynBins, totala)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        if max(xnBins, ynBins) <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCCLCT_BX_vs_ncomparators_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))


        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)
        histA.SetMarkerColor(kRed)
        histA.SetMarkerSize(1.8)
        histA.Draw(drawopt)
        histA.GetXaxis().SetLabelSize(0.04)
        histA.GetYaxis().SetLabelSize(0.04)
        histA.GetXaxis().SetTitleSize(0.04)
        histA.GetYaxis().SetTitleSize(0.04)

        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCALCT_BX_vs_nwires_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt



def CSCALCTCLCTBXDiffNHits2D(plotter, text):
    xTitle = "ALCT-CLCT BX"
    yTitle = "Total Comparator digis"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    xbins = "(7, -3.5, 3.5)"
    ybins  = "(7, 4, 32)"
    xnBins  = int(xbins[1:-1].split(',')[0])
    ynBins  = int(ybins[1:-1].split(',')[0])

    gStyle.SetPaintTextFormat("1.0f");
    #gStyle.SetOptStat(110010);
    for st in range(0,len(cscStations)):

        if st==1 or st==2: continue
        c = newCanvas()
        gPad.SetGrid(1,1)
        gPad.SetRightMargin(0.15)

        clctbx = "max(cscStub.bx_clct_even[%d],cscStub.bx_clct_odd[%d])"%(st, st)
        alctbx = "max(cscStub.bx_alct_even[%d],cscStub.bx_alct_odd[%d])+4"%(st, st)
        xtodraw = alctbx+"-"+clctbx
        bxdiff_even = "cscStub.bx_alct_even[%d]+4-cscStub.bx_clct_even[%d]"%(st,st)
        bxdiff_odd  = "cscStub.bx_alct_odd[%d]+4-cscStub.bx_clct_odd[%d]"%(st,st)
        xtodraw = "(cscStub.has_clct_even[%d] && cscStub.has_alct_even[%d] ? (%s) : (%s))"%(st, st,bxdiff_even,bxdiff_odd)
        cuts = "(cscStub.has_clct_even[%d] && cscStub.has_alct_even[%d]) || (cscStub.has_clct_odd[%d] && cscStub.has_alct_odd[%d])"%(st,st,st,st)
        ytodraw = "(cscStub.has_clct_even[%d] ? cscDigi.totalcomparators_dg_even[%d] : cscDigi.totalcomparators_dg_odd[%d])"%(st,st,st)
        cuts = AND(ok_csc_clct(st), ok_csc_alct(st))


        hist = draw_2D(plotter.tree, title, xbins, ybins, "%s:%s"%(ytodraw, xtodraw), cuts, "")
        for i in range(xnBins):
            total = hist.GetBinContent(i+1, ynBins) + hist.GetBinContent(i+1, ynBins+1)
            hist.SetBinContent(i+1, ynBins, total)
        hist.SetMarkerColor(kRed)
        hist.SetMarkerSize(1.8)
        drawopt = "colz"
        if max(xnBins, ynBins) <= 20:
            drawopt = "colztext"
        hist.Draw(drawopt)
        hist.GetXaxis().SetLabelSize(0.04)
        hist.GetYaxis().SetLabelSize(0.04)
        hist.GetXaxis().SetTitleSize(0.04)
        hist.GetYaxis().SetTitleSize(0.04)
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        txt = drawCSCLabel(text, 0.15,0.75,0.03)

        csc = drawCSCLabel(cscStations[st].label, 0.75,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.8,0.04)

        c.Print("%sRes_CSCALCTCLCTDiff_BX_vs_ncomparators_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, csc, hist, txt


def CSCStub(plotter, text):
    CSCCLCTPos(plotter)
    CSCCLCTPos1(plotter)
    CSCCLCTBend(plotter)
    CSCLCTMatchType(plotter, text)
    CSCLCTBXComparison(plotter, text)
    CSCLCTBXComparisonBadMatch(plotter, text)
    CSCDigiAveragedBX(plotter,text)
    CSCDigiAveragedBXDiff(plotter,text)
    CSCDigiAveragedBXDiffNHits2D(plotter, text)
    CSCDigiAveragedBXNHits2D(plotter, text)
    CSCCLCTALCTBX(plotter,text)
    CSCCLCTALCTBXDiff(plotter,text)
    CSCALCTCLCTBXDiffNHits2D(plotter, text)
    CSCALCTCLCTBXNHits2D(plotter, text)
    CSCCLCTQualityComparisonBadMatch(plotter, text)
    CSCCLCTPatternComparisonBadMatch(plotter, text)
    CSCCLCTHSComparisonBadMatch(plotter, text)

def CSCPosResolutionComparison(plotter, plotter2):

    h11total = []
    h1total = []
    h2total = []
    h3total = []

    for st in range(0,len(cscStations)):

        h_bins = "(100,-1,1)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
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
        toPlot4 = delta_ffhs_clct(st)

        h11 = draw_1D(plotter.tree, title, h_bins, toPlot1, "", "same", kBlack)
        h1 = draw_1D(plotter2.tree, title, h_bins, toPlot1, "", "same", kBlue)
        h2 = draw_1D(plotter2.tree, title, h_bins, toPlot2, "", "same", kGreen+2)
        h3 = draw_1D(plotter2.tree, title, h_bins, toPlot3, "", "same", kRed+1)
#        h4 = draw_1D(plotter2.tree, title, h_bins, toPlot4, "", "same", kOrange)

        h11total.append(h11)
        h1total.append(h1)
        h2total.append(h2)
        h3total.append(h3)

        h11.Scale(1./h11.GetEntries())
        h1.Scale(1./h1.GetEntries())
        h2.Scale(1./h2.GetEntries())
        h3.Scale(1./h3.GetEntries())
#        h4.Scale(1./h4.GetEntries())
        base.SetMaximum(h3.GetBinContent(h3.GetMaximumBin()) * 1.5)
        h11.Draw("histsame")
        h1.Draw("histsame")
        h2.Draw("histsame")
        h3.Draw("histsame")
#        h4.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h11, "1/2 strip (Run-2)","pl")
        leg.AddEntry(h1,  "1/2 strip (Run-3)","pl")
        leg.AddEntry(h2,  "1/4 strip (Run-3)","pl")
        leg.AddEntry(h3,  "1/8 strip (Run-3)","pl")
#        leg.AddEntry(h4,  "True strip (Run-3)","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sRes_CSCCLCT_poscomparison_%s%s"%(plotter2.targetDir + subdirectory, cscStations[st].labelc,  plotter2.ext))

        del base, h2, leg, csc, h1, h3, c, h11


    h_bins = "(100,-1,1)"
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

    h11 = h11total[0]
    for i in range(3,11):
        h11 += h11total[i]

    h1 = h1total[0]
    for i in range(3,11):
        h1 += h1total[i]

    h2 = h2total[0]
    for i in range(3,11):
        h2 += h2total[i]

    h3 = h3total[0]
    for i in range(3,11):
        h3 += h3total[i]

    h11.Scale(1./h11.GetEntries())
    h1.Scale(1./h1.GetEntries())
    h2.Scale(1./h2.GetEntries())
    h3.Scale(1./h3.GetEntries())
    base.SetMaximum(h3.GetBinContent(h3.GetMaximumBin()) * 1.5)
    h11.Draw("histsame")
    h1.Draw("histsame")
    h2.Draw("histsame")
    h3.Draw("histsame")

    print(h11.GetMean(), h11.GetMeanError())
    print(h1.GetMean(), h1.GetMeanError())
    print(h2.GetMean(), h2.GetMeanError())
    print(h2.GetMean(), h3.GetMeanError())

    leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.AddEntry(h11, "1/2 strip (Run-2)","pl")
    leg.AddEntry(h1,  "1/2 strip (Run-3)","pl")
    leg.AddEntry(h2,  "1/4 strip (Run-3)","pl")
    leg.AddEntry(h3,  "1/8 strip (Run-3)","pl")
    leg.Draw("same");

    c.Print("%sRes_CSCCLCT_poscomparison_%s"%(plotter2.targetDir + subdirectory, plotter2.ext))

    del base, h2, leg, h1, h3, c, h11


def CSCBendResolutionComparison(plotter, plotter2):

    xTitle = "Slope_{L1T} - Slope_{SIM} [Strips/layer]"
    yTitle = "Normalized"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    h11total = []
    h1total = []
    h2total = []
    h3total = []

    for st in range(0,len(cscStations)):

        h_bins = "(100,-1,1)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGrid(1,1)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0)
        base.SetMaximum(0.08)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        toPlot1 = delta_bend_clct(st)

        h11 = draw_1D(plotter.tree, title, h_bins, toPlot1, "", "same", kBlack)
        h1 = draw_1D(plotter2.tree, title, h_bins, toPlot1, "", "same", kBlue)
        print(h11.GetEntries(), h1.GetEntries())
        print(h11.GetBinContent(0), h1.GetBinContent(0))
        print(h11.GetBinContent(101), h1.GetBinContent(101))
        h11total.append(h11)
        h1total.append(h1)

        h11.Scale(1./h11.GetEntries())
        h1.Scale(1./h1.GetEntries())

        base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.5)
        h11.Draw("histsame")
        h1.Draw("histsame")

        leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h11, "CLCT (Run-2)","pl")
        leg.AddEntry(h1,  "CLCT (Run-3)","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sRes_CSCCLCT_bendcomparison_%s%s"%(plotter2.targetDir + subdirectory, cscStations[st].labelc,  plotter2.ext))

        del base, leg, csc, h1, c, h11


    h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(0.08)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

    h11 = h11total[0]
    for i in range(3,11):
        h11 += h11total[i]

    h1 = h1total[0]
    for i in range(3,11):
        h1 += h1total[i]

    h11.Scale(1./h11.GetEntries())
    h1.Scale(1./h1.GetEntries())
    base.SetMaximum(h1.GetBinContent(h1.GetMaximumBin()) * 1.5)
    h11.Draw("histsame")
    h1.Draw("histsame")

    print(h11.GetMean(), h11.GetMeanError())
    print(h1.GetMean(), h1.GetMeanError())

    leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.AddEntry(h11, "CLCT (Run-2)","pl")
    leg.AddEntry(h1,  "CLCT (Run-3)","pl")
    leg.Draw("same");

    c.Print("%sRes_CSCCLCT_bendcomparison_%s"%(plotter2.targetDir + subdirectory, plotter2.ext))

    del base, leg, h1, c, h11

def CSCStubComparison1D(plotterlist, variable, varsuffix, h_bins, st, cuts, text):

    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    xTitle = varsuffix
    yTitle = "Events"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(0.08)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    hlist = []
    ymax = 0.0
    for i, plotter in enumerate(plotterlist):
        toPlot1 = variable
        hlist.append(draw_1D(plotter.tree, title, h_bins, toPlot1, cuts, "same", kcolors[i], markers[i]))
        #hlist[-1].Scale(1./hlist[-1].GetEntries())
        hlist[i].SetBinContent(1, hlist[i].GetBinContent(0))
        hlist[i].SetBinContent(nBins, hlist[i].GetBinContent(nBins+1))
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())

    base.SetMaximum(ymax * 1.3)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    leg = TLegend(0.15,0.6,.48,0.9, "", "brNDC");
    #leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.04)
    for i in range(0, len(hlist)):
        integral =  hlist[i].Integral(1, nBins-1)
        hlist[i].Draw("histsame")
        leg.AddEntry(hlist[i], plotterlist[i].legend+": %d"%integral,"pl")
    leg.Draw("same");
    
    csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sCSCStub_compare_%s_%s%s"%(plotterlist[0].targetDir + subdirectory, varsuffix,  cscStations[st].labelc, plotterlist[0].ext))

    del base, leg, hlist, csc, txt, c

def CSCStubComparison1DAll(plotterlist, text):
    ## slope 0-15
    ## Run2 pattern, 0-16
    bins = "(17, 0, 17)"
    qbins = "(8, 0, 8)"
    fslopebins = "(80, -5.0, 5.0)"
    for st in range(0, len(cscStations)):
        cuts = ok_csc_clct(st)
        lctcuts = ok_csc_lct(st)
        alctbx = "max(cscStub.bx_alct_even[%d], cscStub.bx_alct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  alctbx, "ALCT_BX", bins, st, ok_csc_alct(st), text)

        run2pid = "max(cscStub.pattern_clct_even[%d], cscStub.pattern_clct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  run2pid, "CLCT_Run2pattern", bins, st, cuts, text)

        #run3slope = "max(cscStub.run3slope_clct_even[%d], cscStub.run3slope_clct_odd[%d])"%(st,st)
        #CSCStubComparison1D(plotterlist,  run3slope, "CLCT_Run3slope", bins, st, cuts, text):

        slope = "max(cscStub.slope_clct_even[%d], cscStub.slope_clct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  slope, "CLCT_slope", fslopebins, st, cuts, text)

        clctqual = "max(cscStub.quality_clct_even[%d], cscStub.quality_clct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  clctqual, "CLCT_quality", qbins, st, cuts, text)
        clctbx = "max(cscStub.bx_clct_even[%d], cscStub.bx_clct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  clctbx, "CLCT_BX", bins, st, cuts, text)


        lctrun2pid = "max(cscStub.bend_lct_even[%d], cscStub.bend_lct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  lctrun2pid, "LCT_Run2pattern", bins, st, lctcuts, text)
        lctqual = "max(cscStub.quality_lct_even[%d], cscStub.quality_lct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  lctqual, "LCT_quality", bins, st, lctcuts, text)
        lctbx = "max(cscStub.bx_lct_even[%d], cscStub.bx_lct_odd[%d])"%(st,st)
        CSCStubComparison1D(plotterlist,  lctbx, "LCT_BX", bins, st, lctcuts, text)

        toPlot1 = delta_fhs_clct(st)
        CSCStubComparison1D(plotterlist,  toPlot1, "LCT_strip-simhits_strip_hs", fslopebins, st, lctcuts, text)
        toPlot2 = delta_fqs_clct(st)
        CSCStubComparison1D(plotterlist,  toPlot2, "LCT_strip-simhits_strip_qs", fslopebins, st, lctcuts, text)
        toPlot3 = delta_fes_clct(st)
        CSCStubComparison1D(plotterlist,  toPlot3, "LCT_strip-simhits_strip_es", fslopebins, st, lctcuts, text)
