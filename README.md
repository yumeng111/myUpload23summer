# GEMCode


## Instruction to use GEMCode package for CMSSW_13_0_x

```
cmsrel CMSSW_13_0_0_pre4
cd CMSSW_13_0_0_pre4/src
cmsenv
git clone  https://github.com/gem-sw/GEMCode
scram b -j 9
```


## GEMCSCAnalyzer: simtrack based analyzer to analyze muon trigger MC efficiency
![GEMCSCAnalyzer scheme](https://github.com/gem-sw/GEMCode/blob/for-CMSSW_12_0_1_X/docs/GEMCSCAnalyzer.png?raw=true)

The MC simluation:
  - Firstly gen particles are generated either from collisions or from particle guns. 
  - The charged particle (here we only care about muons) can genrate the simtrack when it flies through the detectors,  
and meanwhile register the simhits in the detectors. The simtrack and simhits can be associated through track id. 
  - Then simhits in detectors produce the digi in simulation.  
  - The trigger emulator uses digis in the detectors to build trigger stubs,
like anode local charged track(ALCT), cathode-LCT (CLCT) and LCT for CSC muon triggering. 
  - The next step is that track-finder builds muon track by collecting and using trigger stubs from different muons stations. EMTF emulator is the muon track-finder for endcap, OMTF
is the  muon track-finder for overlap region (0.9<eta<1.1) and BMTF is for barrel region. 
  - Finally the muon track is sent to regional muon trigger and global muon trigger for muon triggering
  - After LS3 upgrade, inner tracker would join the L1 trigger system and L1Track could also trigger on muons.  
  
GEMCSC analyzer is designed to analyze the muon trigger efficienies in different steps by matching simhits/digis/trigger stub/muon tracks to simtrack. Each part in the matching would initialize and fill one TTree, and match information for one simtrack would fill one entry in TTree.  

### GenParticle Matcher
GenParticle matcher is to associate the gen particle with simtrack by comparing the pdgId and eta-phi position. The code is in GEMValidation/src/Matchers/GenParticleMatcher.cc

### SimHits Matchers
The simHits matchers are under CMSSW module Validation/MuonHits/src. These matchers match the CSC/GEM/ME0/RPC/DT simhits to simtrack by track id. 

### CSC/GEM/ME0/DT/RPCDigi Matchers
The CSCDigi matcher is in Validation/MuonCSCDigis/src/CSCDigiMatcher.cc, GEMDigi matcher for GE11 and GE21 is in Validation/MuonGEMDigis/src/GEMDigiMatcher.cc and ME0/DT/RPCDigi matchers are under GEMValidation/src/Matchers.

The digi matcher is associating the digi to simtrack via the simhits. The matching process is done by comparing the simhit position and digi position in each layer. 

### CSC/ME0Stub Matchers
CSCStub matcher is in Validation/MuonCSCDigis and ME0Stub matcher is NOT included yet in this package.  ME0 trigger stub emulation is not fully implemented yet.

The trigger stub is built by using the digi in different layers and therefore the stub and simtrack association is done by comparing the stub position with digi position

### L1Mu Matcher
L1Mu matcher associates the L1Mu, include EMTF track, region muon cand and global muon cand, to simtrack by comparing the delta R between simtrack and L1Mu. The code is in GEMValidation/src/Matchers/L1MuMatcher.cc.

### Rechits Matchers
Rechits are built in reco step and rechit efficiency study is not included here.

CSCRechits matcher is in Validation/CSCRecHits and GEMValidation/src/Matchers

### Configuration to run GEMCSCAnalyzer
The example configuration to run GEMCSCAnalyzer is GEMValidation/test/runGEMCSCAnalyzer_cfg.py

### scripts to make plot from GEMCSCAnalyzer output
GEMValidation/scripts/makePlots.py is used to plot efficiency and resolution etc from GEMCSCAnalyzer ntuples.

## MuonNtuplizer: rate study
MuonNtuplizer could be used for trigger rate study by filling track information into TTree

## Typical RelVal Samples 
| Data path     | Description |
| ----------- | ----------- |
| /RelValSingleMuPt10*/CMSSW_X_Y_Z_*/GEN-SIM-DIGI-RAW with X/Y/Z replaced by real CMSSW version number | fixed pt, 10/100/1000 GeV, PU0|
| /RelValZpToMM_m6000_14TeV/CMSSW_X_Y_Z_*/GEN-SIM-DIGI-RAW with X/Y/Z replaced by real CMSSW version number | Zprime sample with mass = 6000 GeV | 

 

## Deprecated instruction for old CMSSW Release 
<details>
<summary>CMSSW_12_6_x</summary>

```
cmsrel CMSSW_12_6_0_pre2
cd CMSSW_12_6_0_pre2/src
cmsenv
git clone  https://github.com/gem-sw/GEMCode
scram b -j 9
```
To run GEMCode with phase2 MC sample, the following configuration from GEMCode/GEMValidation/test could be used:
```
cmsRun runGEMCSCAnalyzer_Phase2_cfg.py
```
The one of 125X muon sample is /SingleMuon_Pt-0To200_Eta-1p4To3p1-gun/Phase2Fall22DRMiniAOD-PU200_125X_mcRun4_realistic_v2-v1/GEN-SIM-DIGI-RAW-MINIAOD
and the full 125X sample compain is explained in the twiki: https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideL1TPhase2Instructions#Phase2Fall22_Campaign_125X_sampl

The samples can be queried from cms DAS website through: 
https://cmsweb.cern.ch/das/request?input=dataset+dataset%3D%2F%2A%2FPhase2Fall22DRMiniAOD-PU200%2A125X%2A%2FGEN-SIM-DIGI-RAW-MINIAOD&instance=prod/global&idx=150&limit=50
</details>

