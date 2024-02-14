import FWCore.ParameterSet.Config as cms

showerPSet = cms.PSet(
    cscShower = cms.PSet(
        verbose = cms.int32(0),
        run = cms.bool(True),
        inputTag = cms.InputTag("simCscTriggerPrimitiveDigis"),
        minBX = cms.int32(0),
        maxBX = cms.int32(0),
    ),
    emtfShower = cms.PSet(
        verbose = cms.int32(0),
        run = cms.bool(True),
        inputTag = cms.InputTag("simEmtfShowers","EMTF"),
        minBX = cms.int32(0),
        maxBX = cms.int32(0),
    ),
    muonShower = cms.PSet(
        verbose = cms.int32(0),
        run = cms.bool(True),
        inputTag = cms.InputTag("simGmtShowerDigis"),
        minBX = cms.int32(0),
        maxBX = cms.int32(0),
    )
)
