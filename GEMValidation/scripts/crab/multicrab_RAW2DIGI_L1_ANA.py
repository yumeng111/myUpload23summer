from CRABClient.UserUtilities import config
from samples import *
import argparse
from datetime import datetime
import sys

parser = argparse.ArgumentParser()
parser.add_argument("--sample", action="store", default = 0, help='0: central signal; 1: private signal; 2: data')
parser.add_argument("--test", action="store_true", default=False, help='Test for central MC')
parser.add_argument("--jobString", action="store", default="_ProtoL1_ANA", help='Test for central MC')
parser.add_argument("--dryRun", action="store_true", default=False)
args = parser.parse_args()

if int(args.sample) !=0 and args.test:
    sys.exit("Error: CRAB test only configured for private MC signal.")

currentDateTime = datetime.now().strftime("%Y%m%d_%H%M%S")
jobString = "{}_{}".format(args.jobString, currentDateTime)

chosenSample = []
## make a choice
if int(args.sample) == 0:
    chosenSample = central_signal
    if args.test:
        chosenSample = central_signal_test

if int(args.sample) == 1:
    chosenSample = private_signal

if int(args.sample) == 2:
    chosenSample = data

if args.test:
    jobString += "_TEST"

if len(chosenSample) == 0:
    sys.exit("Error: no sample was chosen.")

if __name__ == '__main__':
    from CRABAPI.RawCommand import crabCommand
    for sample in chosenSample:
        cconfig = config()
        cconfig.General.workArea = 'crab_projects' + jobString
        cconfig.General.transferLogs = True
        cconfig.General.requestName = sample[0] + jobString
        cconfig.JobType.pluginName = 'Analysis'
        cconfig.JobType.psetName = sample[3]
        cconfig.JobType.pyCfgParams = ["runWithCrab=True"]
        cconfig.Site.storageSite = 'T3_US_FNALLPC'
        cconfig.Data.splitting = 'FileBased'
        cconfig.Data.unitsPerJob = 5
        cconfig.Data.outLFNDirBase = '/store/user/dildick/'
        cconfig.Data.inputDataset = sample[1]
        cconfig.Data.inputDBS = sample[2]
        if args.dryRun:
            print cconfig
        else:
            crabCommand('submit', config = cconfig)
