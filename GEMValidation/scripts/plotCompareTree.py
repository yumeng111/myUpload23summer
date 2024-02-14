from ROOT import *
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

## scripts to compare two TTrees
def CompareTrees(filelist, var, leglist, xtitle, h_bins, cuts, plotsuffix, text, outfolder):
    """ compare the variable between stubs used by EMTF and stubs matched to simtrack """

    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    xTitle = xtitle
    yTitle = "Entries"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(0.08)
    base.GetXaxis().SetLabelSize(0.04)
    base.GetYaxis().SetLabelSize(0.04)
    base.GetXaxis().SetTitleSize(0.04)
    base.GetYaxis().SetTitleSize(0.04)
    chlist = []
    treename = "compareTree"
    hlist = []
    ymax = 0.0
    for i in range(len(filelist)):
        chlist.append(TChain(treename))
        chlist[i].Add(filelist[i])
        hlist.append(draw_1D(chlist[i], title, h_bins, var, cuts, "same", kcolors[i], markers[i], 1))
        hlist[-1].SetBinContent(nBins, hlist[-1].GetBinContent(nBins+1))
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())

    base.SetMaximum(ymax * 1.2)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    #leg = TLegend(0.15,0.6,.45,0.9, "", "brNDC");
    leg = TLegend(0.58,0.6,0.88,0.6+len(filelist)*0.05, "", "brNDC");
    #leg.SetHeader(plotsuffix)
    #leg.SetHeader("gen|#eta|>1.65, EMTF pT >= %d GeV"%ptcut)
    #leg.SetHeader("Run2 EMTF pT> Run3 EMTF pT")
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.035)
    for i in range(0, int(len(hlist))):
        hlist[i].Draw("histsame")
        leg.AddEntry(hlist[i], leglist[i],"l")

    leg.Draw("same");
    if "phi" in var:
        base.SetMinimum(1)
        gPad.SetLogy()

    txt = drawCSCLabel(text, 0.75,0.25,0.035)
    #var_ptstr = variable+"_pT%d_"%ptcut
    #var_ptstr = variable1+"_pT%d_geneta1p65"%ptcut
    var_ptstr = plotsuffix
    c.Print(outfolder+"CompareTree_"+plotsuffix+".pdf")

    del base, leglist, hlist, c



def EMTFStubResolution(tree, varlist, leglist, xtitle, h_bins, cuts, plotsuffix, text, outfolder):
    """ compare the variable between stubs used by EMTF and stubs matched to simtrack """

    ptlist = [0, 5, 10, 15, 20]
    #h_bins = "(100,-1,1)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGrid(1,1)
    xTitle = xtitle
    yTitle = "Entries"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(0.08)
    base.GetXaxis().SetLabelSize(0.04)
    base.GetYaxis().SetLabelSize(0.04)
    base.GetXaxis().SetTitleSize(0.04)
    base.GetYaxis().SetTitleSize(0.04)
    hlist = []
    ymax = 0.0
    for i in range(len(varlist)):
        toPlot1 = varlist[i]
        if len(varlist) == len(leglist):
            hlist.append(draw_1D(tree, title, h_bins, toPlot1, cuts, "same", kcolors[i], markers[i], 1))
        else:
            hlist.append(draw_1D(tree, title, h_bins, toPlot1, cuts, "same", kcolors[int(i/2)], markers[i], i%2+1))
        #hlist[-1].Scale(1./hlist[-1].GetEntries())
        #print("i ", i, " entries ", hlist[i].GetEntries())
        if hlist[-1].GetBinContent(hlist[-1].GetMaximumBin()) > ymax:
            ymax = hlist[-1].GetBinContent(hlist[-1].GetMaximumBin())

    base.SetMaximum(ymax * 1.2)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    #leg = TLegend(0.15,0.6,.45,0.6+len(leglist)*0.05, "", "brNDC");
    leg = TLegend(0.55,0.6,.85,0.6+len(leglist)*0.05, "", "brNDC");
    #leg.SetHeader(plotsuffix)
    #leg.SetHeader("gen|#eta|>1.65, EMTF pT >= %d GeV"%ptcut)
    #leg.SetHeader("EMTF pT >= %d GeV"%ptcut)
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextFont(42)
    leg.SetTextSize(0.03)
    if len(hlist) == len(leglist):
        for i in range(0, int(len(hlist))):
            hlist[i].Draw("histsame")
            leg.AddEntry(hlist[i], leglist[i],"pl")
    else:
        for i in range(0, len(leglist)):
            hlist[2*i].Draw("histsame")
            hlist[2*i+1].Draw("histsame")
            leg.AddEntry(hlist[2*i], leglist[i],"pl")
    leg.Draw("same");
    if "phi" in varlist[0]:
        base.SetMinimum(1)
        gPad.SetLogy()

    #txt = drawCSCLabel(text, 0.15,0.25,0.035)
    txt = drawCSCLabel(text, 0.5,0.25,0.035)
    #var_ptstr = variable+"_pT%d_"%ptcut
    #var_ptstr = variable1+"_pT%d_geneta1p65"%ptcut
    var_ptstr = plotsuffix
    c.Print(outfolder+"CompareTree_"+plotsuffix+".pdf")

    del base, leglist, hlist, c



