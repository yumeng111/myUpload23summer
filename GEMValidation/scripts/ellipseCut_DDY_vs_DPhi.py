# Run quiet mode
import sys
sys.argv.append( '-b' )
import ROOT 
ROOT.gROOT.SetBatch(1)
#from Helpers import *
ROOT.gErrorIgnoreLevel=1001
from ROOT import * 
import random
import os
import numpy as np

ROOT.gROOT.SetBatch(1)
ROOT.gStyle.SetStatW(0.07)
ROOT.gStyle.SetStatH(0.06)

ROOT.gStyle.SetOptStat(111110)

#ROOT.gStyle.SetErrorX(0)
#ROOT.gStyle.SetErrorY(0)

ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.10)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)

ROOT.gStyle.SetMarkerStyle(1)



def frange(end,start=0,inc=0,precision=1):
    """A range function that accepts float increments."""
    import math

    if not start:
        start = end + 0.0
        end = 0.0
    else: end += 0.0

    if not inc:
        inc = 1.0
    count = int(math.ceil((start - end) / inc))

    L = [None] * (count+1)

    L[0] = end
    for i in (xrange(1,count)):
        L[i] = L[i-1] + inc
    L[count] = start
    return L

## draw an ellipse that includes 95% of the entries

signalAcceptance = 0.90

def getEllipse(x,y,a,b):
  return x*x/(a*a) + y*y/(b*b)

def passEllipse(x,y,a,b):
    return getEllipse(x,y,a,b) <= 1

def failEllipse(x,y,a,b):
    return getEllipse(x,y,a,b) > 1

def getBackgroundRejectionEllipse(a_axis, b_axis, signalHist, backgroundHist):
    if signalHist.GetEntries()==0 or backgroundHist.GetEntries()==0:
    	print "warning!!! entries (S and B) ",signalHist.GetEntries(),backgroundHist.GetEntries()
    	return (1.0,0)
    #print "signal and bg, integral/entris ", signalHist.Integral() / signalHist.GetEntries(), backgroundHist.Integral() / backgroundHist.GetEntries()
    signalEntriesTotal = signalHist.Integral()*1.0
    backgroundEntriesTotal = backgroundHist.Integral()*1.0

    entriesInEllipseSignal  = 0
    entriesOutEllipseBackground  = 0

    signalAcceptanceFactor = 0
    backgroundRejectionFactor = 0
    
    ## loop on entries in histogram
    for j in range(1,signalHist.GetNbinsX()+1):
        for k in range(1,signalHist.GetNbinsY()+1):
            
            ## get the bin centers
            signal_x = signalHist.GetXaxis().GetBinCenter(j)
            signal_y = signalHist.GetYaxis().GetBinCenter(k)

            background_x = backgroundHist.GetXaxis().GetBinCenter(j)
            background_y = backgroundHist.GetYaxis().GetBinCenter(k)

            ## signal passes
            if passEllipse(signal_x, signal_y, a_axis, b_axis):
                entriesInEllipseSignal += signalHist.GetBinContent(j,k)

            ## background fails
            if failEllipse(background_x, background_y, a_axis, b_axis):
                entriesOutEllipseBackground += backgroundHist.GetBinContent(j,k)

            ## current signal acceptance
            signalAcceptanceFactor = entriesInEllipseSignal / signalEntriesTotal

            ## current background rejection
            backgroundRejectionFactor = entriesOutEllipseBackground / backgroundEntriesTotal
            #if backgroundRejectionFactor<0.50:
            #    break
            #print j,k,signalAcceptanceFactor,backgroundRejectionFactor

    return signalAcceptanceFactor, backgroundRejectionFactor
            



def get_proptionality_factor(eta, npar):
	slope=0.0
	slopes_1 = [0,0.645, 0.852,0]
	slopes_2 = [0.630, .364, .541, .325]
	#slopes_1 = [1.279, 0.6457, 1.001, 0.5252]
	#slopes_2 = [0.648, 0.3542, 0.5636, 0.3217]
	if (abs(eta)<1.6):
		slope = slopes_1[npar]
	if (abs(eta)>=1.6):
		slope = slopes_2[npar]
    
	return slope


