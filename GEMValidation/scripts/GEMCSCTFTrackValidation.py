import ROOT
ROOT.gROOT.SetBatch(1)
#gStyle from TStyle
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

ROOT.gStyle.SetOptStat("ou")
ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
#ROOT.gStyle.SetTitleTextColor(4)
ROOT.gStyle.SetTitleXSize(0.05)
ROOT.gStyle.SetTitleYSize(0.05)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.14)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)
#ROOT.gROOT.ForceStyle()
    
c1 = ROOT.TCanvas()
c1.SetGridx()
c1.SetGridy()
c1.SetTickx()
c1.SetTicky()

def dphiCut(h, fractionToKeep):
    """Get the dPhi cut corresponding to the fractionToKeep [95,98,99]"""

    if h.GetEntries() == 0:
        return 2.0000
    
    ax = h.GetXaxis()
    total = h.Integral()
    bin = 1
    fractionToKeep = fractionToKeep/100.
    for b in range(ax.GetNbins()):
        if (h.Integral(0,b)/total > fractionToKeep):
            bin = b - 1
            break

    ## interpolate
    x1 = ax.GetBinUpEdge(bin)
    x2 = ax.GetBinUpEdge(bin + 1)
    y1 = h.Integral(0, bin)/total
    y2 = h.Integral(0, bin + 1)/total
    x = x1 + (fractionToKeep - y1)/(y2-y1)*(x2-x1)
    return x

#_______________________________________________________
def getPurity(file,dir,den,num,xaxis,h_bins):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH1F("b1","b1",nBins,minBin,maxBin)
    b1.GetYaxis().SetRangeUser(0.60,1.02)
    b1.GetYaxis().SetTitleOffset(1.2)
    b1.GetYaxis().SetNdivisions(520)
    b1.GetYaxis().SetTitle("TFTrack GE11dphicut Efficiency")
    b1.GetXaxis().SetTitle("%s of Simtrack"%xaxis)
    b1.SetTitle("Track reco in TrackFinder"+" "*16 + "CMS Simulation Preliminary")
    b1.SetStats(0)

    h1 = ROOT.TH1F("h1","h1",nBins,minBin,maxBin)
    t.Draw("abs(%s) >> h1"%xaxis,den)
    h2 = ROOT.TH1F("h2","h2",nBins,minBin,maxBin)
    t.Draw("abs(%s) >> h2"%xaxis,num)
    e = ROOT.TEfficiency(h2,h1)
    
    b1.Draw()
    e.SetLineColor(ROOT.kRed)
    e.Draw("same")
    legend = ROOT.TLegend(0.23,0.60,0.80,0.82)
#legend.SetFillColor(ROOT.kWhite)
    legend.SetHeader("PU140,simPt > 10Gev")
#legend.AddEntry(e1,"","l")
#legend.Draw("same")
    tex = ROOT.TLatex(0.30,0.50,"PU140, p_{T}^{sim} > 15Gev")
    tex.SetTextSize(0.05)
    tex.SetNDC()
    tex.Draw("same")
	
    c1.SaveAs("TFTrack_reco_eff_%s_Pt10_GE11dphi_PU140.pdf"%xaxis)
    c1.SaveAs("TFTrack_reco_eff_%s_Pt10_GE11dphi_PU140.png"%xaxis)
