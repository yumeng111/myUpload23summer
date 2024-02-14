import FWCore.ParameterSet.Config as cms
from Configuration.Eras.Modifier_phase2_muon_cff import phase2_muon
import os

def runOn110XMC(process, runWithCrab = False):
    # customize unpacker
    path = os.path.expandvars("$CMSSW_BASE/src/GEMCode/GEMValidation/data/GEMeMapDummy.db")
    if runWithCrab:
        path = "../data/GEMeMapDummy.db"
    print("Info: Using path \"{}\".".format(path))
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("GEMeMapRcd"),
                 tag = cms.string("GEMeMapDummy"),
                 connect = cms.string("sqlite_file:{}".format(path))
             )
    )
    process.muonGEMDigis.useDBEMap = True
    process.simMuonGEMPadDigis.InputCollection = "muonGEMDigis"
    return process

def runOn110XMC_IgnoreIncorrectGEMDB(process, runWithCrab = False):
    # customize unpacker
    process.muonGEMDigis.useDBEMap = True
    process.simMuonGEMPadDigis.InputCollection = "muonGEMDigis"
    return process

def runOn120XMC(process):
    ## customize unpacker
    process.muonGEMDigis.useDBEMap = True
    process.simMuonGEMPadDigis.InputCollection = "muonGEMDigis"
    return process

def dropCaloDigis(process):
    process.pdigi_valid.remove(process.simEcalTriggerPrimitiveDigis)
    process.pdigi_valid.remove(process.simEcalDigis)
    process.pdigi_valid.remove(process.simEcalPreshowerDigis)
    process.pdigi_valid.remove(process.simHcalTriggerPrimitiveDigis)
    process.pdigi_valid.remove(process.simHcalDigis)
    process.pdigi_valid.remove(process.simHcalTTPDigis)
    ## don't need DT digis
    process.pdigi_valid.remove(process.simMuonDTDigis)
    return process

def addCSCTriggerRun3(process):
    ## Run-2 patterns without ILT
    process.simCscTriggerPrimitiveDigis.commonParam.runME11ILT = False
    process.simCscTriggerPrimitiveDigis.commonParam.runME21ILT = False
    process.simCscTriggerPrimitiveDigis.commonParam.runCCLUT = cms.bool(False)
    process.simCscTriggerPrimitiveDigis.commonParam.runCCLUT_OTMB = cms.bool(False)
    process.simCscTriggerPrimitiveDigis.commonParam.runCCLUT_TMB = cms.bool(False)
    process.simCscTriggerPrimitiveDigis.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
    process.simEmtfDigis.CSCInput = cms.InputTag(
        'simCscTriggerPrimitiveDigis','MPCSORTED',process._Process__name)

    ## Run-2 patterns with ILT
    process.simCscTriggerPrimitiveDigisILT = process.simCscTriggerPrimitiveDigis.clone()
    process.simCscTriggerPrimitiveDigisILT.commonParam.runCCLUT = cms.bool(False)
    process.simCscTriggerPrimitiveDigisILT.commonParam.runCCLUT_OTMB = cms.bool(False)
    process.simCscTriggerPrimitiveDigisILT.commonParam.runCCLUT_TMB = cms.bool(False)
    process.simCscTriggerPrimitiveDigisILT.commonParam.runME11ILT = True
    process.simCscTriggerPrimitiveDigisILT.commonParam.runME21ILT = False
    process.simCscTriggerPrimitiveDigisILT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
    process.simEmtfDigisILT = process.simEmtfDigis.clone()
    process.simEmtfDigisILT.CSCInput = cms.InputTag(
        'simCscTriggerPrimitiveDigisILT','MPCSORTED',process._Process__name)

    ## Run-3 patterns with CCLUT, without ILT
    process.simCscTriggerPrimitiveDigisRun3CCLUT = process.simCscTriggerPrimitiveDigis.clone()
    process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT_OTMB = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runCCLUT_TMB = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.runME11ILT = False
    process.simCscTriggerPrimitiveDigisRun3CCLUT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("")
    process.simEmtfDigisRun3CCLUT = process.simEmtfDigis.clone()
    process.simEmtfDigisRun3CCLUT.CSCInput = cms.InputTag(
        'simCscTriggerPrimitiveDigisRun3CCLUT','MPCSORTED',process._Process__name)

    ## Run-3 patterns with CCLUT, with ILT
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT = process.simCscTriggerPrimitiveDigis.clone()
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT_OTMB = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runCCLUT_TMB = cms.bool(True)
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.runME11ILT = True
    process.simCscTriggerPrimitiveDigisRun3CCLUTILT.commonParam.GEMPadDigiClusterProducer = cms.InputTag("simMuonGEMPadDigiClusters")
    process.simEmtfDigisRun3CCLUTILT = process.simEmtfDigis.clone()
    process.simEmtfDigisRun3CCLUTILT.CSCInput = cms.InputTag(
        'simCscTriggerPrimitiveDigisRun3CCLUTILT','MPCSORTED',process._Process__name)

    ## redefine the L1-step
    process.SimL1Emulator = cms.Sequence(
        process.simMuonGEMPadDigis *
        process.simMuonGEMPadDigiClusters *
        process.simCscTriggerPrimitiveDigis *
        process.simCscTriggerPrimitiveDigisILT *
        process.simCscTriggerPrimitiveDigisRun3CCLUT *
        process.simCscTriggerPrimitiveDigisRun3CCLUTILT *
        process.simEmtfDigis *
        process.simEmtfDigisILT *
        process.simEmtfDigisRun3CCLUT *
        process.simEmtfDigisRun3CCLUTILT
    )

    return process