def drawEllipse(hist, hist2, a, b, eff1, eff2, xtitle, ytitle,st_title, text, picname):
    gStyle.SetTitleBorderSize(0);
    gStyle.SetPadLeftMargin(0.126);
    gStyle.SetPadRightMargin(0.04);
    gStyle.SetPadTopMargin(0.06);
    gStyle.SetPadBottomMargin(0.13);
    gPad.SetTickx(1)
    gPad.SetTicky(1)
    el2 = TEllipse(0,0,a,b);
    el2.SetLineColor(kBlack);
    #el2.SetLineWidth(3);
    el2.SetFillStyle(4000)
    el2.SetPhimin(0)
    el2.SetPhimax(90)
    c = TCanvas("c_%d_%d"%(int(a*100),int(b*100)),"c_%d_%d"%(int(a*100),int(b*100)),900,400)
    #c.Clear()
    c.Divide(2,1)
    c.cd(1)
    hist.Draw("colz")
    hist.SetTitle(st_title[1])

    el2.Draw("same")

    tex1 = ROOT.TLatex(0.25,.7,"%s"%(text))
    #tex1 = TLatex(0.2,.7,"%s"%(st_title))
    tex1.SetNDC()
    tex3 = TLatex(0.15, 0.8, "a=%.3f, b=%.3f, Acceptance %.2f"%(a,b,eff1))
    tex3.SetTextSize(0.05)
    tex3.SetNDC()
    tex3.Draw("same")
    tex1.Draw("same")
    c.cd(2)
    hist2.Draw("colz")
    hist2.SetTitle(st_title[0])
    el2.Draw("same")
    tex2 = TLatex(0.15, 0.8, "a=%.3f, b=%.3f, Rejection %.2f"%(a,b,eff2))
    tex2.SetTextSize(0.05)
    tex2.SetNDC()
    tex2.Draw("same")
    c.cd()

    
    c.SaveAs(picname+ "_ellipse.png")

