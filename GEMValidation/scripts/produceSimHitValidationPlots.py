import sys

from ROOT import *

from cuts import *
from drawPlots import *
from BaseValidation import *
from TrackValidation import *
from GEMSimHitValidation import *
from ME0SimHitValidation import *
from RPCSimHitValidation import *
from CSCSimHitValidation import *
from gemChamberNumbering import *

## run quiet mode
sys.argv.append( '-b' )
ROOT.gROOT.SetBatch(1)

#_______________________________________________________________________________
def produceOccupanyPlots(plotter):
  for i in range(len(plotter.sel)):

    gemSimHitOccupancyXY(plotter,i)
    gemSimHitOccupancyRZ(plotter,i)
    gemSimHitTOF(plotter,i)
    cscSimHitOccupancyXY(plotter,i)
    rpcSimHitOccupancyXY(plotter,i)

    me0SimHitOccupancyRZ(plotter,i)
    me0SimHitTOF(plotter,i)
    me0SimHitOccupancyXY(plotter,i)
    #gemSimhitMomentum(plotter,i) 
    #GEMSimValidation.SimhitMomentum(plotter,i)

    """
    rpcSimHitOccupancyXY(plotter,i)
    rpcSimHitOccupancyRZ(plotter,i)
    rpcSimHitTOF(plotter,i)
    
    cscSimHitOccupancyXY(plotter,i)
    cscSimHitOccupancyRZ(plotter,i)
    cscSimHitTOF(plotter,i)
    
    """


#_______________________________________________________________________________
def produceMatchingPlots(plotter):
  gemSimTrackToSimHitMatchingLX(plotter) 
  gemSimTrackToSimHitMatchingLY(plotter) 
  gemSimTrackToSimHitMatchingEta(plotter) 
  gemSimTrackToSimHitMatchingPhi(plotter)
  me0SimTrackToSimHitMatchingLX(plotter) 
  me0SimTrackToSimHitMatchingLY(plotter) 
  me0SimTrackToSimHitMatchingEta(plotter) 
  me0SimTrackToSimHitMatchingPhi(plotter)
  

#_______________________________________________________________________________
if __name__ == "__main__":

  ## Style
  gStyle.SetStatStyle(0);

  plotter = SimHitPlotter()
  """
  simTrackProperties(plotter)
  gemChamberNumbering(plotter)
  
  """
  produceMatchingPlots(plotter)
  produceOccupanyPlots(plotter)
