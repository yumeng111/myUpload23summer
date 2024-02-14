#!/usr/bin/python

import awkward1 as ak
import uproot4
import matplotlib.pyplot as plt
from objects import getObjects, id_to_station
from ROOT import TH1F, TEfficiency, SetOwnership, kBlue
#import numba as nb
import numpy as np
import awkward1 as ak
from objects import embed_crossref

"""
Plots needed
- alct rate per chamber type
- lct rate per chamber type
- anode/cathode pretrigger rate per chamber type
-1;95;0c alct rate per chamber
- lct rate per chamber

with/without CCLUT enabled

- Same plots for DAQ rates

- Same plots for Run-3 and Phase-2 (PU200) with recent HGCal

- emtf rate

- l1mu rate
"""

mycache = {}

tree = uproot4.open("out_ana_phase2.ntuple.root")["MuonNtuplizer"]["FlatTree"].arrays(array_cache=mycache)

csc_clct, csc_alct, csc_lct, gem_cluster, emtftrack, l1mu, sim_muon = getObjects(tree)

## number of events (get from tree)
numberOfTotalEvents = 18000.

## Number of bunches
numberOfBunches = 2760.

## LHC bunch frequency
lhcFrequency = 11.246

## Effective LHC rate
rate = numberOfBunches * lhcFrequency

## normalization
normalization = rate / numberOfTotalEvents

## Rates per chamber type
alct_trigger_rate = []
clct_trigger_rate = []
lct_trigger_rate = []
alct_daq_rate = []
clct_daq_rate = []
lct_daq_rate = []

gem_trigger_rate = []
gem_daq_rate = []

"""
## trigger rates for each station
for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = ((csc_alct.bx==3) &
            (csc_alct.station == station) &
            (csc_alct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("Trigger Rate for ALCT for CSC station = %d, ring = %d (BX3): %f kHz"%(station, ring, objectRate))

for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = ((csc_clct.bx==7) &
            (csc_clct.station == station) &
            (csc_clct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("Trigger Rate for CLCT for CSC station = %d, ring = %d (BX7): %f kHz"%(station, ring, objectRate))


for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = ((csc_lct.bx==8) &
            (csc_lct.station == station) &
            (csc_lct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("Trigger Rate for LCT for CSC station = %d, ring = %d (BX8): %f kHz"%(station, ring, objectRate))

## DAQ rates!!!
for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = (((csc_alct.bx==3) | (csc_alct.bx==2) | (csc_alct.bx==1) | (csc_alct.bx==4) | (csc_alct.bx==5)) &
            (csc_alct.station == station) &
            (csc_alct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("DAQ Rate for ALCT for CSC station = %d, ring = %d: %f kHz"%(station, ring, objectRate))

for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = (((csc_clct.bx==7) | (csc_clct.bx==6) | (csc_clct.bx==8) | (csc_clct.bx==5) | (csc_clct.bx==9) | (csc_clct.bx==4) | (csc_clct.bx==10)) &
            (csc_clct.station == station) &
            (csc_clct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("DAQ Rate for CLCT for CSC station = %d, ring = %d: %f kHz"%(station, ring, objectRate))


for ids in range(0,9):
    station = id_to_station[ids][0]
    ring = id_to_station[ids][1]
    cuts = (((csc_lct.bx==8) | (csc_lct.bx==7) | (csc_lct.bx==6) | (csc_lct.bx==5) | (csc_lct.bx==9) | (csc_lct.bx==10) | (csc_lct.bx==11)) &
            (csc_lct.station == station) &
            (csc_lct.ring == ring))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("DAQ Rate for LCT for CSC station = %d, ring = %d: %f kHz"%(station, ring, objectRate))

## GEM cluster rates
for station in range(1,3):
    cuts = ((gem_cluster.bx == 0) &
            (gem_cluster.station == station))
    ak.to_list(cuts)
    passEvents = ak.sum(cuts)
    objectRate = passEvents * normalization
    print("Trigger Cluster Rate for GEM station = %d, ring = %d: %f kHz"%(station, ring, objectRate))


## EMTF rate
cuts = ((emtftrack.bx == 0) & (emtftrack.pt > 0))
ak.to_list(cuts)
passEvents = ak.sum(cuts)
objectRate = passEvents * normalization
print("EMTF track-finder rate: %f kHz"%(objectRate))

## L1Mu rate
cuts = ((l1mu.bx == 0) & (l1mu.pt > 0))
ak.to_list(cuts)
passEvents = ak.sum(cuts)
objectRate = passEvents * normalization
print("L1Mu trigger rate: %f kHz"%(objectRate))

## efficiency
#for ids in range(0,9):
#    station = id_to_station[ids][0]
#    ring = id_to_station[ids][1]
#cuts = (sim_muon.pt>10)
#cuts2 = (len(sim_muon.id_l1mu) > 0)
#print(sim_muon.id_l1mu)
#print(sim_muon.pt)
#print(sim_muon.eta)
#print(sim_muon.charge)
#print(csc_clct.bx)
#print(csc_clct.tpid)
#print(cuts)
#print(cuts2)
#ak.to_list(cuts)
#passEvents = ak.sum(cuts)
#print(passEvents)

"""
iEvent = 0

