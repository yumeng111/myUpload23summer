import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon

# the analyzer configuration
from GEMCode.GEMValidation.simTrackMatching_cfi import simTrackPSet
MuonNtuplizer = cms.EDAnalyzer(
    "MuonNtuplizer",
    simTrackPSet,
    useGEMs = cms.bool(True),
    runSim = cms.bool(True),
    runDigi = cms.bool(True),
    runStub = cms.bool(True),
    runL1 = cms.bool(True),
    runReco = cms.bool(False),
    verbose = cms.int32(0),
    minNHitsChamberGEMSimHit = cms.int32(1),
    minNHitsChamberCSCSimHit = cms.int32(3),
    minNHitsChamberCSCDigi = cms.int32(4),
    minNHitsChamberGEMDigi = cms.int32(1),
    minNHitsChamberCSCStub = cms.int32(3),
)

MuonNtuplizer.simTrack.minEta = 0.9
MuonNtuplizer.simTrack.maxEta = 2.4
MuonNtuplizer.simTrack.minPt = 3
MuonNtuplizer.gemStripDigi.matchDeltaStrip = 2
MuonNtuplizer.cscLCT.addGhostLCTs = cms.bool(True)
MuonNtuplizer.gemStripDigi.run = cms.bool(True)
MuonNtuplizer.gemPadDigi.run = cms.bool(True)
MuonNtuplizer.gemPadCluster.run = cms.bool(True)
MuonNtuplizer.gemCoPadDigi.run = cms.bool(True)

phase2_muon.toModify(MuonNtuplizer, l1Track = dict(run = cms.bool(False) ) )
phase2_muon.toModify(MuonNtuplizer, l1TkMuon = dict(run = cms.bool(False) ) )
