import FWCore.ParameterSet.Config as cms
#from Configuration.Eras.Era_Phase2C10_cff import Phase2C10
from Configuration.Eras.Era_Phase2C17I13M9_cff import Phase2C17I13M9
#from Configuration.Eras.Era_Run3_cff import Run3

#process = cms.Process('ANA',Phase2C10)
process = cms.Process('ANA', Phase2C17I13M9)
#process = cms.Process("ReL1ANA", Run3)

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
process.load('L1Trigger.L1TMuon.simDigis_cff')
#process.load('Configuration.StandardSequences.SimL1Emulator_cff')
#process.load('GEMCode.GEMValidation.GEMCSCAnalyzer_cff')

"""
#yumeng
options = VarParsing('analysis')
options.register("selectedChambers", "", VarParsing.multiplicity.list, VarParsing.varType.string,
                 "Chambers you want to explicitly mask.")
options.register("skipEvents", 0, VarParsing.multiplicity.singleton, VarParsing.varType.int,
	          "number of events to skip when processing, Default = 0  ")
options.register("processEvent", 0, VarParsing.multiplicity.singleton, VarParsing.varType.int,
	          "event number to process, Default = 0  ")
options.register("runNumber", 1, VarParsing.multiplicity.singleton, VarParsing.varType.int,
	          "run number to process, Default = 1  ")
options.parseArguments()
print("Make sure the verbose debug choice is selected")
"""

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
#inputFile = "file:/eos/cms/store/mc/Phase2Fall22DRMiniAOD/SingleMuon_Pt-0To200_Eta-1p4To3p1-gun/GEN-SIM-DIGI-RAW-MINIAOD/PU200_125X_mcRun4_realistic_v2-v1/30000/12bed16a-7ef3-4c16-a82c-8ca35078352a.root"
#inputFile = "file:/eos/cms/store/group/upgrade/GEMCSC_Trigger_PhaseII/mu_pt100_phase2fall22/mu_pt100_phase2fall22/step2_DIGIL1/231001_174037/0000/step2_930.root"
#inputFile = "file:/afs/cern.ch/user/y/yumeng/CMSSW_13_0_14/src/L1Trigger/CSCTriggerPrimitives/test/step2.root"
inputFile = "file:/eos/user/y/yumeng/CMSSW_13_0_14/mu_pt100_phase2fall22_el9/step2_local500.root"
process.source = cms.Source(
  "PoolSource",
  #fileNames = cms.untracked.vstring('file:step2bis_phase2.root'),
  fileNames = cms.untracked.vstring(inputFile),
)

#yumeng
#print("event range to process ", process.source.eventsToProcess, " selected chambers ", options.selectedChambers)
#nEvents = options.maxEvents

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-100) )

process.TFileService = cms.Service("TFileService",
#    fileName = cms.string("out_ana_phase2.root")
fileName = cms.string("out_shower8_50.root")
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T15', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '125X_mcRun4_realistic_v2', '')

#yumeng start
#process.simCscTriggerPrimitiveDigis.selectedChambers = options.selectedChambers

processName = process._Process__name

## Run-3 patterns with CCLUT, with ILT
process.simCscTriggerPrimitiveDigisRun3CCLUTILT = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase1.verbosity = cms.int32(5)
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2.clctLocalShowerZone = 20
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2.clctLocalShowerThresh = 10
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM.clctLocalShowerZone = 20
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM.clctLocalShowerThresh = 10
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run-3 patterns with CCLUT,CLCT phase2 ", process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2)
process.simEmtfDigisRun3CCLUTILT = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTILT.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTILT','MPCSORTED',process._Process__name)
#yumeng end

process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow50=process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow50.clctPhase2.clctLocalShowerZone = 50
print("clctLocalShowerZone = 50 ", process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow50.clctPhase2)
process.simEmtfDigisRun3CCLUTILTshow50 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTILTshow50.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTILTshow50','MPCSORTED',process._Process__name)

process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow8=process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow8.clctPhase2.clctLocalShowerZone = 8
print("clctLocalShowerZone = 8 ", process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow8.clctPhase2)
process.simEmtfDigisRun3CCLUTILTshow8 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTILTshow8.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTILTshow8','MPCSORTED',process._Process__name)


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

#yumeng start:
useUnpacked = False

if useUnpacked:
    ana.gemStripDigi.inputTag = "muonGEMDigis"
    ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")
    ana.gemStripDigi.matchToSimLink = False

##yumeng start Run3CCLUTILT
process.GEMCSCAnalyzerRun3CCLUTILT = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTILT.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILT","",processName)

process.GEMCSCAnalyzerRun3CCLUTILTshow50 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTILTshow50.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow50","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow50.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow50","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow50.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow50","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow50.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow50","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow50.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILTshow50","",processName)

process.GEMCSCAnalyzerRun3CCLUTILTshow8 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTILTshow8.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow8","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow8.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow8","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow8.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow8","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow8.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTshow8","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILTshow8.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILTshow8","",processName)


process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

#yumeng start:
process.SimL1Emulator = cms.Sequence(
    process.simMuonGEMPadDigis *
    process.simMuonGEMPadDigiClusters *
    #process.simCscTriggerPrimitiveDigis *
    #process.simCscTriggerPrimitiveDigisILT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
    #process.simEmtfDigis *
    #process.simEmtfDigisILT *
    process.simEmtfDigisRun3CCLUTILT
    )

process.Analysis = cms.Sequence(
	process.GEMCSCAnalyzer *
	process.GEMCSCAnalyzerRun3CCLUTILT *
    process.GEMCSCAnalyzerRun3CCLUTILTshow50 *
    process.GEMCSCAnalyzerRun3CCLUTILTshow8
	)

process.debug = cms.Sequence(
    process.simMuonGEMPadDigis *
    process.simMuonGEMPadDigiClusters *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow50 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILTshow8 *
    process.simEmtfDigisRun3CCLUTILT *
    process.simEmtfDigisRun3CCLUTILTshow50 *
    process.simEmtfDigisRun3CCLUTILTshow8
        )

process.p = cms.Path(
    process.debug *
    #process.SimL1Emulator *
    process.Analysis
    #ana
    #process.GEMCSCAnalyzer
)
#yumeng:end

## messages
print
#print 'Input files:'
print('----------------------------------------')
print(process.source.fileNames)
print('Output file:')
print('----------------------------------------')
print(process.TFileService.fileName)
