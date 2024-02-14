def gemSimHitTOF(plotter,i):
  draw_1D(plotter.targetDir, "sh_gem_tof_rm1_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit TOF: region-1, layer1;Time of flight [ns];entries",
          "h_", "(40,18,22)", "timeOfFlight", AND(re(-1),la(1),plotter.sel[i]))
  draw_1D(plotter.targetDir, "sh_gem_tof_rm1_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit TOF: region-1, layer2;Time of flight [ns];entries",
          "h_", "(40,18,22)", "timeOfFlight", AND(re(-1),la(2),plotter.sel[i]))
  draw_1D(plotter.targetDir, "sh_gem_tof_rp1_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit TOF: region1, layer1;Time of flight [ns];entries",
          "h_", "(40,18,22)", "timeOfFlight", AND(re(1),la(1),plotter.sel[i]))
  draw_1D(plotter.targetDir, "sh_gem_tof_rp1_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit TOF: region1, layer2;Time of flight [ns];entries",
          "h_", "(40,18,22)", "timeOfFlight", AND(re(1),la(2),plotter.sel[i]))
