#!/usr/bin/bash

csctriggerdir='../../../L1Trigger/CSCTriggerPrimitives/'

## clean up old directories
rm -rf $csctriggerdir/data
rm -rf $csctriggerdir/output_3layers
rm -rf $csctriggerdir/output_4layers
rm -rf $csctriggerdir/figures_3layers
rm -rf $csctriggerdir/figures_4layers

## make new directories
mkdir $csctriggerdir/data
mkdir $csctriggerdir/test/macros/output_3layers
mkdir $csctriggerdir/test/macros/output_4layers
mkdir $csctriggerdir/test/macros/figures_3layers
mkdir $csctriggerdir/test/macros/figures_4layers

## set environment
eval `scramv1 runtime -sh`

## run the script
cd $csctriggerdir/test/macros/
root -l -q -b CCLUTLinearFitWriter.cpp++'(3)'
root -l -q -b CCLUTLinearFitWriter.cpp++'(4)'
cp output_3layers/*txt ../../data
cd ../../../../
