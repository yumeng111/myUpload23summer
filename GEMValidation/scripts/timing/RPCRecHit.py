for i in range(len(plotter.stationsToUse)):
  continue
  st = plotter.stationsToUse[i]

  print "Processing station ", i, plotter.stations.reverse_mapping[st]
  makePlot(plotter, st, "rpc_time", -5, 5, 100, AND(has_rpc, TCut("pt>10")), "RPC hit timing [ns]", "RPCTiming", "RecHit")
  makePlot(plotter, st, "rpc_timeError", 0, 3, 30, AND(has_rpc, TCut("pt>10")), "RPC hit time error [ns]", "RPCTimingError", "RecHit")
  makePlot(plotter, st, "rpc_bunchX", -5, 5, 100, AND(has_rpc, TCut("pt>10")), "RPC bunch crossing", "RPCBunchX", "RecHit")
  makePlot(plotter, st, "rpc_firstClusterStrip", 0, 100, 100, AND(has_rpc, TCut("pt>10")), "RPC cluster strips", "RPCFirstClusterStrip", "RecHit")
  makePlot(plotter, st, "rpc_clusterSize", 0, 10, 10, AND(has_rpc, TCut("pt>10")), "RPC cluster size", "RPCClusterSize", "RecHit")


makePlotTDR(plotter,"rpc_time", 4, -5, 5, 50, AND(has_rpc, TCut("pt>10")), "RPC hit timing [ns]", "RPCTiming", "RecHit")
makePlotTDR(plotter,"rpc_time", 7, -5, 5, 50, AND(has_rpc, TCut("pt>10")), "RPC hit timing [ns]", "RPCTiming", "RecHit")
