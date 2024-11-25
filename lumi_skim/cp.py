import os
import shutil
from pprint import pprint
import re

# Source 1
rootDir1 = "/volatile/halld/home/somov/ver05_lumi/"
logDir1  = "/w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual"
resDir1  = "/w/halld-scshelf2101/home/zhikun/lumi_skim/res.txt"

# source 2
rootDir2 = "/volatile/halld/home/zhikun/ver05_lumi/"
logDir2  = "/w/halld-scshelf2101/halld3/home/somov/lumi_skim/lumi_primex3/individual"
resDir2  = "/w/halld-scshelf2101/home/zhikun/lumi_skim/res_new.txt"


# Destination
destination_root_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/"
destination_log_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/log/"
destination_res_dir = ""

# 文件路径定义
list_file_path = "/w/halld-scshelf2101/home/zhikun/lumi_skim/list_of_runs_primex3/list_of_runs_he3"
individual_dir = "/w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual"
log_base_dir = "/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/"
res_file = "res.txt"

def copy_all_log():
    with open(list_file_path, "r") as list_file:
            for run_id in list_file:
                run_id = run_id.strip()
                if not run_id:
                    continue

                # 构建源目录
                run_dir = f"Run{run_id}"
                source_run_dir = os.path.join(logDir1, run_dir)

                # 构建目标目录
                destination_run_dir = os.path.join(destination_log_dir, run_dir)

                print(f"Processing directory: {source_run_dir}")

                # 检查源目录是否存在
                if not os.path.exists(source_run_dir):
                    print(f"Source directory {source_run_dir} does not exist.")
                    continue
                
                # 复制整个目录
                shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
                print(f"Copied {source_run_dir} to {destination_run_dir}")



def copy_all_root():
    with open(list_file_path, "r") as list_file:
            for run_id in list_file:
                run_id = run_id.strip()
                if not run_id:
                    continue

                # 构建源目录
                run_dir = f"Run{run_id}"
                source_run_dir = os.path.join(rootDir1, run_dir)

                # 构建目标目录
                destination_run_dir = os.path.join(destination_root_dir, run_dir)

                print(f"Processing directory: {source_run_dir}")

                # 检查源目录是否存在
                if not os.path.exists(source_run_dir):
                    print(f"Source directory {source_run_dir} does not exist.")
                    continue
                
                # 复制整个目录
                shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
                print(f"Copied {source_run_dir} to {destination_run_dir}")




def copy_all():
    try:
       copy_all_log()
       copy_all_root()

    except Exception as e:
        print(f"An error occurred: {e}")
        exit(1)

def read_res(resfile):
    res = []
    with open(resDir1) as results_file:
        temp_res = results_file.readlines()
    for line in temp_res:
        line = line.strip()
        if line:
            res.append(line)
    return res
             
def format_all_error():
    result = {}
    # 定义两种匹配模式
    pattern1 = re.compile(r"Error: The file (.+(\d{6})) is empty.")
    pattern2 = re.compile(r"Error: (.+) (/w.+?(\d{6})_(\d{3}).log)")
   # 尝试匹配第一种格式
    match1 = pattern1.search(message)
    match2 = pattern2.search(message)
    if match1:
        first_number = match1.group(2)  # 第一个数字（六位）
        result[(first_number, '*')] = 'empty_file'  # 错误类型为 'empty_file'
        return result

    # 尝试匹配第二种格式

    elif match2:
        error_type = match2.group(1)  # 捕获的错误类型
        first_number = match2.group(3)  # 第一个数字（六位）
        second_number = match2.group(4)  # 第二个数字（三位，只有通过动态匹配捕获时才存在）
        # 将匹配到的结果添加到字典，根据错误类型定义不同的值
        result[(first_number, second_number)] = error_type.strip()  # 动态错误类型
        return result

    else :
    # 若两种格式都不匹配，抛出异常并退出
        raise ValueError(f"No matching error pattern found in:\n {message}\n")

# 收集所有匹配的结果
all_results = {}

# 处理每个错误信息
for message in error_messages:
    try:
        result = match_error(message)
        all_results.update(result)  # 更新结果字典
    except ValueError as e:
        print(f"\nError: {e}")
        sys.exit(1)  # 退出程序

# 打印所有匹配的结果
print("Matched Results:", all_results)




def check_all():
    try:
        res_1 = read_res(resDir1)
        res_2 = read_res(resDir2)
        pprint(res_1)
        pprint(res_2)
        pattern = re.compile(r"Error: .*?(/w[a-zA-Z0-9/_]+/ps_(\d{6})_(\d{3}).log)")

    except Exception as e:
        print(f"{e}")


# copy_all()
check_all()