def compare2D(t, title, xbins, ybins, xtodraw, ytodraw, cuts, text, plotsuffix, outfolder):
    c = newCanvas()
    gPad.SetGrid(1,1)
    gPad.SetRightMargin(0.15)

    xnBins  = int(xbins[1:-1].split(',')[0])
    ynBins  = int(ybins[1:-1].split(',')[0])

    hist = draw_2D(t, title, xbins, ybins, "%s:%s"%(ytodraw, xtodraw), cuts, "")
    drawopt = "colz"
    if max(xnBins, ynBins) <= 20:
        hist.SetMarkerSize(2)
        hist.SetMarkerColor(kRed)
        drawopt = "colztext"
    hist.Draw(drawopt)
    hist.GetXaxis().SetLabelSize(0.04)
    hist.GetYaxis().SetLabelSize(0.04)
    hist.GetXaxis().SetTitleSize(0.04)
    hist.GetYaxis().SetTitleSize(0.04)
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)
    txt = drawCSCLabel(text, 0.15,0.75,0.03)

    c.Print(outfolder+"CompareTree_"+plotsuffix+".pdf")
    del c,hist

def EMTFEtaEff(tree, dencutlist, numcutlist, leglist, text, plotsuffix, outfolder):

    ## variables for the plot
    xTitle = "Generated muon |#eta|"
    yTitle = "EMTF efficiency"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    toPlot = "TMath::Abs(tree1_gen_eta)"

    #h_bins = "(20,1.2,2.4)"
    h_bins = "(14,1.56,2.4)"
    nBins = int(h_bins[1:-1].split(',')[0])
    minBin = float(h_bins[1:-1].split(',')[1])
    maxBin = float(h_bins[1:-1].split(',')[2])

    c = newCanvas()
    gPad.SetGridx(xGrid)
    gPad.SetGridy(yGrid)

    base  = TH1F("base",title,nBins,minBin,maxBin)
    base.SetMinimum(0)
    base.SetMaximum(1.05)
    base.GetXaxis().SetLabelSize(0.05)
    base.GetYaxis().SetLabelSize(0.05)
    base.GetXaxis().SetTitleSize(0.05)
    base.GetYaxis().SetTitleSize(0.05)
    base.Draw("")
    CMS_lumi.CMS_lumi(c, iPeriod, iPos)

    
    leg = TLegend(0.45,0.15,.75,0.18+0.04*len(dencutlist), "", "brNDC");
    leg.SetBorderSize(0)
    leg.SetFillStyle(0)
    leg.SetTextSize(0.03)
    leg.SetHeader(plotsuffix)

    hlist = []
    for i in range(len(dencutlist)):
        hlist.append(draw_geff(tree, title, h_bins, toPlot, dencutlist[i], numcutlist[i],  "same", kcolors[i], markers[i]))
        leg.AddEntry(hlist[-1], leglist[i],"l")
    leg.Draw("same");
    txt = drawCSCLabel(text, 0.15,0.25,0.035)


    c.Print(outfolder+"CompareTree_"+plotsuffix+".pdf")
    del c, base, hlist



pT= 1000
suffix="_condor_looseLCTmatch20220908v2"
outfolder = "Compareplots%dGeV%s/"%(pT,suffix)
if not os.path.isdir(outfolder):
    os.mkdir(outfolder)
