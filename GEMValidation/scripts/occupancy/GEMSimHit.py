def gemSimHitOccupancyXY(plotter,i):

  ## per station
  draw_occ(plotter.targetDir, "sh_gem_xy_st1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: station1;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(st(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_st2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: station2;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(st(2),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_st3" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: station3;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(st(3),plotter.sel[i]), "COLZ")

  ## per station and per layer
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer1;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(2),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1 ,layer1;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(2),plotter.sel[i]), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer1;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(2),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, layer1;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(2),plotter.sel[i]), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer1;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(2),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, layer1;globalX [cm];globalY [cm]",
             "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l2" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, layer2;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(2),plotter.sel[i]), "COLZ")

  ## per station and per layer, odd/even
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1 ,layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(2),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),la(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1 ,layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),la(2),plotter.sel[i],even), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2 ,layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(2),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),la(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2 ,layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),la(2),plotter.sel[i],even), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3 ,layer1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, layer2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(2),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),la(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3 ,layer1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_l2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, layer2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),la(2),plotter.sel[i],even), "COLZ")

    ## per endcap, station and odd/even
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1 , Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(-1),st(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1 , Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st1_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station1, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-260,260,100,-260,260)", "globalX:globalY", AND(re(1),st(1),plotter.sel[i],even), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2 , Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2 , Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st2_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station2, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(2),plotter.sel[i],even), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3 , Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),plotter.sel[i],odd), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_odd" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, Odd;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),plotter.sel[i],odd), "COLZ")

  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rm1_st3_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region-1, station3, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(-1),st(3),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3 , Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),plotter.sel[i],even), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_xy_rp1_st3_even" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits,
           plotter.pre[i] + " SimHit occupancy: region1, station3, Even;globalX [cm];globalY [cm]",
           "h_", "(100,-400,400,100,-400,400)", "globalX:globalY", AND(re(1),st(3),plotter.sel[i],even), "COLZ")


#_______________________________________________________________________________
def gemSimHitOccupancyRZ(plotter,i):
  draw_occ(plotter.targetDir, "sh_gem_zr_rm1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit occupancy: region-1;globalZ [cm];globalR [cm]",
           "h_", "(200,-573,-564,110,130,240)", "sqrt(globalX*globalX+globalY*globalY):globalZ", AND(re(-1),plotter.sel[i]), "COLZ")
  draw_occ(plotter.targetDir, "sh_gem_zr_rp1" + plotter.suff[i], plotter.ext, plotter.treeGEMSimHits, plotter.pre[i] + " SimHit occupancy: region1;globalZ [cm];globalR [cm]",
           "h_", "(200,564,573,110,130,240)", "sqrt(globalX*globalX+globalY*globalY):globalZ", AND(re(1),plotter.sel[i]), "COLZ")

#_______________________________________________________________________________
