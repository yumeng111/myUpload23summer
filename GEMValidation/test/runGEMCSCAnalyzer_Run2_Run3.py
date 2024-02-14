import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from Configuration.Eras.Era_Run3_cff import Run3

##this config is used to debug some special events with only certain CSC chambers 
##Here is how to use this config to debug chamber + thiseventnumber. run number is always 1 for MC and need to be changed for data 
##command = "cmsRun runGEMCSCAnalyzer_Run2_Run3.py maxEvents=1 selectedChambers={chamber} processEvent={processEvent}".format(chamber=chamber,processEvent = key)


process = cms.Process("ReL1ANA", Run3)

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
#process.load('GEMCode.GEMValidation.GEMCSCAnalyzer_cff')

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
inputFile = 'file:/eos/user/t/tahuang/RelValSamples/CMSSW_12_4_0_pre3/1000GeV/27a95851-6358-485b-b15b-619f3404d795.root'
inputFile = ['/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/87eaae84-afff-43d2-89c6-7f3014aefda8.root','/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/dd11e39c-9b3a-4a80-81a9-05e99e7a58c4.root']
process.source = cms.Source(
  "PoolSource",
  eventsToProcess = cms.untracked.VEventRange('%d:%d-%d:%d'%(options.runNumber, options.processEvent, options.runNumber, options.processEvent)),
  fileNames = cms.untracked.vstring(inputFile)
)
print("event range to process ", process.source.eventsToProcess, " selected chambers ", options.selectedChambers)
nEvents = options.maxEvents
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEvents) )

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out_gemcscana_test.root")
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '123X_mcRun3_2021_realistic_v14', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '125X_mcRun3_2022_realistic_v3', '')


process.simCscTriggerPrimitiveDigis.selectedChambers = options.selectedChambers
## Run-2 emulation
process.simCscTriggerPrimitiveDigisRun2 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun2.commonParam.runME11Up = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runME21Up = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runME31Up = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runME41Up = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runCCLUT = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runCCLUT_OTMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun2.commonParam.runME11ILT = False
process.simCscTriggerPrimitiveDigisRun2.commonParam.run3 = False
process.simCscTriggerPrimitiveDigisRun2.commonParam.runPhase2 = False
process.simCscTriggerPrimitiveDigisRun2.clctPhase1.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun2.alctPhase1.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun2.selectedChambers = options.selectedChambers
process.simCscTriggerPrimitiveDigisRun2.tmbPhase1.verbosity = cms.int32(10)
#print("Run2 emulation, common parameter ", process.simCscTriggerPrimitiveDigisRun2.commonParam)
#print("Run2 emulation, CLCT parameter ", process.simCscTriggerPrimitiveDigisRun2.clctPhase1)
process.simCscTriggerPrimitiveDigisRun2.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
process.simEmtfDigisRun2 = process.simEmtfDigis.clone()
process.simEmtfDigisRun2.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun2','MPCSORTED',process._Process__name)

