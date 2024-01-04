import ROOT
import random
import os
import sys
import numpy as np
import array
import math

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


xmin=0
xmax=60
xbins=30
BINM=22
#binLow = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0,14.0,16.0,18.0,20.0,24.0,28.0,32.0,36.0,42.0,50.0,60.0]
binLow = [0.0,1.0,2.0,3.0,4.0,5.0,6.0,7.0,8.0,9.0,10.0,12.0,14.0,16.0,18.0,20.0,24.0,28.0,32.0,36.0,42.0,50.0]
ptbins = np.asarray(binLow)

def getslopeandintercept(npar, eta):
	if (npar==0):
		if ( 1.6<abs(eta) and abs(eta)<1.8):
			return (4.408,8.693)
		elif(1.8<abs(eta) and abs(eta)<2.0):
			return (7.035,11.794)
		elif(2.0<abs(eta) and abs(eta)<2.2):
			return (8.741,10.506)
	if (npar==1):
		if ( 1.6<abs(eta) and abs(eta)<1.8):
			return (4.399, 8.467)
		elif(1.8<abs(eta) and abs(eta)<2.0):
			return (7.143, 11.713)
		elif(2.0<abs(eta) and abs(eta)<2.2):
			return (8.829, 11.387)
	if (npar==2):
		if ( 1.6<abs(eta) and abs(eta)<1.8):
			return (5.233,10.694)
		elif(1.8<abs(eta) and abs(eta)<2.0):
			return (8.037,13.710)
		elif(2.0<abs(eta) and abs(eta)<2.2):
			return (9.120,11.393)
	if (npar==3):
		if ( 1.6<abs(eta) and abs(eta)<1.8):
			return (4.841,9.532)
		elif(1.8<abs(eta) and abs(eta)<2.0):
			return (7.862,12.205)
		elif(2.0<abs(eta) and abs(eta)<2.2):
			return (8.893,9.775)
	return (0.0,0.0)	
	
