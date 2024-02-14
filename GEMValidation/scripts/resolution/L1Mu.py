from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack, kMagenta, gPad

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
yTitle = "Events"
subdirectory = "resolution/L1Mu/"

setTDRStyle()

iPeriod = 0 # use -1 to hide PU
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

## check EMTF variable distribution for different pT
def EMTFTrackInput1D(plotter, variable, h_bins, text):

    ptlist = [0, 5, 10, 15, 20]
    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)

    xTitle = variable
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
    for i, ptcut in enumerate(ptlist):

        #cuts = ok_emtf(ptcut)
        cuts = AND(ok_emtf(ptcut), ok_eta(1.65, 2.4))
        toPlot1 = variable
        hlist.append(draw_1D(plotter.tree, title, h_bins, toPlot1, cuts, "same", kcolors[i], markers[i]))
        #hlist[-1].Scale(1./hlist[-1].GetEntries())
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())

    base.SetMaximum(ymax * 1.2)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    leg.SetHeader("gen|#eta|>1.65")
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    for i in range(0, len(hlist)):
        hlist[i].Draw("histsame")
        #print("hlist entries ", hlist[i].GetEntries())
        leg.AddEntry(hlist[i], "pT >= %d"%ptlist[i],"pl")
    leg.Draw("same");
    if "deltaphi" in variable:
        base.SetMinimum(1)
        gPad.SetLogy()


    txt = drawCSCLabel(text, 0.15,0.25,0.035)
    #c.Print("%sEMTFInputs_ptcompare_%s%s"%(plotter.targetDir + subdirectory, variable,  plotter.ext))
    c.Print("%sEMTFInputs_ptcompare_%s_geneta1p65%s"%(plotter.targetDir + subdirectory, variable,  plotter.ext))

    del base, leg, hlist, c

def EMTFTrackComparison1D(plotterlist, variable, h_bins, cuts, plotsuffix, text):

    ptlist = [0, 5, 10, 15, 20]
    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    xTitle = variable
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
        #print("i ", i, " analyzer ", plotter.analyzer, " legend ", plotter.legend)
        #cuts = AND(ok_emtf(ptcut), ok_eta(1.65, 2.4))
        toPlot1 = variable
        hlist.append(draw_1D(plotter.tree, title, h_bins, toPlot1, cuts, "same", kcolors[i], markers[i]))
        #hlist[-1].Scale(1./hlist[-1].GetEntries())
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())

    base.SetMaximum(ymax * 1.3)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    #leg.SetHeader("gen|#eta|>1.65, EMTF pT >= %d GeV"%ptcut)
    #leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
    leg.SetHeader(plotsuffix)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.04)
    for i in range(0, len(hlist)):
        #print("i ", i, " analyzer ", plotterlist[i].analyzer, " legend ", plotterlist[i].legend)
        hlist[i].Draw("histsame")
        leg.AddEntry(hlist[i], plotterlist[i].legend,"pl")
    leg.Draw("same");
    if "deltaphi" in variable:
        base.SetMinimum(1)
        gPad.SetLogy()

    txt = drawCSCLabel(text, 0.15,0.25,0.035)
    #var_ptstr = variable+"_pT%d_"%ptcut
    var_ptstr = variable+"_"+plotsuffix
    c.Print("%sEMTFInputs_compare_%s%s"%(plotterlist[0].targetDir + subdirectory, var_ptstr,  plotterlist[0].ext))

    del base, leg, hlist, c



