#!/bin/tcsh -f

# foreach file (`awk '{print}' list_of_runs_primex3_remain`)

# foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he3`)
# foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he4`)
# foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he5`)

foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he6`)



 set my_file = `echo $file | sed -e s/run_/Run0/g`   

 set run_num = `echo $file | sed -e s/run_//g`

 echo $my_file



# ls /cache/halld/RunPeriod-2021-11/calib/ver01/PS/$my_file/*.evio  | wc

# ls /mss/halld/RunPeriod-2021-11/calib/ver01/PS/$my_file/*.evio  | wc

# ls /mss/halld/RunPeriod-2021-11/calib/ver00/PS/$my_file/*.evio  | wc
# ls /mss/halld/RunPeriod-2021-11/rawdata/$my_file/*.evio  | wc


# ls /cache/halld/RunPeriod-2022-08/calib/ver00/PS/$run_num/*.evio 
# ls /mss/halld/RunPeriod-2022-08/calib/ver00/PS/$run_num/*.evio


# PrimEx III

# ls /cache/halld/RunPeriod-2022-08/rawdata/$run_num/*.evio | wc
## ls /mss/halld/RunPeriod-2022-08/rawdata/Run$run_num/*.evio  | wc 
# ls /mss/halld/RunPeriod-2022-08/calib/ver01/PS/$run_num/*.evio | wc 
## ls /mss/halld/RunPeriod-2022-08/recon/ver01/ps/$run_num/*.evio | wc

## ls /cache/halld/RunPeriod-2022-08/recon/ver01/ps/$run_num/*.evio | wc

jcache get /mss/halld/RunPeriod-2022-08/recon/ver01/ps/$run_num/*.evio

 # New files
ls /mss/halld/RunPeriod-2022-08/rawdata/Run$run_num/*.evio  | wc 
# ls /mss/halld/RunPeriod-2022-08/recon/ver01/ps/$run_num/*.evio | wc
ls /cache/halld/RunPeriod-2022-08/recon/ver01/ps/$run_num/*.evio | wc

# OLD files
# ls /mss/halld/RunPeriod-2022-08/calib/ver01/PS/Run$run_num/*.evio | wc
ls /cache/halld/RunPeriod-2022-08/calib/ver01/PS/Run$run_num/*.evio | wc

jcache get /mss/halld/RunPeriod-2022-08/calib/ver01/PS/Run$run_num/*.evio


# jcache get /mss/halld/RunPeriod-2022-08/production/PS/${my_file}/*.evio



#ls /mss/halld/RunPeriod-2021-11/recon/ver01/ps/0$run_num/*.evio | wc
#ls /cache/halld/RunPeriod-2021-11/recon/ver01/ps/0$run_num/*.evio | wc


# PrimEx
# ls /mss/halld/RunPeriod-2021-08/rawdata/$my_file/*.evio  | wc
# ls /cache/halld/RunPeriod-2021-08/rawdata/ $my_file/*.evio  | wc

# ls /mss/halld/RunPeriod-2021-08/calib/ver01/PS/$my_file/*.evio | wc
# ls /cache/halld/RunPeriod-2021-08/calib/ver01/PS/$my_file/*.evio | wc

# jcache get  /mss/halld/RunPeriod-2022-08/rawdata/$run_num/*.evio

# jcache get /mss/halld/RunPeriod-2021-08/rawdata/$my_file/*.evio  | wc



# jcache get /mss/halld/RunPeriod-2021-08/calib/ver01/PS/$my_file/*.evio 





# SRC
# jcache get /mss/halld/RunPeriod-2021-11/recon/ver01/ps/0$run_num/*.evio
# jcache get /mss/halld/RunPeriod-2021-11/calib/ver01/PS/$my_file/*.evio 
# jcache get /mss/halld/RunPeriod-2021-11/calib/ver00/PS/$my_file/*.evio 


end
