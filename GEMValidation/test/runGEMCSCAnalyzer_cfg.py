import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from Configuration.Eras.Era_Run3_cff import Run3

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
options.register("samplepT", -1, VarParsing.multiplicity.singleton, VarParsing.varType.int,
	          "pt of bench mark samples, Default = -1  ")
options.parseArguments()



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
inputFile1000 = ['/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/87eaae84-afff-43d2-89c6-7f3014aefda8.root','/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt1000/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/dd11e39c-9b3a-4a80-81a9-05e99e7a58c4.root']
inputFile100 = ['/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/9e6dbd61-4249-4e63-a194-327c061d846c.root','/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt100/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/e30167a2-c51e-47f5-bed5-24638162d69a.root']
inputFile10 = ['/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/27262e68-3f2d-4827-9e2f-db55721ce1e9.root','/store/relval/CMSSW_12_6_0_pre2/RelValSingleMuPt10/GEN-SIM-DIGI-RAW/125X_mcRun3_2022_realistic_v3-v1/2580000/3fd49fee-c01c-4f38-9718-df9873de1b68.root']
samplepT = options.samplepT
inputFiledict = {
        1000: inputFile1000,
        100: inputFile100,
        10: inputFile10,
        -1: "test_fake.root"
        }
inputFile = inputFiledict[samplepT]
process.source = cms.Source(
  "PoolSource",
  #fileNames = cms.untracked.vstring('file:/uscms_data/d3/dildick/work/HadronicShowerTrigger/CMSSW_11_3_0_pre3/src/TSG-Run3Winter20GS-00040.root'),
  fileNames = cms.untracked.vstring(inputFile)
)
#nEvents = -1
nEvents = options.maxEvents
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(nEvents) )

outFileName = "/eos/user/t/tahuang/CSCEmulation/out_gemcscana_RelValpT%dGeV_all_condor_CLCTsortflaglooseLCT20220926.root"%samplepT
if samplepT == -1: outFileName = "out_gemcsc.root"
#process.options.numberOfThreads=cms.untracked.uint32(4)
process.TFileService = cms.Service("TFileService",
    fileName = cms.string(outFileName)
)

