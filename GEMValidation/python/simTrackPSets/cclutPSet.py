import FWCore.ParameterSet.Config as cms

cclutPSet = cms.PSet(
    positionLUTFiles = cms.vstring(
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat0_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat1_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat2_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat3_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat4_v1.txt"
    ),

    positionFloatLUTFiles = cms.vstring(
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat0_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat1_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat2_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat3_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePosOffsetLUT_pat4_float_v1.txt"
    ),

    slopeLUTFiles = cms.vstring(
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat0_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat1_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat2_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat3_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat4_v1.txt"
    ),

    slopeFloatLUTFiles = cms.vstring(
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat0_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat1_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat2_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat3_float_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodeSlopeLUT_pat4_float_v1.txt"
    ),

    patternConversionLUTFiles = cms.vstring(
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePatternConversionLUT_pat0_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePatternConversionLUT_pat1_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePatternConversionLUT_pat2_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePatternConversionLUT_pat3_v1.txt",
        "L1Trigger/CSCTriggerPrimitives/data/CCLUT/CSCComparatorCodePatternConversionLUT_pat4_v1.txt"
    )
)
