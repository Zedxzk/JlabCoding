#!/bin/tcsh -f

# 创建临时文件用于记录错误
set error_flag = 0
set res_file = "rescsh.txt"

# 清空或创建 res.txt 文件
echo "" > $res_file

# 遍历文件
foreach file (`awk '{print}' list_of_runs_primex3/list_of_runs_he3`)
    set my_file = `echo $file | sed -e s/run_/Run0/g`
    set n = 0
    set n1 = 0
    set evt_total = 0
    echo $my_file

    foreach file1 (`awk '{print}' lumi_primex3/individual/list_Run$my_file`)
        set my_file_log = `echo $file1 | sed -e s/ps.//g -e s/hd_rawdata/ps/g -e s/evio/log/g`
        set log_path='/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run'${my_file}'/'$my_file_log''
        # echo $log_path

        @ n = $n + 1

        # 检查是否存在错误 'words_left_in_file'
        set bad_evt=`less $log_path | grep -i 'words_left_in_file' $log_path`
        if ( "$bad_evt" != "") then
            echo "\e[1;34m '$log_path' \e[0m"
            # 记录错误到 res.txt
            echo "Error: 'words_left_in_file' found in $log_path" >> $res_file
            set error_flag = 1
        endif

        # 获取事件数量
        set evt=`less $log_path | grep Nevents | sed -e s/Nevents://g`
        # echo $evt

        if ( "$evt" == "") then
            echo "\e[1;31m No events in file '$log_path' \e[0m"
            # 记录错误到 res.txt
            echo "Error: No events in file $log_path" >> $res_file
            set error_flag = 1
        else
            set aaa = `echo "${evt_total} + ${evt}" | bc`
            @ evt_total = $aaa + 0
            @ n1 = $n1 + 1
        endif
    end

    echo " Run Files Good Events " $my_file $n $n1 $evt_total
    echo " "
end

# 如果没有错误，删除 res.txt 文件
if ( $error_flag == 0 ) then
    rm -f $res_file
endif
