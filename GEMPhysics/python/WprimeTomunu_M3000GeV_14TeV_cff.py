import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUEZ2starSettings_cfi import *
generator = cms.EDFilter("Pythia6GeneratorFilter",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    filterEfficiency = cms.untracked.double(1.0),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(14000.0),
    crossSection = cms.untracked.double(0.02123),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring(
	    'MSTU(21)=1     ! Check on possible errors during program execution', 
            'MSTJ(22)=2     ! Decay those unstable particles', 
            'PARJ(71)=10 .  ! for which ctau  10 mm', 
            'MSTP(33)=0     ! no K factors in hard cross sections', 
            'MSTP(2)=1      ! which order running alphaS', 
            'MSTP(51)=10042 ! structure function chosen (external PDF CTEQ6L1)', 
            'MSTP(52)=2     ! work with LHAPDF', 
            'PARP(82)=1.921 ! pt cutoff for multiparton interactions', 
            'PARP(89)=1800. ! sqrts for which PARP82 is set', 
            'PARP(90)=0.227 ! Multiple interactions: rescaling power', 
            'MSTP(95)=6     ! CR (color reconnection parameters)', 
            'PARP(77)=1.016 ! CR', 
            'PARP(78)=0.538 ! CR', 
            'PARP(80)=0.1   ! Prob. colored parton from BBR', 
            'PARP(83)=0.356 ! Multiple interactions: matter distribution parameter', 
            'PARP(84)=0.651 ! Multiple interactions: matter distribution parameter', 
            'PARP(62)=1.025 ! ISR cutoff', 
            'MSTP(91)=1     ! Gaussian primordial kT', 
            'PARP(93)=10.0  ! primordial kT-max', 
            'MSTP(81)=21    ! multiple parton interactions 1 is Pythia default', 
            'MSTP(82)=4     ! Defines the multi-parton model'),
        processParameters = cms.vstring(
	    'MSEL        = 0    !User defined processes', 
            'MSUB(142)   = 1    !Wprime  production', 
            'PMAS(34,1)  = 3000.!mass of Wprime', 
            "MDME(311,1) = 0    !W\' decay into dbar u", 
            "MDME(312,1) = 0    !W\' decay into dbar c", 
            "MDME(313,1) = 0    !W\' decay into dbar t", 
            "MDME(315,1) = 0    !W\' decay into sbar u", 
            "MDME(316,1) = 0    !W\' decay into sbar c", 
            "MDME(317,1) = 0    !W\' decay into sbar t", 
            "MDME(319,1) = 0    !W\' decay into bbar u", 
            "MDME(320,1) = 0    !W\' decay into bbar c", 
            "MDME(321,1) = 0    !W\' decay into bbar t", 
            "MDME(327,1) = 0    !W\' decay into e+ nu_e", 
            "MDME(328,1) = 1    !W\' decay into mu+ nu_mu", 
            "MDME(329,1) = 0    !W\' decay into tau+ nu_tau"),
        parameterSets = cms.vstring(
	    'pythiaUESettings', 
            'processParameters')
    )
)
