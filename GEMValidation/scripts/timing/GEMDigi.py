def gemDigiBX(plotter):
  draw_bx(plotter.targetDir, "strip_digi_bx_rm1_l1", plotter.ext, plotter.treeGEMDigis, "Bunch crossing: region-1, layer1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(-1),la(1)))
  draw_bx(plotter.targetDir, "strip_digi_bx_rm1_l2", plotter.ext, plotter.treeGEMDigis, "Bunch crossing: region-1, layer2;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(-1),la(2)))
  draw_bx(plotter.targetDir, "strip_digi_bx_rp1_l1", plotter.ext, plotter.treeGEMDigis, "Bunch crossing: region1, layer1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(1),la(1)))
  draw_bx(plotter.targetDir, "strip_digi_bx_rp1_l2", plotter.ext, plotter.treeGEMDigis, "Bunch crossing: region1, layer2;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(1),la(2)))
