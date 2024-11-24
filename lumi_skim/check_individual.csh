#!/bin/tcsh -f

# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he1`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he2`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he3`)

<<<<<<< HEAD
foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he3`)

# foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_test`)
=======
# foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he3`)

foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_test`)
>>>>>>> 1ae420e50e02d38ab9ee9eb5dec97407be6fc269


#  echo $file
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he4`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he5`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he6`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he7`)
# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he8`)

# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he_empty2`)

# foreach file (`awk '{print}' list_of_runs_primex2/list_of_runs_he_empty3`)


  set my_file = `echo $file | sed -e s/run_/Run0/g`

  set n  = 0
  set n1 = 0

  set evt_total = 0

  echo $my_file

#  foreach file1 (`awk '{print}' lumi_primex2/individual/list_$my_file `)
  foreach file1 (`awk '{print}' lumi_primex3/individual/list_Run$my_file `)
#    echo $file1

   set my_file_log = `echo $file1 | sed -e s/ps.//g -e s/hd_rawdata/ps/g -e s/evio/log/g `

#     set my_file_log = `echo $file1 | sed  -e s/hd_rawdata/ps/g -e s/evio/log/g `

#      set my_file_log = `echo $file1 | sed  -e s/evio/log/g `
    
    echo $my_file_log

#    set log_path='/work/halld3/home/somov/lumi_skim/lumi_primex2/individual/log/'${my_file}'/'$my_file_log''

    set log_path='/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run'${my_file}'/'$my_file_log''
    
     echo $log_path

    @ n = $n + 1


   set bad_evt=`less $log_path | grep -i 'words_left_in_file' $log_path `

   grep -i 'words_left_in_file' $log_path 

#  echo SASHA0
   
  if ( "$bad_evt" != "") then
	echo  "\e[1;34m  '$log_path' \e[0m"   
   endif


   set evt=`less $log_path | grep Nevents | sed -e s/Nevents://g`

   echo $evt 

    echo SASHA1
    
    if ( "$evt" == "") then
	echo "\e[1;31m No events in file  '$log_path' \e[0m"	
    else
    
	set aaa = `echo "${evt_total} + ${evt}" | bc` 

	@ evt_total = $aaa  + 0

	@ n1 = $n1 + 1
    endif

#   echo SASHA2
    
#     echo $evt_total

  end

  echo " Run   Files  Good      Events    "   $my_file    $n     $n1      $evt_total
  echo "   " 


end
