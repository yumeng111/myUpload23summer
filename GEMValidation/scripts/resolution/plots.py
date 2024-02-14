import os

from resolution.GEMStub import *
from resolution.CSCStub import *
from resolution.L1Mu import *

## need to create directory structure - assume it does not exist yet
def makeDir(plotter):
    base = plotter.baseDir + "/"
    analyzer = plotter.analyzer
    paths = [
        base,
        base + analyzer,
        base + analyzer + "/resolution",
        base + analyzer + "/resolution/GEMStub",
        base + analyzer + "/resolution/CSCStub",
        base + analyzer + "/resolution/L1Mu",
    ]
    for p in paths:
        if not os.path.exists(p):
            os.mkdir(p)

def makeResolutionPlots(plotter, text):
    makeDir(plotter)
    CSCStub(plotter, text)
    GEMStub(plotter)
    makeNDigis(plotter, text)

def makeResolutionComparisonPlots(plotter, plotter2):
    CSCPosResolutionComparison(plotter, plotter2)
    CSCBendResolutionComparison(plotter, plotter2)

def makeComparePlots(plotterlist, text):
    CSCStubComparison1DAll(plotterlist, text)
    EMTFInputsCompare(plotterlist, text) 
