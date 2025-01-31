import os
import time
import subprocess

list_of_runs_file = "test.txt"  # 运行列表文件
source_dir = r"/volatile/halld/home/zhikun/merged_new_runs/"  # 源目录
dest_dir = r"/mss/halld/RunPeriod-2022-08/production/all_lumi_new/"  # 目标目录

pausetime = 5
batch_submission_time = 5 * 60
batch_number = 5

with open(list_of_runs_file, "r") as f:
    runs = [line.strip() for line in f if line.strip()]

def submit_jobs(run):
    source_path = os.path.join(source_dir, f"Run{run}")
    dest_path = os.path.join(dest_dir, f"Run{run}")
    command = f"nohup jmirror {source_path}  {source_path}  {dest_path} &"
    subprocess.Popen(command, shell=True)

def main():
    start_time = time.time()
    time_to_submit_jobs = True
    run_in_total = len(runs)
    submitted_runs = 0
    finished_runs = []
    running_runs = []
    last_submission_time = 0  # 初始化变量
    while True:
        if time_to_submit_jobs:
            for i in range(batch_number):
                if submitted_runs < run_in_total:  # 修改条件判断
                    run = runs[submitted_runs]
                    submit_jobs(run)
                    running_runs.append(run)
                    submitted_runs += 1
                else:
                    pass  # 修改为break，避免无意义的循环
            if submitted_runs % batch_number == 0 and submitted_runs > 0:
                time_to_submit_jobs = False
                last_submission_time = time.time()

        for run in running_runs[:]:  # 使用切片避免在遍历时修改列表
            source_path = os.path.join(source_dir, f"Run{run}")
            dest_path = os.path.join(dest_dir, f"Run{run}")
            # print(os.listdir(source_path))
            # print(os.listdir(dest_path))
            if os.path.exists(dest_path) and set(os.listdir(source_path)) == set(os.listdir(dest_path)):
                running_runs.remove(run)
                finished_runs.append(run)
        # os.system("clear")
        if len(running_runs) == 0 or (not time_to_submit_jobs and time.time() - last_submission_time >= batch_submission_time):
            time_to_submit_jobs = True
        print(f"\rSubmitted runs = {submitted_runs}, running runs = {len(running_runs)}, finished runs = {len(finished_runs)}, running time = {time.time() - start_time:.1f}s   ", end = "", flush=True)
        if submitted_runs == run_in_total and not running_runs:  # 添加结束条件
            print("All runs finished")
            break
        time.sleep(pausetime)

if __name__ == "__main__":
    main()