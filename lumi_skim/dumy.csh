#!/bin/tcsh -f


set n = 1


foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he5`)

# foreach file (`awk '{print}' list_of_runs_test`)

  echo $n
  @ n = $n + 1 

  set my_run = $file
  set out_dir=Run$my_run

  echo $out_dir
  
# rm  lumi_primex3/individual/list_Run$my_run

end
