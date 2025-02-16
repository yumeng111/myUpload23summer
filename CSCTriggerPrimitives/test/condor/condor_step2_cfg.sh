#!/bin/sh
#!/bin/bash

# Source CMS environment
#source /cvmfs/cms.cern.ch/cmsset_default.sh

# Go to your CMSSW project directory
#cd /path/to/your/CMSSW_X_Y_Z/src
cd /afs/cern.ch/user/y/yumeng/CMSSW_13_0_14/src/L1Trigger/CSCTriggerPrimitives/test/condor
eval `scramv1 runtime -sh` # Set CMSSW environment

cp step_2_cfg_template.py step_2_cfg_$1.py

sed -i "s/placeholder/$1/g" step_2_cfg_$1.py

# Run CMSSW
cmsRun step_2_cfg_$1.py

rm step_2_cfg_$1.py

:
