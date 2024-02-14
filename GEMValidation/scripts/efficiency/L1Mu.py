from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack, kMagenta, gPad

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

xGrid = 2
yGrid = 2

topTitle = ""
yTitle = "Efficiency"
subdirectory = "efficiency/L1Mu/"

setTDRStyle()

iPeriod = 0 # use -1 to hide PU
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def EMTFPt(plotter, text):

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
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_emtf(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_emtf(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_emtf(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_emtf(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, denom_cut, ok_emtf(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("EMTF")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");

    txt = drawCSCLabel(text, 0.15,0.25,0.035)
    c.Print("%sEff_EMTF_pt_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5


def EMTFEta(plotter, text):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(%s)"%(geneta)

    h_bins = "(20,1.2,2.4)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>20")), ok_emtf(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>15")), ok_emtf(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>10")), ok_emtf(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>5")), ok_emtf(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>3")), ok_emtf(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("EMTF")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sEff_EMTF_eta_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5


def EMTFEta2(plotter, text):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = geneta

    h_bins = "(25,-2.5,2.5)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>20")), ok_emtf(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>15")), ok_emtf(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>10")), ok_emtf(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>5")), ok_emtf(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>3")), ok_emtf(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("EMTF")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sEff_EMTF2_eta_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5


def EMTFPhi(plotter, text):

    ## variables for the plot
    xTitle = "Generated muon #phi"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = genphi

    h_bins = "(20,-3.2,3.2)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>20")), ok_emtf(20), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>15")), ok_emtf(15), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>10")), ok_emtf(10), "same", kGreen+2)
    h4 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>5")), ok_emtf(5), "same", kOrange+2)
    h5 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>3")), ok_emtf(0), "same", kBlack)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.SetHeader("EMTF")
    leg.AddEntry(h1, "p_{T} > 20 GeV","l")
    leg.AddEntry(h2, "p_{T} > 15 GeV","l")
    leg.AddEntry(h3, "p_{T} > 10 GeV","l")
    leg.AddEntry(h4, "p_{T} > 5 GeV","l")
    leg.AddEntry(h5, "p_{T} > 0 GeV","l")
    leg.Draw("same");
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sEff_EMTF_phi_Pt20_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg, h2, h3, h4, h5

def L1Mu(plotter, text):
    EMTFPt(plotter, text)
    EMTFEta(plotter, text)
    EMTFEta2(plotter, text)
    EMTFPhi(plotter, text)

def L1MuComparisonEta(plotterlist, text, denom_cut, num_cut, plotsuffix):

    toPlot = "TMath::Abs(eta)"
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    ptList = [0, 3, 5, 10, 15, 20]
    #h_bins = "(25,-2.5,2.5)"
    h_bins = "(20,1.2,2.4)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    for ptcut in ptList:
        hlist = []

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)

        base  = TH1F("base",title,nBins,minBin,maxBin)

        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotterlist[0].yMin)
        #base.SetMinimum(0.0)
        base.SetMaximum(plotterlist[0].yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        #denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())
        #h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), ok_emtf(ptcut), "same", kRed)
        #h3 = draw_geff(plotter2.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), ok_emtf(ptcut), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        #leg.AddEntry(h3, plotter2.legend,"pl")
        for i, plotter in enumerate(plotterlist):
            hlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), AND(num_cut, ok_emtf(ptcut)), "same", kcolors[i], markers[i]))
            leg.AddEntry(hlist[-1], plotter.legend,"pl")
        leg.Draw("same");


        txt = drawCSCLabel(text, 0.15,0.25,0.035)
        ptstr = "pT%d_%s"%(ptcut, plotsuffix)
        c.Print("%sEff_EMTFEta_comparison_%s%s"%(plotterlist[0].targetDir + subdirectory, ptstr, plotterlist[0].ext))
        del c, base, leg, hlist


def L1MuComparisonEta2(plotterlist, leglist, text, denom_cutlist, num_cutlist, plotsuffix):

    toPlot = "TMath::Abs(eta)"
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    ptList = [0, 3, 5, 10, 15, 20]
    #h_bins = "(25,-2.5,2.5)"
    h_bins = "(20,1.2,2.4)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    for ptcut in ptList:
        hlist = []

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)

        base  = TH1F("base",title,nBins,minBin,maxBin)

        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotterlist[0].yMin)
        #base.SetMinimum(0.0)
        base.SetMaximum(plotterlist[0].yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        #denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())
        #h2 = draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), ok_emtf(ptcut), "same", kRed)
        #h3 = draw_geff(plotter2.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), ok_emtf(ptcut), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        #leg.AddEntry(h3, plotter2.legend,"pl")
        for i, plotter in enumerate(plotterlist):
            denom_cut = denom_cutlist[i]
            num_cut = num_cutlist[i]
            leg = leglist[i]
            hlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, AND(denom_cut, TCut("genParticle.pt>%d"%ptcut)), AND(num_cut, ok_emtf(ptcut)), "same", kcolors[i], markers[i]))
            leg.AddEntry(hlist[-1], leg,"pl")
        leg.Draw("same");


        txt = drawCSCLabel(text, 0.15,0.25,0.035)
        ptstr = "pT%d_%s"%(ptcut, plotsuffix)
        c.Print("%sEff_EMTFEta_comparison_%s%s"%(plotterlist[0].targetDir + subdirectory, ptstr, plotterlist[0].ext))
        del c, base, leg, hlist