def gethist1D(chain,den, todraw="pt"):
	
    	h1 = ROOT.TH1F("h1","h1",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h1"%todraw,den)
	#print "gethist1D, den ",den
	#h1.Print("ALL")
	return h1
		


def getAllEff(chain,den, num, picname, txt, todraw="pt"):
	print "den ",den, " num ",num ," todraw ",todraw
	
	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(-0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.045)
	b1.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
	b1.SetStats(0)

#	treename2 = "HLTBendingAngle/trk_eff_csc_ME21" 
    	h1 = ROOT.TH1F("h1","h1",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h1"%todraw,den)
    	h2 = ROOT.TH1F("h2","h2",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h2"%todraw,num+"&&"+den)
	#h1.Print("ALL")
	#h2.Print("ALL")
    	e = ROOT.TEfficiency(h2,h1)
	"""
    	c0 = ROOT.TCanvas("c0","c0")
    	c0.SetGridx()
    	c0.SetGridy()
    	c0.SetTickx()
    	c0.SetTicky()
    	b1.Draw()
    	e.Draw("esame")
        tex1 = ROOT.TLatex(0.3,.4,"%s"%txt)
        tex1.SetNDC()
        tex1.Draw("same")
    	c0.SaveAs("%s"%picname+".png")
	"""
	return e

def getUpperlimit(h, fractionToKeep):

    ## just a safety to prevent it from crashing
    if h.GetEntries() == 0:
        return 0.0000
    
    ax = h.GetXaxis()
    total = h.Integral()
    bin = 1
    fractionToKeep = fractionToKeep/100.0
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

#_____________________________________________________________

def FitHistoFunction(b1, xBins, xmin, xmax, yBins, ymin, ymax, fraction, printa): 
    
    fraction = fraction/100.0
    Xs = array.array('d')
    Ys = array.array('d')
    exl = array.array('d')
    eyl = array.array('d')
    exh = array.array('d')
    eyh = array.array('d')
    xBins = b1.GetXaxis().GetNbins()
    yBins = b1.GetYaxis().GetNbins()
    xmin = b1.GetXaxis().GetXmin()
    xmax = b1.GetXaxis().GetXmax()
    ymin = b1.GetYaxis().GetXmin()
    ymax = b1.GetYaxis().GetXmax()
    ybinwidth = (ymax-ymin)/(1.0*yBins) 
    xbinwidth = (xmax-xmin)/(1.0*yBins) 
    Totfreq = b1.Integral(0,xBins+1,0,yBins+1)
    if Totfreq < 10:
	return (0,0,0,0,0,0)
    print "Totfreq ",Totfreq," fraction ",fraction
    for x in range(1, xBins+1):

        totalfreq = b1.Integral(x,x,0,yBins+1)
      	probSum = array.array('d', [fraction])
      	q = array.array('d', [0.0]*len(probSum))
      	totalfreq = b1.Integral(x,x,0,yBins+1)
      ## do not compute quantiles for empty histograms!!!
      	if totalfreq <10:
        	continue
      	tempHist = b1.ProjectionY("bin1",x,x)
      	tempHist.GetQuantiles(len(probSum), q, probSum)
        
	Xs.append(b1.GetBinCenter(x))
	Ys.append(q[0])
	exl.append(xbinwidth/2.0)
	exh.append(xbinwidth/2.0)
	eyl.append(fabs(q[0])*1.0/math.sqrt(totalfreq*(1-fraction)))  
	eyh.append(fabs(q[0])*1.0/math.sqrt(totalfreq*(fraction)))  
    	if printa>0:	print "x ",b1.GetBinCenter(x)," y ",y," exl ",xbinwidth/2.," exh ",xbinwidth/2," eyl ",math.sqrt(totalfreq*(1-fraction))," eyh ",math.sqrt(totalfreq*(fraction))

    return (Xs, Ys, exl, exh, eyl, eyh)                               #Return the histogram 1D 
                             #Return the histogram 1D 



	
def getAllEffV2(chain,den, num,pt,fractionToKeep, todraw="pt"):
	
	hist = ROOT.TH1F("hist","hist",500,0.0,0.2)
        chain.Draw("TMath::Abs(%s)>>hist"%(num),den+"&& pt>%f && pt<%f"%(pt-1,pt+1))
	upperlim = getUpperlimit(hist, fractionToKeep)
	#e = ROOT.TEfficiency()	
    	h1 = ROOT.TH1F("h1","h1",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h1"%todraw,den)
    	h2 = ROOT.TH1F("h2","h2",len(ptbins)-1, ptbins)
    	chain.Draw("fabs(%s) >> h2"%todraw,den+"&& fabs(%s)<%f"%(num, upperlim))
	print "den ",den," + num ","fabs(%s)<%f"%(num, upperlim)
	#h2.Sumw2()
	#h1.Sumw2()
    	e = ROOT.TEfficiency(h2,h1)
	return e

def DrawProfileAndScatter_pT(filedir,treename,fraction, xaxis,yaxis0, yaxis,x_bins,y_bins0, y_bins,xtitle,ytitle0, ytitle,st_title, etamin, etamax, cuts,text,picname):
    
    ROOT.gStyle.SetOptFit(0111)
    ROOT.gStyle.SetOptStat(0)

        
    chain = ROOT.TChain(treename)
    chain2 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME12")
    chain3 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME22")
    if os.path.isdir(filedir):
    	  ls = os.listdir(filedir)
    	  for x in ls:
		x = filedir[:]+x
    		if os.path.isdir(x):
			continue
		chain.Add(x)
		chain2.Add(x)
		chain3.Add(x)
    elif os.path.isfile(filedir):
	  chain.Add(filedir)
	  chain2.Add(filedir)
	  chain3.Add(filedir)
    else:
	  print " it is not file or dir ", filedir

    chain.AddFriend(chain2)
    chain.AddFriend(chain3)

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins0 = int(y_bins0[1:-1].split(',')[0])
    yminBin0 = float(y_bins0[1:-1].split(',')[1])
    ymaxBin0 = float(y_bins0[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])

    todrawb1 = "%s"%yaxis+":"+"%s>>b1"%xaxis
    todrawb0 = "%s"%yaxis0+":"+"%s>>b0"%xaxis
    
    c0 = ROOT.TCanvas("c0","c0")
    c0.SetGridx()
    c0.SetGridy()
    c0.SetTickx()
    c0.SetTicky()
    b0 = ROOT.TH2F("b0","b0",xBins,xminBin,xmaxBin,yBins0,yminBin0,ymaxBin0)
    b0.GetXaxis().SetTitle("%s"%xtitle)
    b0.GetYaxis().SetTitle("%s"%ytitle0)
    b0.GetXaxis().SetTitleSize(.04)
    b0.GetYaxis().SetTitleSize(.04)
    b0.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
    #b0.SetTitle("%s Vs %s,%s"%(ytitle0, xtitle, st_title)) 
    #b1.SetTitleFont(62)
    b0.SetTitleSize(0.05)
    #b1.SetMaximum(30)
    b0.SetStats(1)
    #hasxy = "&& fabs(%s)>0 && fabs(%s)>0"%(xaxis, yaxis0)
    chain.Draw(todrawb0,cuts,"colz")
    print "todraw ",todrawb0, " cut ",cuts

    #tex1 = TLatex(0.35,.8,"#splitline{%s}{%.1f<|#eta|<%.1f,20<p_{T}<50}"%(st_title,etamin,etamax))
    #tex1 = ROOT.TLatex(0.15,.8,"#splitline{%s}{%s}"%(st_title,text))
    #tex1 = TLatex(0.2,.7,"%s"%(st_title))
    tex1 = ROOT.TLatex(0.2,.8,"%s"%(text))
    tex1.SetNDC()
    tex1.Draw("same")
    c0.SaveAs("%s"%picname+"_0.png")
    c0.SaveAs("%s"%picname+"_0.C")
    """
    printa = 0
    (Xs, Ys, exl, exh, eyl, eyh) = FitHistoFunction(b0,xBins, xminBin, xmaxBin, yBins0, yminBin0,ymaxBin0, fraction, printa)
    r0 = ROOT.TGraphAsymmErrors(len(Xs),Xs,Ys,exl,exh,eyl,eyh)
    r0.GetXaxis().SetTitle("%s"%xtitle)
    r0.GetYaxis().SetTitle("%s"%ytitle)
    r0.SetTitle("%s Vs %s,%s"%(ytitle, xtitle, st_title)) 
    r0.GetYaxis().SetRangeUser(yminBin, ymaxBin)
    r0.SetMarkerColor(ROOT.kBlack)
    r0.SetLineColor(ROOT.kBlack)
    r0.Draw("esame")

    c1 = ROOT.TCanvas("c1","c1")
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
    binxmax = xBins
    b1 = ROOT.TH2F("b1","b1",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b1.GetXaxis().SetTitle("%s"%xtitle)
    b1.GetYaxis().SetTitle("%s"%ytitle)
    b1.SetTitle("%s Vs %s,%s"%(ytitle, xtitle, st_title)) 
    #b1.SetTitleFont(62)
    b1.SetTitleSize(0.05)
    #b1.SetMaximum(30)
    b1.SetStats(1)

    binxmax = xBins
    chain.Draw(todrawb1,cuts,"colz")
    print "todraw ",todrawb1, " cut ",cuts
    #b1.Print()
    tex1.Draw("same")

    
    printa = 1

    (Xs, Ys, exl, exh, eyl, eyh) = FitHistoFunction(b1,xBins, xminBin, xmaxBin, yBins, yminBin,ymaxBin, fraction, printa)
    r1 = ROOT.TGraphAsymmErrors(len(Xs),Xs,Ys,exl,exh,eyl,eyh)
    r1.GetXaxis().SetTitle("%s"%xtitle)
    r1.GetYaxis().SetTitle("%s"%ytitle)
    r1.SetTitle("%s Vs %s,%s"%(ytitle, xtitle, st_title)) 
    r1.GetYaxis().SetRangeUser(yminBin, ymaxBin)
    r1.SetMarkerColor(ROOT.kBlack)
    r1.SetLineColor(ROOT.kBlack)
    r1.Draw("esame")
    

 #   c1.SaveAs("%s"%picname+".pdf")
    c1.SaveAs("%s"%picname+"_1.png")
    return (0,0)
    #r1.GetXaxis().SetTitle("p_{T} of Simulated Muon Track [GeV]")
    #r1.GetYaxis().SetTitle("|\Delta \phi _{"+stx+"}| ^{-1}")
    #r1.SetTitle("|\Delta \phi _{"+stx+"}| ^{-1} vs p^{SimT}_{T}, ct %d mm"%ctau+" on %.1f < | \eta | < %.1f"%(etamin,etamax))
    #r1.SetTitle(" 1/ | \Delta \phi_{ 1 - 2} |  vs p^{simT} / cosh(GP.eta), ct 0 mm")
    #r1.Draw("same")
    c2 = ROOT.TCanvas("c2","c2",800,600)
    #r1.Sumw2()
    r1.Fit("pol1", "", "0S",xminBin, xmaxBin)
    fit=r1.GetFunction("pol1")
    try:
	fit.GetParameter(0)
    except (RuntimeError, TypeError, NameError):
	return (0,0)
    #print "fitting para0 ", fit.GetParameter(0)," paraerror ", fit.GetParError(0)
    
    b1.Draw("colz")
    r1.Draw("esame")
    tex1.Draw("same")
    #tex1 = TLatex(0.35,.8,"#splitline{%s}{%.1f<|#eta|<%.1f,20<p_{T}<50}"%(st_title,etamin,etamax))
    tex2 = ROOT.TLatex(0.20,.82,"#splitline{p0=%.4f +/- %.4f}{p1=%.3f +/- %.3f}"%(fit.GetParameter(0),fit.GetParError(0),fit.GetParameter(1),fit.GetParError(1)))
    tex2.SetNDC()
    tex2.SetTextColor(ROOT.kRed)
    tex2.Draw("same")

 
#    c2.SaveAs("%s"%picname+"_combined_median.pdf")
    #c2.SaveAs("%s"%picname+"_combined_median.png")
    return (fit.GetParameter(0)*(-1), fit.GetParameter(1))
    #c3 = TCanvas("c2","c2",800,600)
    #r1.Draw("e")
    #tex1.Draw("same")
    """

def makeEffplot(filedir,todraw, treename0,ptthreshold,fractionToKeep, den, num, num2 , xtitle,ytitle,leg1, leg2,txt,picname):
	c1 = ROOT.TCanvas()
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(-0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.045)
        b1.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
	b1.SetStats(0)

    	chain = ROOT.TChain(treename0)
#	treename2 = "HLTBendingAngle/trk_eff_csc_ME21" 
   	chain2 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME12")
   	chain3 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME22")
    	if os.path.isdir(filedir):
    		ls = os.listdir(filedir)
    	 	for x in ls:
			x = filedir[:]+x
    			if os.path.isdir(x):
				continue
			chain.Add(x)
			chain2.Add(x)
			chain3.Add(x)
    	elif os.path.isfile(filedir):
		chain.Add(filedir)
		chain2.Add(filedir)
		chain3.Add(filedir)
    	else:
	  print " it is not file or dir ", filedir
	chain.AddFriend(chain2)
	chain.AddFriend(chain3)
        
	#hist = ROOT.TH1F("hist","hist",2000,0.0,0.2)
        #chain.Draw("TMath::Abs(%s)>>hist"%(num),den+"&& pt>%f"%pt)
	#upperlim = getUpperlimit(hist, fractionToKeep)
	#hist2 = ROOT.TH1F("hist2","hist2",2000,0.0,0.2)
	#chain.Draw("TMath::Abs(%s)>>hist2"%(num2),den+"&& pt>%f"%pt)
	#upperlim2 = getUpperlimit(hist2, fractionToKeep)
	#print "pt ",pt," fraction to keep ",fractionToKeep," upperlim1 ",upperlim," upperlim2 ",upperlim2
	
	e1 =  getAllEffV2(chain,den, num, ptthreshold, fractionToKeep, "pt")
	#e1 = getAllEff(chain, den, "%s>%f"%(num,ptthreshold),"test", "test",todraw)
	e2 =  getAllEffV2(chain,den, num2, ptthreshold, fractionToKeep, "pt")
	#e2 = getAllEff(chain, den, "%s>%f"%(num2,ptthreshold),"test","test", todraw)

	#e3.SetFillColor(ROOT.kBlue+1)
	#e2.SetFillColor(ROOT.kMagenta+2)
	#e1.SetFillColor(ROOT.kGreen+2)
	e2.SetLineColor(ROOT.kRed)
	e1.SetLineColor(ROOT.kBlue)
	#e3.SetLineStyle(2)
	#e2.SetLineStyle(2)
	#e1.SetLineStyle(2)
	#e3.SetLineWidth(3)
	#e2.SetLineWidth(3)
	#e1.SetLineWidth(3)
	e2.SetMarkerColor(ROOT.kRed)
	e1.SetMarkerColor(ROOT.kBlue)
	e2.SetMarkerStyle(21)
	e1.SetMarkerStyle(23)
	e2.SetMarkerSize(1)
	e1.SetMarkerSize(2)
	#e30.SetFillColor(ROOT.kBlue-1)
	#e20.SetFillColor(ROOT.kMagenta-1)
	#e10.SetFillColor(ROOT.kGreen-1)
	#e30.SetLineColor(ROOT.kBlue-4)
	#e20.SetLineColor(ROOT.kMagenta)
	#e10.SetLineColor(ROOT.kGreen-4)
	#e30.SetLineWidth(2)
	#e20.SetLineWidth(2)
	#e10.SetLineWidth(2)

	b1.Draw()
	e1.Draw("esame")
	#e2.Draw("esame")
	#e3.Draw("lsame")
	#e10.Draw("same")
	#e20.Draw("same")
	#e30.Draw("same")
	#e30.Draw("e3same")

	legend = ROOT.TLegend(0.6,0.14,0.96,0.43)
	legend.SetFillColor(ROOT.kWhite)
	legend.SetTextFont(42)
#legend.SetBorderSize(0)
#legend.SetTextSize()
#legend.SetHeader("p_{T}^{sim}>10,2.14>|#eta|>1.64, has at least 3stubs and hasME1")
	legend.AddEntry(e1,"%s"%leg1,"pl")
	#legend.AddEntry(e2,"%s"%leg2,"pl")#"f"
	#legend.AddEntry(e3,"3+stubs, p_{T}^{reco}>20 GeV","f")
#legend.AddEntry(e2,"#splitline{Phase,3+stubs}{stub in ME11}","f")
	legend.Draw("same")

	tex = ROOT.TLatex(0.45,0.57,"#splitline{%s}{%d%% eff above %d [GeV]}"%(txt,fractionToKeep,ptthreshold))
	tex.SetTextSize(0.05)
	tex.SetTextFont(62)
	tex.SetNDC()
	tex.Draw("same")



#	c1.SaveAs("%s.pdf"%picname)
	c1.SaveAs("%s_Plateau%d_ThresholdPt%d.png"%(picname, fractionToKeep,ptthreshold))

def printcuts(filedir, treename0, den, num, pt, npar,etamin, etamax, fractionToKeep, xtitle, ytitle, txt, picname):
    		chain = ROOT.TChain(treename0)
    		if os.path.isdir(filedir):
    			ls = os.listdir(filedir)
    	 		for x in ls:
	      			if not(x.endswith(".root")):
					#print "x.endswith(.root) ", x.endswith(".root")
					continue
				x = filedir[:]+x
    				if os.path.isdir(x):
				    continue
				chain.Add(x)
    		elif os.path.isfile(filedir):
			chain.Add(filedir)
    		else:
	  		print " it is not file or dir ", filedir
		evenodds = ["odd,even","odd,odd","even,even","even,odd","all pairs"]
		hasnum = "&& fabs(%s)>0"%(num)
		hist = ROOT.TH1F("hist","hist",3000,0.0,0.6)
	        if pt>2 and pt<10:
    			deltapt=.5
		else:
	   		deltapt=1.0	   
        	chain.Draw("TMath::Abs(%s)>>hist"%(num),den+"&& npar==%d && pt>%f && pt<%f && TMath::Abs(%s)<1"%(npar,pt-deltapt, pt+deltapt, num)+hasnum)
       		#print "cuts ",den+"&& npar==%d && pt>%f && pt<%f && TMath::Abs(%s)<.6"%(npar,pt-1, pt+1, num)+hasnum
       			#hist.Print("ALL")
		upperlim = getUpperlimit(hist, fractionToKeep)
		print "eta%dto%dnpar%d%spt%dfraction%d:%f"%(int(etamin*10), int(etamax*10), npar,num,pt,fractionToKeep,upperlim)
		c1 = ROOT.TCanvas()
		c1.SetGridx()
		c1.SetGridy()
		c1.SetTickx()
		c1.SetTicky()
                hist.Draw()
     		hist.GetXaxis().SetTitle("%s"%xtitle)
     		hist.GetYaxis().SetTitle("%s"%ytitle)
		hist.SetTitle("%s distribution "%xtitle)
	#tex = ROOT.TLatex(0.15,0.87,"%s"%txt)
		tex = ROOT.TLatex(0.4,0.6,"#splitline{%s}{p_{T}>%d GeV, cut:%.4f}"%(txt,pt, upperlim))
		tex.SetTextSize(0.05)
		tex.SetTextFont(62)
		tex.SetNDC()
		tex.Draw("same")
        	#return Teffs
		#c1.Update()
		c1.SaveAs("%s_Plateau%d_binsimPt%d.png"%(picname, fractionToKeep,pt))
		#c1.SaveAs("%s_Plateau%d_binsimPt%d.pdf"%(picname, fractionToKeep,pt))
		#c1.SaveAs("%s_Plateau%d_binsimPt%d.C"%(picname, fractionToKeep,pt))

def printallcuts(filedir, treename0, den, num, netas, pts, fractionToKeep, xtitle, ytitle, txt,picname):
    evenodds = ["odd,even","odd,odd","even,even","even,odd","all pairs"]
    for pt in pts:
    	for neta in range(len(netas)-1):
	
       		if (netas[neta]<1.6):
	 		allnpar = [0,1,2,3]
	 		ring = 2
       		if (netas[neta]>=1.6):
	 		allnpar = [0,1,2,3]
	 		ring =1
       		etamin = netas[neta]
       		etamax = netas[neta+1]
		for npar in allnpar:
			me11 = evenodds[npar].split(',')[0]
			me21 = evenodds[npar].split(',')[1]
			chambers = ",%.1f<|#eta|<%.1f,ME1%d %s,ME2%d %s"%(etamin,etamax,ring, me11, ring, me21)
			txt1 = txt+chambers
			picname1 = picname+"_st2eta%dto%d_npar%d"%(int(netas[neta]*10), int(netas[neta+1]*10), npar)
			den1 = den+"&& fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f && meRing==%d"%(netas[neta], netas[neta+1],ring)
		        printcuts(filedir, treename0, den1, num, pt, npar,etamin,etamax, fractionToKeep, xtitle, ytitle, txt1, picname1)
		 


#def makeEffplot_v2(filedirs,todraw, treename0, den, num, pt, allnpar, fractionToKeep,  xtitle,ytitle,leg, txt,picname, Teffs=[]):
def makeEffplot_v2(filedirs,todraw, treename0, den, num, pt, allnpar, fractionToKeep,  xtitle,ytitle,leg, txt,picname):
	
	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(0.0,1.05)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.05)
	b1.GetYaxis().SetTitleOffset(1.1)
	b1.GetYaxis().SetTitleSize(.05)
	b1.GetYaxis().SetLabelSize(.05)
	b1.GetXaxis().SetLabelSize(.05)
        b1.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
	
	
	Teffs = []
	hdens = []
	hnums = []
	npars = 4
	Upperlimits = [0 for x in range(npars)]
        color = [ROOT.kBlue, ROOT.kRed, ROOT.kMagenta+2, ROOT.kGreen+2,ROOT.kCyan+2]
        maker = [20,21,22,23,33]
	print "filedirs ",filedirs
	
	#legend = ROOT.TLegend(0.4,0.15,0.92,0.5)
	legend = ROOT.TLegend(0.4,0.5,0.8,0.5)
	legend.SetFillColor(ROOT.kWhite)
	legend.SetTextFont(42)
	legend.SetTextSize(.05)
	legend.SetHeader("stub alignment algo")
	muons = ["Prompt muon","#splitline{Displaced muon}{5<|d_{xy}|<50cm}"]
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
   		chain2 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME12")
   		chain3 = ROOT.TChain("GEMCSCAnalyzer/trk_eff_CSC_ME22")
    		if os.path.isdir(filedir):
    			ls = os.listdir(filedir)
    	 		for x in ls:
	      			if not(x.endswith(".root")):
					#print "x.endswith(.root) ", x.endswith(".root")
					continue
				x = filedir[:]+x
    				if os.path.isdir(x):
				    continue
				chain.Add(x)
				chain2.Add(x)
				chain3.Add(x)
    		elif os.path.isfile(filedir):
			chain.Add(filedir)
			chain2.Add(filedir)
			chain3.Add(filedir)
    		else:
	  		print " it is not file or dir ", filedir
		#chain.AddFriend(chain2)
		#chain.AddFriend(chain3)
		evenodds = ["odd,even","odd,odd","even,even","even,odd","all pairs"]
		#allnpar = [0,1,2,3]
		hasnum = "&& fabs(%s)>0"%(num[n])
		for xpar in range(len(allnpar)):
		     npar = allnpar[xpar]	
		     if n==0:
			hist = ROOT.TH1F("hist","hist",4000,0.0,0.4)
        		chain.Draw("TMath::Abs(%s)>>hist"%(num[n]),den[n]+"&& npar==%d && pt>%f && pt<%f && TMath::Abs(%s)<1"%(npar,pt-1, pt+1, num[n])+hasnum)
       			print "cuts ",den[n]+"&& npar==%d && pt>%f && pt<%f && TMath::Abs(%s)<1.0"%(npar,pt-1, pt+1, num[n])+hasnum
       			#hist.Print("ALL")
			upperlim = getUpperlimit(hist, fractionToKeep)
    			Upperlimits[xpar] = upperlim
			print "npar%d%spt%dfraction%d:%f"%(npar,num[n],pt,fractionToKeep,upperlim)
		     #txt_tmp = "#splitline{%s,%s}{%s,threshold: %d, Eff %d%%}"%(muons[n],txt.split(',')[0],evenodds[npar], pt, fractionToKeep)
		     #getAllEff(chain, den[n]+"&& npar==%d"%npar,"fabs(%s)<%f"%(num[n], Upperlimits[xpar]),picname+"_%s_npar%d_%s_pt%d_fraction%d"%(muons[n],npar,num[n],pt,fractionToKeep),txt_tmp,todraw)
		     hdens[n].Add(gethist1D(chain, den[n]+"&& npar==%d && TMath::Abs(%s)<.6"%(npar,num[n])+hasnum,todraw))
		     hnums[n].Add(gethist1D(chain, den[n]+"&& npar==%d && fabs(%s)<%f"%(npar, num[n], Upperlimits[xpar])+hasnum, todraw))


	c1 = ROOT.TCanvas()
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

	b1.SetStats(0)
	b1.Draw()
	for n in range(len(filedirs)):
		Teffs.append(ROOT.TEfficiency(hnums[n],hdens[n]))	
		m = len(Teffs)-1
        	ROOT.SetOwnership(Teffs[m], False)
		Teffs[m].SetLineColor(color[n])
		Teffs[m].SetMarkerColor(color[n])
		Teffs[m].SetMarkerStyle(maker[n])
		Teffs[m].Draw("same")
		legend.AddEntry(Teffs[n],"%s"%leg[n],"pl")
	print "Teffs ",Teffs
	#Teffs[0].Print("ALL")
	legend.Draw("same")

	#tex = ROOT.TLatex(0.15,0.87,"%s"%txt)
	tex = ROOT.TLatex(0.4,0.6,"#splitline{%s}{p_{T}>%d GeV}"%(txt,pt))
	tex.SetTextSize(0.05)
	tex.SetTextFont(62)
	tex.SetNDC()
	tex.Draw("same")
        #return Teffs
	#c1.Update()
	c1.SaveAs("%s_Plateau%d_binsimPt%d.png"%(picname, fractionToKeep,pt))
	c1.SaveAs("%s_Plateau%d_binsimPt%d.pdf"%(picname, fractionToKeep,pt))
	c1.SaveAs("%s_Plateau%d_binsimPt%d.C"%(picname, fractionToKeep,pt))
	return Teffs

def makeEffplot_v3(filedirs,todraw, treename0, den, num, Pts, netas, allnpar, fractionToKeep,  xtitle,ytitle,leg, txt,picname):
	
	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.2)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.05)
	b1.GetYaxis().SetTitleOffset(1.1)
	b1.GetYaxis().SetTitleSize(.05)
	b1.GetYaxis().SetLabelSize(.05)
	b1.GetXaxis().SetLabelSize(.05)
        b1.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
	
	
	Teffs = []
	tex = []
        c1 = ROOT.TCanvas("c1","c1",1200,900)
	c1.Divide(len(Pts), len(netas)-1)
	for neta in range(len(netas)-1):
	    for npt in range(len(Pts)):    
		pt = Pts[npt]
		npad = neta*len(Pts)+npt
		c1.cd(npad+1)
		legend = ROOT.TLegend(0.4,0.2,0.8,0.5)
		legend.SetFillColor(ROOT.kWhite)
		legend.SetTextFont(42)
	        legend.SetTextSize(.05)
		legend.SetHeader("stubs alignment algo")
    		den_v2 = []
    		for x in den:
			den_v2.append(x+"&& fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f"%(netas[neta],netas[neta+1]))
		txt_v2 = "%.1f<|#eta|<%.1f, "%(netas[neta],netas[neta+1])+txt	
    		makeEffplot_v2(filedirs,todraw, treename0, den_v2, num, pt, allnpar, fractionToKeep,  xtitle,ytitle,leg, txt_v2,picname+"_neta%d"%neta, Teffs)
		b1.SetStats(0)
		b1.Draw()
		for n in range(len(filedirs)):
			Teffs[npad*2+n].Draw("same")
			legend.AddEntry(Teffs[npad*2+n],"%s"%leg[n],"pl")
		tex.append(ROOT.TLatex(0.55,0.57,"#splitline{%s}{%d%% eff at %d [GeV]}"%(txt_v2,fractionToKeep,pt)))
		tex[npad].SetTextSize(0.05)
		tex[npad].SetTextFont(62)
		tex[npad].SetNDC()
		legend.Draw("same")
		tex[npad].Draw("same")
		print "neta*len(Pts)+npt+1 ",neta*len(Pts)+npt+1, " Teffs ",Teffs
		c1.Modified()
	c1.cd()

	c1.SaveAs("%s_Plateau%d_combined.png"%(picname, fractionToKeep))
	c1.SaveAs("%s_Plateau%d_combined.pdf"%(picname, fractionToKeep))


def makeplots(Teffs, legs, text, picname):

	b1 = ROOT.TH1F("b1","b1",len(ptbins)-1, ptbins)
	b1.GetYaxis().SetRangeUser(0.0,1.05)
	b1.GetYaxis().SetTitleOffset(1.1)
	b1.GetYaxis().SetNdivisions(520)
	b1.GetYaxis().SetTitle("Efficiency")
	b1.GetXaxis().SetTitle("True muon p_{T} [GeV]")
	b1.GetXaxis().SetTitleSize(0.05)
	b1.GetXaxis().SetLabelSize(0.05)
	b1.GetYaxis().SetTitleSize(0.05)
	b1.GetYaxis().SetLabelSize(0.05)
        b1.SetTitle("#scale[1.4]{#font[61]{CMS}} #font[52]{Simulation preliminary}"+"  "*10+" 14 TeV, 0 PU")
	
	
	c1 = ROOT.TCanvas()
	c1.SetGridx()
	c1.SetGridy()
	c1.SetTickx()
	c1.SetTicky()

        color = [ROOT.kBlue, ROOT.kRed, ROOT.kMagenta+2, ROOT.kGreen+2,ROOT.kCyan+2]
        maker = [20,21,22,23,33]
	legend = ROOT.TLegend(0.4,0.25,0.8,0.45)
	legend.SetFillColor(ROOT.kWhite)
	legend.SetTextFont(42)
	legend.SetTextSize(.05)
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

	tex = ROOT.TLatex(0.35,0.6,"%s"%text)
	tex.SetTextSize(0.05)
	tex.SetTextFont(62)
	tex.SetNDC()
	tex.Draw("same")
	#c1.Update()
	c1.SaveAs("%s.png"%(picname))
	c1.SaveAs("%s.pdf"%(picname))
	c1.SaveAs("%s.C"%(picname))

##############################################################3



#filedir1="out_ana_ctau0_20160216.root"
filedir1 = "/eos/uscms/store/user/tahuang/SLHC26_patch1_2023Muon_1M_Ana_PU0_Pt2_50_20160727/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0817_v6/160818_214142/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0822_v6/160822_165535/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0825/160825_224018/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0826/160826_200750/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0827/160901_002000/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0901/160901_222813/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0901_v2/160830_170002/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0901/160901_043538/0000/"
#filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0925/160926_050053/0000/"
filedir16 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0925_v2/160926_194628/0000/"
filedir14 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0812_v4/160813_141149/0000/"
filedir10 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0812_v0/160813_204308/0000/"
filedir15 = "/eos/uscms/store/user/tahuang/SLHC23_patch1_2023Muon_gen_sim_Pt2_50_1M/GEMCSCAna_ctau0_Pt2_50_0816_v5/160815_185029/0000/"
#filedir15 = "/eos/uscms/store/user/tahuang/SLHC26_patch1_2023Muon_1M_Ana_PU0_Pt2_50_20160727_v5/"
filedir2 = "GEMCSC_outana_0712_ctau10.root"
filedir3 = "GEMCSC_outana_0712_ctau100.root"
filedir4 = "/eos/uscms/store/user/tahuang/SLHC26_patch1_2023Muon_1M_Ana_PU0_ctau1000_20160727/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0817_v6/160818_214431/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0822_v6/160822_165636/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0825/160825_223538/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0826/160826_215302/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0827/160901_001931/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0901/160901_223815/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0901/160830_152048/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0901/160901_043623/0000/"
#filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0925/160926_050000/0000/"
filedir46 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0925_v2/160926_194711/0000/"
filedir44 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0812_v4/160813_141529/0000/"
filedir40 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0812_v0/160813_204646/0000/"
filedir45 = "/eos/uscms/store/user/tahuang/DarkSUSY_MH-125_MGammaD-20000_ctau1000_14TeV_madgraph-pythia6-tauola/GEMCSCAna_DarkSUSY_ctau1000_0816_v5/160815_185325/0000/"
treename0 = "GEMCSCAnalyzer/trk_eff_CSC_ALL"
#den = "abs(eta)<2.4 && abs(eta)>1.75"
#den = "abs(eta)<1.8 && abs(eta)>1.60 && hasSt1St2St3_sh"
#den = "abs(eta)<2.4 && abs(eta)>1.6 && hasSt1St2St3_sh"
#num= "nstubs>2 && has_tfTrack>0 && hasME1 &&"+den
den = "pt_direction_smeared>0 && pt_direction_sh>0 && fabs(eta)<1.8 && fabs(eta)>1.64"
num1 = "pt_direction_sh>10"
num2 = "pt_direction_smeared>10"
#num1 = "hasSt1St2St3 && pt_position_sh>10 &&"+den
#num2 = "hasSt1St2St3 && pt_position_sh>20 &&"+den
#num1 = "((passGE11 &&abs(eta)<2.14 && abs(eta)>2.06)) &&"+num
leg1="Direction based, p_{T}^{reco}>10"
leg2="Direction based, p_{T}^{reco}>10 + Smearing"
evenodds = ["odd,even,even","odd,odd,even","even,even,even","even,odd,odd",""]
#makeEffplot(filedir1,"pt", treename0, den, num1, num2,"true muon p_T", "Efficiency",leg1, leg2,"#splitline{1.64<|#eta|<1.80}{%s}"%evenodds[4],"GEMCSC_DirectionOnly_Eff_Pt10_recopt_20160704_nparall_eta1")

beta1={"ME11":{"even":{"ME21":{"even":-15.16, "odd":-15.71}}, 
		"odd":{"ME21":{"even":-12.92, "odd":-13.63}}}
	}
beta2={"ME11":{"even":{"ME21":{"even":-26.17, "odd":-27.76}}, 
		"odd":{"ME21":{"even":-21.68, "odd":-24.3}}}
	}
deltaphi1={"ME11":{"even":{"ME21":{"even":0.007, "odd":0.015}}, 
		"odd":{"ME21":{"even":0.01, "odd":0.015}}}
	}
deltaphi2={"ME11":{"even":{"ME21":{"even":0.007, "odd":0.02}}, 
		"odd":{"ME21":{"even":0.015, "odd":0.02}}}
	}
xtitle="True muon p_{T} GeV"
ytitle="efficiency"
a1=2
dxymin = [0,5,50]
dxymax = [5,50,100]
method=["direction","position"]
method=["direction"]
evenodds = ["odd,even","odd,odd","even,even","even,odd","all pairs"]
#evenodds = ["odd,even"]
etamin = [1.6,1.2,0.9,1.6,1.0,1.7,1.1,1.8,0.9]
etamax = [2.4,1.6,1.1,2.4,1.6,2.4,1.7,2.4,1.8]
netas = [1.6,1.8,2.0,2.2]
netas = [1.2,1.4,1.6,1.8,2.0,2.2]
#netas = [1.2,1.4,1.6]

filedirs = ["PromptMuon_0724_GEMCSC_v2.root","PromptMuon_0727_GEMCSC_v2.root"]
filedirs = [filedir1, filedir4]
filedirs_v6 = [filedir16, filedir46]
filedirs_v4 = [filedir14, filedir44]
filedirs_v0 = [filedir10, filedir40]
filedirs_v5 = [filedir15, filedir45]
#Pts = [10, 15, 20, 30]
Pts = [10]
#for neta in range(len(netas)-1):
dens =  ["fabs(csc_bending_angle12_xfactor_L1_2) <1 &&hasSt3orSt4_sh","fabs(csc_bending_angle12_xfactor_L1_2) <1&&hasSt3orSt4_sh && fabs(genGdMu_dxy)>5 && fabs(genGdMu_dxy)<50 && fabs(genGdMu_dR)<0.1"]
dens_loweta =  ["fabs(csc_bending_angle12_xfactor_smear3) <1","fabs(csc_bending_angle12_xfactor_smear3) <1 && fabs(genGdMu_dxy)>5 && fabs(genGdMu_dxy)<50 && fabs(genGdMu_dR)<0.1"]
#nums = ["csc_bending_angle12_xfactor_smear3","csc_bending_angle12_xfactor_smear3"]
#nums = ["csc_bending_angle12_xfactor","csc_bending_angle12_xfactor"]
nums = ["csc_bending_angle12_xfactor_L1_2","csc_bending_angle12_xfactor_L1_2"]
allnpar = [0,1,2,3]
#allnpar = [2,3]
legs = ["Prompt muon", "#splitline{Displaced muon}{5<|d_{xy}|<50cm}"]
nums_loweta = ["csc_bending_angle12_xfactor_smear3","csc_bending_angle12_xfactor_smear3"]
#makeEffplot_v3(filedirs,"pt", treename0, dens, nums, Pts, netas, 90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0803/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160803_npar%d_v"%(npar))
#makeEffplot_v3(filedirs_v6,"pt", treename0, dens, nums, Pts, netas,allnpar, 90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0817/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160817_npar%d_v6"%(npar))
#makeEffplot_v3(filedirs_v6,"pt", treename0, dens_loweta, nums_loweta, Pts, [1.2,1.4,1.6], [1,2] ,90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0817/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160822_npar%d_loweta_v6"%(npar))
#makeEffplot_v3(filedirs_v4,"pt", treename0, dens, nums, Pts, netas, allnpar, 90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0812/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160812_npar%d_v4"%(npar))
#makeEffplot_v3(filedirs_v0,"pt", treename0, dens, nums, Pts, netas, allnpar, 90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0812/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160812_npar%d_v0"%(npar))
#makeEffplot_v3(filedirs_v5,"pt", treename0, dens, nums, Pts, netas, allnpar, 90, "true muon p_T", "Efficiency",legs, evenodds[npar],"DirectionPt_Dxy_0815/GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160815_npar%d_v5"%(npar))

#printallcuts(filedir16, treename0,"hasSt3orSt4_sh", "csc_bending_angle12_xfactor_L1_2", netas, [2,3,4,5,7,10,15,20,30,40], 90, "|#Delta#phi_{dir}|", "Entries", "digi level ","Direction_PT_Cut_0909/GEMCSC_ctau0_directionOnly_20160909")

if len(sys.argv)>=2:
	#take dir name from arguments, condor mode
	outputdir = sys.argv[1]
	filedirs_v6 = ["out_ana_prompt.root","out_ana_displaced.root"]
	if outputdir[-1] != "/":
		outputdir = outputdir+"/"
else:
	outputdir = "Direction_PT_0925/"
def plotalleta(pt, netas, fraction=95, yaxis = "(csc_bending_angle12_xfactor_L1_2)",ME21CSConly=False):
    Teffs_0 = []
    Teffs_1 = []
    extrapic=""
    extratxt=" "
    if ME21CSConly:
    	extrapic = "_ME21CSConly"
        extratxt = ", ME21 CSC only"
    elif not(ME21CSConly) and netas[0]>=1.6:
        extratxt = ", with GE21"
    print "extratxt ",extratxt," pic ",extrapic	
    for neta in range(len(netas)-1):
   #for npt in range(len(Pts)):	
       #pt = Pts[npt]
       if (netas[neta]<1.6):
	 allnpar = [0,1,2,3]
	 ring = 2
       if (netas[neta]>=1.6):
	 allnpar = [0,1,2,3]
	 ring =1
       legs = ["Prompt muon", "#splitline{Displaced muon}{10<|d_{xy}|<50 cm}"]
       Tefftotal = []
       Tefftotal_csc = []
       for npar in allnpar:
        allnpars = [npar]
	me11 = evenodds[npar].split(',')[0]
	me21 = evenodds[npar].split(',')[1]
	chambers = "ME1%d %s,ME2%d %s"%(ring, me11, ring, me21)
	#dens =  ["hasSt3orSt4_sh && fabs(csc_bending_angle12_xfactor_L1_1)>0 && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f"%(netas[neta],netas[neta+1]),"hasSt3orSt4_sh && fabs(csc_bending_angle12_xfactor_L1_1)>0 && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f && fabs(genGdMu_dxy)>10 && fabs(genGdMu_dxy)<50 && fabs(genGdMu_dR)<0.1"%(netas[neta],netas[neta+1])]
	nums_L1 = [yaxis, yaxis]
	dens_L1 =  ["hasSt3orSt4_sh && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f"%(netas[neta],netas[neta+1])+"&& fabs(%s)>0"%nums_L1[0]+" && meRing==%d"%(ring),"hasSt3orSt4_sh && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f && fabs(genGdMu_dxy)>10 && fabs(genGdMu_dxy)<50 && fabs(genGdMu_dR)<0.1"%(netas[neta],netas[neta+1])+"&&fabs(%s)>0"%nums_L1[0]+" && meRing==%d"%(ring)]
	text = "#splitline{%s}{%.1f<|#eta|<%.1f, p_{T}^{L1}>%d GeV%s}"%(chambers, netas[neta],netas[neta+1],pt, extratxt)
	Teffs = makeEffplot_v2(filedirs_v6,"pt", treename0, dens_L1, nums_L1, pt, allnpars, fraction, "true muon p_T", "Efficiency",legs, text,outputdir+"GEMCSC_ctau0andctau1000_directionOnly_Eff_recopt_20160925_dxy10_50_st2eta%dto%d_npar%d%s"%(int(netas[neta]*10), int(netas[neta+1]*10), npar,extrapic))
	Tefftotal.append(Teffs)
       print "Tefftotal len ",len(Tefftotal),Tefftotal
       Teff0 = Tefftotal[0][0]
       Teff1 = Tefftotal[0][1]
       Teff0.SetName("directioneta%dto%d"%(int(netas[neta]*10),int(netas[neta+1]*10))+"promptmuonpt%d"%(pt))
       Teff1.SetName("directioneta%dto%d"%(int(netas[neta]*10),int(netas[neta+1]*10))+"displacedmuonpt%d"%(pt))
       text_h = "#splitline{Stub alignment algorithm}{%.1f<|#eta|<%.1f, p_{T}^{L1}>%d GeV%s}"%(netas[neta],netas[neta+1], pt, extratxt)
       for xpar in range(len(Tefftotal)-1):
   	Teff0.Add(Tefftotal[xpar+1][0])		   
   	Teff1.Add(Tefftotal[xpar+1][1])		   
       makeplots([Teff0, Teff1], legs, text_h,outputdir+"GEMCSC_ctau0andctau1000_direction_Eff_recopt_20160925_dxy10_50_st2eta%dto%d_allnpar_Plateau%d_binsimPt%d%s"%(int(netas[neta]*10), int(netas[neta+1]*10),fraction, pt, extrapic))
       """
       Teff_out.Reopen("Update")
       Teff0.Write(Teff_out)
       Teff1.Write(Teff_out)
       Teff_out.Close()
       """
       Teffs_0.append(Teff0)
       Teffs_1.append(Teff1)
    Teffs0_alleta = Teffs_0[0] 
    Teffs1_alleta = Teffs_1[0] 
    for xeta in range(len(Teffs_0)-1):
       Teffs0_alleta.Add(Teffs_0[xeta+1])	 
       Teffs1_alleta.Add(Teffs_1[xeta+1])	 
    Teffs0_alleta.SetName("directionpromptmuonpt%d"%(pt))
    Teffs1_alleta.SetName("directiondisplacedmuonpt%d"%(pt))
    text_alleta = "#splitline{Stub alignment algorithm}{%.1f<|#eta|<%.1f, p_{T}^{L1}>%d GeV%s}"%(netas[0],netas[-1], pt, extratxt)
    makeplots([Teffs0_alleta, Teffs1_alleta], legs, text_alleta,outputdir+"GEMCSC_ctau0andctau1000_direction_Eff_20160925_pt%d_fraction%d_St2eta%dto%d_allnpar%s"%(pt, fraction,int(netas[0]*10),int(netas[-1]*10), extrapic)) 
    """
    Teff_out.Reopen("Update")
    Teffs0_alleta.Write(Teff_out)
    Teffs1_alleta.Write(Teff_out)
    Teff_out.Close()
    """
if not os.path.exists(outputdir):
        os.makedirs(outputdir)
plotalleta(10,[1.6,1.8,2.0,2.2],90,"(csc_bending_angle12_xfactor_L1_2)",False)
plotalleta(10,[1.6,1.8,2.0,2.2],90,"(csc_bending_angle12_xfactor_L1_1)",True)
plotalleta(10,[1.2,1.4,1.6],90,"(csc_bending_angle12_xfactor_L1_2)",False)

#plotalleta(20,[1.6,1.8,2.0,2.2],90,"(csc_bending_angle12_xfactor_L1_2)",False)
#plotalleta(20,[1.6,1.8,2.0,2.2],90,"(csc_bending_angle12_xfactor_L1_1)",True)
#plotalleta(20,[1.2,1.4,1.6],90,"(csc_bending_angle12_xfactor_L1_2)",False)

#netas = [1.6,1.8,2.0,2.2]
netas = [1.6,2.2]
for ptthreshold1 in [20,30]:
	for neta in range(len(netas)-1):
	    #for ndxy in range(len(dxymin)):
	    for npar in range(4):
		x = "direction"	
	    #for npar in [1,2]:
		#npar=4
		me11 = evenodds[npar].split(',')[0]
		me21 = evenodds[npar].split(',')[1]
		#print "me11 ",me11, " me21 ",me21
		#den = "pt_%s_sh>0 && fabs(eta)>%f && fabs(eta)<%f && npar==%d"%(x,netas[neta],netas[neta+1],npar)
		#(slope, intercept) = getslopeandintercept(npar, (netas[neta]+netas[neta+1])/2.0)
		#print "slope ",slope," intercept ",intercept
		hascscsh = "(trk_eff_CSC_ME11.nlayers_csc_sh_odd>=4 || trk_eff_CSC_ME11.nlayers_csc_sh_even>=4) && (trk_eff_CSC_ME21.nlayers_csc_sh_odd>=4 || trk_eff_CSC_ME21.nlayers_csc_sh_even>=4)"
	        hasgemsh = "(fabs(trk_eff_CSC_ME11.dphi_sh_odd)<1 || fabs(trk_eff_CSC_ME11.dphi_sh_even)<1) && (fabs(trk_eff_CSC_ME21.dphi_sh_odd)<1 || fabs(trk_eff_CSC_ME21.dphi_sh_even)<1)"	
		#hascscsh = "(trk_eff_CSC_ME11.nlayers_csc_sh_%s>=4 && trk_eff_CSC_ME21.nlayers_csc_sh_%s>=4)"%(me11, me21)
	        #hasgemsh = "(fabs(trk_eff_CSC_ME11.phiM_gemcsc_%s)<4 && fabs(trk_eff_CSC_ME21.phiM_gemcsc_%s)<4)"%(me11, me21)
		#den = hascscsh+"&&"+hasgemsh+"&&pt_%s_xfactor>0 && pt_%s_sh>0 && fabs(eta_st1_sh)>%f && fabs(eta_st1_sh)<%f && hasSt3orSt4_sh && fabs(genGdMu_dxy)>%f && fabs(genGdMu_dxy)<%f && fabs(genGdMu_dR)<0.1"%(x, x,netas[neta],netas[neta+1],dxymin[ndxy], dxymax[ndxy])
	#	den = " pt_%s_sh>0 && fabs(eta_st1_sh)>%f && fabs(eta_st1_sh)<%f && fabs(eta_st2_sh)>%f && fabs(eta_st2_sh)<%f && hasSt3orSt4_sh && fabs(genGdMu_dxy)>%f && fabs(genGdMu_dxy)<%f && fabs(genGdMu_dR)<0.1"%(x,netas[neta],netas[neta+1],netas[neta],netas[neta+1],dxymin[ndxy], dxymax[ndxy])
		#den = "pt_%s_sh>0 && (((npar==3||npar==2) && fabs(trk_eff_CSC_ME11.eta_cscsh_even)>%f && fabs(trk_eff_CSC_ME11.eta_cscsh_even)<%f) ||((npar==0 || npar==1) && fabs(trk_eff_CSC_ME11.eta_cscsh_odd)>%f && fabs(trk_eff_CSC_ME11.eta_cscsh_odd)<%f))&& hasSt3orSt4_sh && fabs(genGdMu_dxy)>%f && fabs(genGdMu_dxy)<%f && fabs(genGdMu_dR)<0.1"%(x,netas[neta],netas[neta+1],netas[neta],netas[neta+1], dxymin[ndxy], dxymax[ndxy])
		#den = "pt_position_sh>0 && pt_%s_sh>0 && (((npar==0||npar==2) && fabs(trk_eff_CSC_ME21.eta_cscsh_even)>%f && fabs(trk_eff_CSC_ME21.eta_cscsh_even)<%f) ||((npar==1 || npar==3) && fabs(trk_eff_CSC_ME21.eta_cscsh_odd)>%f && fabs(trk_eff_CSC_ME21.eta_cscsh_odd)<%f))&& hasSt3orSt4_sh"%(x,netas[neta],netas[neta+1],netas[neta],netas[neta+1])
		nums = ["pt_%s_xfactor>10"%(x),"pt_%s_xfactor_smear0>10"%(x),"pt_%s_xfactor_smear1>10"%(x),"pt_%s_xfactor_smear2>10"%(x),"pt_%s_xfactor_smear3>10"%(x)]
		num1 = "csc_bending_angle12_xfactor_L1_2"
		num2 = "csc_bending_angle12_xfactor_smear3"
		leg1 = "Digi level with fits"
		leg2 = "L1 level by Smearing"
		#legs = ["Sim Level","+smearing #Delta#Phi_{GEMCSC}^{st1}","+smearing #Delta#Phi_{GEMCSC}^{st2}","+smearing Radius"]
		legs = ["Sim Level","+smearing #Phi_{ME11}","+smearing #Phi_{GE11}","+smearing #Phi_{ME21}","+smearing #Phi_{GE21}"]
		#leg2="#splitline{%s based, p_{T}^{reco}>10}{ direction from GEM-CSC bending}"%(x)
		fraction = 90
		y = "csc_bending_angle12_xfactor_L1_2"
		x_bins="(25,1.0,51.0)"
		y_bins="(120,-0.01,0.5)"
		x_title = "true muon p_{T} [GeV]"
		cuts_ctau1000 = "npar==%d && %f<fabs(eta_st2_sh) && fabs(eta_st2_sh)<%f"%(npar, netas[neta],netas[neta+1])+"&&  fabs(genGdMu_dxy)>%f && fabs(genGdMu_dxy)<%f && fabs(genGdMu_dR)<0.1"%(10,50)
		cuts = "npar==%d && %f<fabs(eta_st2_sh) && fabs(eta_st2_sh)<%f"%(npar, netas[neta],netas[neta+1])
		y_bins1 ="(60,0,.6)"
		y_bins2 ="(100,0,400)"
		y_title2 = "|#Delta#Phi^{momentum}|"
		sttitle="c#tau=0, ME11 %s,ME21 %s"%(me11, me21)
		sttitle_ctau1000="displaced muon, 10<|d_{xy}|<50, ME11 %s,ME21 %s"%(me11, me21)
		#text = " %.1f<|#eta|<%.1f, with GE21"%(netas[neta],netas[neta+1])
		text = "GE2/1+ME2/1"
		phi_bins = "(80,0,3.2)"
		phi_bins2 = "(90,0,3.6)"
		title_phi1 = "muon #phi direction in St. 1"
		title_phi2 = "muon #phi direction in St. 2"
		hasxy1 = "&& fabs(phiM_st1_L1_1)>0 && fabs(phiM_st2_L1_1)>0 && fabs(csc_bending_angle12_xfactor_L1_1)>0"
		hasxy2 = "&& fabs(phiM_st1_L1_2)>0 && fabs(phiM_st2_L1_2)>0 && fabs(csc_bending_angle12_xfactor_L1_2)>0"
		#DrawProfileAndScatter_pT(filedir16,treename0, fraction, "pt","fabs(%s)"%y,"1/fabs("+y+")",x_bins,y_bins1, y_bins2,x_title,y_title2, y_title2+"^{-1}",sttitle,netas[neta],netas[neta+1],cuts+hasxy2,text,"Profile_Xfactor_PT_0901/GEMCSC_ctau0_%s_20160901_fraction%d_st2eta%dto%d_npar%d_withGE21"%(y,fraction,int(netas[neta]*10), int(netas[neta+1]*10), npar))
		#DrawProfileAndScatter_pT(filedir46,treename0, fraction, "pt","fabs(%s)"%y,"1/fabs("+y+")",x_bins,y_bins1, y_bins2,x_title,y_title2, y_title2+"^{-1}",sttitle_ctau1000,netas[neta],netas[neta+1],cuts_ctau1000,text,"Profile_Xfactor_PT_0901/GEMCSC_dxy10_50_ctau1000_%s_20160901_fraction%d_st2eta%dto%d_npar%d_withGE21"%(y,fraction,int(netas[neta]*10), int(netas[neta+1]*10), npar))
		#DrawProfileAndScatter_pT(filedir16,treename0, fraction, "abs(phiM_st1_L1_2)","abs(phiM_st2_L1_2)","1/fabs("+y+")", phi_bins,phi_bins2, y_bins2, title_phi1, title_phi2, y_title2+"^{-1}",sttitle,netas[neta],netas[neta+1],cuts+"&& pt>10"+hasxy2, text+",p_{T}>10 GeV","Profile_Xfactor_PT_0925/GEMCSC_ctau0_20160925_pt10_st2eta%dto%d_npar%d_withGE21"%(int(netas[neta]*10), int(netas[neta+1]*10), npar))
		#DrawProfileAndScatter_pT(filedir46,treename0, fraction, "abs(phiM_st1_L1_2)","abs(phiM_st2_L1_2)","1/fabs("+y+")", phi_bins,phi_bins2, y_bins2, title_phi1, title_phi2, y_title2+"^{-1}",sttitle_ctau1000,netas[neta],netas[neta+1],cuts_ctau1000+"&&pt>10"+hasxy2, text+",p_{t}>10 GeV","Profile_Xfactor_PT_0925/GEMCSC_ctau1000_20160925_pt10_st2eta%dto%d_npar%d_withGE21"%(int(netas[neta]*10), int(netas[neta+1]*10), npar))
		y = "csc_bending_angle12_xfactor_L1_1"
		#text2 = " %.1f<|#eta|<%.1f, without GE21"%(netas[neta],netas[neta+1])
		text2 = "ME2/1 only"
		#DrawProfileAndScatter_pT(filedir16,treename0, fraction, "pt","fabs(%s)"%y,"1/fabs("+y+")",x_bins,y_bins1, y_bins2,x_title,y_title2, y_title2+"^{-1}",sttitle,netas[neta],netas[neta+1],cuts+hasxy1,text2,"Profile_Xfactor_PT_0925/GEMCSC_ctau0_%s_20160925_fraction%d_st2eta%dto%d_npar%d_CSConly"%(y,fraction,int(netas[neta]*10), int(netas[neta+1]*10), npar))
		#DrawProfileAndScatter_pT(filedir46,treename0, fraction, "pt","fabs(%s)"%y,"1/fabs("+y+")",x_bins,y_bins1, y_bins2,x_title,y_title2, y_title2+"^{-1}",sttitle_ctau1000,netas[neta],netas[neta+1],cuts_ctau1000,text2,"Profile_Xfactor_PT_0901/GEMCSC_ctau1000_%s_20160901_fraction%d_st2eta%dto%d_npar%d_CSConly"%(y,fraction,int(netas[neta]*10), int(netas[neta+1]*10), npar))
		
		#DrawProfileAndScatter_pT(filedir16,treename0, fraction, "abs(phiM_st1_L1_1)","abs(phiM_st2_L1_1)","1/fabs("+y+")", phi_bins,phi_bins2, y_bins2, title_phi1, title_phi2, y_title2+"^{-1}",sttitle,netas[neta],netas[neta+1],cuts+"&& pt>10"+hasxy1,text2+",p_{T}>10 GeV","Profile_Xfactor_PT_0925/GEMCSC_ctau0_20160925_pt10_st2eta%dto%d_npar%d_CSConly"%(int(netas[neta]*10), int(netas[neta+1]*10), npar))
		#DrawProfileAndScatter_pT(filedir46,treename0, fraction, "abs(phiM_st1_L1_1)","abs(phiM_st2_L1_1)","1/fabs("+y+")", phi_bins,phi_bins2, y_bins2, title_phi1, title_phi2, y_title2+"^{-1}",sttitle_ctau1000,netas[neta],netas[neta+1],cuts_ctau1000+"&&pt>10"+hasxy1,text2+"p_{T}>10 GeV","Profile_Xfactor_PT_0925/GEMCSC_ctau1000_20160925_pt10_st2eta%dto%d_npar%d_CSConly"%(int(netas[neta]*10), int(netas[neta+1]*10), npar))
		y0 = "ptphi_diff_sh"
		text0 = " %.1f<|#eta|<%.1f, SIM level"%(netas[neta],netas[neta+1])
		#DrawProfileAndScatter_pT(filedir1,treename0, fraction, "pt","fabs(%s)"%y0,"1/fabs("+y0+")",x_bins,y_bins1, y_bins2,x_title,y_title2, y_title2+"^{-1}",sttitle,netas[neta],netas[neta+1],cuts,text0,"Profile_Xfactor_PT_0802/GEMCSC_ctau0_%s_20160802_fraction%d_npar%d_St1eta%d"%(y0,fraction, npar, neta))


	"""
	statbox = ROOT.TPaveText()
	statbox.SetX1NDC(0.15)
	statbox.SetX2NDC(0.4)
	statbox.SetY1NDC(0.5)
	statbox.SetY2NDC(0.85)
	statbox.SetFillStyle(0)
	statbox.SetBorderSize(0)
	statbox.SetTextFont(42)
    	stathead = statbox.AddText("Resolution")
	stathead.SetTextColor(ROOT.kRed)
	me11 = statbox.AddText("ME11: 0.00045")
	me11.SetTextColor(ROOT.kRed)
	ge11 = statbox.AddText("GE11: 0.00021")
	ge11.SetTextColor(ROOT.kRed)
	me21 = statbox.AddText("ME21: 0.00075")
	me21.SetTextColor(ROOT.kRed)
	ge21 = statbox.AddText("GE21: 0.00036")
	ge21.SetTextColor(ROOT.kRed)
	
	statbox.Draw("same")
	c1.Modified()
	"""
	#c1.SaveAs("%s_Plateau%d_simPt%d.png"%(picname, fractionToKeep,pt))
	#return c1
