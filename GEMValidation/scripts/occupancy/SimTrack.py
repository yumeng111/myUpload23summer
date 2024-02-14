from ROOT import gStyle, TH1F, TCanvas, TLegend, kRed, kBlue, kOrange, kGreen, kBlack, kMagenta, gPad,gDirectory
import numpy as np
import os

from helpers.cuts import *
from helpers.Helpers import *
from helpers.stations import *
from style.tdrstyle import *
import style.CMS_lumi as CMS_lumi
from style.canvas import newCanvas

xGrid = 2
yGrid = 2

setTDRStyle()
iPeriod = -1
iPos = 0
if( iPos==0 ): CMS_lumi.relPosX = 0.12
subdirectory = "occupancy/SimTrack/"

def draw_1D(target_dir, c_title, ext, t, title, h_bins, to_draw, cut, text, opt = ""):
  nBins = int(h_bins[1:-1].split(',')[0])
  minBin = float(h_bins[1:-1].split(',')[1])
  maxBin = float(h_bins[1:-1].split(',')[2])

  #gStyle.SetOptStat(1110)
  c = TCanvas("c","c",600,600)
  c.Clear()
  if 'pt' in to_draw:
      c.SetLogx()
  h = TH1F("h", title, nBins, minBin, maxBin)
  t.Draw(to_draw + ">> h" , cut)
  h.SetLineWidth(2)
  h.SetLineColor(kBlue)
  h.SetMinimum(0.)
  h.SetMaximum(h.GetBinContent(h.GetMaximumBin()) * 1.2)
  h.Draw(opt)
  CMS_lumi.CMS_lumi(c, iPeriod, iPos)
  txt = drawCSCLabel(text, 0.15,0.25,0.035)

  c.SaveAs(target_dir +"simTrack_"+ c_title + ext)
  del c,h,txt
  gStyle.SetStatStyle(0)


## draw pt for [0,inf]
def draw1DPt(plotter, text):
    ##use logy
    pts = [2, 5, 10, 20, 30, 60, 100, 400, 1000, 10000.0]
    xtitle = "simTrack p_{T} [GeV]"
    ptbins =  np.asarray(pts)
    c = newCanvas()
    h = TH1F("h","simTrack_pt",len(ptbins)-1, ptbins)
    gPad.SetGrid(1,1)
    h.GetXaxis().SetLabelSize(0.05)
    h.GetYaxis().SetLabelSize(0.05)
    h.GetXaxis().SetTitleSize(0.05)
    h.GetYaxis().SetTitleSize(0.05)
    h.GetXaxis().SetTitle(xtitle)
    h.GetYaxis().SetTitle("Events")
    plotter.tree.Draw("pt>> h", "1", "goff")
    c.SetLogx()

    h.SetMinimum(0)
    h.SetMaximum(h.GetBinContent(h.GetMaximumBin()) * 1.3)
    h.SetMarkerSize(2.)
    h.SetLineColor(kBlue)
    h.Draw("histtext")

    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    txt = drawCSCLabel(text, 0.15,0.75,0.035)

    c.Print("%ssimTrack_pT_2To10000GeV%s"%(plotter.targetDir + subdirectory,  plotter.ext))

    del h, c, txt

def draw2DPtvsEta(plotter, text):
    etabins = "(50,0,2.5)"
    nBins   = int(etabins[1:-1].split(',')[0])
    minBin = float(etabins[1:-1].split(',')[1])
    maxBin = float(etabins[1:-1].split(',')[2])
    binwidth = (maxBin-minBin)/nBins
    etas = [minBin+n*binwidth for n in range(0,nBins+1)]
    etabins_np = np.asarray(etas)
    pts = [2, 5, 10, 20, 30, 60, 100, 400, 1000, 10000.0]
    ptbins =  np.asarray(pts)
    xtitle = "simTrack p_{T} [GeV]"
    ytitle = "simTrack |#eta|"
    c = newCanvas()
    gPad.SetGrid(1,1)
    gPad.SetRightMargin(0.15)
    h = TH2F("h","simTrack_ptvseta",len(ptbins)-1, ptbins, len(etabins_np)-1, etabins_np)
    h.GetXaxis().SetLabelSize(0.05)
    h.GetYaxis().SetLabelSize(0.05)
    h.GetXaxis().SetTitleSize(0.05)
    h.GetYaxis().SetTitleSize(0.05)
    h.GetXaxis().SetTitle(xtitle)
    h.GetYaxis().SetTitle(ytitle)
    c.SetLogx()
    plotter.tree.Draw("abs(eta):pt>> h", "1", "colz")

    h.SetMinimum(0)

    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    txt = drawCSCLabel(text, 0.15,0.25,0.035)

    c.Print("%ssimTrack_pTvseta_2To10000GeV%s"%(plotter.targetDir + subdirectory,  plotter.ext))

    del h, c, txt



#_______________________________________________________________________________
def simTrackProperties(plotter, text):
  gStyle.SetOptStat(111010);
  outputdir = os.path.join(plotter.targetDir, subdirectory)
  draw1DPt(plotter, text)
  draw2DPtvsEta(plotter, text)

  draw_1D(outputdir, "pt", plotter.ext, plotter.tree, "Track p_{T};Track p_{T} [GeV];Entries", "(100,2,2000)", "pt", "1",text, "")
  draw_1D(outputdir, "eta", plotter.ext, plotter.tree, "Track #eta;Track #eta;Entries", "(100,-2.5,2.5)", "eta", "1",text, "")
  draw_1D(outputdir, "phi", plotter.ext, plotter.tree, "Track #phi;Track #phi [rad];Entries", "(60,-3.14159265358979312,3.14159265358979312)", "phi", "1",text, "")



