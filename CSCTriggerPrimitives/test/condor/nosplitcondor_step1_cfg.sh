#!/bin/sh
#!/bin/bash

# Source CMS environment
#source /cvmfs/cms.cern.ch/cmsset_default.sh

# Go to your CMSSW project directory
#cd /path/to/your/CMSSW_X_Y_Z/src
cd /afs/cern.ch/user/y/yumeng/CMSSW_13_0_14/src/L1Trigger/CSCTriggerPrimitives/test/condor
eval `scramv1 runtime -sh` # Set CMSSW environment

# Run CMSSW
cmsRun step_1_cfg.py


