import FWCore.ParameterSet.Config as cms

l1TrackPSet = cms.PSet(
    l1Track = cms.PSet(
        verbose = cms.int32(1),
        run = cms.bool(False),
        minBX = cms.int32(0),
        maxBX = cms.int32(0),
        inputTag = cms.InputTag("TTTracksFromTrackletEmulation","Level1TTTracks"),
        deltaR = cms.double(0.5),
    ),
    l1TkMuon = cms.PSet(
        verbose = cms.int32(1),
        run = cms.bool(False),
        inputTag = cms.InputTag("L1TkMuons"),
        minBX = cms.int32(0),
        maxBX = cms.int32(0),
        deltaR = cms.double(0.5),
    )
)
