import FWCore.ParameterSet.Config as cms

genParticlePSet = cms.PSet(
    genParticle = cms.PSet(
        pdgIds = cms.vint32(13,-13),
        #for LLP efficiency studies
        #pdgIds = cms.vint32(9000006, -9000006),
        stableParticle = cms.bool(False),
        etaMin = cms.double(-2.4),
        etaMax = cms.double(2.4),
        inputTag = cms.InputTag("genParticles"),
        verbose = cms.int32(0),
        run = cms.bool(True),
    )
)
