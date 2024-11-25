import os
import shutil
import re
import sys
import logging
from pprint import pprint
import glob

# Paths Setup
rootDir1 = "/volatile/halld/home/zhikun/ver05_lumi/"
logDir1 = "/w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual/log"
resDir1 = "/w/halld-scshelf2101/home/zhikun/lumi_skim/res_old_he4.txt"

rootDir2 = "/volatile/halld/home/somov/ver05_lumi/"
logDir2 = "/w/halld-scshelf2101/halld3/home/somov/lumi_skim/lumi_primex3/individual/log"
resDir2 = "/w/halld-scshelf2101/home/zhikun/lumi_skim/res_new_he4.txt"

destination_root_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/"
destination_log_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/log/"
destination_res_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/copy_and_replace_res_he4.txt"
logfile_path = "/lustre24/expphy/volatile/halld/home/test_lumi/loginfo_he4.log"

list_file_path = "/w/halld-scshelf2101/home/zhikun/lumi_skim/list_of_runs_primex3/list_of_runs_he4"

stars = "****************************"

# Logger configuration
logging.basicConfig(filename=logfile_path,
                    level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s')

# 用于收集消息的列表
quick_view_messages = []

def log(msg, is_error=False):
    """将消息添加到快速浏览列表和日志文件中，并根据is_error标志选择颜色输出。"""
    if is_error:
        formatted_msg = "[ERROR] " + msg  # 添加 [ERROR] 标志
        logging.info(formatted_msg)  # 写入日志文件
        print("\033[91m" + formatted_msg + "\033[0m")  # 红色输出
    else:
        formatted_msg = "[SUCCESS] " + msg  # 添加 [SUCCESS] 标志
        logging.info(formatted_msg)  # 写入日志文件
        print("\033[92m" + formatted_msg + "\033[0m")  # 绿色输出

    quick_view_messages.append(formatted_msg)  # 收集快速浏览信息

def save_log(destination):
    """将收集的快速浏览日志保存到指定文件"""
    with open(destination, 'w') as f:
        for message in quick_view_messages:
            f.write(message + '\n\n')

def copy_one_root(run_id, file_id):
    """复制根目录中的单个文件到目标目录。"""
    run_dir = f"Run{run_id}"  # 构建目录名称
    source_run_dir = os.path.join(rootDir2, run_dir)  # 源目录
    destination_run_dir = os.path.join(destination_root_dir, run_dir)  # 目标目录
    
    # 构建源文件路径
    source_file_pattern = os.path.join(source_run_dir, f"*{run_id}_{file_id}.root")

    if not os.path.exists(destination_run_dir):
        os.makedirs(destination_run_dir)

    source_file_paths = glob.glob(source_file_pattern)

    if source_file_paths:
        for source_file_path in source_file_paths:
            file_name = os.path.basename(source_file_path)
            destination_file_path = os.path.join(destination_run_dir, file_name)

            # log(f"Copying from {source_file_path} to {destination_file_path}")

            shutil.copy2(source_file_path, destination_file_path)
            log(f"Successfully copied {source_file_path} to {destination_file_path}")
    else:
        log(f"No source files match the pattern: {source_file_pattern}")

def copy_one_log(run_id, file_id):
    """复制日志目录中的单个文件到目标目录。"""
    file_type = ".log"
    run_dir = f"Run{run_id}"
    source_run_dir = os.path.join(logDir2, run_dir)
    destination_run_dir = os.path.join(destination_log_dir, run_dir)
    
    source_file_pattern = os.path.join(source_run_dir, f"*{run_id}_{file_id}" + file_type)

    if not os.path.exists(destination_run_dir):
        os.makedirs(destination_run_dir)

    source_file_paths = glob.glob(source_file_pattern)

    if source_file_paths:
        for source_file_path in source_file_paths:
            file_name = os.path.basename(source_file_path)
            destination_file_path = os.path.join(destination_run_dir, file_name)

            # log(f"Copying from {source_file_path} to {destination_file_path}")

            shutil.copy2(source_file_path, destination_file_path)
            log(f"Successfully copied {source_file_path} to {destination_file_path}")
    else:
        log(f"No source files match the pattern: {source_file_pattern}")

def copy_all_log():
    """复制所有日志文件。"""
    with open(list_file_path, "r") as list_file:
        for run_id in list_file:
            run_id = run_id.strip()
            if not run_id:
                continue

            run_dir = f"Run{run_id}"
            source_run_dir = os.path.join(logDir1, run_dir)
            destination_run_dir = os.path.join(destination_log_dir, run_dir)

            log(f"Processing directory: {source_run_dir}")

            if not os.path.exists(source_run_dir):
                log(f"Source directory {source_run_dir} does not exist.")
                continue
            
            shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
            log(f"Copied {source_run_dir} to {destination_run_dir}")

def copy_all_root():
    """复制所有根文件。"""
    with open(list_file_path, "r") as list_file:
        for run_id in list_file:
            run_id = run_id.strip()
            if not run_id:
                continue

            run_dir = f"Run{run_id}"
            source_run_dir = os.path.join(rootDir1, run_dir)
            destination_run_dir = os.path.join(destination_root_dir, run_dir)

            log(f"Processing directory: {source_run_dir}")

            if not os.path.exists(source_run_dir):
                log(f"Source directory {source_run_dir} does not exist.")
                continue
            
            shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
            log(f"Copied {source_run_dir} to {destination_run_dir}")

def copy_all():
    """复制所有文件和目录。"""
    try:
        copy_all_log()
        copy_all_root()
    except Exception as e:
        log(f"An error occurred: {e}")
        exit(1)

def read_res(resfile):
    """读取结果文件。"""
    with open(resfile) as results_file:
        return [line.strip() for line in results_file if line.strip()]

def format_error(message):
    """格式化错误消息。"""
    pattern1 = re.compile(r"(Error: The file (.+?(\d{6})) is empty.)")
    pattern2 = re.compile(r"(Error: (.+?) (/w.+?/ps_(\d{6})_(\d{3}).log))")

    result = {}
    match1 = pattern1.search(message)
    if match1:
        first_number = match1.group(3)
        result[(first_number, '*')] = match1.group(1)
        return result

    match2 = pattern2.search(message)
    if match2:
        first_number = match2.group(4)
        second_number = match2.group(5)
        result[(first_number, second_number)] = match2.group(1)
        return result

    raise ValueError(f"No matching error pattern found in:\n {message}\n")  

def process_error_messages(messages):
    """处理错误消息。"""
    all_results = {}
    for message in messages:
        try:
            result = format_error(message)
            all_results.update(result)
        except ValueError as e:
            log(f"\nError: {e}")
            sys.exit(1)

    return all_results

def check_all():
    """检查所有结果。"""
    try:
        res_1 = read_res(resDir1)
        res_2 = read_res(resDir2)
        error_1 = process_error_messages(res_1)
        error_2 = process_error_messages(res_2)
        return error_1, error_2
    except Exception as e:
        log(f"\n Error :\n{e}")

def replace_all(errors_1, errors_2):
    """替换所有错误信息。"""
    stars = "*" * 40
    for key, value in errors_1.items():
        if key[1] == "*":
            run_id = key[0]
            run_dir = f"Run{run_id}"
            source_run_dir = os.path.join(logDir2, run_dir)
            msg = '\n'.join([
                stars,
                f"Errors of : {key} ",
                value,
                f"\nTry to replace with files from {source_run_dir}",
                stars
            ])
            log(msg, is_error=True)  # 此处用红色输出并标记为错误

            if key in errors_2:
                log(f"source_run_dir = {source_run_dir} is also empty, PLEASE CHECK MANNUALLY!!!!", is_error=True)
            else:
                destination_run_dir = os.path.join(destination_log_dir, run_dir)

                if os.path.exists(source_run_dir):
                    # log(f"{source_run_dir} not empty, copying to {destination_run_dir}")
                    shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)

                source_run_dir = os.path.join(rootDir2, run_dir)
                destination_run_dir = os.path.join(destination_root_dir, run_dir)

                if os.path.exists(source_run_dir):
                    # log(f"{source_run_dir} not empty, copying to {destination_run_dir}")
                    shutil.copytree(source_run_dir, destination_run_dir, dirs_exist_ok=True)
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)

            matching_errors = {k: v for k, v in errors_2.items() if k[0] == run_id}

            if matching_errors:
                log(f"Found matching errors in {source_run_dir}:", is_error=True)
                for match_key, match_value in matching_errors.items():
                    log(f"{match_key} -> {match_value}", is_error=True)
                log("PLEASE CHECK MANUALLY!!!", is_error=True)
            else:
                log(f"No matching errors found in {source_run_dir}.")  # 成功消息用绿色输出
            
        else:
            if key in errors_2.keys():
                msg = '\n'.join([
                    stars,
                    f"Errors of : {key} exist in both directory!! PLEASE CHECK MANNUALLY",
                    errors_1[key],
                    errors_2[key],
                    stars
                ])
                log(msg, is_error=True)  # 此处用红色输出，并标记为错误
            else:
                run_id = key[0]  
                run_dir = f"Run{run_id}"  
                
                source_run_dir = os.path.join(rootDir2, run_dir)
                destination_run_dir = os.path.join(destination_root_dir, run_dir)

                msg = '\n'.join([
                    stars, 
                    f"Error of {key} only exists in \n{errors_1[key]}",
                    f"Replace with files from {rootDir2} and {logDir2}",
                    stars
                ])
                log(msg, is_error=True)  # 此处用红色输出，并标记为错误

                if os.path.exists(source_run_dir):
                    # log(f"Copying from {source_run_dir} to {destination_run_dir}")
                    copy_one_root(run_id, key[1])
                    copy_one_log(run_id, key[1])
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)  # 此处用红色输出，并标记为错误
    return errors_1

# 执行所有操作
copy_all()  # Uncomment to run copy operations

errors_1, errors_2 = check_all()

replace_all(errors_1, errors_2)

# 保存快速浏览日志到目标文件
save_log(destination_res_dir)
