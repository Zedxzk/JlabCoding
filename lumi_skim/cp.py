import os
import re
import sys
import logging
import glob
import subprocess


he_version = "he7"

extraInfo = "zhikun_reverse"
# Paths Setup
# rootDir1 = "/volatile/halld/home/zhikun/ver05_lumi/"
# logDir1 = "/w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual/log"
# resDir1 = f"/w/halld-scshelf2101/home/zhikun/lumi_skim/res_old_{he_version}_test.txt"

# rootDir2 = "/volatile/halld/home/somov/ver05_lumi/"
# logDir2 = "/w/halld-scshelf2101/halld3/home/somov/lumi_skim/lumi_primex3/individual/log"
# resDir2 = f"/w/halld-scshelf2101/home/zhikun/lumi_skim/res_new_{he_version}_test.txt"

rootDir2 = "/volatile/halld/home/zhikun/ver05_lumi/"
logDir2 = "/w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual/log"
resDir2 = f"/w/halld-scshelf2101/home/zhikun/lumi_skim/res_old_{he_version}_test.txt"

rootDir1 = "/volatile/halld/home/somov/ver05_lumi/"
logDir1 = "/w/halld-scshelf2101/halld3/home/somov/lumi_skim/lumi_primex3/individual/log"
resDir1 = f"/w/halld-scshelf2101/home/zhikun/lumi_skim/res_new_{he_version}_test.txt"



destination_root_dir = "/volatile/halld/home/test_lumi/"
destination_log_dir = "/volatile/halld/home/test_lumi/log/"

destination_res_dir = f"/volatile/halld/home/test_lumi/copy_and_replace_res_{he_version}_test_{extraInfo}.txt"
logfile_path = f"/volatile/halld/home/test_lumi/loginfo_{he_version}_test_{extraInfo}.log"
manual_check_path = f"/volatile/halld/home/test_lumi/manual_check_{he_version}_{extraInfo}.log"

list_file_path = f"/work/halld/home/zhikun/lumi_skim/list_of_runs_primex3/list_of_runs_{he_version}"

# make copy_files if you want to debug, look at log files and don't need to copy files.
copy_files = True

def main():
    # 执行所有操作
    # copy_all()  # Uncomment to run copy operations
    errors_1, errors_2 = check_all()
    replace_all(errors_1, errors_2)
    # 保存快速浏览日志到目标文件
    save_log(destination_res_dir)
    save_manual_check_log(manual_check_path)





# stars = "****************************"

# Logger configuration
logging.basicConfig(filename=logfile_path,
                    level=logging.INFO,
                    format='%(asctime)s - %(levelname)s - %(message)s')

# 用于收集消息的列表
quick_view_messages = []
manual_check_messages = []
events_manual_check = 0

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

def save_manual_check_log(message):
    """将需要人工检查的消息保存到 manual_check.log 文件"""
    with open(manual_check_path, 'w') as f:  # 以追加方式打开文件
        f.write(f"Total files to be checked mannually =  {events_manual_check}")
        for message in manual_check_messages:
            message = message.strip()
            message = f"\n\n{message}" if message[0] == "*" else message
            message += "\n"
            f.write(message)
        f.write(f"\n\nTotal files to be checked mannually =  {events_manual_check}\n")

def copy_with_subprocess(src, dst, is_dir=False):
    """使用 subprocess 复制文件或目录"""
    if is_dir:
        command = ["rsync", "-r", src + "/", dst + "/"]  # 使用 rsync 复制目录
        # command = ["rsync", "-r","--no-times", "--no-perms", src + "/", dst + "/"]  # 使用 rsync 复制目录
    else:
        command = ["cp", "-p", src, dst]  # 使用 cp 复制文件

    try:
        subprocess.run(command, check=True)
        log(f"Successfully copied from {src} to {dst}")
    except subprocess.CalledProcessError as e:
        log(f"Error occurred while copying: {e}", is_error=True)

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
            if not copy_files:
                copy_with_subprocess(source_file_path, destination_file_path, is_dir=False)
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
            if not copy_files:
                copy_with_subprocess(source_file_path, destination_file_path, is_dir=False)
    else:
        log(f"No source files match the pattern: {source_file_pattern}")

def copy_all_log():
    """复制所有日志文件。"""
    if not os.path.exists(list_file_path):
        log(f"List file {list_file_path} does not exist.", is_error=True)
        return

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
            if not copy_files:
                copy_with_subprocess(source_run_dir, destination_run_dir, is_dir=True)

def copy_all_root():
    """复制所有根文件。"""
    if not os.path.exists(list_file_path):
        log(f"List file {list_file_path} does not exist.", is_error=True)
        return

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
            if not copy_files:
                copy_with_subprocess(source_run_dir, destination_run_dir, is_dir=True)

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
    if not os.path.exists(resfile):
        log(f"Result file {resfile} does not exist.", is_error=True)
        return []

    with open(resfile) as results_file:
        return [line.strip() for line in results_file if line.strip()]

def format_error(message):
    """格式化错误消息。"""
    pattern1 = re.compile(r"(Error: The directory (.+?(\d{6})) is empty.)")
    pattern2 = re.compile(r"(Error: (.+?) (/w.+?/ps_(\d{6})_(\d{3}).log))")

    result = {}
    match1 = pattern1.search(message)
    if match1:
        first_number = match1.group(3)
        result[(first_number, '*')] = match1.group(1).strip()
        return result

    match2 = pattern2.search(message)
    print()
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
        error_1 = process_error_messages(res_1) if res_1 else {}
        error_2 = process_error_messages(res_2) if res_2 else {}
        return error_1, error_2
    except Exception as e:
        log(f"\n Error :\n{e}", is_error=True)
        return {}, {}

