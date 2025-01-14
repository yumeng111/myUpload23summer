import sys
import os

from ROOT import TFile, TDirectory, TTree, gROOT
from efficiency.plots import *
from resolution.plots import *
from occupancy.plots import *

from analyzeTree import *

## Run quiet mode
sys.argv.append('-b')
gROOT.SetBatch(1)

class GEMCSCStubPlotter():
    def __init__(self, inputFile, baseDir, sample, analyzer="GEMCSCAnalyzer"):
        self.inputFile = inputFile
        self.baseDir = baseDir
        if not os.path.isdir(self.baseDir):
            os.mkdir(self.baseDir)
        self.ext = ".pdf"
        self.legend = analyzer
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

# def comparePlotters(plotter1, plotter2, text, tree_name):
#     """
#     Compare two plotters by analyzing the specified tree.
#     """
#     print(f"Comparing tree '{tree_name}' between two files...")
#     analyzeTwoTrees(plotter1, plotter2, tree_name, f"{text}_{tree_name}_comparison")

# def analyzeTwoTrees(plotter1, plotter2, tree_name, output_name):
#     """
#     Analyze and compare two trees from different files.
#     """
#     tree1 = plotter1.getTree()
#     tree2 = plotter2.getTree()

#     totalEntries1 = tree1.GetEntries()
#     totalEntries2 = tree2.GetEntries()

#     print(f"Total entries in {tree_name} from first file: {totalEntries1}")
#     print(f"Total entries in {tree_name} from second file: {totalEntries2}")

#     # Example comparison logic
#     if totalEntries1 != totalEntries2:
#         print(f"Mismatch: {totalEntries1} entries vs {totalEntries2} entries")
#     else:
#         print(f"Entries match: {totalEntries1}")

#     # add more detailed comparisons(e.g., histograms, branch values)


pT = 1000
suffix = ""
#inputFile1 = "file:/afs/cern.ch/user/y/yumeng/CMSSW_14_0_0/src/GEMCode/GEMValidation/test/out_run2run3Noghost.root"
inputFile1 = "file:/eos/user/y/yumeng/CMSSW_14_0_0/GEMCode/out_run2run3Noghost.root"
inputFile2 = "file:/eos/user/y/yumeng/CMSSW_14_0_0/GEMCode/out_run2run3.root"
baseDir = "/eos/user/y/yumeng/CMSSW_14_0_0/GEMCode/plot1000_2files/"
text = "pT1000"


# Plotters for the first file (File 1 - NoGhost)
plotter1_run3 = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzer")
plotter1_run3.setLegend("Run3Emulator_NoGhost")

plotter1_run2 = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzerRun2")
plotter1_run2.setLegend("Run2Emulator_NoGhost")

plotter1_nolocaldead = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzerRun3nolocaldead")
plotter1_nolocaldead.setLegend("Run3NoLocalDead_NoGhost")

plotter1_localshower = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzerRun3localshower")
plotter1_localshower.setLegend("Run3LocalShower_NoGhost")

# Plotters for the second file (File 2 - AddGhost)
plotter2_run3 = GEMCSCStubPlotter(inputFile2, baseDir, text, "GEMCSCAnalyzer")
plotter2_run3.setLegend("Run3Emulator_AddGhost")

plotter2_run2 = GEMCSCStubPlotter(inputFile2, baseDir, text, "GEMCSCAnalyzerRun2")
plotter2_run2.setLegend("Run2Emulator_AddGhost")

plotter2_nolocaldead = GEMCSCStubPlotter(inputFile2, baseDir, text, "GEMCSCAnalyzerRun3nolocaldead")
plotter2_nolocaldead.setLegend("Run3NoLocalDead_AddGhost")

plotter2_localshower = GEMCSCStubPlotter(inputFile2, baseDir, text, "GEMCSCAnalyzerRun3localshower")
plotter2_localshower.setLegend("Run3LocalShower_AddGhost")

# Compare each tree across the two files using makeEfficiencyComparisonPlots
plotterlist_run3 = [plotter1_run3, plotter2_run3]
makeEfficiencyComparisonPlots(plotterlist_run3, text + "_Run3")

plotterlist_run2 = [plotter1_run2, plotter2_run2]
makeEfficiencyComparisonPlots(plotterlist_run2, text + "_Run2")

plotterlist_nolocaldead = [plotter1_nolocaldead, plotter2_nolocaldead]
makeEfficiencyComparisonPlots(plotterlist_nolocaldead, text + "_NoLocalDead")

plotterlist_localshower = [plotter1_localshower, plotter2_localshower]
makeEfficiencyComparisonPlots(plotterlist_localshower, text + "_LocalShower")

# Compare the same tree across the two files
#comparePlotters(plotter1_run3, plotter2_run3, text, "GEMCSCAnalyzer")