#____________________________________________________________________
def draw2D(file,dir,num,xaxis,yaxis,x_bins,y_bins):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH2F("b1","b1",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b1.GetYaxis().SetTitle("%s"%yaxis)
    b1.GetXaxis().SetTitle("%s"%xaxis)
    b1.SetTitle("TFTrack reco , PU140"+" "*16 + "CMS Simulation Preliminary")
    b1.SetStats(1)
    todraw = "(%s)"%yaxis+":"+"(%s)>>b1"%xaxis
    t.Draw(todraw,num,"colz")
    b1.SetMaximum(150)
    b1.Draw("colz")
    ROOT.gPad.SetLogz() 
    legend = ROOT.TLegend(0.15,0.56,0.40,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetHeader("PU140")
#legend.AddEntry(e1,"","l")
# legend.Draw("same")
    Num = b1.Integral(1,xBins,1,yBins)
    print "number of event ", Num

    tex = ROOT.TLatex(0.15,0.30,"#splitline{p_{T}^{sim}>20GeV,#frac{(pt-trackpt)}{pt}<-0.5}{stubs in TF matcehd to simtracks, Entries %s}"%Num)
#    tex = ROOT.TLatex(0.15,0.30,"p_{T}^{sim}>20GeV, #frac{abs(pt-trackpt)}{pt}<0.5, Entries %s"%Num)
#    tex = ROOT.TLatex(0.20,0.30,"#frac{(pt-trackpt)}{pt}>0.5, Entries %s"%Num)
#    tex = ROOT.TLatex(.2,0.3,"all stubs in TF matched to simtrack ")
    tex.SetTextSize(0.05)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
	
    c1.SaveAs("TFTrack_reco_%s"%xaxis+"_VS_%s_bad2_stubsmatched_SLH17_PU140.pdf"%yaxis)
    c1.SaveAs("TFTrack_reco_%s"%xaxis+"_VS_%s_bad2_stubsmatched_SLH17_PU140.png"%yaxis)



#___________________________________________
def gemdphi(file,dir,cut,xaxis,x_bins,pt_min,pt_max):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.GetYaxis().SetTitle("counts")
    b1.GetXaxis().SetTitle("%s"%xaxis)
    b1.SetTitle("LCT reco"+" "*16 + "CMS Simulation Preliminary")
    b1.SetStats(0)
    todraw = "%s>>b1"%xaxis
    t.Draw(todraw,cut,"colz")
    b1.Draw()
    b1.SetBinContent(0,0) 
    b1.SetBinContent(xBins+1,0) 
    dphi_cut = dphiCut(b1,98)
    print "dphicut for simtrack pt %s, 98 "%pt_min, dphi_cut
    legend = ROOT.TLegend(0.15,0.46,0.45,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
    legend.SetHeader("PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max)
#legend.AddEntry(e1,"","l")
#    legend.Draw("same")
    line1 = "PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max
    line2 = "98 dphicut %f"%dphi_cut
    tex = ROOT.TLatex(0.15,0.45,line1)
    tex.SetTextSize(0.05)
    tex.SetNDC()
    tex.Draw("same")
    tex2 = ROOT.TLatex(0.15,0.35,line2)
    tex2.SetTextSize(0.05)
    tex2.SetNDC()
    tex2.Draw("same")
	
    c1.SaveAs("LCT_reco_pt_%s"%xaxis+"_%s"%pt_min+"_%s_PU140.pdf"%pt_max)
    c1.SaveAs("LCT_reco_pt_%s"%xaxis+"_%s"%pt_min+"_%s_PU140.png"%pt_max)



#_______________________________________________________________________________
if __name__ == "__main__":

#file = "PU140_10k_Pt20_2023_update_GEMCSC.root"
#   file = "PU140_200k_Pt2-50_2023_lct_GEMCSC.root"
    file = "PU140_200k_Pt2-50_GEMCSC_LCTTiming.root"
    dir = "GEMCSCAnalyzer/trk_eff_ALL"
    den = " pt>15 && eta<-1.60 && eta>-2.4 && has_tfTrack>0"
    num = " pt>15 && eta<-1.60 && eta>-2.4 && has_tfTrack>0 && passGE11"

    xaxis1 = "pt"
    Pt_bins = "(50,0,50)"
    Pt_bins2 = "(145,0,145)"
#   getPurity(file,dir,den,num,xaxis1,Pt_bins)

    xaxis2 = "eta"
    Eta_bins = "(50,1.6,2.4)"
#    getPurity(file,dir,den,num,xaxis2,Eta_bins)
    
    Eta_bins1 = "(40,-2.5,-1.5)"
    Eta_bins2 = "(40,1.5,2.5)"
    Phi_bins = "(60,-3.14,3.14)"
    DPhi_bins = "(100,-0.25,0.25)"
    DPhi12_bins = "(260,0,260)"
    TrackPt_bins = "(50,0,100)"
    xaxis = "pt"
    yaxis = "deltaphi12"
    cut1 = "pt>10 && has_tfTrack>0 "
#   draw2D(file,dir,cut1,xaxis,yaxis,Pt_bins,DPhi12_bins)
#    draw2D(file,dir,cut1,xaxis,yaxis,Pt_bins,DPhi12_bins)
    
    xaxis_pt = "pt"
    yaxis_pt = "trackpt"
    cut2 ="has_tfTrack>0"
#    draw2D(file,dir,cut2,xaxis_pt,yaxis_pt,Pt_bins,Pt_bins)
    
#for PU140, use negative endcap only 
    xaxis_tf_eta = "-eta_ME1_TF"
    yaxis_propagate_eta = "-eta_propagated_ME1"
    xaxis_tf_phi = "phi_ME1_TF"
    yaxis_propagate_phi = "phi_propagated_ME1"
    cut1 = "eta<0 && pt>20 && has_tfTrack>0 &&  phi_ME1_TF != -9 && (has_csc_sh&1)>0 && allstubs_matched_TF>0"
    cut2 = "eta<0 && pt>20 && has_tfTrack>0 &&  phi_ME2_TF != -9 && (has_csc_sh&2)>0 && allstubs_matched_TF>0"
    cut3 = "eta<0 && pt>20 && has_tfTrack>0 &&  phi_ME3_TF != -9 && (has_csc_sh&4)>0 && allstubs_matched_TF>0"
    cut4 = "eta<0 && pt>20 && has_tfTrack>0 &&  phi_ME1_TF != -9 && (pt-trackpt)/pt<-0.5 && allstubs_matched_TF>0"
    cut5 = "eta<0 && has_tfTrack>0 &&  phi_ME2_TF != -9 && abs(pt-trackpt)/pt<0.5 && allstubs_matched_TF>0"
    cut6 = "eta<0 && has_tfTrack>0 &&  phi_ME3_TF != -9 && abs(pt-trackpt)/pt<0.5 && allstubs_matched_TF>0"
    cut7 = "pt>20 && has_tfTrack>0 &&  phi_ME1_TF != -9 && abs(pt-trackpt)/pt<0.5 && allstubs_matched_TF>0"
    cut8 = "pt>20 && has_tfTrack>0 &&  phi_ME2_TF != -9 && abs(pt-trackpt)/pt<0.5 && allstubs_matched_TF>0"
    cut9 = "pt>20 && has_tfTrack>0 &&  phi_ME3_TF != -9 && abs(pt-trackpt)/pt<0.5 && allstubs_matched_TF>0"
    cut10 = "eta<0 && pt>20 && has_tfTrack>0 && (abs(phi_ME1_TF-phi_propagated_ME1)>0.025 || abs(phi_ME2_TF-phi_interStat12)>0.025) && allstubs_matched_TF>0"
    cut11 = "eta<0 && pt>20 && has_tfTrack>0 && (abs(phi_ME3_TF-phi_interStat23)>0.025 || abs(phi_ME2_TF-phi_interStat12)>0.025) && allstubs_matched_TF>0"
    cut12 = "eta<0 && pt>20 && has_tfTrack>0 && (abs(phi_ME3_TF-phi_interStat13)>0.025 || abs(phi_ME2_TF-phi_interStat12)>0.025) && allstubs_matched_TF>0"
    
    DeltaPt_bins = "(150,-100,50)"
    DeltaPhi_bins = "(40,-0.15,0.15)"
#draw2D(file,dir,cut1,xaxis_tf_eta,yaxis_propagate_eta,Eta_bins2,Eta_bins2)
# draw2D(file,dir,cut1,xaxis_tf_phi,yaxis_propagate_phi,Phi_bins,Phi_bins)
#    draw2D(file,dir,cut1,"-eta_ME1_TF","-eta_propagated_ME1",Eta_bins2,Eta_bins2) 
#    draw2D(file,dir,cut1,"phi_ME1_TF","phi_propagated_ME1",Phi_bins,Phi_bins)
#draw2D(file,dir,cut2,"-eta_ME2_TF","-eta_propagated_ME2",Eta_bins2,Eta_bins2) 
#   draw2D(file,dir,cut2,"phi_ME2_TF","phi_propagated_ME2",Phi_bins,Phi_bins)
#   draw2D(file,dir,cut3,"-eta_ME3_TF","-eta_propagated_ME3",Eta_bins2,Eta_bins2)
#    draw2D(file,dir,cut3,"phi_ME3_TF","phi_propagated_ME3",Phi_bins,Phi_bins)
#    draw2D(file,dir,cut4,"phi_ME1_TF-phi_propagated_ME1","pt-trackpt",DeltaPhi_bins,DeltaPt_bins)
#draw2D(file,dir,cut5,"phi_ME2_TF-phi_propagated_ME2","pt-trackpt",DeltaPhi_bins,DeltaPt_bins)
#    draw2D(file,dir,cut6,"phi_ME3_TF-phi_propagated_ME3","pt-trackpt",DeltaPhi_bins,DeltaPt_bins)
#    draw2D(file,dir,cut5,"phi_ME2_TF-phi_interStat12","pt-trackpt",DeltaPhi_bins,DeltaPt_bins)
#    draw2D(file,dir,cut6,"phi_ME3_TF-phi_interStat23","pt-trackpt",DeltaPhi_bins,DeltaPt_bins)
    draw2D(file,dir,cut4,"phi_ME1_TF-phi_propagated_ME1","phi_ME2_TF-phi_interStat12",DeltaPhi_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut5,"phi_ME2_TF-phi_interStat12","phi_ME3_TF-phi_interStat23",DeltaPhi_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut6,"phi_ME2_TF-phi_interStat12","phi_ME3_TF-phi_interStat13",DeltaPhi_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut10,"phi_ME1_TF-phi_propagated_ME1","phi_ME2_TF-phi_interStat12",DeltaPhi_bins,DeltaPhi_bins)
#   draw2D(file,dir,cut11,"phi_ME2_TF-phi_interStat12","phi_ME3_TF-phi_interStat23",DeltaPhi_bins,DeltaPhi_bins)
#draw2D(file,dir,cut12,"phi_ME2_TF-phi_interStat12","phi_ME3_TF-phi_interStat13",DeltaPhi_bins,DeltaPhi_bins)
 
    
#    draw2D(file,dir,cut4,"pt","phi_ME1_TF-phi_propagated_ME1",Pt_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut5,"pt","phi_ME2_TF-phi_propagated_ME2",Pt_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut5,"pt","phi_ME2_TF-phi_interStat12",Pt_bins,DeltaPhi_bins)
#    draw2D(file,dir,cut6,"pt","phi_ME3_TF-phi_interStat23",Pt_bins,DeltaPhi_bins)
#   draw2D(file,dir,cut6,"pt","phi_ME3_TF-phi_propagated_ME3",Pt_bins,DeltaPhi_bins)
#   draw2D(file,dir,cut6,"pt","phi_ME3_TF-phi_interStat13",Pt_bins,DeltaPhi_bins)
    
    xaxis_eta = "eta"
    yaxis_eta = "tracketa"
#    cut2 =" has_tfTrack>0"
#    draw2D(file,dir,cut2,xaxis_eta,yaxis_eta,Eta_bins,Eta_bins)

    cut3 = "pt>10 && station==1 && abs(eta)>2.05"
    pt_min = "10"
    pt_max = "50"
    gemdphige11 = "dphi"
    DPhi_bins = "(100,-0.5,0.5)"
#    gemdphi(file,dir,cut3,gemdphige11,DPhi_bins,pt_min,pt_max)