inputFile = "testout_compare_%dGeV_all%s.root"%(pT, suffix)
text= "RelValpT%dGeV"%pT
tfile = TFile.Open(inputFile)
treename = "compareTree"
tree = tfile.Get(treename)

topTitle = "EMTF Track Mode Comparison"
topTitle = ""
xTitle = "Run2Emulator EMTF track mode"
yTitle = "Run3Emulator EMTF track mode"
title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
xtodraw="tree0_emtf_mode"
ytodraw="tree1_emtf_mode"
xbins = "(16,0,16)"
emtfcut = TCut("abs(tree1_gen_eta)>1.65 && tree0_emtf_pt>20 && tree1_emtf_pt<=20")
plotsuffix= "EMTFTrackMode_Run2vsRun3_Run2EMTFpTG20_Run3EMTFpTL20"
#compare2D(tree, title, xbins, xbins, xtodraw, ytodraw, emtfcut, text, plotsuffix, outfolder)
xbins_pt = "(16,0,256)"
xtodraw_pt = "tree0_emtf_pt"
ytodraw_pt = "tree1_emtf_pt"
xTitle = "Run2Emulator EMTF pT"
yTitle = "Run3Emulator EMTF pT"
for n in range(6, 17):
    if n==16: n=100
    topTitle = "EMTF pT comparison, Total hits <= %d for all 4 stations, ring1"%n
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    emtfcut = TCut("tree0_emtf_pt>0 && tree1_emtf_pt>0 && abs(tree1_gen_eta)>1.65 && totalcomps_dg_ME11 <=%d && totalcomps_dg_ME21 <= %d && totalcomps_dg_ME31<=%d && totalcomps_dg_ME41<=%d"%(n,n,n,n))
    plotsuffix= "EMTFTrackpT_Run2vsRun3_ring1_allstation_ncompsthresh%d"%n
    compare2D(tree, title, xbins_pt, xbins_pt, xtodraw_pt, ytodraw_pt, emtfcut, text, plotsuffix, outfolder)
    emtfcut = TCut("tree0_emtf_pt>0 && tree1_emtf_pt>0 && abs(tree1_gen_eta)>1.65 && totalwires_dg_ME11 <=%d && totalwires_dg_ME21 <= %d && totalwires_dg_ME31<=%d && totalwires_dg_ME41<=%d"%(n,n,n,n))
    plotsuffix= "EMTFTrackpT_Run2vsRun3_ring1_allstation_ncompsthresh%d"%n
    compare2D(tree, title, xbins_pt, xbins_pt, xtodraw_pt, ytodraw_pt, emtfcut, text, plotsuffix, outfolder)
    topTitle = "EMTF pT comparison, Total hits >= %d for at least one stations, ring1"%n
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    emtfcut = TCut("tree0_emtf_pt>0 && tree1_emtf_pt>0 && abs(tree1_gen_eta)>1.65 && (totalcomps_dg_ME11 >=%d || totalcomps_dg_ME21 >= %d || totalcomps_dg_ME31 >= %d || totalcomps_dg_ME41>=%d)"%(n,n,n,n))
    plotsuffix= "EMTFTrackpT_Run2vsRun3_ring1_allstation_ncompsthreshgreater%d"%n
    compare2D(tree, title, xbins_pt, xbins_pt, xtodraw_pt, ytodraw_pt, emtfcut, text, plotsuffix, outfolder)
    emtfcut = TCut("tree0_emtf_pt>0 && tree1_emtf_pt>0 && abs(tree1_gen_eta)>1.65 && (totalwires_dg_ME11 >=%d || totalwires_dg_ME21 >= %d || totalwires_dg_ME31 >= %d || totalwires_dg_ME41>=%d)"%(n,n,n,n))
    plotsuffix= "EMTFTrackpT_Run2vsRun3_ring1_allstation_nwiresthreshgreater%d"%n
    compare2D(tree, title, xbins_pt, xbins_pt, xtodraw_pt, ytodraw_pt, emtfcut, text, plotsuffix, outfolder)


