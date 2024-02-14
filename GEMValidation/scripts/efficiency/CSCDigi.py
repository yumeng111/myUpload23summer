from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

topTitle = ""
#xTitle = "Generated muon |#eta|"
yTitle = "Efficiency"
#toPlot = "TMath::Abs(eta)"
subdirectory = "efficiency/CSCDigi/"
#title = "%s;%s;%s"%(topTitle,xTitle,yTitle)

setTDRStyle()

iPeriod = 0
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12

def CSCComp(plotter):

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
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_strip(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h2, "Comparators","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCComp_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2

def CSCCompL(plotter):

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
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

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_strip(st), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h2, "Comparators","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCComp_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2
        

def CSCWire(plotter):

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
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_wire(st), "same", kRed)

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Wires","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCWire_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1
        

def CSCWireL(plotter):

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
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

        h1 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), ok_csc_wire(st), "same", kRed)

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC")
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h1, "Wires","l")
        leg.Draw("same")

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCWire_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h1
        

def CSCCompWire(plotter):

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
        base  = TH1F("base",title,nBins,minBin,maxBin)
        base.SetMinimum(plotter.yMin)
        base.SetMaximum(plotter.yMax)
        base.GetXaxis().SetLabelSize(0.05)
        base.GetYaxis().SetLabelSize(0.05)
        base.GetXaxis().SetTitleSize(0.05)
        base.GetYaxis().SetTitleSize(0.05)
        base.Draw("")
        CMS_lumi.CMS_lumi(c, iPeriod, iPos)

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), AND(ok_csc_wire(st), ok_csc_strip(st)), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h2, "Wires and comparators","l")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCCompWire_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2

        

def CSCCompWireL(plotter):

    xTitle = "Generated muon L"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Sqrt((vx*vx)+(vy*vy))"

    for st in range(0,len(cscStations)):

        h_bins = "(50,%f,%f)"%(cscStations[st].l_min,cscStations[st].l_max)
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

        h2 = draw_geff(plotter.tree, title, h_bins, toPlot, ok_csc_sh(st), AND(ok_csc_wire(st), ok_csc_strip(st)), "same")

        leg = TLegend(0.45,0.2,.75,0.35, "", "brNDC");
        leg.SetBorderSize(0)
        leg.SetFillStyle(0)
        leg.SetTextSize(0.06)
        leg.AddEntry(h2, "Wires and comparators","l")
        leg.Draw("same");

        csc = drawCSCLabel(cscStations[st].label, 0.87,0.87,0.05)

        c.Print("%sEff_CSCCompWire_L_%s%s"%(plotter.targetDir + subdirectory, cscStations[st].labelc,  plotter.ext))

        del c, base, leg, csc, h2
        
        

def CSCDigi(plotter):
    CSCComp(plotter)
    CSCCompL(plotter)
    CSCWire(plotter)
    CSCWireL(plotter)
    CSCCompWire(plotter)
    CSCCompWireL(plotter)
