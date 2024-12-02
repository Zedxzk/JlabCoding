import os
from pprint import pprint

mss_dir = "/mss/halld/RunPeriod-2022-08/rawdata"
target_file_dir = "list_of_runs_from_mss"
list_perfix = "list_"
mss_perfix = "hd_rawdata_"
perfix = "ps_"


# 检查目标文件夹是否已经存在
if not os.path.exists(target_file_dir):
    os.makedirs(target_file_dir)  # 创建文件夹
    print(f"Directory '{target_file_dir}' created successfully.")
else:
    print(f"Directory '{target_file_dir}' already exists.")
all_runs = os.listdir(mss_dir)
# pprint(all_runs)
with open(os.path.join(target_file_dir, "last_run.data"),"w") as dataSet:
    dataSet.write(f"run_id                last evio file\n")
    for run_id in all_runs:
        run_dir = os.path.join(mss_dir, run_id)
        fileName = list_perfix + run_id
        fileName = os.path.join(target_file_dir, fileName)
        # pprint(fileName)
        evio_files = [i.replace(mss_perfix, perfix) for i in os.listdir(run_dir) if '.evio' in i]
        evio_files = sorted(evio_files)
        with open(fileName,'w') as file:
            dataSet.write(f"{run_id}           {evio_files[-1]} \n")
            for evio_file in evio_files:
                file.write(evio_file+'\n')
                # pprint(run_dir)

    # pprint(evio_files)