def replace_all(errors_1, errors_2):
    """替换所有错误信息。"""
    global events_manual_check
    stars = "*" * 90
    for key, value in errors_1.items():
        if key[1] == "*":
            run_id = key[0]
            run_dir = f"Run{run_id}"
            source_run_dir = os.path.join(logDir2, run_dir)
            msg = '\n'.join([
                stars,
                f"Errors of : {key} ",
                value,
                f"Try to replace with files from {source_run_dir}",
                stars
            ])
            log(msg, is_error=True)  # 此处用红色输出并标记为错误

            if key in errors_2:
                log(f"source_run_dir = {source_run_dir} is also empty, PLEASE CHECK MANUALLY!!!!", is_error=True)
                manual_check_messages.append(f"source_run_dir = {source_run_dir} is also empty, PLEASE CHECK MANUALLY!!!!")
                events_manual_check += 1
            else:
                destination_run_dir = os.path.join(destination_log_dir, run_dir)

                if os.path.exists(source_run_dir):
                    if not copy_files:
                        copy_with_subprocess(source_run_dir, destination_run_dir, is_dir=True)
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)

                source_run_dir = os.path.join(rootDir2, run_dir)
                destination_run_dir = os.path.join(destination_root_dir, run_dir)

                if os.path.exists(source_run_dir):
                    if not copy_files:
                        copy_with_subprocess(source_run_dir, destination_run_dir, is_dir=True)
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)

            matching_errors = {k: v for k, v in errors_2.items() if k[0] == run_id}

            if matching_errors:
                manual_check_messages.append(msg)
                log(f"Found matching errors in {source_run_dir}:", is_error=True)
                for match_key, match_value in matching_errors.items():
                    log(f"{match_key} -> {match_value}", is_error=True)
                    manual_check_messages.append(f"{match_key} -> {match_value}")
                    events_manual_check += 1
                log("PLEASE CHECK MANUALLY!!!", is_error=True)
            else:
                log(f"No matching errors found in {source_run_dir}.")  # 成功消息用绿色输出
            
        else:
            run_id = key[0]  
            run_dir = f"Run{run_id}"  
            source_run_dir = os.path.join(rootDir2, run_dir)
            destination_run_dir = os.path.join(destination_root_dir, run_dir)
            
            if key in errors_2.keys():
                # 如果直接找到 key，则用 key 访问
                msg_key = key

            else:
                # 如果没有找到，尝试找到与 key[0] 相同且第二个元素为 '*' 的键
                matching_key = next(((k, errors_2[k]) for k in errors_2.keys() if k[0] == key[0] and k[1] == '*'), (None, None))
                msg_key = matching_key[0]  # 获取找到的键，如果没有找到则为 None

            if msg_key is not None:
                # 生成错误消息
                if msg_key[1] == "*":
                    msg = '\n'.join([
                        stars,
                        f"Errors of : {key} exist in both directory!! PLEASE CHECK MANUALLY",
                        errors_1.get(key),
                        errors_2.get(msg_key),
                        stars
                    ])
                    log(msg,is_error=True)
                    manual_check_messages.append(msg)
                lowLevelError1 = True if 'words_left_in_file' in errors_1[key] else False
                lowLevelError2 = True if 'words_left_in_file' in errors_2[msg_key] else False
                if lowLevelError2 and not lowLevelError1:
                    msg = '\n'.join([
                        stars,
                        f"Errors of : {key} exist in both directory!! PLEASE CHECK MANUALLY",
                        errors_1[key],
                        errors_2[msg_key],
                        f"copy file form {source_run_dir}",
                        stars
                    ])
                    manual_check_messages.append(msg)

                    if os.path.exists(source_run_dir):
                        copy_one_root(run_id, key[1])
                        copy_one_log(run_id, key[1])
                        log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                    else:
                        log(f"Source directory {source_run_dir} does not exist.", is_error=True)  # 此处用红色输出，并标记为错误
                    events_manual_check += 1
                    log(msg, is_error=True)  # 此处用红色输出，并标记为错误
                else:
                    msg = '\n'.join([
                        stars,
                        f"Errors of : {key} exist in both directory!! PLEASE CHECK MANUALLY",
                        errors_1[key],
                        errors_2[msg_key],
                        stars
                    ])
                    manual_check_messages.append(msg)
                    events_manual_check += 1
                    log(msg, is_error=True)  # 此处用红色输出，并标记为错误
                
            else:
                msg = '\n'.join([
                    stars, 
                    f"Error of {key} only exists in \n{errors_1[key]}",
                    f"Replace with files from {rootDir2} and {logDir2}",
                    stars
                ])
                log(msg, is_error=True)  # 此处用红色输出，并标记为错误

                if os.path.exists(source_run_dir):
                    copy_one_root(run_id, key[1])
                    copy_one_log(run_id, key[1])
                    log(f"Successfully copied from {source_run_dir} to {destination_run_dir}")  # 成功消息用绿色输出
                else:
                    log(f"Source directory {source_run_dir} does not exist.", is_error=True)  # 此处用红色输出，并标记为错误
    return errors_1



main()
