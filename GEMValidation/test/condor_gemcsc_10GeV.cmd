universe              = vanilla 
executable            = rungemcsc.sh
arguments             = runGEMCSCAnalyzer_condor_all.py 10
output                = out/gemcsc10gev.$(ClusterId).$(ProcId).out
error                 = err/gemcsc10gev.$(ClusterId).$(ProcId).err
log                   = log/gemcsc10gev.$(ClusterId).log
request_cpus          = 2
request_memory        = 4000M
+JobFlavour           = "workday"
Notification          = Complete
notify_user           = taohuang@email.tamu.edu
queue