## global tag for upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
#process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')
#process.GlobalTag = GlobalTag(process.GlobalTag, '123X_mcRun3_2021_realistic_v14', '')
process.GlobalTag = GlobalTag(process.GlobalTag, '125X_mcRun3_2022_realistic_v3', '')


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
print("Run2 emulation, common parameter ", process.simCscTriggerPrimitiveDigisRun2.commonParam)
print("CLCT parameter ", process.simCscTriggerPrimitiveDigisRun2.clctPhase1)
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
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2.clctLocalShowerZone = 15
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2.clctLocalShowerThresh = 0
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM.clctLocalShowerZone = 15
process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2GEM.clctLocalShowerThresh = 0
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUT.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
print("Run3 CCLUTILT, zone=15threhs=0,, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam)
print("CLCT parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUT.clctPhase2)
process.simEmtfDigisRun3CCLUT = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUT.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUT','MPCSORTED',process._Process__name)

## Run-3 patterns with CCLUT, with ILT, no deadtime zone
process.simCscTriggerPrimitiveDigisRun3CCLUTv0 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2.clctLocalShowerZone = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2.clctLocalShowerThresh = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2GEM.clctLocalShowerZone = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2GEM.clctLocalShowerThresh = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run3 CCLUTILT, zone=15thresh=15, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv0.commonParam)
print("CLCT parameter phase2 ", process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2)
print("CLCT parameter phase2GEM ", process.simCscTriggerPrimitiveDigisRun3CCLUTv0.clctPhase2GEM)
process.simEmtfDigisRun3CCLUTv0 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTv0.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTv0','MPCSORTED',process._Process__name)

## Run-3 patterns with CCLUT, No ILT, deadtime zone, deadtime zone 10+10 HS
process.simCscTriggerPrimitiveDigisRun3CCLUTv1 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2.clctLocalShowerZone = 25
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2.clctLocalShowerThresh = 12
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2GEM.clctLocalShowerZone = 25
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2GEM.clctLocalShowerThresh = 12
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run3 CCLUTILT, zone=15threhs=12, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv1.commonParam)
print("CLCT parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv1.clctPhase2)
process.simEmtfDigisRun3CCLUTv1 = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTv1.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTv1','MPCSORTED',process._Process__name)

## Run-3 patterns without CCLUT, without ILT, with deadtime zone
process.simCscTriggerPrimitiveDigisRun3CCLUTv2 = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2.clctLocalShowerZone = 40
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2.clctLocalShowerThresh = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2GEM.clctLocalShowerZone = 40
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2GEM.clctLocalShowerThresh = 15
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run3 CCLUTILT, zone=10threhs=12, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv2.commonParam)
print("CLCT parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTv2.clctPhase2)
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
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2.clctLocalShowerZone = 25
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2.clctLocalShowerThresh = 0
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM.clctLocalShowerZone = 25
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM.clctLocalShowerThresh = 0
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2.sortClctBx     = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2GE11.sortClctBx = False 
process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run-3 patterns with CCLUT, with ILT emulation, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam)
print("Run-3 patterns with CCLUT, with ILT emulation, tmb parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTILT.tmbPhase2GE11)
print("CLCT parameter phase2 ", process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2)
print("CLCT parameter phase2GEM ", process.simCscTriggerPrimitiveDigisRun3CCLUTILT.clctPhase2GEM)
process.simEmtfDigisRun3CCLUTILT = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTILT.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTILT','MPCSORTED',process._Process__name)

## Run-3 patterns with CCLUT, with ILT with sortClctBx on, no qualbend sorting
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold = process.simCscTriggerPrimitiveDigis.clone()
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runME11Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runME21Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runME31Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runME41Up = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runCCLUT = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runCCLUT_OTMB = cms.bool(True)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runCCLUT_TMB = cms.bool(False)
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runME11ILT = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.run3 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.runPhase2 = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.tmbPhase2.sortClctBx = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.tmbPhase2GE11.sortClctBx = True
process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
print("Run-3 patterns old with CCLUT, with ILT emulation, common parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.commonParam)
print("Run-3 patterns old with CCLUT, with ILT emulation, tmb parameter ", process.simCscTriggerPrimitiveDigisRun3CCLUTILTold.tmbPhase2GE11)
process.simEmtfDigisRun3CCLUTILTold = process.simEmtfDigis.clone()
process.simEmtfDigisRun3CCLUTILTold.CSCInput = cms.InputTag(
    'simCscTriggerPrimitiveDigisRun3CCLUTILTold','MPCSORTED',process._Process__name)


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
print("process.GEMCSCAnalyzerRun3CCLUT.cscWireDigi ", process.GEMCSCAnalyzerRun3CCLUT.cscWireDigi)

##Run3CCLUTv0
process.GEMCSCAnalyzerRun3CCLUTv0 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTv0.cscALCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv0","",processName)
process.GEMCSCAnalyzerRun3CCLUTv0.cscCLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv0","",processName)
process.GEMCSCAnalyzerRun3CCLUTv0.cscLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv0","",processName)
process.GEMCSCAnalyzerRun3CCLUTv0.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTv0","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTv0.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTv0","",processName)
print("process.GEMCSCAnalyzerRun3CCLUTv0.cscCLCT ",process.GEMCSCAnalyzerRun3CCLUTv0.cscCLCT)

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
process.GEMCSCAnalyzerRun3CCLUTILT.cscALCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscCLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILT","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILT","",processName)
print("process.GEMCSCAnalyzerRun3CCLUTILT.cscWireDigi ", process.GEMCSCAnalyzerRun3CCLUTILT.cscWireDigi)

##Run3CCLUTILTOld, sortClctBx on = no qualbend sorting
process.GEMCSCAnalyzerRun3CCLUTILTold = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun3CCLUTILTold.cscALCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTold","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTold.cscCLCT.inputTag   = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTold","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTold.cscLCT.inputTag    = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTold","",processName)
process.GEMCSCAnalyzerRun3CCLUTILTold.cscMPLCT.inputTag  = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUTILTold","MPCSORTED",processName)
process.GEMCSCAnalyzerRun3CCLUTILTold.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUTILTold","",processName)
#print("process.GEMCSCAnalyzerRun3CCLUTILT.cscWireDigi ", process.GEMCSCAnalyzerRun3CCLUTILTold.cscWireDigi)

##Run2
process.GEMCSCAnalyzerRun2 = process.GEMCSCAnalyzer.clone()
process.GEMCSCAnalyzerRun2.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","",processName)
process.GEMCSCAnalyzerRun2.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun2","MPCSORTED",processName)
process.GEMCSCAnalyzerRun2.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun2","",processName)
print("process.GEMCSCAnalyzerRun2.cscWireDigi ", process.GEMCSCAnalyzerRun2.cscWireDigi)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

## redefine the L1-step
process.SimL1Emulator = cms.Sequence(
    process.simMuonGEMPadDigis *
    process.simMuonGEMPadDigiClusters *
    #process.simCscTriggerPrimitiveDigis *
    #process.simCscTriggerPrimitiveDigisILT *
    process.simCscTriggerPrimitiveDigisRun2 *
    #process.simCscTriggerPrimitiveDigisRun3CCLUT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTv0 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTv1 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTv2 *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
    process.simCscTriggerPrimitiveDigisRun3CCLUTILTold *
    #process.simEmtfDigis *
    #process.simEmtfDigisILT *
    process.simEmtfDigisRun2 *
    #process.simEmtfDigisRun3CCLUT *
    process.simEmtfDigisRun3CCLUTv0 *
    process.simEmtfDigisRun3CCLUTv1 *
    process.simEmtfDigisRun3CCLUTv2 *
    process.simEmtfDigisRun3CCLUTILT *
    process.simEmtfDigisRun3CCLUTILTold
    )

process.Analysis = cms.Sequence(
	process.GEMCSCAnalyzer *
	process.GEMCSCAnalyzerRun2 *
	process.GEMCSCAnalyzerRun3CCLUTv0 *
	process.GEMCSCAnalyzerRun3CCLUTv1 *
	process.GEMCSCAnalyzerRun3CCLUTv2 *
	process.GEMCSCAnalyzerRun3CCLUTILT *
	process.GEMCSCAnalyzerRun3CCLUTILTold 
	#process.GEMCSCAnalyzerRun3CCLUT
	)

process.p = cms.Path(
    process.SimL1Emulator *
    process.Analysis
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