def addShowerTriggers(process):
    process.SimL1Emulator += process.simEmtfShowers
    process.SimL1Emulator += process.simGmtShowerDigis
    return process

def addAnalysisRun3(process, processName = "ReL1"):

    ana = process.GEMCSCAnalyzer
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","",processName)
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","",processName)
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","",processName)
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED",processName)
    ana.emtfTrack.inputTag = cms.InputTag("simEmtfDigis","",processName)

    process.GEMCSCAnalyzerILT = process.GEMCSCAnalyzer.clone()
    ana = process.GEMCSCAnalyzerILT
    processName = "ReL1"
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","MPCSORTED",processName)
    ana.emtfTrack.inputTag = cms.InputTag("simEmtfDigisILT","",processName)
    ana.gemCoPadDigi.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)

    useUnpacked = False
    if useUnpacked:
        ana.gemStripDigi.inputTag = "muonGEMDigis"
        ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")

    process.GEMCSCAnalyzerRun3CCLUT = process.GEMCSCAnalyzer.clone()
    anaCCLUT = process.GEMCSCAnalyzerRun3CCLUT
    anaCCLUT.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
    anaCCLUT.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
    anaCCLUT.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","",processName)
    anaCCLUT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","MPCSORTED",processName)
    anaCCLUT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisRun3CCLUT","",processName)

    process.GEMCSCAnalyzerNoILT = process.GEMCSCAnalyzer.clone()
    anaCCLUT = process.GEMCSCAnalyzerNoILT
    anaCCLUT.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisNoILT","",processName)
    anaCCLUT.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisNoILT","",processName)
    anaCCLUT.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisNoILT","",processName)
    anaCCLUT.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisNoILT","MPCSORTED",processName)
    anaCCLUT.emtfTrack.inputTag = cms.InputTag("simEmtfDigisNoILT","",processName)

    process.Analysis = cms.Sequence(
        #process.GEMCSCAnalyzer *
        process.GEMCSCAnalyzerILT
        #        process.GEMCSCAnalyzerRun3CCLUT
    )
    return process

def addMuonNtuplizerRun3(process):

    # the analyzer configuration
    ana = process.MuonNtuplizer
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED","ReL1")

    process.MuonNtuplizerILT = process.MuonNtuplizer.clone()
    ana = process.MuonNtuplizerILT
    processName = "ReL1"
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","MPCSORTED",processName)
    ana.emtfTrack.inputTag = cms.InputTag("simEmtfDigisILT","",processName)
    ana.gemCoPadDigi.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisILT","",processName)

    process.MuonNtuplizerRun3CCLUT = process.MuonNtuplizer.clone()
    ana = process.MuonNtuplizerRun3CCLUT
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","","ReL1")
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","","ReL1")
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","","ReL1")
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigisRun3CCLUT","MPCSORTED","ReL1")

    process.Analysis = cms.Sequence(
        process.MuonNtuplizer *
        process.MuonNtuplizerILT
    )
    return process


def addAnalysisRun3HST(process):

    ana = process.GEMCSCAnalyzer
    ana.cscALCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscCLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","","ReL1")
    ana.cscMPLCT.inputTag = cms.InputTag("simCscTriggerPrimitiveDigis","MPCSORTED","ReL1")
    ana.emtfTrack.inputTag = cms.InputTag("simEmtfDigis","","ReL1")

    useUnpacked = True
    if useUnpacked:
        ana.gemStripDigi.matchToSimLink = False
        ana.gemStripDigi.inputTag = "muonGEMDigis"
        ana.muon.inputTag = cms.InputTag("gmtStage2Digis","Muon")

    return process
