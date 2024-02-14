import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018
process = cms.Process("ANA", Run2_2018)

## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_dataRun2_Prompt_v3', '')

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring("debug"),
    debug = cms.untracked.PSet(
 #       extension = cms.untracked.string(".txt"),
        threshold = cms.untracked.string("DEBUG"),
        # threshold = cms.untracked.string("WARNING"),
        lineLength = cms.untracked.int32(132),
        noLineBreaks = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring("cscTriggerPrimitiveDigis")
)

process.source = cms.Source(
  "PoolSource",
  fileNames = cms.untracked.vstring(
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/323/940/00000/090F7307-7EB4-CA45-A6B9-542C3AE60FD4.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/F9C3D433-4AC5-7A4E-BCA8-F8FBDAED6289.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/F3889D0A-EDA2-8E49-B6D7-C0D88AEECBD6.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/C61FDBE1-C082-9549-AD46-DE52DB70BC1C.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/AE7431C1-E425-A743-8A8E-C55D8C80B9F2.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/A929C4F5-CA5A-7248-B211-F72BCBE2B700.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/9F0B113A-866E-8844-8961-2ADDDE139072.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/9E2FFC02-08CD-884D-A9FA-7F75493343B2.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/9D873DE6-941E-384B-9697-169501CB247F.root",
         "/store/data/Run2018D/ZeroBias/RAW/v1/000/324/022/00000/9579D003-6191-8C47-A3DF-283CA7B5AAD4.root",
  ),
    skipEvents = cms.untracked.uint32(0),
)

#process.source.fileNames = cms.untracked.vstring("file:lcts_emulated.root")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)

# CSC raw --> digi unpacker
process.load("EventFilter.CSCRawToDigi.cscUnpacker_cfi")
process.muonCSCDigis.InputObjects = "rawDataCollector"

# CSC Trigger Primitives emulator
process.load("L1Trigger.CSCTriggerPrimitives.cscTriggerPrimitiveDigis_cfi")
process.cscTriggerPrimitiveDigis.CSCComparatorDigiProducer = "muonCSCDigis:MuonCSCComparatorDigi"
process.cscTriggerPrimitiveDigis.CSCWireDigiProducer = "muonCSCDigis:MuonCSCWireDigi"

# the analyzer configuration
process.myDataEmulatorFilter = cms.EDFilter(
    "DataEmulatorFilter",
    alctData = cms.InputTag("muonCSCDigis", "MuonCSCALCTDigi"),
    clctData = cms.InputTag("muonCSCDigis", "MuonCSCCLCTDigi"),
    lctData = cms.InputTag("muonCSCDigis", "MuonCSCCorrelatedLCTDigi"),
    alctEmul = cms.InputTag("cscTriggerPrimitiveDigis"),
    clctEmul = cms.InputTag("cscTriggerPrimitiveDigis"),
    lctEmul = cms.InputTag("cscTriggerPrimitiveDigis","MPCSORTED"),
)

# Output
process.output = cms.OutputModule(
    "PoolOutputModule",
    fileName = cms.untracked.string("lcts_emulated.root"),
    outputCommands = cms.untracked.vstring(
        "drop *",
        "keep *_cscTriggerPrimitive*_*_*",
        "keep *_*csc*_*_*",
        "keep *_*CSC*_*_*",
        "drop CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_CSCTPEmulator",
        "drop CSCDetIdCSCRPCDigiMuonDigiCollection_muonCSCDigis_MuonCSCRPCDigi_CSCTPEmulator",
        "drop CSCDetIdCSCStripDigiMuonDigiCollection_muonCSCDigis_MuonCSCStripDigi_*"
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
    )
)

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.p = cms.Path(
    process.muonCSCDigis *
    process.cscTriggerPrimitiveDigis
    * process.myDataEmulatorFilter
)

process.pp = cms.EndPath(process.output)

## messages
print
#print 'Input files:'
print '----------------------------------------'
print process.source.fileNames
print
print 'Output file:'
print '----------------------------------------'
print process.output.fileName
print
