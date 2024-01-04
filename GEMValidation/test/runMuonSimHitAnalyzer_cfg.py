import FWCore.ParameterSet.Config as cms

process = cms.Process("MUONSIMANA")

## Standard sequence
process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.Geometry.GeometryExtended2023MuonReco_cff')
process.load('Configuration.Geometry.GeometryExtended2023Muon_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_PostLS1_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

## TrackingComponentsRecord required for matchers
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi')
process.load('TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi')

## global tag for 2019 upgrade studies
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'auto:upgradePLS3', '')

# the analyzer configuration
from GEMCode.GEMValidation.simTrackMatching_cfi import SimTrackMatching
process.MuonSimHitAnalyzer = cms.EDAnalyzer("MuonSimHitAnalyzer",
    simTrackMatching = SimTrackMatching
)

## only simhits
from GEMCode.GEMValidation.simTrackMatching_cfi import onlySimHitCollections
process.MuonSimHitAnalyzer = onlySimHitCollections(process.MuonSimHitAnalyzer)

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

process.source = cms.Source("PoolSource",
  fileNames = cms.untracked.vstring("file:out_sim.root")                            
)

## input
from GEMCode.SimMuL1.GEMCSCTriggerSamplesLib import *
from GEMCode.GEMValidation.InputFileHelpers import *
process = useInputDir(process, eosfiles['_pt2-50_SLHC11_2023Muon_PU0'], True)

process.TFileService = cms.Service("TFileService",
  fileName = cms.string("gem_sh_ana.root")
)

process.p = cms.Path(process.MuonSimHitAnalyzer)

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