print(ak.to_list(ak.Array(sim_muon))[2:3])
print(ak.to_list(sim_muon.eta)[2:3])
print(ak.to_list(sim_muon.sim_id_gem_cluster)[2:3])
print(ak.to_list(sim_muon.pt)[2:3])
print(ak.to_list(gem_cluster.station)[2:3])
print(ak.to_list(gem_cluster.bx)[2:3])
print(ak.to_list(gem_cluster.pad)[2:3])

embed_crossref(sim_muon, "sim_id_gem_cluster", gem_cluster, "clusters")

"""
id_global = ak.flatten(
    sim_muon.sim_id_gem_cluster + np.asarray(gem_cluster.layout.starts), axis=None
)

sim_muon["gem_clusters"] = ak.Array(
    ak.layout.ListOffsetArray64(
        sim_muon.layout.offsets,
        ak.layout.ListOffsetArray64(
            sim_muon.layout.content.offsets,
            ak.flatten(gem_cluster)[id_global].layout,
        ),
    )
)

mymask = ak.any(sim_muon.gem_clusters.pad == 168, axis=1)
print(ak.to_list(mymask)[2:3])
mymask = ak.any(sim_muon.gem_clusters.pad == 173, axis=1)
print(ak.to_list(mymask)[2:3])
mymask = ak.any(sim_muon.gem_clusters.pad == 97, axis=1)
print(ak.to_list(mymask)[2:3])
mymask = ak.any(sim_muon.gem_clusters.pad == 169, axis=1)
print(ak.to_list(mymask)[2:3])
#print(ak.any(sim_muon.gem_clusters.station == 1, axis=1))
"""

"""
sim_muons = ak.zip(
    {
        "pt": ak.Array([[], [], [20.0, 30.0]]),
        "cluster_idx": ak.Array([[], [], [[2, 5], [6, 2]]]),
    }
)

gem_clusters = ak.zip(
{
    "value": ak.Array(
        [[10.0, 20.0], [30.0, 40.0], [50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0]]
    ),
    "bx": ak.Array([[-1, 0], [1, 0], [0, 1, 2, -2, 1, 1, 1]]),
}
)  # type "nevents * nclusters * whatever"

id_global = ak.flatten(
    sim_muons.cluster_idx + np.asarray(gem_clusters.layout.starts), axis=None
)

sim_muons["clusters"] = ak.Array(
    ak.layout.ListOffsetArray64(
        sim_muons.layout.offsets,
        ak.layout.ListOffsetArray64(
            sim_muons.layout.content.offsets,
            ak.flatten(gem_clusters)[id_global].layout,
        ),
    )
)

print(ak.to_list(sim_muons)[:])
print()
print(ak.to_list(gem_clusters)[:])
print()
print(ak.to_list(ak.any(sim_muons.clusters.bx == 0, axis=1))[:])
"""
