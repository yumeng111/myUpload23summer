import FWCore.ParameterSet.Config as cms

muonDTStubPSet = cms.PSet(
    dtLocalTrigger = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("simDtTriggerPrimitiveDigis"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    )
)