for st in range(0, len(cscStations)):
    if st == 1 or st==2: continue
    xTitle="LCT match type, Run3 emualtor"
    yTitle="LCT match type, Run2 emualtor"
    title = "%s;%s;%s"%(topTitle,xTitle,yTitle)
    chamberstr = cscStations[st].labelc
    thistext = chamberstr 
    lctmatch_title = "LCT Match type in %s"%chamberstr+"Run2 Emulator vs Run3 Emulator"
    lctmatchtype_bins = "(5,-1,4)"
    matchtypey = "tree0_lct_matchtype_%s"%cscStations[st].labelc
    matchtypex = "tree1_lct_matchtype_%s"%cscStations[st].labelc
    matchtypecut = "tree0_lct_matchtype_%s>0"%cscStations[st].labelc
    plotsuffix = "lct_matchtype_Run2vsRun3_%s"%cscStations[st].labelc
    compare2D(tree, title, lctmatchtype_bins, lctmatchtype_bins, matchtypex, matchtypey, matchtypecut, text, plotsuffix, outfolder)

plotsuffix = "missstub_Run2EMTFpTG20_Run3EMTFpTL20"
plotsuffix2 = "matchstub_Run2EMTFpTG20_Run3EMTFpTL20"
dencutlist = []; numcutlist = []; leglist = []
dencutlist2= []; numcutlist2 = []
for st in range(0, 4):
    dencutlist.append(TCut("abs(tree1_gen_eta)>1.65 && tree0_emtf_pt>20 && tree1_emtf_pt<=20 && tree0_emtf_hasME%d"%(st+1)))
    numcutlist.append(TCut("abs(tree1_gen_eta)>1.65 && tree0_emtf_pt>20 && tree1_emtf_pt<=20 && tree1_emtf_hasME%d"%(st+1)))
    dencutlist2.append(TCut("abs(tree1_gen_eta)>1.65 && tree0_emtf_pt>20 && tree1_emtf_pt<=20 && tree0_emtf_hasME%d && tree1_emtf_hasME%d"%(st+1, st+1)))
    numcutlist2.append(TCut("abs(tree1_gen_eta)>1.65 && tree0_emtf_pt>20 && tree1_emtf_pt<=20 && emtfhit_st%d_match"%(st+1)))
    leglist.append("station %d"%(st+1))
#EMTFEtaEff(tree, dencutlist, numcutlist, leglist, text, plotsuffix, outfolder)
#EMTFEtaEff(tree, dencutlist2,numcutlist2,leglist, text, plotsuffix2,outfolder)
numcutlist_nhits0  = []
numcutlist_nhits1  = []
dencutlist_comps = []; dencutlist_wires = []
leglist_comps = []
leglist_wires = []
dencutlist_comps2 = []; dencutlist_wires2 = []
leglist_comps2 = []
leglist_wires2 = []
for nhit in range(0, 6):
    tothits = nhit*3+6
    leglist_wires.append("at least one station with nwires >=%d"%tothits)
    leglist_comps.append("at least one station with ncomparators >=%d"%tothits)
    ncompscut = TCut("totalcomps_dg_ME11>=%d || totalcomps_dg_ME21>=%d || totalcomps_dg_ME31>=%d || totalcomps_dg_ME41>=%d"%(tothits, tothits, tothits, tothits))
    nwirescut = TCut("totalwires_dg_ME11 >=%d || totalwires_dg_ME21 >= %d || totalwires_dg_ME31 >= %d || totalwires_dg_ME41>=%d"%(tothits,tothits,tothits,tothits))
    leglist_wires2.append("All stations with nwires <=%d"%tothits)
    leglist_comps2.append("All stations with ncomparators <=%d"%tothits)
    ncompscut2 = TCut("totalcomps_dg_ME11<=%d && totalcomps_dg_ME21<=%d && totalcomps_dg_ME31<=%d && totalcomps_dg_ME41<=%d"%(tothits, tothits, tothits, tothits))
    nwirescut2 = TCut("totalwires_dg_ME11<=%d && totalwires_dg_ME21<=%d && totalwires_dg_ME31<=%d && totalwires_dg_ME41<=%d"%(tothits,tothits,tothits,tothits))
    dencutlist_comps.append(AND(ncompscut, TCut("abs(tree1_gen_eta)>1.65")))
    dencutlist_wires.append(AND(nwirescut, TCut("abs(tree1_gen_eta)>1.65")))
    dencutlist_comps2.append(AND(ncompscut2, TCut("abs(tree1_gen_eta)>1.65")))
    dencutlist_wires2.append(AND(nwirescut2, TCut("abs(tree1_gen_eta)>1.65")))
    numcutlist_nhits0.append(TCut("tree0_emtf_pt>=20"))
    numcutlist_nhits1.append(TCut("tree1_emtf_pt>=20"))
