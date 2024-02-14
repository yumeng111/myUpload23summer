import FWCore.ParameterSet.Config as cms
import os

## default location of high-PU filelist
path = os.getenv( "CMSSW_BASE" ) + "/src/GEMCode/SimMuL1/test/"
def_filelist = path + "filelist_minbias_61M_good.txt"

## helper function for pile-up
def addPileUp(process, pu = 140, filelist = def_filelist):
    ff = open(filelist, "r")
    pu_files = ff.read().split('\n')
    ff.close()
    pu_files = filter(lambda x: x.endswith('.root'),  pu_files)

    process.mix.input = cms.SecSource("PoolSource",
        nbPileupEvents = cms.PSet(
            averageNumber = cms.double(pu)
        ),
        type = cms.string('poisson'),
        sequential = cms.untracked.bool(False),
        fileNames = cms.untracked.vstring(*pu_files)
    )
    return process

## helper for files on dCache/EOS (LPC)
def useInputDir(process, inputDir):
    theInputFiles = []
    ## list the files in this directory
    output = os.popen('xrdfs root://cmseos.fnal.gov ls ' + inputDir).readlines()
    for x in output:
        stripx = x.strip()
        if stripx.endswith('root'):
            theInputFiles.extend(['file:root://cmsxrootd.fnal.gov/' + stripx])
    process.source.fileNames = cms.untracked.vstring(*theInputFiles)
    return process
