import FWCore.ParameterSet.Config as cms
import sys, os

## initialization
process = cms.Process('GEMCSCTRGANA')

## CMSSW RELEASE
cmssw = os.getenv( "CMSSW_VERSION" )

## steering
events = -1
sample='dimu'
globalTag = 'upgrade2019'
#sample='minbias'

## readout windows
w=3
if w==3:
    readout_windows = [ [5,7],[5,7],[5,7],[5,7] ]
if w==11:
    readout_windows = [ [1,11],[1,11],[1,11],[1,11] ]
if w==7:
    readout_windows = [ [5,11],[5,11],[5,11],[5,11] ]
if w==61:
    readout_windows = [ [5,10],[1,11],[1,11],[1,11] ]
 

# import of standard configurations
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023Muon_cff')
process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')

#process.load('Configuration.Geometry.GeometryExtended2019Reco_cff')
#process.load('Configuration.Geometry.GeometryExtended2019_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.Digi_cff')
process.load("Configuration.StandardSequences.L1Emulator_cff")
process.load("Configuration.StandardSequences.L1Extra_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgrade2019', '')a
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

process.load('L1TriggerConfig.L1ScalesProducers.L1MuTriggerScalesConfig_cff')
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load("RecoMuon.TrackingTools.MuonServiceProxy_cff")

## GEM geometry customization
from Geometry.GEMGeometry.gemGeometryCustoms import custom_GE11_6partitions_v1
process = custom_GE11_6partitions_v1(process)

## SLHC customization
from SLHCUpgradeSimulations.Configuration.muonCustoms import *
process = unganged_me1a_geometry(process)
process = customise_csc_L1Extra_allsim(process)

## upgrade CSC TrackFinder
from SLHCUpgradeSimulations.Configuration.muonCustoms import customise_csc_L1TrackFinder
process = customise_csc_L1TrackFinder(process)
process.simCsctfTrackDigis.SectorProcessor.isCoreVerbose = cms.bool(True)

## process.load('CSCTriggerPrimitivesReader_cfi')
## process.lctreader.debug = False
## process.lctreader.dataLctsIn = False
## process.lctreader.CSCLCTProducerEmul = "simCscTriggerPrimitiveDigis"
## process.lctreader.CSCComparatorDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCComparatorDigi")
## process.lctreader.CSCWireDigiProducer = cms.InputTag("simMuonCSCDigis","MuonCSCWireDigi")

process.options = cms.untracked.PSet(
    makeTriggerResults = cms.untracked.bool(False),
    wantSummary = cms.untracked.bool(True)
)

process.source = cms.Source("PoolSource",
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
      'file:out_L1.root'
    )
)

from GEMCode.GEMValidation.InputFileHelpers import useInputDir
from GEMCode.SimMuL1.GEMCSCTriggerSamplesLib import eosfiles
suffix = 'PU140_TMB_Baseline_2'
process = useInputDir(process, eosfiles[suffix], True)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(events)
)

## output
outputFileName = 'hp_' + sample + "_" + cmssw + "_" + globalTag  + '_w%d'%(w) + suffix + '_eff.root'

process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outputFileName)
)

process.load('GEMCode.SimMuL1.GEMCSCTriggerEfficiency_cfi')
process.GEMCSCTriggerEfficiency.sectorProcessor = process.simCsctfTrackDigis.SectorProcessor
process.GEMCSCTriggerEfficiency.strips = process.simMuonCSCDigis.strips
process.GEMCSCTriggerEfficiency.minBxALCT = readout_windows[0][0]
process.GEMCSCTriggerEfficiency.maxBxALCT = readout_windows[0][1]
process.GEMCSCTriggerEfficiency.minBxCLCT = readout_windows[1][0]
process.GEMCSCTriggerEfficiency.maxBxCLCT = readout_windows[1][1]
process.GEMCSCTriggerEfficiency.minBxLCT = readout_windows[2][0]
process.GEMCSCTriggerEfficiency.maxBxLCT = readout_windows[2][1]
process.GEMCSCTriggerEfficiency.minBxMPLCT = readout_windows[3][0]
process.GEMCSCTriggerEfficiency.maxBxMPLCT = readout_windows[3][1]
process.GEMCSCTriggerEfficiency.minNHitsChamber = cms.untracked.int32(4)
process.GEMCSCTriggerEfficiency.minSimTrPt = cms.untracked.double(2)
GEMmatching = process.GEMCSCTriggerEfficiency.simTrackMatching
GEMmatching.gemRecHit.input = ""

## sequence, path and schedule
process.ana_seq = cms.Sequence(process.GEMCSCTriggerEfficiency)
#process.reader_seq = cms.Sequence(process.lctreader)

process.l1extra_step = cms.Path(process.L1Extra)
process.ana_step     = cms.Path(process.ana_seq)
#process.reader_step  = cms.Path(process.reader_seq)

process.schedule = cms.Schedule(
#    process.l1extra_step,
    process.ana_step
#    process.reader_step
)

## messages
print
print 'Input files:'
print '----------------------------------------'
print process.source.fileNames
print 
print 'Output file:'
print '----------------------------------------'
print process.TFileService.fileName
print 