plotsuffix = "EMTFEff_Run2Emulator_denncompsmin_numempt20"
EMTFEtaEff(tree, dencutlist_comps, numcutlist_nhits0, leglist_comps, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run3Emulator_denncompsmin_numempt20"
EMTFEtaEff(tree, dencutlist_comps, numcutlist_nhits1, leglist_comps, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run2Emulator_dennwiresmin_numempt20"
EMTFEtaEff(tree, dencutlist_wires, numcutlist_nhits0, leglist_wires, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run3Emulator_dennwiresmin_numempt20"
EMTFEtaEff(tree, dencutlist_wires, numcutlist_nhits1, leglist_wires, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run2Emulator_denncompsmax_numempt20"
EMTFEtaEff(tree, dencutlist_comps2, numcutlist_nhits0, leglist_comps2, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run3Emulator_denncompsmax_numempt20"
EMTFEtaEff(tree, dencutlist_comps2, numcutlist_nhits1, leglist_comps2, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run2Emulator_dennwiresmax_numempt20"
EMTFEtaEff(tree, dencutlist_wires2, numcutlist_nhits0, leglist_wires2, text, plotsuffix, outfolder)
plotsuffix = "EMTFEff_Run3Emulator_dennwiresmax_numempt20"
EMTFEtaEff(tree, dencutlist_wires2, numcutlist_nhits1, leglist_wires2, text, plotsuffix, outfolder)

nhits_bins = "(60,0,60)"
def plotpTcomparison():
    filelist = ["out_compare_10GeV_all.root", "out_compare_100GeV_all.root", "out_compare_1000GeV_all.root"]
    ptlegs = ["sim muon, pT=10GeV","sim muon, pT=100GeV","sim muon, pT=1000GeV"]
    todrawlist = ["totalcomps_dg_","totalwires_dg_"]
    xtitlelist = ["Total comparator digis", "Total wire digis"]
    outfolder_combined = "Compareplots_allpTs/"
    genetacut1 = "abs(tree1_gen_eta)>1.60"
    genetacut2 = "abs(tree1_gen_eta)>1.60 && tree0_emtf_pt > tree1_emtf_pt*2"
    genetacut3 = "abs(tree1_gen_eta)>1.60 && tree0_emtf_pt > 20 && tree1_emtf_pt < 20"

    for st in range(0, len(cscStations)):
        if st == 1 or st==2: continue
        chamberstr = cscStations[st].labelc
        thistext = chamberstr 
        for it, todraw in enumerate(todrawlist):
            todraw = todraw + chamberstr
            xtitle = xtitlelist[it]
            cuts1 = genetacut1 +" && " +todraw +">0 "
            plotsuffix = "differentPt_"+todraw+"_noEMTFptCut"
            CompareTrees(filelist, todraw, ptlegs, xtitle, nhits_bins, cuts1, plotsuffix, thistext, outfolder_combined)
            cuts2 = genetacut2 +" && " +todraw +">0 "
            plotsuffix = "differentPt_"+todraw+"_Run2EMTFPTlargerv2"
            CompareTrees(filelist, todraw, ptlegs, xtitle, nhits_bins, cuts1, plotsuffix, thistext, outfolder_combined)
            cuts3 = genetacut3 +" && " +todraw +">0 "
            plotsuffix = "differentPt_"+todraw+"_Run2EMTFPTlargerv3"
            CompareTrees(filelist, todraw, ptlegs, xtitle, nhits_bins, cuts1, plotsuffix, thistext, outfolder_combined)

plotpTcomparison()
varlist = ["totalcomps_dg_ME11","totalcomps_dg_ME21","totalcomps_dg_ME31","totalcomps_dg_ME41"]
leglist = ["Total comparator hits in ME11", "Total comparator hits in ME21","Total comparator hits in ME31","Total comparator hits in ME41"]
xtitle = "Total comparator hits"
genetacut = "abs(tree1_gen_eta)>1.65"
plotsuffix="NComparatorsdistribution_allstations_ring1"
EMTFStubResolution(tree, varlist, leglist, xtitle, nhits_bins, genetacut, plotsuffix, text, outfolder)
genetacut = "abs(tree1_gen_eta)>1.65 && tree1_emtf_pt<20"
plotsuffix="NComparatorsdistribution_allstations_ring1_Run3emulatorpTlessthan20"
EMTFStubResolution(tree, varlist, leglist, xtitle, nhits_bins, genetacut, plotsuffix, text, outfolder)
varlist = ["totalwires_dg_ME11","totalwires_dg_ME21","totalwires_dg_ME31","totalwires_dg_ME41"]
leglist = ["Total wire hits in ME11", "Total wire hits in ME21","Total wire hits in ME31","Total wire hits in ME41"]
xtitle = "Total wire hits"
genetacut = "abs(tree1_gen_eta)>1.65"
plotsuffix="NWiresdistribution_allstations_ring1"
EMTFStubResolution(tree, varlist, leglist, xtitle, nhits_bins, genetacut, plotsuffix, text, outfolder)
genetacut = "abs(tree1_gen_eta)>1.65 && tree1_emtf_pt<20"
plotsuffix="NWiresdistribution_allstations_ring1_Run3emulatorpTlessthan20"
EMTFStubResolution(tree, varlist, leglist, xtitle, nhits_bins, genetacut, plotsuffix, text, outfolder)


phires_bins = "(100, -0.1, 0.1)"
xtitle = "#phi(stub) - #phi(simhits) [Rad]"
for st in range(0, 4):
    var1 = "tree0_emtfhit_st%d_phi - tree0_simhits_st%d_phi"%(st+1, st+1)
    var2 = "tree0_cscstub_st%d_phi - tree0_simhits_st%d_phi"%(st+1, st+1)
    var3 = "tree1_emtfhit_st%d_phi - tree1_simhits_st%d_phi"%(st+1, st+1)
    var4 = "tree1_cscstub_st%d_phi - tree1_simhits_st%d_phi"%(st+1, st+1)
    varlist = [var1, var2, var3, var4]
    leglist = ["Run2Emulator", "Run3NewEmulator"]
    #cuts =TCut("abs(tree1_gen_eta)>1.65 && tree0_simhits_st%d_phi>-9 && tree1_simhits_st%d_phi>-9 && tree0_emtf_pt>20 && tree1_emtf_pt<=20"%(st+1, st+1)) 
    #plotsuffix="stubphires_eta1p65_st%d_Run2EMTFpTG20_Run3EMTFpTL20"%(st+1)
    cuts =TCut("abs(tree1_gen_eta)>1.65 && tree0_simhits_st%d_phi>-9 && tree1_simhits_st%d_phi>-9"%(st+1, st+1)) 
    plotsuffix="stubphires_eta1p65_st%d"%(st+1)
    #EMTFStubResolution(tree, varlist, leglist, xtitle, phires_bins, cuts, plotsuffix, text, outfolder)

    cuts =TCut("abs(tree1_gen_eta)>1.65 && tree0_simhits_st%d_phi>-9 && tree1_simhits_st%d_phi>-9 && tree0_emtf_pt>20 && tree1_emtf_pt<=20"%(st+1, st+1)) 
    plotsuffix="stubphires_eta1p65_st%d_Run2EMTFpTG20_Run3EMTFpTL20"%(st+1)
    #EMTFStubResolution(tree, varlist, leglist, xtitle, phires_bins, cuts, plotsuffix, text, outfolder)

    phidiff1 = "emtfhit_st%d_phidiff"%(st+1)
    phidiff2 = "cscstub_st%d_phidiff"%(st+1)
    philist = [phidiff1, phidiff2]
    leglist = ["emtfhit", "cscstub"]
    plotsuffix="stubphidiffRun2vsRun3_eta1p65_st%d"%(st+1)
    xtitle = "#phi(Run2Emulator) - #phi(Run3NewEmulator) [Rad]"
    #EMTFStubResolution(tree, philist, leglist, xtitle, phires_bins, cuts, plotsuffix, text, outfolder)
