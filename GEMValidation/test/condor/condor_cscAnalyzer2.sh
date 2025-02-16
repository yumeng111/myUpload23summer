#!/bin/sh
#!/bin/bash

#make filelist, condorTask2plus, out_pr_pu200_2plus, this file:plus, subfile:2.

echo "Starting job on $(date)"
echo "Processing input file: $1"
echo "Job ID: $2"

INPUT_FILE=$1
JOB_ID=$2

# Source CMS environment
#source /cvmfs/cms.cern.ch/cmsset_default.sh

# Go to your CMSSW project directory
#cd /path/to/your/CMSSW_X_Y_Z/src
cd /afs/cern.ch/user/y/yumeng/CMSSW_14_2_0_pre1/src/GEMCode/GEMValidation/test/condor
eval `scramv1 runtime -sh` # Set CMSSW environment

OUTPUT_DIR="/eos/user/y/yumeng/CMSSW_14_2_0_pre1/GEMCode/condor/out_pr_pu200_2plus"
OUTPUT_FILE="${OUTPUT_DIR}/${JOB_ID}.root"

CONFIG_FILE="runGEMCSCAnalyzer_Phase2_cfg_pr_${JOB_ID}.py"
cp condorrunGEMCSCAnalyzer_Phase2_cfg_pr_template.py "condorTask2plus/$CONFIG_FILE"

sed -i "s|PLACEHOLDER_INPUT_FILE|\"$INPUT_FILE\"|g" "condorTask2plus/$CONFIG_FILE"
sed -i "s|PLACEHOLDER_OUTPUT_FILE|\"$OUTPUT_FILE\"|g" "condorTask2plus/$CONFIG_FILE"

#export X509_USER_PROXY=$(voms-proxy-info --path)
export X509_USER_PROXY=$(voms-proxy-info --path 2>/dev/null)

# Ensure proxy exists before running CMSSW
if [ -z "$X509_USER_PROXY" ]; then
    echo "ERROR: No valid VOMS proxy found!"
    exit 1
fi

cmsRun "condorTask2plus/$CONFIG_FILE"

#rm "condorTask2plus/$CONFIG_FILE"

echo "Job finished on $(date)"


