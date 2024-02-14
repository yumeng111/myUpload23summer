universe              = vanilla 
executable            = rungemcsc.sh
arguments             = runGEMCSCAnalyzer_condor_all.py 1000
output                = out/gemcsc1000gev.$(ClusterId).$(ProcId).out
error                 = err/gemcsc1000gev.$(ClusterId).$(ProcId).err
log                   = log/gemcsc1000gev.$(ClusterId).log
request_cpus          = 2
request_memory        = 4000M
+JobFlavour           = "workday"
Notification          = Complete
notify_user           = taohuang@email.tamu.edu
queue
