# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: step2bis.py --filein file:step3.root --fileout file:step2bis.root --mc --eventcontent FEVTDEBUG --datatier GEN-SIM-DIGI-L1 --conditions auto:phase1_2021_realistic --step L1 --geometry DB:Extended --era Run3 --python_filename step2bis_L1.py --no_exec -n 10
import FWCore.ParameterSet.Config as cms

from Configuration.Eras.Era_Run2_2018_cff import Run2_2018

process = cms.Process('ReL1',Run2_2018)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('GEMCode.GEMValidation.MuonNtuplizer_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-100),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
process.source = cms.Source(
    "PoolSource",
    fileNames = cms.untracked.vstring(
        "/store/data/Run2018D/ZeroBias/RAW/v1/000/321/909/00000/B291651B-3CAB-E811-8596-02163E015DDC.root"
    ),
    secondaryFileNames = cms.untracked.vstring(),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)

process.options = cms.untracked.PSet(
    FailPath = cms.untracked.vstring(),
    IgnoreCompletely = cms.untracked.vstring(),
    Rethrow = cms.untracked.vstring(),
    SkipEvent = cms.untracked.vstring(),
    allowUnscheduled = cms.obsolete.untracked.bool,
    canDeleteEarly = cms.untracked.vstring(),
    emptyRunLumiMode = cms.obsolete.untracked.string,
    eventSetup = cms.untracked.PSet(
        forceNumberOfConcurrentIOVs = cms.untracked.PSet(

        ),
        numberOfConcurrentIOVs = cms.untracked.uint32(1)
    ),
    fileMode = cms.untracked.string('FULLMERGE'),
    forceEventSetupCacheClearOnNewRun = cms.untracked.bool(False),
    makeTriggerResults = cms.obsolete.untracked.bool,
    numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(1),
    numberOfConcurrentRuns = cms.untracked.uint32(1),
    numberOfStreams = cms.untracked.uint32(0),
    numberOfThreads = cms.untracked.uint32(1),
    printDependencies = cms.untracked.bool(False),
    sizeOfStackForThreadsInKB = cms.optional.untracked.uint32,
    throwIfIllegalParameter = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(False)
)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    annotation = cms.untracked.string('step2bis.py nevts:10'),
    name = cms.untracked.string('Applications'),
    version = cms.untracked.string('$Revision: 1.19 $')
)

# Output definition

process.FEVTDEBUGoutput = cms.OutputModule("PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-L1'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:step2bis_run3.root'),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out_ana_hst.root")
)

## keep all CSC trigger versions
process.FEVTDEBUGoutput.outputCommands.append('keep *_simCscTriggerPrimitiveDigis*_*_*')
process.FEVTDEBUGoutput.outputCommands.append('keep *_simEmtfDigis*_*_*')

## drop all calorimetry, tracker and raw
process.FEVTDEBUGoutput.outputCommands.append('drop *_simHcal*_*_*')
process.FEVTDEBUGoutput.outputCommands.append('drop *_simEcal*_*_*')
process.FEVTDEBUGoutput.outputCommands.append('drop *_g4SimHits_Tracker*_*')
process.FEVTDEBUGoutput.outputCommands.append('drop *_rawDataCollector_*_*')
process.FEVTDEBUGoutput.outputCommands.append('drop *_g4SimHits_Ecal*_*')
process.FEVTDEBUGoutput.outputCommands.append('drop *_g4SimHits_Hcal*_*')

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '103X_dataRun2_Prompt_v3', '')

from GEMCode.GEMValidation.cscTriggerCustoms import runOn110XMC
#process = runOn110XMC(process)

# the analyzer configuration
ana = process.MuonNtuplizer
ana.verbose = 1
ana.genParticle.pdgIds = cms.vint32(6000113, -6000113)
ana.genParticle.stableParticle = False
ana.genParticle.verbose = 1
ana.simTrack.minEta = 1.2
ana.simTrack.maxEta = 2.4
ana.simTrack.minPt = 5
#ana.simTrack.verbose = 1
ana.simTrack.pdgIds = cms.vint32(6000113, -6000113)

ana.cscSimHit.simMuOnly = False
ana.cscSimHit.discardEleHits = False
#ana.cscSimHit.verbose = 1

ana.gemStripDigi.matchDeltaStrip = 2
ana.cscLCT.addGhostLCTs = cms.bool(True)
#ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
#ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
#ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
#ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED","ReL1")
ana.cscShower.verbose = 1

useUnpacked = False
if useUnpacked:
    ana.gemStripDigi.matchToSimLink = False
    ana.gemStripDigi.inputTag = "muonGEMDigis"
    #ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")

process.simCscTriggerPrimitiveDigis.CSCComparatorDigiProducer = "muonCSCDigis:MuonCSCComparatorDigi"
process.simCscTriggerPrimitiveDigis.CSCWireDigiProducer = "muonCSCDigis:MuonCSCWireDigi"

## customize unpacker
process.SimL1Emulator = cms.Sequence(process.SimL1TMuonTask)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.ana_step = cms.Path(process.MuonNtuplizer)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(
    process.raw2digi_step,
    process.L1simulation_step,
    process.ana_step,
    process.endjob_step
    #,process.FEVTDEBUGoutput_step
)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
