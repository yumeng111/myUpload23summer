universe              = vanilla 
executable            = rungemcsc.sh
arguments             = runGEMCSCAnalyzer_condor_all.py 100
output                = out/gemcsc100gev.$(ClusterId).$(ProcId).out
error                 = err/gemcsc100gev.$(ClusterId).$(ProcId).err
log                   = log/gemcsc100gev.$(ClusterId).log
request_cpus          = 2
request_memory        = 4000M
+JobFlavour           = "workday"
Notification          = Complete
notify_user           = taohuang@email.tamu.edu
queue
