#when pc starts
#sudo ./home/cscdev/load_igb_emu.sh
sudo source ~/load_igb_emu.sh
#if above gives sudo: source not found error, run the following
sudo ~/load_igb_emu.sh





cd /home/cscdev/EMULIB_V14_GEM_mod_by_Sasha/emu/
source ~cscdev/setup_emulib_env_XDAQ14.sh
export set BUILD_HOME=/home/cscdev/EMULIB_V14_GEM_mod_by_Sasha
Set=framework make clean
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log

# Run


cd  /home/cscdev/EMULIB_V14_GEM_mod_by_Sasha/emu/
source ~cscdev/setup_emulib_env_XDAQ14.sh
export set BUILD_HOME=/home/cscdev/EMULIB_V14_GEM_mod_by_Sasha
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V14_GEM_mod_by_Sasha/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V14_GEM_mod_by_Sasha/xml/tamu_test2019_GEM.xml


# for Emulib15
cd /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/emu/
export set BUILD_HOME=/home/cscdev/EMULIB_V15_Master_mod_by_Kyla
source ~cscdev/setup_emulib_env.sh
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/xml/tamu_test2019_GEM.xml
#xdaq.exe -h 10.119.64.4 -p 20016 -e $BUILD_HOME/xml/default.profile_cc7_noXMAS  -c $BUILD_HOME/xml/tamu_test2019_GEM.xml
#Set=framework make -j 8 clean

cd /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/emu/
export set BUILD_HOME=/home/cscdev/EMULIB_V15_Master_mod_by_Kyla
source ~cscdev/setup_emulib_env.sh
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V15_Master_mod_by_Kyla/xml/tamu_test2019_GEM.xml

cd /home/cscdev/EMULIB_V15_Master_mod_by_Yumeng/emu/
export set BUILD_HOME=/home/cscdev/EMULIB_V15_Master_mod_by_Yumeng
source ~cscdev/setup_emulib_env.sh
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log
#### ME11 test configuratoion
#xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V15_Master_mod_by_Yumeng/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V15_Master_mod_by_Yumeng/xml/tamu_test2019_GEM.xml
####ME21 test configuration
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V15_Master_mod_by_Yumeng/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V15_Master_mod_by_Yumeng/xml/tamu_test2021_MEX1_SLHCon.xml
#xdaq.exe -h 10.119.64.4 -p 20016 -e $BUILD_HOME/xml/default.profile_cc7_noXMAS  -c $BUILD_HOME/xml/tamu_test2019_GEM.xml
#Set=framework make -j 8 clean







cd /home/cscdev/EMULIB_V14_GEM_forSasha/emu/
source ~cscdev/setup_emulib_env_XDAQ14.sh
export set BUILD_HOME=/home/cscdev/EMULIB_V14_GEM_forSasha
Set=framework make clean
Set=framework make -j 8 install >& make-install-framework.log
grep error: make-install-framework.log

# Run


cd  /home/cscdev/EMULIB_V14_GEM_forSasha/emu/
source ~cscdev/setup_emulib_env_XDAQ14.sh
export set BUILD_HOME=/home/cscdev/EMULIB_V14_GEM_forSasha
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V14_GEM_forSasha/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V14_GEM_forSasha/xml/tamu_test2019_GEM.xml


cd  ~/EMULIB_V14_GEM/
export set BUILD_HOME=/home/cscdev/EMULIB_V14_GEM/
source ~cscdev/setup_emulib_env_XDAQ14.sh
xdaq.exe -h 10.119.64.4 -p 20016 -e /home/cscdev/EMULIB_V14_GEM/xml/default.profile_cc7_noXMAS  -c /home/cscdev/EMULIB_V14_GEM/xml/tamu_test2019_GEM.xml
