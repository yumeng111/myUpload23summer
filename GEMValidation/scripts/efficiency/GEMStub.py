from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
yTitle = "Efficiency"
subdirectory = "efficiency/GEMStub/"

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def GEMPadEta(plotter):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"

    for st in range(0,len(gemStations)):
        h_bins = "(50,%f,%f)"%(gemStations[st].eta_min,gemStations[st].eta_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_pad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Pad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_GEMPad_eta_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label




def GEMPadPhi(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Pad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon #phi [rad]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "phi"


    for st in range(0,len(gemStations)):
        h_bins = "(50,%f,%f)"%(gemStations[st].phi_min,gemStations[st].phi_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_pad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Pad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMPad_phi_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label
        

def GEMPadL(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Pad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"


    for st in range(0,len(gemStations)):
        h_bins = "(50,%f,%f)"%(gemStations[st].l_min,gemStations[st].l_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_pad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Pad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMPad_L_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label


def GEMPadEta2(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Pad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].eta_min,gemStations[st].eta_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh2(st), ok_gem_pad2(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Two pads","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_GEMPad2_eta_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label




def GEMPadPhi2(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Pad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon #phi [rad]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "phi"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].phi_min,gemStations[st].phi_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh2(st), ok_gem_pad2(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Two pads","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMPad2_phi_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label


def GEMCoPadEta(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM CoPad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].eta_min,gemStations[st].eta_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh2(st), ok_gem_copad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "CoPad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_GEMCoPad_eta_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label




def GEMCoPadPhi(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM CoPad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon #phi [rad]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "phi"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].phi_min,gemStations[st].phi_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh2(st), ok_gem_copad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "CoPad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMCoPad_phi_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label

##################################################################
### addition: Sept. 15th for L = sqrt(vx*vx + vy*vy) vs. eff.

def GEMCoPadL(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM CoPad matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon L:"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].l_min,gemStations[st].l_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh2(st), ok_gem_copad(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "CoPad","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMCoPad_L_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label


        

def GEMClusterEta(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Cluster matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon |#eta|"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(eta)"


    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].eta_min,gemStations[st].eta_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_cluster(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Cluster","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_GEMCluster_eta_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label




def GEMClusterPhi(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Cluster matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon #phi [rad]"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "phi"

    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].phi_min,gemStations[st].phi_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_cluster(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Cluster","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMCluster_phi_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label


def GEMClusterL(plotter):

    ## variables for the plot
    topTitle = " " * 11 + "GEM Cluster matching" + " " * 35 + "CMS Simulation Preliminary"
    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(1,len(gemStations)):

        h_bins = "(50,%f,%f)"%(gemStations[st].l_min,gemStations[st].l_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_gem_sh(st), ok_gem_cluster(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Cluster","l")
        leg.Draw("same")

        label = drawCSCLabel(gemStations[st].label, 0.87,0.87,0.05)
        c.Print("%sEff_GEMCluster_L_%s%s"%(plotter.targetDir + subdirectory, gemStations[st].labelc,  plotter.ext))

        del c, base, h1, leg, label


        

def GEMStub(plotter):
    GEMPadEta(plotter)
    GEMPadPhi(plotter)
    GEMPadEta2(plotter)
    GEMPadPhi2(plotter)
    GEMPadL(plotter)
    GEMCoPadEta(plotter)
    GEMCoPadPhi(plotter)
    GEMCoPadL(plotter)
    GEMClusterEta(plotter)
    GEMClusterPhi(plotter)
    GEMClusterL(plotter)
