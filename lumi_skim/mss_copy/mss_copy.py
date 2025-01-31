import subprocess
import os
import time

list_of_runs_file = "../test.txt"  # 这个文件应包含要复制的运行列表
source_dir = r"/volatile/halld/home/zhikun/merged_new_runs/"  # 源目录
dest_dir = r"/mss/halld/RunPeriod-2022-08/production/all_lumi/"  # 目标目录
pausetime = 10  # 暂停时间（秒）

# 读取运行列表文件
with open(list_of_runs_file, "r") as f:
    runs = f.readlines()

# 遍历每个运行ID并复制相应的文件夹
for run in runs:
    run = run.strip()  # 去除行尾的换行符
    if run:  # 确保行不为空
        source_path = os.path.join(source_dir, f"Run{run}")  # 构建源路径
        dest_path = os.path.join(dest_dir,  f"Run{run}")  # 构建目标路径
        try:
            # 使用 subprocess.call 调用 jmirror 命令复制文件夹
            subprocess.call(["nohup","jmirror", source_path, source_path, dest_path])
            processing_time = 0
            while True:
                if not os.path.exists(dest_path) or os.listdir(dest_path) != os.listdir(source_path):
                    time.sleep(pausetime)
                    processing_time += pausetime
                    print(f"Mirroring {run}, {processing_time} seconds passed")
                else: 
                    break

            print(f"Successfully copied {run} to MSS directory")
        except Exception as e:
            print(f"Error copying {run}: {e}")
        # 可选：暂停一段时间，以避免过于频繁的操作
        time.sleep(pausetime)   
        # 可选：暂停一段时间，以避免过于频繁的操作
