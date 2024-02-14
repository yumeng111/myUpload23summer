import os

from efficiency.CSCSimHit import *
from efficiency.CSCDigi import *
from efficiency.CSCStub import *

from efficiency.GEMSimHit import *
from efficiency.GEMDigi import *
from efficiency.GEMStub import *
from efficiency.GEMRecHit import *

from efficiency.L1Mu import *
from efficiency.L1Track import *

## need to create directory structure - assume it does not exist yet
def makeDir(plotter):
    base = plotter.baseDir + "/"
    analyzer = plotter.analyzer
    paths = [
        base,
        base + analyzer,
        base + analyzer + "/efficiency",
        base + analyzer + "/efficiency/GEMSimHit",
        base + analyzer + "/efficiency/GEMDigi",
        base + analyzer + "/efficiency/GEMStub",
        base + analyzer + "/efficiency/CSCSimHit",
        base + analyzer + "/efficiency/CSCDigi",
        base + analyzer + "/efficiency/CSCStub",
        base + analyzer + "/efficiency/L1Mu",
    ]
    for p in paths:
        if not os.path.exists(p):
            os.mkdir(p)

def makeEfficiencyPlots(plotter, text):
    makeDir(plotter)
    CSCSimHit(plotter)
    CSCDigi(plotter)
    CSCStub(plotter, text)
    GEMSimHit(plotter)
    GEMDigi(plotter)
    GEMStub(plotter)
    L1Mu(plotter, text)
    #L1Track(plotter)

def makeEfficiencyComparisonPlots(plotterlist, text):
    CSCStubComparisonAll(plotterlist, text)
    L1MuComparisonEtaAll(plotterlist, text)
