import FWCore.ParameterSet.Config as cms

recoTrackPSet = cms.PSet(
    recoTrackExtra = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("hltL2Muons"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    recoTrack = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("hltL2Muons"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    recoChargedCandidate = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("hltL2MuonCandidatesNoVtx"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    ),
    recoMuon = cms.PSet(
        verbose = cms.int32(0),
        inputTag = cms.InputTag("hltGlbTrkMuonsNoVtx"),
        run = cms.bool(True),
        minBX = cms.int32(-1),
        maxBX = cms.int32(1),
    )
)
