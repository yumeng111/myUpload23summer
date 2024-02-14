import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Era_Phase2C10_cff import Phase2C10

process = cms.Process('ANA',Phase2C10)

## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
#process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D88Reco_cff')
process.load('Configuration.Geometry.GeometryExtended2026D88_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')
#process.load('GEMCode.GEMValidation.GEMCSCAnalyzer_cff')

"""
process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring("debug"),
    debug = cms.untracked.PSet(
 #       extension = cms.untracked.string(".txt"),
        threshold = cms.untracked.string("DEBUG"),
        # threshold = cms.untracked.string("WARNING"),
        lineLength = cms.untracked.int32(132),
        noLineBreaks = cms.untracked.bool(True)
    ),
    debugModules = cms.untracked.vstring("GEMCSCAnalyzer")
)
"""
inputFile = "file:/eos/cms/store/mc/Phase2Fall22DRMiniAOD/SingleMuon_Pt-0To200_Eta-1p4To3p1-gun/GEN-SIM-DIGI-RAW-MINIAOD/PU200_125X_mcRun4_realistic_v2-v1/30000/12bed16a-7ef3-4c16-a82c-8ca35078352a.root"
process.source = cms.Source(
  "PoolSource",
  #fileNames = cms.untracked.vstring('file:step2bis_phase2.root'),
  fileNames = cms.untracked.vstring(inputFile),
)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-100) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out_ana_phase2.root")
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T15', '')

# the analyzer configuration
from GEMCode.GEMValidation.simTrackMatching_cfi import simTrackPSet
process.GEMCSCAnalyzer = cms.EDAnalyzer(
    "GEMCSCAnalyzer",
    simTrackPSet,
    runSim = cms.bool(True),
    runDigi = cms.bool(True),
    runStub = cms.bool(True),
    runL1 = cms.bool(True),
    runReco = cms.bool(False),
    useGEMs = cms.bool(True),
    verbose = cms.untracked.int32(1),
    minNHitsChamberGEMSimHit = cms.int32(1),
    minNHitsChamberCSCSimHit = cms.int32(3),
    minNHitsChamberCSCDigi = cms.int32(4),
    minNHitsChamberGEMDigi = cms.int32(1),
    minNHitsChamberCSCStub = cms.int32(3),
)

ana = process.GEMCSCAnalyzer
#ana.genParticle.pdgIds = cms.vint32(6000113, -6000113)
ana.genParticle.stableParticle = False
ana.genParticle.verbose = 0
ana.genParticle.run = False
ana.simTrack.minEta = 0.9
ana.simTrack.maxEta = 2.4
ana.simTrack.minPt = 2
ana.simTrack.pdgIds = cms.vint32(13, -13)
ana.gemSimHit.verbose = 0
ana.gemStripDigi.verbose = 0
ana.gemStripDigi.matchDeltaStrip = 2
ana.gemPadDigi.verbose = 0
ana.gemCoPadDigi.verbose = 0
ana.gemPadCluster.verbose = 0
ana.cscComparatorDigi.verbose = 0
ana.cscWireDigi.verbose = 0
ana.cscStripDigi.matchDeltaStrip = 2 ## not 4
ana.cscWireDigi.matchDeltaWG = 2 ## not 4
ana.cscALCT.verbose = 0
ana.cscCLCT.verbose = 0
ana.cscLCT.verbose = 0
ana.cscLCT.addGhosts = cms.bool(True)
ana.cscLCT.matchTypeTight = cms.bool(False)
ana.gemStripDigi.run = cms.bool(True)
ana.gemPadDigi.run = cms.bool(True)
ana.gemPadCluster.run = cms.bool(True)
ana.gemCoPadDigi.run = cms.bool(True)
ana.emtfTrack.run = cms.bool(True)
ana.emtfTrack.verbose = 0
#print("emtfPset ", ana.emtfTrack)
ana = process.GEMCSCAnalyzer
ana.simTrack.minEta = 0.9
ana.simTrack.maxEta = 2.4
ana.simTrack.minPt = 2
ana.gemStripDigi.matchDeltaStrip = 2
ana.cscWireDigi.matchDeltaWG = 2 ## not 4
ana.cscALCT.verbose = 0
ana.cscCLCT.verbose = 0
ana.cscLCT.verbose = 0
ana.cscLCT.addGhosts = cms.bool(True)
ana.cscLCT.matchTypeTight = cms.bool(False)
ana.gemStripDigi.run = cms.bool(True)
ana.gemPadDigi.run = cms.bool(True)
ana.gemPadCluster.run = cms.bool(True)
ana.gemCoPadDigi.run = cms.bool(True)
ana.emtfTrack.run = cms.bool(True)
ana.emtfTrack.verbose = 0
#print("emtfPset ", ana.emtfTrack)

from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon
phase2_muon.toModify(ana, l1Track = dict(run = cms.bool(False) ) )
phase2_muon.toModify(ana, l1TkMuon = dict(run = cms.bool(False) ) )

#from GEMCode.GEMValidation.cscTriggerCustoms import addAnalysisRun3
#process = addAnalysisRun3(process)


process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.p = cms.Path(
    #process.SimL1Emulator *
    #process.Analysis
    ana
    #process.GEMCSCAnalyzer
)

## messages
print
#print 'Input files:'
print('----------------------------------------')
print(process.source.fileNames)
print('Output file:')
print('----------------------------------------')
print(process.TFileService.fileName)