## Run-3 patterns with CCLUT, without ILT
process.simCscTriggerPrimitiveDigisRun3CCLUT = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME11ILT = False
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
process.simCscTriggerPrimitiveDigisRun3CCLUT.selectedChambers = options.selectedChambers
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase2.verbosity = cms.int32(10)
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase1.verbosity = cms.int32(5)
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2.clctLocalShowerZone = 30
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2.clctLocalShowerThresh = 12
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM.clctLocalShowerZone = 30
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM.clctLocalShowerThresh = 12
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase2GE11.sortClctBx = False 
print("Run3 CCLUT, CLCT parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2)
print("Run3 CCLUT, CLCTGEM parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM)
process.simEmtfDigisRun3CCLUT = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUT.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUT','MPCSORTED',process._Process__name)


## Run-3 patterns with CCLUT, without ILT, no deadtime zone
process.simCscTriggerPrimitiveDigisRun3CCLUTv1 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME11ILT = False
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2.useDeadTimeZoning = False
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
#print("Run3 CCLUT nodeadtime, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam)
process.simEmtfDigisRun3CCLUTv1 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTv1.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTv1','MPCSORTED',process._Process__name)

## Run-3 patterns without CCLUT, without ILT,
process.simCscTriggerPrimitiveDigisRun3CCLUTv2 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT_OTMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME11ILT = False
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2.useDeadTimeZoning = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
#print("Run3 CCLUT nodeadtime, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam)
process.simEmtfDigisRun3CCLUTv2 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTv2.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTv2','MPCSORTED',process._Process__name)

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
ana.genParticle.verbose = 1
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
ana.cscALCT.verbose = 0
ana.cscCLCT.verbose = 0
ana.cscLCT.verbose = 0
ana.cscLCT.addGhostLCTs = cms.bool(True)
ana.gemStripDigi.run = cms.bool(True)
ana.gemPadDigi.run = cms.bool(True)
ana.gemPadCluster.run = cms.bool(True)
ana.gemCoPadDigi.run = cms.bool(True)
ana.emtfTrack.run = cms.bool(True)
ana.emtfTrack.verbose = 0
ana.emtfShower.verbose = 0
#print("emtfPset ", ana.emtfTrack)
#print("Shower parameter anode ", ana.anodeShower, " cathode ",ana.cathodeShower)

from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon
phase2_muon.toModify(ana, l1Track = dict(run = cms.bool(False) ) )
phase2_muon.toModify(ana, l1TkMuon = dict(run = cms.bool(False) ) )

#from GEMCode.GEMValidation.cscTriggerCustoms import addAnalysisRun3
#process = addAnalysisRun3(process)

useUnpacked = False
'''
## disable most
ana.cscALCT.run = False
ana.cscCLCT.run = False
ana.cscLCT.run = False
ana.cscMPLCT.run = False
ana.emtfTrack.run = False
ana.emtfCand.run = False
ana.muon.run = False
'''

if useUnpacked:
    ana.gemStripDigi.inputTag = "muonGEMDigis"
    ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")
    ana.gemStripDigi.matchToSimLink = False

processName = process._Process__name
##Run3CCLUT
process.GEMCSCAnalyzerRun3CCLUT = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUT.cscALCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
process.GEMCSCAnalyzerRun3CCLUT.cscCLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
process.GEMCSCAnalyzerRun3CCLUT.cscLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
process.GEMCSCAnalyzerRun3CCLUT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUT","",processName)
process.GEMCSCAnalyzerRun3CCLUT.cscLCT.verbose = 2
process.GEMCSCAnalyzerRun3CCLUT.emtfTrack.verbose = 1
#print("LCT config for analyzer ", process.GEMCSCAnalyzerRun3CCLUT.cscLCT)
#print("LCT config for analyzer ", process.GEMCSCAnalyzerRun3CCLUT.cscMPLCT)

##Run3CCLUTv1
process.GEMCSCAnalyzerRun3CCLUTv1 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTv1.cscALCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv1","",processName)
process.GEMCSCAnalyzerRun3CCLUTv1.cscCLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv1","",processName)
process.GEMCSCAnalyzerRun3CCLUTv1.cscLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv1","",processName)
process.GEMCSCAnalyzerRun3CCLUTv1.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv1","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTv1.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTv1","",processName)

##Run3CCLUTv2
process.GEMCSCAnalyzerRun3CCLUTv2 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTv2.cscALCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv2","",processName)
process.GEMCSCAnalyzerRun3CCLUTv2.cscCLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv2","",processName)
process.GEMCSCAnalyzerRun3CCLUTv2.cscLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv2","",processName)
process.GEMCSCAnalyzerRun3CCLUTv2.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv2","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTv2.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTv2","",processName)

##Run3CCLUTILT
process.GEMCSCAnalyzerRun3CCLUTILT = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTILT.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILT","",processName)

##Run2
process.GEMCSCAnalyzerRun2 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun2.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","MPCSORTED",processName)
process.GEMCSCAnalyzerRun2.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscLCT.verbose = 2
process.GEMCSCAnalyzerRun2.emtfTrack.verbose = 1

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )

## redefine the L1-step
process.SimL1Emulator = cms.Sequence(
    process.simMuonGEMPadDigis *
    process.simMuonGEMPadDigiClusters *
    #process.simCscTriggerPrimitiveDigis *
    #process.simCscTriggerPrimitiveDigisILT *
    process.simCscTriggerPrimitiveDigisRun2 *
    process.simCscTriggerPrimitiveDigisRun3CCLUT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTv1 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTv2 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
    #process.simEmtfDigis *
    #process.simEmtfDigisILT *
    process.simEmtfDigisRun2 *
    process.simEmtfDigisRun3CCLUT *
    process.simEmtfDigisRun3CCLUTv1 *
    process.simEmtfDigisRun3CCLUTv2 *
    process.simEmtfDigisRun3CCLUTILT
    )

process.Analysis = cms.Sequence(
	process.GEMCSCAnalyzer *
	process.GEMCSCAnalyzerRun2 *
	process.GEMCSCAnalyzerRun3CCLUTv1 *
	process.GEMCSCAnalyzerRun3CCLUTv2 *
	process.GEMCSCAnalyzerRun3CCLUTILT *
	process.GEMCSCAnalyzerRun3CCLUT
	)

process.debug = cms.Sequence(
    process.simMuonGEMPadDigis *
    process.simMuonGEMPadDigiClusters *
    process.simCscTriggerPrimitiveDigisRun3CCLUT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
    process.simCscTriggerPrimitiveDigisRun2 *
    process.simEmtfDigisRun2 *
    #process.simEmtfDigisRun3CCLUTv1 
    process.simEmtfDigisRun3CCLUT
        )
process.p = cms.Path(
    process.debug *
    process.GEMCSCAnalyzerRun2 *
    process.GEMCSCAnalyzerRun3CCLUT
    #process.SimL1Emulator *
    #process.Analysis
    #ana
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
