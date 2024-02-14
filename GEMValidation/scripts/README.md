## scripts for making plots
This folder is to keep the scripts to make plots using ntuples produced by GEMCSCAnalyzer

### Master script: makePlots.py

Several parts in the master script:
  - Part1: define the object GEMCSCStubPlotter() with input file and analyzer name
  - Part2: make plots for each GEMCSCStubPlotter object
  - Part3: make plots to compare performance among a list of GEMCSCStubPlotter objects
  - Part4: read in the entries from two GEMCSCStubPlotter objects and fill the information into a new TTree to further comparison, imported from analyzeTree.py

## Subfolder and other scripts
  - resolution/: folder to keep scripts that make distribution and resolution plots
  - efficiency/: folder to keep scripts that make efficiency plots 
  - style/: folder to keep CMS style scripts
  - helpers/: folder to keep supportive scripts like draw1D, draw2D in Helpers.py and CSC label in stations.py
  - cuts/: folder to keep the scripst defining the cuts 
  - analyzeTree.py and plotCompareTree.py: read in entries from two TTrees and make plots to compare two TTrees
