# Auto generated configuration file
# using:
# Revision: 1.19
# Source: /local/reps/CMSSW/CMSSW/Configuration/Applications/python/ConfigBuilder.py,v
# with command line options: step2bis.py --filein file:step3.root --fileout file:step2bis.root --mc --eventcontent FEVTDEBUG --datatier GEN-SIM-DIGI-L1 --conditions auto:phase1_2021_realistic --step L1 --geometry DB:Extended --era Run3 --python_filename step2bis_L1.py --no_exec -n 10
import FWCore.ParameterSet.Config as cms
from FWCore.ParameterSet.VarParsing import VarParsing

options = VarParsing('analysis')
options.register("usePileUp", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.register("runTest", False, VarParsing.multiplicity.singleton, VarParsing.varType.bool)
options.parseArguments()

from Configuration.Eras.Era_Phase2C10_cff import Phase2C10

process = cms.Process('ReL1',Phase2C10)

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.Geometry.GeometryExtended2026D49Reco_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.SimL1Emulator_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

nEvents = options.maxEvents
if options.runTest:
    nEvents = 100

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(nEvents),
    output = cms.optional.untracked.allowed(cms.int32,cms.PSet)
)

# Input source
from GEMCode.GEMValidation.relValSamples import *
from GEMCode.GEMValidation.relValSamplesPU200 import *

inputFiles = RelValSingleMuPt10_110X_mcRun4_realistic_v2_2026D49noPU
inputFiles += RelValSingleMuFlatPt2To100_110X_mcRun4_realistic_v2_2026D49noPU
#inputFiles = RelValSingleMuFlatPt1p5To8_112X_mcRun4_realistic_v2_2026D49noPU
#inputFiles += RelValSingleMuFlatPt2To100_112X_mcRun4_realistic_v2_2026D49noPU

if options.usePileUp:
    inputFiles = RelValSingleMuPt10_110X_mcRun4_realistic_v3_2026D49PU200

process.source = cms.Source(
    "PoolSource",
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
outputFile = "step2bis_phase2.root"
if options.usePileUp:
    outputFile = "step2bis_phase2_pu200.root"

process.FEVTDEBUGoutput = cms.OutputModule(
    "PoolOutputModule",
    dataset = cms.untracked.PSet(
        dataTier = cms.untracked.string('GEN-SIM-DIGI-L1'),
        filterName = cms.untracked.string('')
    ),
    fileName = cms.untracked.string('file:' + outputFile),
    outputCommands = process.FEVTDEBUGEventContent.outputCommands,
    splitLevel = cms.untracked.int32(0)
)

## keep all CSC trigger versions
process.FEVTDEBUGoutput.outputCommands.append('keep *_simCscTriggerPrimitiveDigis*_*_*')
process.FEVTDEBUGoutput.outputCommands.append('keep *_simEmtfDigis*_*_*')

# Additional output definition

# Other statements
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:phase2_realistic_T15', '')
#process.GlobalTag.globaltag = '110X_mcRun4_realistic_v2'

from GEMCode.GEMValidation.cscTriggerCustoms import addCSCTriggerRun3
process = addCSCTriggerRun3(process)

process.SimL1Emulator = cms.Sequence(
    process.simCscTriggerPrimitiveDigis
    * process.simCscTriggerPrimitiveDigisRun3CCLUT
    * process.simEmtfDigis
    * process.simEmtfDigisRun3CCLUT
)

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1simulation_step = cms.Path(process.SimL1Emulator)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.FEVTDEBUGoutput_step = cms.EndPath(process.FEVTDEBUGoutput)

# Schedule definition
process.schedule = cms.Schedule(
    process.L1simulation_step,process.endjob_step,process.FEVTDEBUGoutput_step)
from PhysicsTools.PatAlgos.tools.helpers import associatePatAlgosToolsTask
associatePatAlgosToolsTask(process)


# Customisation from command line

# Add early deletion of temporary data products to reduce peak memory need
from Configuration.StandardSequences.earlyDeleteSettings_cff import customiseEarlyDelete
process = customiseEarlyDelete(process)
# End adding early deletion

from GEMCode.GEMValidation.sampleProductionCustoms import dropNonMuonCollections
process = dropNonMuonCollections(process)
