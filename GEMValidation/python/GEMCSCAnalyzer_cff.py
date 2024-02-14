import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon

# the analyzer configuration
from GEMCode.GEMValidation.simTrackMatching_cfi import simTrackPSet
GEMCSCAnalyzer = cms.EDAnalyzer(
    "GEMCSCAnalyzer",
    simTrackPSet,
    runSim = cms.bool(True),
    runDigi = cms.bool(True),
    runStub = cms.bool(True),
    runL1 = cms.bool(True),
    runReco = cms.bool(False),
    verbose = cms.untracked.int32(0),
    useGEMs = cms.bool(True),
    minNHitsChamberGEMSimHit = cms.int32(1),
    minNHitsChamberCSCSimHit = cms.int32(3),
    minNHitsChamberCSCDigi = cms.int32(4),
    minNHitsChamberGEMDigi = cms.int32(1),
    minNHitsChamberCSCStub = cms.int32(3),
)

GEMCSCAnalyzer.simTrack.minEta = 0.9
GEMCSCAnalyzer.simTrack.maxEta = 2.4
GEMCSCAnalyzer.simTrack.minPt = 3
GEMCSCAnalyzer.simTrack.pdgIds = cms.vint32(13,-13),
GEMCSCAnalyzer.gemSimHit.verbose = 0
GEMCSCAnalyzer.gemStripDigi.verbose = 0
GEMCSCAnalyzer.gemStripDigi.matchDeltaStrip = 2
GEMCSCAnalyzer.gemPadDigi.verbose = 0
GEMCSCAnalyzer.gemCoPadDigi.verbose = 0
GEMCSCAnalyzer.gemPadCluster.verbose = 0
GEMCSCAnalyzer.cscComparatorDigi.verbose = 0
GEMCSCAnalyzer.cscWireDigi.verbose = 0
GEMCSCAnalyzer.cscALCT.verbose = 0
GEMCSCAnalyzer.cscCLCT.verbose = 0
GEMCSCAnalyzer.cscLCT.verbose = 0
GEMCSCAnalyzer.cscLCT.addGhostLCTs = cms.bool(True)

phase2_muon.toModify(GEMCSCAnalyzer, l1Track = dict(run = cms.bool(False) ) )
phase2_muon.toModify(GEMCSCAnalyzer, l1TkMuon = dict(run = cms.bool(False) ) )