def EMTFTrackInputResolution(plotterlist, variable1, variable2, xtitle, h_bins, cuts, plotsuffix, text):
    """ compare the variable between stubs used by EMTF and stubs matched to simtrack """

    ptlist = [0, 5, 10, 15, 20]
    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    xTitle = xtitle
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
        #print("i ", i, " analyzer ", plotter.analyzer, " legend ", plotter.legend)
        toPlot1 = variable1
        hlist.append(draw_1D(plotter.tree, title, h_bins, toPlot1, cuts, "same", kcolors[i], markers[i], 1))
        toPlot2 = variable2
        hlist.append(draw_1D(plotter.tree, title, h_bins, toPlot2, cuts, "same", kcolors[i], markers[i], 3))
        #hlist[-1].Scale(1./hlist[-1].GetEntries())
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())
        if hlist[-2].GetBinContent(hlist[-2].GetMaximumBin()) > ymax:
            ymax = hlist[-2].GetBinContent(hlist[-2].GetMaximumBin())

    base.SetMaximum(ymax * 1.3)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    leg.SetHeader(plotsuffix)
    #leg.SetHeader("gen|#eta|>1.65, EMTF pT >= %d GeV"%ptcut)
    #leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.04)
    #print("plotsuffix ", plotsuffix, " var ", variable1, " nhist ", len(hlist))
    #for i in range(0, int(len(hlist)/2)):
    for i in range(0, len(plotterlist)):
        #print("i ", i, " analyzer ", plotterlist[i].analyzer, " legend ", plotterlist[i].legend, " num ", hlist[i*2].GetEntries())
        hlist[i*2].Draw("histsame")
        hlist[i*2+1].Draw("histsame")
        leg.AddEntry(hlist[i*2], plotterlist[i].legend,"pl")
    leg.Draw("same");
    if "phi" in variable1:
        base.SetMinimum(1)
        gPad.SetLogy()

    txt = drawCSCLabel(text, 0.15,0.25,0.035)
    #var_ptstr = variable+"_pT%d_"%ptcut
    #var_ptstr = variable1+"_pT%d_geneta1p65"%ptcut
    var_ptstr = plotsuffix
    c.Print("%sEMTFInputs_compare_%s%s"%(plotterlist[0].targetDir + subdirectory, var_ptstr,  plotterlist[0].ext))

    del base, leg, hlist, c



