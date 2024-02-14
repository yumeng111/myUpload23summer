import FWCore.ParameterSet.Config as cms

dtRecHitPSet = cms.PSet(
    dtRecHit = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("dt1DRecHits"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    dtRecSegment2D = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("dt2DSegments"),
        run = cms.bool(False),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    dtRecSegment4D = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("dt4DSegments"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
)
