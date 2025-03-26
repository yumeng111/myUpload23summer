setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
loadProjectFile -file "/home/cscdev/XilinxProj/otmb_mcs_download.ipf"
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
setMode -bs
attachflash -position 1 -bpi "XCF128X"
setMode -bs
setMode -bs
setMode -bs
setMode -bs
attachflash -position 1 -bpi "XCF128X"
assignfiletoattachedflash -position 1 -file "/home/cscdev/XilinxProj/ProdTestMCS/NewTAMU-UCLA-240t.mcs"
ReadUsercode -p 1 
ReadUsercode -p 1 
Program -p 1 -dataWidth 16 -rs1 NONE -rs0 NONE -bpionly -e -loadfpga 
ReadUsercode -p 1 
setMode -bs
setMode -bs
deleteDevice -position 1
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
