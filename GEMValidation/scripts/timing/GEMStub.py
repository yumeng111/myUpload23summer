

def gemPadBX(plotter):
  draw_bx(plotter.targetDir, "pad_dg_bx_rm1_l1", plotter.ext, plotter.treeGEMCSPadDigis, "Bunch crossing: region-1, layer1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(-1),la(1)))
  draw_bx(plotter.targetDir, "pad_dg_bx_rm1_l2", plotter.ext, plotter.treeGEMCSPadDigis, "Bunch crossing: region-1, layer2;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(-1),la(2)))
  draw_bx(plotter.targetDir, "pad_dg_bx_rp1_l1", plotter.ext, plotter.treeGEMCSPadDigis, "Bunch crossing: region1, layer1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(1),la(1)))
  draw_bx(plotter.targetDir, "pad_dg_bx_rp1_l2", plotter.ext, plotter.treeGEMCSPadDigis, "Bunch crossing: region1, layer2;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", AND(re(1),la(2)))



def gemCoPadBX(plotter):
  draw_bx(plotter.targetDir, "copad_dg_bx_rm1", plotter.ext, plotter.treeGEMCSCoPadDigis, "Bunch crossing: region-1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", rm1)
  draw_bx(plotter.targetDir, "copad_dg_bx_rp1", plotter.ext, plotter.treeGEMCSCoPadDigis, "Bunch crossing: region1;bunch crossing;entries",
	  "h_", "(11,-5.5,5.5)", "bx", rp1)
