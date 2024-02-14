from WMCore.Configuration import Configuration
config = Configuration()

config.section_("General")
config.General.requestName = 'GEMCSCanalyzerZpToMuMuM6000NoPU0911v2'
config.General.workArea = 'crab_projects'
config.General.transferLogs=False
config.section_("JobType")
config.JobType.pluginName = 'Analysis'
config.JobType.psetName = 'runGEMCSCAnalyzer_cfg.py'
#config.JobType.scriptExe = ''
config.JobType.maxMemoryMB = 4000
config.JobType.inputFiles = [] #hadd nano will not be needed once nano tools are in cmssw
config.JobType.sendPythonFolder	 = True
config.JobType.allowUndistributedCMSSW = True
config.section_("Data")
config.Data.inputDataset = '/RelValZpToMM_m6000_14TeV/CMSSW_12_3_0_pre2-122X_mcRun3_2021_realistic_v5-v1/GEN-SIM-DIGI-RAW'
config.Data.inputDBS = 'global'
config.Data.splitting = 'FileBased'
#config.Data.splitting = 'EventAwareLumiBased'
config.Data.unitsPerJob = 1
#config.Data.unitsPerJob = 100
#config.Data.totalUnits = 2000
#config.Data.inputDBS='phys03'
username = "tahuang"
config.Data.outLFNDirBase = '/store/user/%s/CSCEmulation/%s/'%(username, config.General.requestName)
config.Data.publication = False
config.section_("Site")
config.Site.storageSite = "T3_CH_CERNBOX" ## or T3_US_FNALLPC

