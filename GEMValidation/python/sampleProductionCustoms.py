import FWCore.ParameterSet.Config as cms

def dropNonMuonCollections(process):
    alist=['AODSIM','RECOSIM','FEVTSIM','FEVTDEBUG','FEVTDEBUGHLT','RECODEBUG','RAWRECOSIMHLT','RAWRECODEBUGHLT']

    collections = [
        ## raw
        '*_rawDataCollector_*_*',
        ## calorimeter
        '*_simHGCal*_*_*',
        '*_simHcal*_*_*',
        '*_simEcal*_*_*',
        '*_g4SimHits_Ecal*_*',
        '*_g4SimHits_Hcal*_*',
        '*_g4SimHits_HGC*_*',
        '*_simCalo*_*_*',
        'PCaloHits_*_*_*',
        ## tracker
        '*_g4SimHits_Tracker*_*',
        '*_simSiStrip*_*_*',
        '*_simSiDigis*_*_*',
        '*_simSiPixel*_*_*',
        '*_TT*_*_*',
        ## jets
        '*_*GenJets*_*_*',
        '*_*genMet*_*_*',
        ## other
        'PSimHits_*_Totem*_*',
        'PSimHits_*_FP420*_*',
        'PSimHits_*_BSC*_*',
        'PSimHits_*_BSM*_*',
        'PSimHits_g4SimHits_BHMHits_*',
        'PSimHits_g4SimHits_PLTHits_*',
        '*_mix_*_*',
        'PSimHits_g4SimHits_FastTimerHits*_*',
        'PSimHits_g4SimHits_CTPPS*_*',
        ## barrel BMTF
        '*_simBmtfDigis_*_*',
        '*_simOmtfDigis_*_*',
        '*_*_*BMTF*_*',
        '*_*_*OMTF*_*',
        ## uGT and HLT
        '*_hltGtStage2ObjectMap_*_*',
        '*_simGtStage2Digis_*_*',
        '*_hltTriggerSummary*_*_*'
    ]

    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            for coll in collections:
                getattr(process,b).outputCommands.append('drop ' + coll)

    return process

def keepMuonTrackVertexCollections(process):
    alist=['AODSIM','RECOSIM','FEVTSIM','FEVTDEBUG','FEVTDEBUGHLT','RECODEBUG','RAWRECOSIMHLT','RAWRECODEBUGHLT']
    for a in alist:
        b=a+'output'
        if hasattr(process,b):
            getattr(process,b).outputCommands.append('drop *_*_*_*')
            getattr(process,b).outputCommands.append('keep SimTracks_*_*_*')
            getattr(process,b).outputCommands.append('keep SimVertexs_*_*_*')
            getattr(process,b).outputCommands.append('keep *_*_MuonCSCHits_*')
            getattr(process,b).outputCommands.append('keep *_*_MuonRPCHits_*')
            getattr(process,b).outputCommands.append('keep *_*_MuonDTHits_*')
            getattr(process,b).outputCommands.append('keep *_*_MuonGEMHits_*')
            getattr(process,b).outputCommands.append('keep *_*_MuonME0Hits_*')
    return process
