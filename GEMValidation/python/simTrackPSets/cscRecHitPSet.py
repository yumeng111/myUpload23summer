import FWCore.ParameterSet.Config as cms

cscRecHitPSet = cms.PSet(
    #csc CLCT, central BX 7
    cscPreCLCT = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
        minBX = cms.int32(6),
        maxBX = cms.int32(8),
        minNHitsChamber = cms.int32(3),
    ),
    cscRecHit = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("csc2DRecHits"),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    cscSegment = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("cscSegments"),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    )
)
