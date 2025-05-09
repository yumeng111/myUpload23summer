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
#change:
#inputFile1 = "file:/afs/cern.ch/user/y/yumeng/CMSSW_14_0_0/src/GEMCode/GEMValidation/test/out_run2run3Noghost.root"
#inputFile1 = "file:/eos/user/y/yumeng/CMSSW_14_2_0_pre1/GEMCode/condor/condor_out_pr_pu200_1420pre1pretr224.root"
inputFile1 = "file:/eos/user/y/yumeng/CMSSW_14_2_0_pre1/GEMCode/test.root"

baseDir = "/eos/user/y/yumeng/CMSSW_14_2_0_pre1/GEMCode/"

text = ""

# Plotters for the first file
plotter1_run3 = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzerRun3CCLUTILT")
plotter1_run3.setLegend("cmssw_1409")

plotter1_run3layers = GEMCSCStubPlotter(inputFile1, baseDir, text, "GEMCSCAnalyzerRun3CCLUTILTclct3layers")
plotter1_run3layers.setLegend("cmssw_1409")

#makeEfficiencyComparisonPlots(plotter1_run3, text + " ")
#makeResolutionComparisonPlots (plotter1_run3, plotter4_run3)
#makeComparePlots (plotterlist_run3, text + " ")
makeEfficiencyPlots(plotter1_run3, text + " ")
makeEfficiencyComparisonPlots(plotter1_run3, text + " ")

#makeEfficiencyComparisonPlots(plotter1_run3layers, text + "clctNHitPattern=3")
# makeResolutionComparisonPlots (plotter1_run3, plotter4_run3)
#makeComparePlots (plotterlist_run3layers, text + " ")

# Compare the same tree across the two files
#comparePlotters(plotter1_run3, plotter2_run3, text, "GEMCSCAnalyzer")
