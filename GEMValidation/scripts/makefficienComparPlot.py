import sys
import os

from ROOT import TFile, TDirectory, TTree, gROOT
from efficiency.plots import *
from resolution.plots import *
from occupancy.plots import *

from analyzeTree import *

## run quiet mode
sys.argv.append( '-b' )
gROOT.SetBatch(1)

class GEMCSCStubPlotter():
  def __init__(self, inputFile, baseDir, sample, analyzer = "GEMCSCAnalyzer"):
    self.inputFile = inputFile
    self.baseDir = baseDir
    if not os.path.isdir(self.baseDir): 
        os.mkdir(self.baseDir)
    self.ext = ".pdf"
    self.legend= analyzer
    self.analyzer = analyzer
    self.targetDir = self.baseDir + self.analyzer + "/"
    self.file = TFile.Open(self.inputFile)
    self.sample = sample
    self.dirAna = (self.file).Get(self.analyzer)
    self.tree = self.dirAna.Get("simTrack")
    self.treeFriends = ["genParticle", "cscSimHit", "cscDigi", "cscStub",
                        "gemSimHit", "gemDigi", "gemStub", "l1Mu", "l1Track"]
    for p in self.treeFriends:
      self.tree.AddFriend(self.dirAna.Get(p))
    self.yMin = 0.45
    self.yMax = 1.08
  def setLegend(self, leg):
    self.legend = leg
  def getTree(self):
    return self.tree

pT=100
#suffix = "_condor_looseLCTmatch20220908v2"
#inputFile = "out_gemcscana_%dGeV_all%s.root"%(pT, suffix)
#baseDir = "RelValpT%dplots%s/"%(pT, suffix)
#text="RelValpT%dGeV"%pT
suffix = ""
inputFile = "/eos/user/y/yumeng/GEMCode/out_Phase2C17I13M9_mu_pt100/mu_pt100_phase2fall22/crab_GEMCSCanalyzer_Phase2C17I13M9_mu_pt100/231005_131952/out_gemcscana.root"
inputFileTao = "/eos/user/y/yumeng/GEMCode/out_Phase2C17I13M9_mu_pt100/Tao_out_gemcscana_RelValpT100GeV_all_condor_CLCTsortlooseLCT20220911v2.root"
#baseDir = "RelValpT%dplots%s/"%(pT, suffix)
baseDir = "mu_pt100_efficienCompar/"
text="mu_pt100"

#inputFile = "out_gemcscana_test.root"
#baseDir = "TestRelValpT%dplots/"%pT
#text="RelValpT%dGeV"%pT
## needs to be cleaned up
#plotter  = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzer")
#plotter.setLegend("Run3OldEmulator")
#plotter0 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun2")
#plotter0.setLegend("Run2Emulator")
#plotter2 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUTILT")
#plotter2.setLegend("Run3NewEmulator")
#plotter3 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUTv1")
#plotter3.setLegend("Run3WithDeadTimeNoILT")
#plotter4 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUTv2")
#plotter4.setLegend("Run3NoCCLUTNoILT")
#plotter4.setLegend("Run3PreTrigMatch")
#plotter5 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUT")
#plotter5.setLegend("Run3CCLUTNoILT")
#plotter6 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUTv0")
#plotter6.setLegend("Run3NoDeadTimezone")

#makeEfficiencyPlots(plotter , text)
#makeResolutionPlots(plotter , text)

#makeEfficiencyPlots(plotter2, text)
#makeResolutionPlots(plotter2, text)

#makeEfficiencyPlots(plotter0, text)
#makeResolutionPlots(plotter0, text)

#makeEfficiencyPlots(plotter3, text)
#makeResolutionPlots(plotter3, text)

#makeEfficiencyPlots(plotter4, text)
#makeResolutionPlots(plotter4, text)

#plotterlist = [plotter0, plotter, plotter2, plotter3, plotter6]
### efficiency comparison
#makeEfficiencyComparisonPlots(plotterlist, text)
#makeResolutionComparisonPlots(plotter0, plotter2)
#makeComparePlots(plotterlist, text)
#treename = "compareTree"
#analyzeTwoTrees(plotter0, plotter2, treename, "test"+inputFile.replace("gemcscana","compare"))

plotter2 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzer")
#plotter2 = GEMCSCStubPlotter(inputFile, baseDir, text, "GEMCSCAnalyzerRun3CCLUTILT")
plotter2.setLegend("Run3NewEmulator")
#plotter0 = GEMCSCStubPlotter(inputFileTao, baseDir, text, "GEMCSCAnalyzer")
plotter0 = GEMCSCStubPlotter(inputFileTao, baseDir, text, "GEMCSCAnalyzerRun3CCLUTILT")
plotter0.setLegend("Tao_Run3NewEmulator")

#makeEfficiencyPlots(plotter2, text)
#makeResolutionPlots(plotter2, text)

#makeEfficiencyPlots(plotter0, text)
#makeResolutionPlots(plotter0, text)

plotterlist = [plotter0, plotter2]
### efficiency comparison
makeEfficiencyComparisonPlots(plotterlist, text)
#makeResolutionComparisonPlots(plotter0, plotter2)
#makeComparePlots(plotterlist, text)
treename = "compareTree"
#analyzeTwoTrees(plotter0, plotter2, treename, "test"+inputFile.replace("gemcscana","compare"))
