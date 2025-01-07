#!/bin/tcsh -f


set runlist=$1

set n = 1

# foreach file (`awk '{print}' ../list_of_runs_primex3/test`)
# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he3.txt`)

# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he4.txt`)

# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he5.txt`)

# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he6.txt`)

# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he7.txt`)
# 
# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he8.txt`)

# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he9.txt`)


# foreach file (`awk '{print}' list_of_good_runs_after_merging/goodRuns_he10.txt`)

# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he3`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he4`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he5`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he6`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he7`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he8`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he9`)
# foreach file (`awk '{print}' list_of_runs_from_mss/list_of_runs_he10`)


foreach file (`awk '{print}' /work/halld/home/zhikun/lumi_skim/list_of_runs/list_of_blanks/list_of_blanks`)





# foreach file (`awk '{print}' ../list_of_runs_primex2/list_of_runs_he_empty2`)

# foreach file (`awk '{print}' ../list_of_runs_primex2/list_of_runs_he_empty3`)

  echo $n
  @ n = $n + 1

  set my_file = `echo $file | sed -e s/run_/Run0/g`



  echo $my_file


  set stringList = ""

  set nfile = 0
  
#  foreach file1 (`awk '{print}'  individual/list_Run$my_file `)
  
  foreach file1 (`awk '{print}'  /work/halld/home/zhikun/lumi_skim/list_of_files/list_of_files_after_exclusion/list_Run$my_file `)
     set tmp  = `echo $file1 | sed -e s/.ps//g -e s/evio/root/g`

#     set tmp1 = `echo $tmp | sed -e s/hd_rawdata/ps/g`

     echo $tmp

     set stringList =  "$stringList  $tmp"

     @ nfile = $nfile + 1
     
     
  end

  echo Number of files $nfile
  
# cd /volatile/halld/home/test_lumi/Run$my_file
cd /volatile/halld/home/zhikun/merged_new_runs/Run$my_file

# rm  ps_$my_file.root

# cd /cache/halld/RunPeriod-2021-08/analysis/ver04_lumi/$my_file

echo `hadd -f ps_$my_file.root $stringList` 

echo $stringList 

cd  /work/halld/home/zhikun/lumi_skim/

end 

