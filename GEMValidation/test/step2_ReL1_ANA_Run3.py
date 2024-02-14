# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: step2bis.py --filein file:step3.root --fileout file:step2bis.root --mc --eventcontent FEVTDEBUG --datatier GEN-SIM-DIGI-L1 --conditions auto:phase1_2021_realistic --step L1 --geometry DB:Extended --era Run3 --python_filename step2bis_L1.py --no_exec -n 10
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing
from Configuration.Eras.Era_Run3_cff import Run3

options = VarParsing('analysis')
options.register ("test", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.register ("runOnRaw", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.register ("runAna", True, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.register ("crab", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.parseArguments()

process = cms.Process('TEST',Run3)

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
process.load('GEMCode.GEMValidation.GEMCSCAnalyzer_cff')

nEvents = -1
if options.test:
    nEvents = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(nEvents),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
from GEMCode.GEMValidation.relValSamples import *
inputFiles = RelValSingleMuPt10_120X_mcRun3_realistic_v10_noPU

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(*inputFiles),
    secondaryFileNames = cms.untracked.vstring()
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
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase1_2021_realistic', '')

from GEMCode.GEMValidation.cscTriggerCustoms import addCSCTriggerRun3
process = addCSCTriggerRun3(process)

from GEMCode.GEMValidation.cscTriggerCustoms import addAnalysisRun3
process = addAnalysisRun3(process)

process.GEMCSCAnalyzer.gemStripDigi.verbose = 1
process.GEMCSCAnalyzer.gemStripDigi.matchToSimLink = True
process.GEMCSCAnalyzer.gemPadDigi.verbose = 1
process.GEMCSCAnalyzer.gemPadCluster.verbose = 1
process.GEMCSCAnalyzer.gemCoPadDigi.verbose = 1

from GEMCode.GEMValidation.cscTriggerCustoms import runOn110XMC
if options.runOnRaw:
    process = runOn110XMC(process)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.muonGEMDigis)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.ana_step = cms.Path(
    process.GEMCSCAnalyzer
    * process.GEMCSCAnalyzerRun3CCLUT
)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

process.TFileService = cms.Service("TFileService",
    fileName = cms.string("out_ana_run3.root")
)

# Schedule definition
process.schedule = cms.Schedule()
if options.runOnRaw:
    process.schedule.extend([process.raw2digi_step])
process.schedule.extend([process.L1simulation_step])
if options.runAna:
    process.schedule.extend([process.ana_step])
process.schedule.extend([process.endjob_step])
#process.schedule.extend([process.FEVTDEBUGoutput_step])

from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion
