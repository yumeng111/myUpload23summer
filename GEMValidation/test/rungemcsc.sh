#!/bin/bash
echo 'start run  with ',$0,$1,$2
pushd /afs/cern.ch/work/t/tahuang/CSCEmulation/L1CSCtriggerPR/CMSSW_12_6_0_pre2/src/GEMCode/GEMValidation/test
# If workspace does not exist, create it once
eval `scramv1 runtime -sh`
##example cmsRun runGEMCSCAnalyzer_condor_all.py samplepT=10 maxEvents=1
cmsRun $1 samplepT=$2 maxEvents=-1
popd
echo 'finish!'