def EMTFInputsCompare(plotterlist, text):
    ## quality, 0-15
    ## deltaphi, -200, 200
    ## lctpattern, 0-15
    nomatch = False
    qbins = "(16, 0, 16)"
    dphibins = "(104, -260.0, 260.0)"
    phires_bins = "(100, -0.1, 0.1)"
    ptlist = [0, 5, 10, 15, 20]
    dphilist = ["deltaphi12", "deltaphi13", "deltaphi14", "deltaphi23","deltaphi24","deltaphi34"]
    qlist = ["quality","lctpattern1", "lctpattern2","lctpattern3","lctpattern4"]


    for st in range(1, 5):
        var1 = "l1Mu.cscstub_st%d_phi-l1Mu.simhits_st%d_phi"%(st, st)
        var2 = "l1Mu.emtfhit_st%d_phi-l1Mu.simhits_st%d_phi"%(st, st)
        cuts = AND(ok_eta(1.65, 2.4), TCut("l1Mu.has_emtfTrack && l1Mu.hasME%d && l1Mu.cscstub_st%d_found && l1Mu.emtfhit_st%d_ring==1"%(st, st, st)))
        xtitle = "#phi(stub) - #phi(simhits) [Rad]"
        plotsuffix="phiResolution_st%d_emtf_genEta1p64_2p40"%st
        EMTFTrackInputResolution(plotterlist, var2, var1, xtitle, phires_bins, cuts, plotsuffix, text)

    for pt in ptlist:
        ptcut = AND(ok_emtf(pt), ok_eta(1.65, 2.4))## ring 1 region only
        ptetasuffix="emtfpT%d_genEta1p65_2p40"%pt

        plotsuffix = "nstubs_matched" + "_"+ptetasuffix
        EMTFTrackComparison1D(plotterlist, "l1Mu.nstubs_matched_TF", "(5, 0, 5)", ptcut, plotsuffix, text)
        plotsuffix = "nstubs" + "_"+ptetasuffix
        EMTFTrackComparison1D(plotterlist, "l1Mu.nstubs", "(5, 0, 5)", ptcut, plotsuffix, text)
        plotsuffix = "mode" + "_"+ptetasuffix
        EMTFTrackComparison1D(plotterlist, "l1Mu.mode", "(16, 0, 16)", ptcut, plotsuffix, text)
        plotsuffix = "quality" + "_"+ptetasuffix
        EMTFTrackComparison1D(plotterlist, "l1Mu.quality", "(16, 0, 16)", ptcut, plotsuffix, text)

        for dphi in dphilist:
            var = "l1Mu."+dphi
            plotsuffix = dphi + "_"+ptetasuffix
            EMTFTrackComparison1D(plotterlist, var, dphibins, ptcut, plotsuffix, text)

            st1 = int(dphi[-2]); st2 = int(dphi[-1])
            nomatchcut = TCut("!l1Mu.cscstub_st%d_bestmatched || !l1Mu.cscstub_st%d_bestmatched"%(st1, st2))
            var2 = "l1Mu.emtfhit_st%d_phi-l1Mu.emtfhit_st%d_phi"%(st1, st2)
            var1 = "l1Mu.cscstub_st%d_phi-l1Mu.cscstub_st%d_phi"%(st1, st2)
            xtitle =dphi+ " [Rad]"
            cuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.cscstub_st%d_found && l1Mu.hasME%d && l1Mu.cscstub_st%d_found"%(st1, st1, st2, st2)), nomatchcut)
            plotsuffix = dphi+"Rad_"+ptetasuffix+"_nomatch"
            shvar1 = "l1Mu.simhits_st%d_phi-l1Mu.simhits_st%d_phi"%(st1, st2)
            shcuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.simhits_st%d_phi>-6 && l1Mu.hasME%d && l1Mu.simhits_st%d_phi>-6"%(st1, st1, st2, st2)), nomatchcut)
            shplotsuffix = dphi+"Rad_simhits_"+ptetasuffix+"_nomatch"
            if not nomatch:
                cuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.cscstub_st%d_found && l1Mu.hasME%d && l1Mu.cscstub_st%d_found"%(st1, st1, st2, st2)))
                plotsuffix = dphi+"Rad_"+ptetasuffix
                shcuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.simhits_st%d_phi>-6 && l1Mu.hasME%d && l1Mu.simhits_st%d_phi>-6"%(st1, st1, st2, st2)))
                shplotsuffix = dphi+"Rad_simhits_"+ptetasuffix
            print("cscstub cuts ",  cuts," plotsuffix ", plotsuffix, " variable ", var1)
            print("simhits cuts ", shcuts," plotsuffix ", shplotsuffix, " variable ", shvar1)
            EMTFTrackInputResolution(plotterlist, var2, var1, xtitle, phires_bins, cuts, plotsuffix, text)
            EMTFTrackInputResolution(plotterlist, var2, shvar1, xtitle, phires_bins, shcuts, shplotsuffix, text)

        for q in qlist:
            var = "l1Mu."+q
            plotsuffix = q + "_"+ptetasuffix
            EMTFTrackComparison1D(plotterlist, var, qbins, ptcut, plotsuffix, text)
            for st in range(1, 5):
                nomatchcut = TCut("!l1Mu.cscstub_st%d_bestmatched"%st)
                cuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.cscstub_st%d_found"%(st, st)), nomatchcut)
                plotsuffix =   "stubphi_st%d_"%st+ptetasuffix+"_nomatch"
                if not nomatch:
                    cuts = AND(ptcut, TCut("l1Mu.hasME%d && l1Mu.cscstub_st%d_found"%(st, st)))
                    plotsuffix =   "stubphi_st%d_"%st+ptetasuffix
                cscstub_phi_res = "l1Mu.cscstub_st%d_phi-l1Mu.simhits_st%d_phi"%(st, st)
                emtfhit_phi_res = "l1Mu.emtfhit_st%d_phi-l1Mu.simhits_st%d_phi"%(st, st)
                xtitle = "stub_phi - simhits_phi @ station%d [Rad]"%st
                EMTFTrackInputResolution(plotterlist, emtfhit_phi_res, cscstub_phi_res, xtitle, phires_bins, cuts, plotsuffix, text)
                cscstub_pattern = "l1Mu.cscstub_st%d_pattern"%(st)
                emtfhit_pattern = "l1Mu.emtfhit_st%d_pattern"%(st)
                xtitle = "LCT pattern @ station%d"%st
                plotsuffix =   "stubpattern_st%d_"%st+ptetasuffix+"_nomatch"
                if not nomatch:
                    plotsuffix =   "stubpattern_st%d_"%st+ptetasuffix
                EMTFTrackInputResolution(plotterlist, emtfhit_pattern, cscstub_pattern, xtitle,qbins, cuts, plotsuffix, text)
    #for plotter in plotterlist:
    #    for dphi in dphilist:
    #        var = "l1Mu."+dphi
    #        EMTFTrackInput1D(plotter, var, dphibins, text)
    #    for q in qlist:
    #        var = "l1Mu."+q
    #        EMTFTrackInput1D(plotter, var, qbins, text)
