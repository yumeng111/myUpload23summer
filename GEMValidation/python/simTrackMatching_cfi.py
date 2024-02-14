import FWCore.ParameterSet.Config as cms

## in CMSSW
from Validation.MuonHits.muonSimHitMatcherPSet import muonSimHitMatcherPSet
from Validation.MuonCSCDigis.muonCSCDigiPSet import muonCSCDigiPSet
from Validation.MuonCSCDigis.muonCSCStubPSet import muonCSCStubPSet
from Validation.MuonGEMDigis.muonGEMDigiPSet import muonGEMDigiPSet
from Validation.MuonGEMRecHits.muonGEMRecHitPSet import gemRecHit
#from L1Trigger.CSCTriggerPrimitives.params.showerParams import showerPSet as cmsswshowerPSet 

CMSSWPSets = cms.PSet(
    muonSimHitMatcherPSet,
    muonCSCDigiPSet,
    muonCSCStubPSet,
    muonGEMDigiPSet,
    gemRecHit = gemRecHit
)

## in GEMCode
from GEMCode.GEMValidation.simTrackPSets.genParticlePSet import genParticlePSet
from GEMCode.GEMValidation.simTrackPSets.me0HitPSet import me0HitPSet
from GEMCode.GEMValidation.simTrackPSets.muonRPCDigiPSet import muonRPCDigiPSet
from GEMCode.GEMValidation.simTrackPSets.muonDTDigisPSet import muonDTDigisPSet
from GEMCode.GEMValidation.simTrackPSets.muonDTStubPSet import muonDTStubPSet
from GEMCode.GEMValidation.simTrackPSets.l1MuonPSet import l1MuonPSet
from GEMCode.GEMValidation.simTrackPSets.l1TrackPSet import l1TrackPSet
from GEMCode.GEMValidation.simTrackPSets.rpcRecHitPSet import rpcRecHitPSet
from GEMCode.GEMValidation.simTrackPSets.cscRecHitPSet import cscRecHitPSet
from GEMCode.GEMValidation.simTrackPSets.dtRecHitPSet import dtRecHitPSet
from GEMCode.GEMValidation.simTrackPSets.recoTrackPSet import recoTrackPSet
from GEMCode.GEMValidation.simTrackPSets.cclutPSet import cclutPSet
from GEMCode.GEMValidation.simTrackPSets.showerPSet import showerPSet
from GEMCode.GEMValidation.simTrackPSets.showerParams import showerPSet  as cmsswshowerPSet

GEMCodePSet = cms.PSet(
    genParticlePSet,
    me0HitPSet,
    muonRPCDigiPSet,
    muonDTDigisPSet,
    muonDTStubPSet,
    l1MuonPSet,
    l1TrackPSet,
    cscRecHitPSet,
    rpcRecHitPSet,
    dtRecHitPSet,
    recoTrackPSet,
    cclutPSet,
    cmsswshowerPSet,
    showerPSet
)

## combine into single pset
simTrackPSet = cms.PSet(
    CMSSWPSets,
    GEMCodePSet,
)
