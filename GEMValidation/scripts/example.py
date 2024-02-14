import numpy as np
import awkward1 as ak
from objects import embed_crossref

sim_muons = ak.zip({
    "pt": ak.Array([
        [], [], [20.0, 30.0, 40, 50, 60]
    ]),
    "cluster_idx":  ak.Array([
        [], [], [[2, 5], [6, 2], [0], [0], [0]]
    ]),
    "cluster2_idx": ak.Array([
        [], [], [[1, 4], [2, 3], [0], [0], [0]]
    ])
}, depth_limit=None
)

print(ak.to_list(sim_muons))

gem_clusters = ak.zip(
    {
        "value": ak.Array(
            [[10.0, 20.0], [30.0, 40.0], [50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0]]
        ),
        "bx": ak.Array([[-1, 0], [1, 0], [0, 1, 2, -2, 1, 1, 1]]),
    }
)  # type "nevents * nclusters * whatever"


gem_clusters2 = ak.zip(
    {
        "value": ak.Array(
            [[10.0, 20.0], [30.0, 40.0, 50], [50.0, 60.0, 70.0, 80.0, 90.0, 100.0, 110.0]]
        ),
        "bx": ak.Array([[0, 1], [0, 1, 2], [0, 2, 3, -3, 4, 1, 2]]),
    }
)  # type "nevents * nclusters * whatever"


embed_crossref(sim_muons, "cluster_idx", gem_clusters, "clusters")
embed_crossref(sim_muons, "cluster2_idx", gem_clusters2, "clusters2")

print(ak.any(sim_muons.clusters.bx == 0, axis=2))
print(ak.any(sim_muons.clusters2.bx == 2, axis=2))
