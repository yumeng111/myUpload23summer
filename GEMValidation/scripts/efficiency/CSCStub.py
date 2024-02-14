from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack, kMagenta, kYellow,gPad

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
#xTitle = "Generated muon |#eta|"
yTitle = "Efficiency"
subdirectory = "efficiency/CSCStub/"
#title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
xGrid=2
yGrid=2
setTDRStyle()

iPeriod = 0 ## use -1 to hide PU
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def CSCALCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_alct(st), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCALCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc

def CSCALCTL(plotter):

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_alct(st), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCALCT_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc


def CSCCLCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct(st), "same",kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "CLCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCCLCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h2, leg, csc

def CSCCLCTL(plotter):

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct(st), "same",kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "CLCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCCLCT_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h2, leg, csc


def CSCALCTCLCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), AND(ok_csc_alct(st), ok_csc_clct(st)), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCALCTCLCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc

def CSCALCTorCLCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), OR(ok_csc_alct(st), ok_csc_clct(st)), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h1, "ALCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCALCTorCLCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, csc

def CSCCLCTPattern(plotter):

    toPlot = "pt"

    for st in range(0,len(cscStations)):

        topTitle = ""
        xTitle = "Generated p_{T} GeV"
        yTitle = "Efficiency"
        title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

        h_bins = "(20,0,20)"
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0.0)
        base.SetMaximum(1.1)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        def ok_csc_clct_pattern(st, p1, p2):
            return AND(ok_csc_clct(st),
                       OR(ok_pattern(st, p1),
                          ok_pattern(st, p2)) )

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct_pattern(st, 2, 3), "same",kBlack)
        h3 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct_pattern(st, 4, 5), "same",kRed)
        h4 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct_pattern(st, 6, 7), "same",kBlue)
        h5 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct_pattern(st, 8, 9), "same",kOrange)
        h6 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct_pattern(st, 10, 10), "same",kGreen+2)

        leg = TLegend(0.7,0.15,.95,0.45, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.SetHeader("PID")
        leg.AddEntry(h2, "#geq 2","pl")
        leg.AddEntry(h3, "#geq 4","pl")
        leg.AddEntry(h4, "#geq 6","pl")
        leg.AddEntry(h5, "#geq 8","pl")
        leg.AddEntry(h6, "#geq 10","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCCLCT_pattern_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, h2, leg, csc, h3, h4, h5, h6


def CSCLCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_lct(st), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "LCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCLCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2

def CSCLCTL(plotter):

    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"
    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_lct(st), "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "LCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_CSCLCT_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2


def GEMCSCLCT(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in [0,5]:

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        gemst = 1
        if st==5:
            gemst = 2

        ok_alct_clct = ok_csc_clct(st) and ok_csc_alct(st) and ok_gem_pad(gemst)
        ok_alct_gem = ok_csc_alct(st) and ok_gem_copad(gemst)
        ok_clct_gem = ok_csc_clct(st) and ok_gem_copad(gemst)

        ok_any_combination = ok_alct_clct or ok_alct_gem or ok_clct_gem

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_any_combination, "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "LCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_GEMCSCLCT_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2

def GEMCSCLCTL(plotter):

    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"
    xTitle = "Generated Muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    for st in [0,5]:

        h_bins = "(25,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        gemst = 1
        if st==5:
            gemst = 2

        ok_alct_clct = ok_csc_clct(st) and ok_csc_alct(st) and ok_gem_pad(gemst)
        ok_alct_gem = ok_csc_alct(st) and ok_gem_copad(gemst)
        ok_clct_gem = ok_csc_clct(st) and ok_gem_copad(gemst)

        ok_any_combination = ok_alct_clct or ok_alct_gem or ok_clct_gem

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_any_combination, "same", kBlue)

        leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.05)
        leg.AddEntry(h2, "LCT","pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)

        c.Print("%sEff_GEMCSCLCT_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2




def MultipleCSCLCTPt(plotter):

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(2), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(3), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(4), "same", kGreen+2)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    leg.AddEntry(h1, "#geq 2 LCTs","pl")
    leg.AddEntry(h2, "#geq 3 LCTs","pl")
    leg.AddEntry(h3, "4 LCTs","pl")
    leg.Draw("same");

    c.Print("%sEff_MultiLCTs_pt_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg


def MultipleCSCLCTEta(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(2), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(3), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(4), "same", kGreen+2)

    leg = TLegend(0.45,0.2,.75,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.06)
    leg.AddEntry(h1, "#geq 2 LCTs","pl")
    leg.AddEntry(h2, "#geq 3 LCTs","pl")
    leg.AddEntry(h3, "4 LCTs","pl")
    leg.Draw("same");

    c.Print("%sEff_MultiLCTs_eta_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg

def MultipleCSCLCTPhi(plotter):

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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(2), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(3), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(4), "same", kGreen+2)

    leg = TLegend(0.2,0.2,.5,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.06)
    leg.AddEntry(h1, "#geq 2 LCTs","pl")
    leg.AddEntry(h2, "#geq 3 LCTs","pl")
    leg.AddEntry(h3, "4 LCTs","pl")
    leg.Draw("same");

    c.Print("%sEff_MultiLCTs_phi_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg



def MultipleCSCLCTL(plotter):

    ## variables for the plot
    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    h_bins = "(34,0,170)"
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

    h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(2), "same", kBlue)
    h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(3), "same", kRed)
    h3 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_eta(1.2, 2.4), ok_csc_lcts(4), "same", kGreen+2)

    leg = TLegend(0.2,0.2,.5,0.5, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.06)
    leg.AddEntry(h1, "#geq 2 LCTs","pl")
    leg.AddEntry(h2, "#geq 3 LCTs","pl")
    leg.AddEntry(h3, "4 LCTs","pl")
    leg.Draw("same");

    c.Print("%sEff_MultiLCTs_L_%s"%(plotter.targetDir + subdirectory, plotter.ext))

    del c, base, h1, leg

def CSCALCTEffVsNHits(plotter, text, Nhits_thresholds):

    xTitle = "Generated muon |#eta|"
    yTitle = "ALCT Efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"
    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0.0)
        base.SetMaximum(plotter.yMax+0.05)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)

        histlist = []
        for i,nhits in enumerate(Nhits_thresholds):
            hitscut = None
            if i < len(Nhits_thresholds)/2.0:
                hitscut = TCut("(has_csc_sh_even[%d] && totalwires_dg_even[%d]<=%d) || (has_csc_sh_odd[%d] && totalwires_dg_odd[%d]<=%d)"%(st,st,nhits,st,st,nhits))
            else:
                hitscut = TCut("(has_csc_sh_even[%d] && totalwires_dg_even[%d]>=%d) || (has_csc_sh_odd[%d] && totalwires_dg_odd[%d]>=%d)"%(st,st,nhits,st,st,nhits))
            histlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, hitscut, ok_csc_alct(st), "same", kcolors[i], markers[i]))

        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        leg = TLegend(0.45,0.15,.75,0.15+0.04*len(Nhits_thresholds), "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        for i, hist in enumerate(histlist):
            hist.Draw("same")
            if i < len(Nhits_thresholds)/2.0:
                leg.AddEntry(hist, "Total wire digis <= %d"%Nhits_thresholds[i],"pl")
            else:
                leg.AddEntry(hist, "Total wire digis >= %d"%Nhits_thresholds[i],"pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        c.Print("%sEff_CSCALCT_vs_totalwiredigis_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        del c, base, leg, csc, histlist,txt


def CSCCLCTEffVsNHits(plotter, text, Nhits_thresholds):

    xTitle = "Generated muon |#eta|"
    yTitle = "CLCT Efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"
    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0.0)
        base.SetMaximum(plotter.yMax+0.05)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)

        histlist = []
        for i,nhits in enumerate(Nhits_thresholds):
            hitscut = None
            if i < len(Nhits_thresholds)/2.0:
                hitscut = TCut("(has_csc_sh_even[%d] && totalcomparators_dg_even[%d]<=%d) || (has_csc_sh_odd[%d] && totalcomparators_dg_odd[%d]<=%d)"%(st,st,nhits,st,st,nhits))
            else:
                hitscut = TCut("(has_csc_sh_even[%d] && totalcomparators_dg_even[%d]>=%d) || (has_csc_sh_odd[%d] && totalcomparators_dg_odd[%d]>=%d)"%(st,st,nhits,st,st,nhits))
            histlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, hitscut, ok_csc_clct(st), "same", kcolors[i], markers[i]))

        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        leg = TLegend(0.45,0.15,.78,0.15+0.04*len(Nhits_thresholds), "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        for i, hist in enumerate(histlist):
            hist.Draw("same")
            if i < len(Nhits_thresholds)/2.0:
                leg.AddEntry(hist, "Total comparator digis <= %d"%Nhits_thresholds[i],"pl")
            else:
                leg.AddEntry(hist, "Total comparator digis >= %d"%Nhits_thresholds[i],"pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        c.Print("%sEff_CSCCLCT_vs_totalcompdigis_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        del c, base, leg, csc, histlist,txt


def CSCLCTEffVsNHits(plotter, text, Nhits_thresholds):

    xTitle = "Generated muon |#eta|"
    yTitle = "LCT Efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"
    for st in range(0,len(cscStations)):

        h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
        nBins = int(h_bins[1:-1].split(',')[0])
        minBin = float(h_bins[1:-1].split(',')[1])
        maxBin = float(h_bins[1:-1].split(',')[2])

        c = newCanvas()
        gPad.SetGridx(xGrid)
        gPad.SetGridy(yGrid)
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(0.0)
        base.SetMaximum(plotter.yMax+0.05)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)

        histlist = []
        for i,nhits in enumerate(Nhits_thresholds):
            hitscut = None
            if i < len(Nhits_thresholds)/2.0:
                hitscut = TCut("(has_csc_sh_even[%d] && totalcomparators_dg_even[%d]<=%d) || (has_csc_sh_odd[%d] && totalcomparators_dg_odd[%d]<=%d)"%(st,st,nhits,st,st,nhits))
            else:
                hitscut = TCut("(has_csc_sh_even[%d] && totalcomparators_dg_even[%d]>=%d) || (has_csc_sh_odd[%d] && totalcomparators_dg_odd[%d]>=%d)"%(st,st,nhits,st,st,nhits))
            histlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, hitscut, ok_csc_lct(st), "same", kcolors[i], markers[i]))

        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)
        leg = TLegend(0.45,0.15,.78,0.15+0.04*len(Nhits_thresholds), "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.04)
        for i, hist in enumerate(histlist):
            hist.Draw("same")
            if i < len(Nhits_thresholds)/2.0:
                leg.AddEntry(hist, "Total comparator digis <= %d"%Nhits_thresholds[i],"pl")
            else:
                leg.AddEntry(hist, "Total comparator digis >= %d"%Nhits_thresholds[i],"pl")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
        txt = drawCSCLabel(text, 0.15,0.25,0.035)

        c.Print("%sEff_CSCLCT_vs_totalcompdigis_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))
        del c, base, leg, csc, histlist,txt


def CSCStub(plotter, text=""):
    CSCALCT(plotter)
    CSCALCTL(plotter)
    CSCCLCT(plotter)
    CSCCLCTL(plotter)
    CSCALCTCLCT(plotter)
    CSCALCTorCLCT(plotter)
    CSCLCT(plotter)
    CSCLCTL(plotter)
    GEMCSCLCT(plotter)
    GEMCSCLCTL(plotter)
    CSCCLCTPattern(plotter)
    MultipleCSCLCTPt(plotter)
    MultipleCSCLCTEta(plotter)
    MultipleCSCLCTPhi(plotter)
    MultipleCSCLCTL(plotter)
    Nhits_thresholds = [6, 9, 12, 15,18, 21]
    CSCALCTEffVsNHits(plotter, text, Nhits_thresholds)
    CSCCLCTEffVsNHits(plotter, text, Nhits_thresholds)
    CSCLCTEffVsNHits(plotter, text, Nhits_thresholds)

def CSCStubComparison(plotterlist, st, dencut, numcut, plotsuffix, text):

    toPlot = "TMath::Abs(eta)"
    xTitle = "Generated muon |#eta|"
    yTitle = plotsuffix + " Efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

    hlist = []
    h_bins = "(25,%f,%f)"%(cscStations[st].eta_min,cscStations[st].eta_max)
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(plotterlist[0].yMin)
    base.SetMaximum(plotterlist[0].yMax)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

  

    leg = TLegend(0.45,0.15,.75,0.15+len(plotterlist)*0.05, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    for i, plotter in enumerate(plotterlist):
        #hlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_clct(st), "same", kcolors[i], markers[i]))
        hlist.append(draw_geff(plotter.tree, title, h_bins, toPlot, dencut, numcut, "same", kcolors[i], markers[i]))
        leg.AddEntry(hlist[-1], plotter.legend,"pl")

    leg.Draw("same");

    csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sEff_CSC%s_comparison_%s%s"%(plotterlist[0].targetDir + subdirectory, plotsuffix, cscStations[st].labelc,  plotterlist[0].ext))

    del c, base, leg, csc, hlist

def CSCStubComparisonPt(plotterlist, st, dencut, numcut, plotsuffix, text):

    toPlot = "pt"
    xTitle = "Generated muon p_{T} [GeV]"
    yTitle = plotsuffix + " Efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    hlist = []

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)
    base  = TH1F("base",title,len(ptbins)-1, ptbins)
    base.SetMinimum(plotterlist[0].yMin)
    base.SetMaximum(plotterlist[0].yMax)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    c.SetLogx()
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

  

    leg = TLegend(0.45,0.15,.75,0.15+len(plotterlist)*0.05, "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.05)
    for i, plotter in enumerate(plotterlist):
        hlist.append(draw_geff_ptbins(plotter.tree, title, ptbins, toPlot, dencut, numcut, "same", kcolors[i], markers[i]))
        leg.AddEntry(hlist[-1], plotter.legend,"pl")

    leg.Draw("same");

    csc = drawCSCLabel(cscStations[st].label, 0.85,0.85,0.05)
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%sPtEff_CSC%s_comparison_%s%s"%(plotterlist[0].targetDir + subdirectory, plotsuffix, cscStations[st].labelc,  plotterlist[0].ext))

    del c, base, leg, csc, hlist


def CSCStubComparisonAll(plotterlist, text):

    
    for st in range(0,len(cscStations)):
        lctcut = TCut("(cscStub.has_lct_even[%d] && cscStub.matchtype_lct_even[%d]==3) || (cscStub.has_lct_odd[%d] && cscStub.matchtype_lct_odd[%d]==3)"%(st,st,st,st))
        CSCStubComparison(plotterlist, st, ok_csc_sh(st), ok_csc_clct(st), "CLCT", text)
        CSCStubComparison(plotterlist, st, ok_csc_sh(st), ok_csc_alct(st), "ALCT", text)
        #print("LCT eff ",st," dencut ",ok_csc_sh(st)," numcut ",  ok_csc_lct(st))
        #CSCStubComparison(plotterlist, st, ok_csc_sh(st), ok_csc_lct(st),   "LCT", text)
        CSCStubComparison(plotterlist, st, ok_csc_sh(st), lctcut,   "LCT", text)
        CSCStubComparisonPt(plotterlist, st, ok_csc_sh(st), ok_csc_clct(st), "CLCT", text)
        CSCStubComparisonPt(plotterlist, st, ok_csc_sh(st), ok_csc_alct(st), "ALCT", text)
        CSCStubComparisonPt(plotterlist, st, ok_csc_sh(st), lctcut,   "LCT", text)
