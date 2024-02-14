import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Run3_cff import Run3

process = cms.Process('ANA',Run3)

## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('GEMCode.GEMValidation.MuonNtuplizer_cff')

process.source = cms.Source(
  "PoolSource",
  fileNames = cms.untracked.vstring('file:step2.root'),
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-100) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out_ana.root")
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')

from GEMCode.GEMValidation.cscTriggerCustoms import addCSCTriggerRun3
process = addCSCTriggerRun3(process)

# the analyzer configuration
ana = process.MuonNtuplizer
ana.genParticle.run = False
ana.simTrack.minEta = 0.9
ana.simTrack.maxEta = 2.4
ana.simTrack.minPt = 2
ana.verbose = 1
ana.gemStripDigi.matchDeltaStrip = 2
ana.cscLCT.addGhostLCTs = cms.bool(True)
#ana.genParticle.pdgIds = cms.vint32(13, -13)
ana.simTrack.pdgIds = cms.vint32(13, -13)
ana.runSim = False

useUnpacked = False
if useUnpacked:
    ana.gemStripDigi.inputTag = "muonGEMDigis"
    ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")

process.MuonNtuplizerRun3CCLUT = process.MuonNtuplizer.clone()
process.MuonNtuplizerRun3CCLUT.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",process._Process__name)
process.MuonNtuplizerRun3CCLUT.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",process._Process__name)
process.MuonNtuplizerRun3CCLUT.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",process._Process__name)
process.MuonNtuplizerRun3CCLUT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","MPCSORTED",process._Process__name)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.p = cms.Path(
    process.SimL1Emulator *
    process.MuonNtuplizer *
    process.MuonNtuplizerRun3CCLUT
)