def loopEllipse(filedir, treename,fraction, astart, bstart, xaxis, yaxis,x_bins, y_bins,xtitle, ytitle,st_title, etamin, etamax, cuts,text,picname):

    gStyle.SetOptFit(0111)
    gStyle.SetOptStat(0)
    if (etamin<1.6):
    	ring = 2
    if (etamin>=1.6):
        ring = 1

    chain = TChain(treename)
    chain2 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME1%d"%ring)
    chain3 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME2%d"%ring)
    chain4 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME3%d"%ring)
    if os.path.isdir(filedir[0]):
    	  ls = os.listdir(filedir[0])
    	  for x in ls:
		x = filedir[0][:]+x
    		if os.path.isdir(x):
			continue
		chain.Add(x)
		chain2.Add(x)
		chain3.Add(x)
		chain4.Add(x)
    elif os.path.isfile(filedir0[0]):
	  chain.Add(filedir[0])
	  chain2.Add(filedir[0])
	  chain3.Add(filedir[0])
	  chain4.Add(filedir[0])
    else:
	  print " it is not file or dir ", filedir[0]

    chain.AddFriend(chain2)
    chain.AddFriend(chain3)
    chain.AddFriend(chain4)
    
    chain1 = TChain(treename)
    chain12 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME1%d"%ring)
    chain13 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME2%d"%ring)
    chain14 = TChain("GEMCSCAnalyzer/trk_eff_CSC_ME3%d"%ring)
    if os.path.isdir(filedir[1]):
    	  ls = os.listdir(filedir[1])
    	  for x in ls:
		x = filedir[1][:]+x
    		if os.path.isdir(x):
			continue
		chain1.Add(x)
		chain12.Add(x)
		chain13.Add(x)
		chain14.Add(x)
    elif os.path.isfile(filedir[1]):
	  chain1.Add(filedir[1])
	  chain12.Add(filedir[1])
	  chain13.Add(filedir[1])
	  chain14.Add(filedir[1])
    else:
	  print " it is not file or dir ", filedir[1]

    chain1.AddFriend(chain12)
    chain1.AddFriend(chain13)
    chain1.AddFriend(chain14)

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    xbinwidth = (xmaxBin-xminBin)/xBins
    ybinwidth = (ymaxBin-yminBin)/yBins
    a_range = frange(astart, xmaxBin, xbinwidth)
    b_range = frange(bstart, ymaxBin, ybinwidth)
    #print "arange ",a_range, " brange ",b_range
    print "arange astart ",astart, " xbinwidth ", xbinwidth ," xbins ",x_bins, " ybins ",y_bins

    todrawb0 = "%s"%yaxis+":"+"%s>>b0"%xaxis
    todrawb1 = "%s"%yaxis+":"+"%s>>b1"%xaxis
    b0 = TH2F("b0","b0",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b0.GetXaxis().SetTitle("%s"%xtitle)
    b0.GetYaxis().SetTitle("%s"%ytitle)
    #b0.SetTitle("%s Vs %s,%s"%(ytitle, xtitle, st_title)) 
    #b0.SetTitleSize(0.05)
    b0.SetStats(1)
    chain.Draw(todrawb0,cuts[0],"colz")#background
    print "background todraw ",todrawb0, " cuts ", cuts[0]

    b1 = TH2F("b1","b1",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b1.GetXaxis().SetTitle("%s"%xtitle)
    b1.GetYaxis().SetTitle("%s"%ytitle)
    #b1.SetTitle("%s Vs %s,%s"%(ytitle, xtitle, st_title)) 
    #b1.SetTitleFont(62)
    #b1.SetTitleSize(0.05)
    #b1.SetMaximum(30)
    b1.SetStats(1)
    chain1.Draw(todrawb1,cuts[1],"colz")#signal
    print "signal todraw ",todrawb1, " cuts ", cuts[1]
    if (b1.GetEntries()<1 or b0.GetEntries() <1 ):
    	print "signal entries ",b1.GetEntries(), " bg entries ",b0.GetEntries() 
    print "signal integral/entris ", b1.Integral() / b1.GetEntries(),"   Bg ", b0.Integral() / b0.GetEntries()
    preselected_axes_signalAcc_backRej = []
    n=0
    fraction = fraction/100.0
    lena = len(a_range)
    lenb = len(b_range)
    maxRej = 0
    maxAccept = 0
    max_a = xmaxBin
    max_b = ymaxBin
    max_b_lowedge = 0.0
    max_b_highedge = ymaxBin
    b_axis = max_b_highedge
    for a_axis in a_range:
        max_b_lowedge = 0.0	
	signalAcceptanceFactor, backgroundRejectionFactor = getBackgroundRejectionEllipse(a_axis, b_axis, b1, b0)
        #print " a ", a_axis, " b ", b_axis, " bhigh ",max_b_highedge, " blow ", max_b_lowedge, " signal ", signalAcceptanceFactor, " bg ",backgroundRejectionFactor
    	if signalAcceptanceFactor < fraction:
		continue
	step = max_b_highedge - max_b_lowedge
	while (step> ybinwidth):
        #for b_axis in b_range:
	    b_axis = (max_b_highedge+max_b_lowedge)/2.0
	    n = n+1
            signalAcceptanceFactor, backgroundRejectionFactor = getBackgroundRejectionEllipse(a_axis, b_axis, b1, b0)
            #print "n ", n," bstep ",step, " a ", a_axis, " b ", b_axis, " signal ", signalAcceptanceFactor, " bg ",backgroundRejectionFactor
            if signalAcceptanceFactor > fraction:
	    	max_b_highedge = b_axis
	    	if backgroundRejectionFactor > maxRej:
            		print "n ", n," bstep ",step, " a ", a_axis, " b ", b_axis, " signal ", signalAcceptanceFactor, " bg ",backgroundRejectionFactor
			maxRej = backgroundRejectionFactor
			maxAccept = signalAcceptanceFactor
			max_a = a_axis
			max_b = b_axis
    	    		drawEllipse(b1, b0, a_axis, b_axis, signalAcceptanceFactor, backgroundRejectionFactor, xtitle, ytitle,st_title, text, picname+"_%d"%n)
                #preselected_axes_signalAcc_backRej.append([a_axis, b_axis, signalAcceptanceFactor, backgroundRejectionFactor])
	    else:
     		max_b_lowedge = b_axis	     
	    step = max_b_highedge - max_b_lowedge	
	max_b_highedge = b_axis
	
    
    signalAcceptanceFactor, backgroundRejectionFactor = getBackgroundRejectionEllipse(a_axis, b_axis, b1, b0)
    drawEllipse(b1, b0, a_range[lena-1], b_range[lenb-1], signalAcceptanceFactor, backgroundRejectionFactor, xtitle, ytitle,st_title, text, picname+"_%d_last"%(lena*lenb))
    #ellipes = "%s*%s/(%f*%f)+%s*%s/(%f*%f)<1.0"%(xaxis, xaxis, max_a, max_a, yaxis, yaxis, max_b, max_b)
    print "max_a ",max_a," max_b ",max_b ," signalAcceptanceFactor ",maxAccept," backgroundRejectionFactor ",maxRej
    return (max_a, max_b)
            

def gethist1D(chain,den, todraw="pt"):
	
    	h1 = ROOT.TH1F("h1","h1",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h1"%todraw,den)
	#print "gethist1D, den ",den
	#h1.Print("ALL")
	return h1
		
def makeEffplot_v2(filedirs,todraw, treename0, den, num, etamin, etamax, xtitle,ytitle,leg, txt,picname):
	
	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.045)
	b1.SetTitle("CMS Simulation Preliminary"+"  "*10+" PU0, 14TeV")
	
	
	Teffs = []
	hdens = []
	hnums = []
	npars = 4
	Upperlimits = [0 for x in range(npars)]
        color = [ROOT.kBlue, ROOT.kRed, ROOT.kMagenta+2, ROOT.kGreen+2,ROOT.kCyan+2]
        maker = [20,21,22,23,33]
	print "filedirs ",filedirs
	
    	if (etamin<1.6):
    		ring = 2
    	if (etamin>=1.6):
        	ring = 1
	legend = ROOT.TLegend(0.45,0.15,0.9,0.5)
	legend.SetFillColor(ROOT.kWhite)
	legend.SetTextFont(42)
	legend.SetHeader("Hybrid algo")
	muons = ["Prompt Muons","Displaced Muons, 5<|d_{xy}|<50cm"]
	for n in range(len(filedirs)):
		print "n ",n
		hden = ROOT.TH1F("hden%d"%n,"hden%d"%n,len(ptbins)-1, ptbins)
		hnum = ROOT.TH1F("hnum%d"%n,"hnum%d"%n,len(ptbins)-1, ptbins)
		for i in range(len(ptbins)):
			hden.SetBinContent(i, 0)
			hnum.SetBinContent(i,.0)
		#Teffs.append(ROOT.TEfficiency(hnum, hden))
		hdens.append(hden)
		hnums.append(hnum)
		filedir = filedirs[n]
    		chain = ROOT.TChain(treename0)
#		treename2 = "HLTBendingAngle/trk_eff_csc_ME21" 
   		chain2 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME1%d"%ring)
   		chain3 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME2%d"%ring)
   		chain4 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME3%d"%ring)
    		if os.path.isdir(filedir):
    			ls = os.listdir(filedir)
    	 		for x in ls:
				x = filedir[:]+x
    				if os.path.isdir(x):
				    continue
				chain.Add(x)
				chain2.Add(x)
				chain3.Add(x)
				chain4.Add(x)
    		elif os.path.isfile(filedir):
			chain.Add(filedir)
			chain2.Add(filedir)
			chain3.Add(filedir)
			chain4.Add(filedir)
    		else:
	  		print " it is not file or dir ", filedir
		chain.AddFriend(chain2)
		chain.AddFriend(chain3)
		chain.AddFriend(chain4)
		hdens[n].Add(gethist1D(chain, den[n],todraw))
		hnums[n].Add(gethist1D(chain, den[n]+" && %s"%(num[n]), todraw))


	c1 = ROOT.TCanvas()
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1.SetStats(0)
	b1.Draw()
	for n in range(len(filedirs)):
		Teffs.append(ROOT.TEfficiency(hnums[n],hdens[n]))	
        	SetOwnership(Teffs[n], False)
		m = len(Teffs)-1
		Teffs[m].SetLineColor(color[n])
		Teffs[m].SetMarkerColor(color[n])
		Teffs[m].SetMarkerStyle(maker[n])
		Teffs[m].Draw("same")
		legend.AddEntry(Teffs[n],"%s"%leg[n],"pl")
	#print "Teffs ",Teffs
	#Teffs[0].Print("ALL")
	legend.Draw("same")

	tex = ROOT.TLatex(0.45,0.57,"%s"%txt)
	#tex = ROOT.TLatex(0.45,0.57,"#splitline{%s}{%d%% eff at %d [GeV]}"%(txt,fractionToKeep,pt))
	#tex = ROOT.TLatex(0.45,0.57,"#splitline{%s}{check the sign of #Delta Y_{12} and #Delta Y_{23}}"%(txt))
	tex.SetTextSize(0.05)
	tex.SetTextFont(62)
	tex.SetNDC()
	tex.Draw("same")
	#c1.Update()
	c1.SaveAs("%s.png"%(picname))
	c1.SaveAs("%s.pdf"%(picname))
	c1.SaveAs("%s.C"%(picname))
        return Teffs
	

def makeplots(Teffs, legs, text, picname):

	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.045)
	b1.SetTitle("CMS Simulation Preliminary"+"  "*10+" PU0, 14TeV")
	
	
	c1 = ROOT.TCanvas()
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        color = [ROOT.kBlue, ROOT.kRed, ROOT.kMagenta+2, ROOT.kGreen+2,ROOT.kCyan+2]
        maker = [20,21,22,23,33]
	legend = ROOT.TLegend(0.45,0.15,0.93,0.5)
	legend.SetFillColor(ROOT.kWhite)
	legend.SetTextFont(42)
	#legend.SetHeader("%"%legheader)
	b1.SetStats(0)
	b1.Draw()
	for n in range(len(Teffs)):
		Teffs[n].SetLineColor(color[n])
		Teffs[n].SetMarkerColor(color[n])
		Teffs[n].SetMarkerStyle(maker[n])
		Teffs[n].Draw("same")
		legend.AddEntry(Teffs[n],"%s"%legs[n],"pl")
	legend.Draw("same")

	tex = ROOT.TLatex(0.5,0.7,"%s"%text)
	tex.SetTextSize(0.05)
	tex.SetTextFont(62)
	tex.SetNDC()
	tex.Draw("same")
	#c1.Update()
	c1.SaveAs("%s.png"%(picname))
	c1.SaveAs("%s.pdf"%(picname))
	c1.SaveAs("%s.C"%(picname))

##############################################################3


treename = "GEMCSCAnalyzer/trk_eff_CSC_ALL"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0828/160828_200750/0000/"
filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0828/160828_222813/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0828/160828_215302/0000/"
filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0828/160828_223815/0000/"

binLow = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0,14.0,16.0,18.0,20.0,24.0,28.0,32.0,36.0,42.0,50.0]
ptbins = np.asarray(binLow)
evenodds = ["odd,even","odd,odd","even,even","even,odd","all pairs"]
netas = [1.2,1.4,1.6,1.8,2.0,2.2]
netas = [1.6,1.8,2.0,2.2]
allnpar = [0,1,2,3]
Pts = [10, 20]
Pts_1 = [5,7]
filedirs_v6 = [filedir16, filedir46]
for neta in range(len(netas)-1):
   for npt in range(len(Pts)):	
       if (netas[neta]<0 or netas[neta+1]<0):
	    continue
       Tefftotal = []
       legs = ["Prompt muon", "Displaced Muon, 10<|d_{xy}|<50"]
       fraction  =90
       for npar in allnpar:

        pt = Pts[npt]
	pt1 = Pts_1[npt]
	if (netas[neta]<1.6 and (npar==0 or npar ==3)):
	    continue
	
	if (netas[neta]<1.6):
    		ring = 2
	if (netas[neta]>=1.6):
    		ring = 1
	me11 = evenodds[npar].split(',')[0]
	me21 = evenodds[npar].split(',')[1]
	hasfitcut = "&& fabs(trk_eff_CSC_ME1%d.phi_layer3_fit_%s)<4 && fabs(trk_eff_CSC_ME2%d.phi_layer3_fit_%s)<4 && fabs(trk_eff_CSC_ME3%d.phi_layer3_fit_%s)<4"%(ring, me11, ring, me21, ring, me21)
	chambers = "ME1%d %s,ME2%d %s,ME3%d %s"%(ring, me11, ring, me21, ring, me21)
	slope = get_proptionality_factor(netas[neta], npar)
    	#deltay12 = "deltay12_fit"
    	#deltay23 = "deltay23_fit"
    	#checksign = " && (%s*%s>0 || (fabs(%s)<6 && fabs(%s)<3))"%(deltay12, deltay23, deltay12, deltay23)
	deltay12 = ["deltay12_fit","deltay12_fit"]
	deltay23 = ["deltay23_fit","deltay23_fit"]
	st_title = ["Prompt muon, 2<p_{T}<%d"%pt1, "Displaced Muon, 10<|d_{xy}|<50, p_{T}>%d"%pt]
	xaxis = "fabs(deltay23_fit-deltay12_fit*%f)"%(slope)#deltadeltay
	#yaxis = "fabs(csc_bending_angle12_xfactor_L1_2)"#deltaphi
	yaxis = "fabs(csc_bending_angle12_xfactor_L1_1)"#deltaphi, only in high eta region, CSC only
    	#checkvalue = "  &&fabs(phiM_st1_L1_2)>0 && fabs(phiM_st2_L1_2)>0 && fabs(deltay23_fit)>0 && fabs(deltay12_fit)>0 && %s<%f && %s<%f && %s>0 && %s>0"%(xaxis, 20., yaxis, .5, xaxis, yaxis)
    	checkvalue = " &&fabs(phiM_st1_L1_1)>0 && fabs(phiM_st2_L1_1)>0 && fabs(deltay23_fit)>0 && fabs(deltay12_fit)>0 && %s<%f && %s<%f && %s>0 && %s>0"%(xaxis, 20., yaxis, .5, xaxis, yaxis)
	cuts =  ["hasSt1St2St3 && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f"%(netas[neta],netas[neta+1])+hasfitcut+checkvalue, "hasSt1St2St3 && fabs(deltay12_fit)<50 &&  fabs(deltay23_fit)<50 && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f && fabs(genGdMu_dxy)>10 && fabs(genGdMu_dxy)<50 && fabs(genGdMu_dR)<0.1"%(netas[neta],netas[neta+1])+hasfitcut+checkvalue]
	dens_L1 = [cuts[0]+" && pt>2 && pt<%f"%(pt1), cuts[1]+"&& pt>%f"%(pt)]
	x_bins = "(200, 0, 20.0)"
	y_bins = "(120, 0,.6)"
	xtitle = "#Delta#Delta Y"
	ytitle = "#Delta#phi_{dir}"
	text = "#splitline{%s, with CSC only}{%.1f<|#eta|<%.1f, p_{T}>%d GeV}"%(chambers,netas[neta],netas[neta+1],pt)
	astart = .6
	bstart = .0# not used 
	(maxa, maxb) = loopEllipse(filedirs_v6, treename, fraction, astart, bstart, xaxis, yaxis,x_bins, y_bins,xtitle, ytitle,st_title, netas[neta], netas[neta+1], dens_L1,text,"Profile_Ellipse_PT_0828_v2/GEMCSC_ctau0andctau1000_profile_20160828_pt%d_ptbg%d_fraction%d_st2eta%dto%d_npar%d_ME21CSConly"%(pt, pt1, fraction, int(netas[neta]*10),int(netas[neta+1]*10), npar))
	ellipes = "%s*%s/(%f*%f)+%s*%s/(%f*%f)<1.0"%(xaxis, xaxis, maxa, maxa, yaxis, yaxis, maxb, maxb)
	Teffs = makeEffplot_v2(filedirs_v6, "pt", treename, cuts, [ellipes, ellipes], netas[neta], netas[neta+1],"true muon p_{T} GeV","Efficiency",legs ,text,"Hybrid_Ellipse_PT_0828_v2/GEMCSC_ctau0andctau1000_eff_20160828_pt%d_ptbg%d_fraction%d_st2eta%dto%d_npar%d_ME21CSConly"%(pt, pt1, fraction, int(netas[neta]*10),int(netas[neta+1]*10), npar))
	Tefftotal.append(Teffs)
       print "Tefftotal len ",len(Tefftotal),Tefftotal
       Teff0 = Tefftotal[0][0]
       Teff1 = Tefftotal[0][1]
       text_h = "#splitline{Hybrid algorithm, with CSC only}{%.1f<|#eta|<%.1f, p_{T}>%d GeV}"%(netas[neta],netas[neta+1], pt)
       for xpar in range(len(Tefftotal)-1):
   	Teff0.Add(Tefftotal[xpar+1][0])		   
   	Teff1.Add(Tefftotal[xpar+1][1])		   
       makeplots([Teff0, Teff1], legs, text_h,"Hybrid_Ellipse_PT_0828_v2/GEMCSC_ctau0andctau1000_eff_20160828_pt%d_ptbg%d_fraction%d_St2eta%dto%d_allnpar_ME21CSConly"%(pt, pt1, fraction,  int(netas[neta]*10),int(netas[neta+1]*10)))
       