def L1MuComparisonPt(plotterlist, text, denom_cut, num_cut, plotsuffix):

    toPlot = "pt"
    xTitle = "Generated muon p_{T} [GeV]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    ptList = [0, 3, 5, 10, 15, 20]
    etacuttext = "|#eta_{sim}|>1.65"

    for ptcut in ptList:
        hlist = []

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)
        c.SetLogx()

        base  = TH1F("base",title,len(ptbins)-1, ptbins)
        base.SetMinimum(plotterlist[0].yMin)
        #base.SetMinimum(0.0)
        base.SetMaximum(plotterlist[0].yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)


        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetHeader(etacuttext+",EMTF p_{T} >= %d GeV"%ptcut)
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        for i, plotter in enumerate(plotterlist):
            hlist.append(draw_geff_ptbins(plotter.tree, title, ptbins, toPlot, denom_cut, AND(num_cut, ok_emtf(ptcut)), "same", kcolors[i], markers[i]))
            leg.AddEntry(hlist[-1], plotter.legend,"pl")
        leg.Draw("same");


        txt = drawCSCLabel(text, 0.15,0.25,0.035)
        ptstr = "pT%d_%s"%(ptcut, plotsuffix)
        c.Print("%sPtEff_EMTFPt_comparison_%s%s"%(plotterlist[0].targetDir + subdirectory, ptstr, plotterlist[0].ext))
        del c, base, leg, hlist


def L1MuComparisonEtaAll(plotterlist, text):

    #denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_lcts())
    denom_cut = AND(ok_eta(1.2, 2.4), ok_2_csc_sh())
    num_cut = TCut("")
    #pt
    plotsuffix=""
    L1MuComparisonEta(plotterlist, text, denom_cut, num_cut, plotsuffix)
    denometa_cut = AND(ok_eta(1.65, 2.4), ok_2_csc_sh())
    plotsuffix="eta1p65"
    L1MuComparisonPt(plotterlist, text, denom_cut, num_cut, plotsuffix)
    return ##ignore the following part for now
    

    dcut = AND(denom_cut, TCut("l1Mu.has_emtfTrack"))
    ncut = TCut("l1Mu.nstubs == l1Mu.nstubs_matched_TF")
    plotsuffix = "allstubsMatched"
    #print("dcut ",dcut, " ncut ", ncut)
    L1MuComparisonEta(plotterlist, text, dcut, ncut, plotsuffix)
    n1cut = TCut("l1Mu.nstubs == l1Mu.nstubs_bestmatched_TF")
    plotsuffix = "allstubsbestMatched"
    L1MuComparisonEta(plotterlist, text, dcut, n1cut, plotsuffix)

    for csc in cscStations:
        st = csc.station
        ring = csc.ring
        if csc.labelc == "ME1a" or csc.labelc == "ME1b":
            continue
        #if csc.labelc == "ME11":
        #    dcut = AND(ok_eta(1.2, 2.4), TCut("l1Mu.has_emtfTrack && (l1Mu.emtfhit_st1_ring==1 || l1Mu.emtfhit_st1_ring==4) && l1Mu.hasME1"))
        #    ncut = TCut("(l1Mu.cscstub_st1_ring==1 || l1Mu.cscstub_st1_ring==4) && l1Mu.cscstub_st1_found")
        #    plotsuffix = "stubMatched_%s"%csc.labelc
        #else:
        dcut = AND(ok_eta(1.2, 2.4), TCut("l1Mu.has_emtfTrack && l1Mu.emtfhit_st%d_ring==%d && l1Mu.hasME%d"%(st, ring, st)))
        ncut = TCut("l1Mu.cscstub_st%d_ring==%d && l1Mu.cscstub_st%d_matched"%(st, ring, st))
        plotsuffix = "stubMatched_%s"%csc.labelc
        L1MuComparisonEta(plotterlist, text, dcut, ncut, plotsuffix)
        n1cut = TCut("l1Mu.cscstub_st%d_ring==%d && l1Mu.cscstub_st%d_bestmatched"%(st, ring, st))
        plotsuffix = "stubbestMatched_%s"%csc.labelc
        L1MuComparisonEta(plotterlist, text, dcut, n1cut, plotsuffix)


