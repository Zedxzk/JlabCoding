import os

empty_line_printed = False

# he_versions = ["he3", "he4", "he5", "he6", "he7", "he8", "he9", "he10"]
he_versions = ["he2"]
# he_version = "he3"
# extra_info = "reprocessed"
extra_info = "of_he2_merged"

# 文件路径定义
list_of_runs_path_template = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/list_of_runs_{he_version}"
# list_of_evio_files = "/work/halld/home/zhikun/lumi_skim/list_of_files/list_of_files_after_exclusion/"
list_of_evio_files = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/"
log_base_dir = "/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/new_runs_merged_log/"
# log_base_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual/log"
# log_base_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual/log_first"
# log_base_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3_new/individual/log"
res_file_dir = "/work/halld/home/zhikun/lumi_skim/scan_res_dir/"
res_file_name_template = "res_new_{he_version}_{extra_info}.txt"



# list_of_runs_path_template = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/list_of_runs_{he_version}"
# list_of_evio_files = "/work/halld/home/zhikun/lumi_skim/list_of_runs_from_mss/"
# log_base_dir = "/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log"
# # log_base_dir = "/work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/new_runs_merged_log"
# res_file_dir = "/work/halld/home/zhikun/lumi_skim/scan_res_dir/"
# res_file_name_template = "res_old_{he_version}_{extra_info}.txt"

# 初始化标志变量
has_error = False


def print_error(message, color_code):
    """打印带颜色的错误消息"""
    color_code = "\033[1;31m" if color_code == "red" else "\033[1;33m" if color_code == "yellow" else "\033[1;34m"
    print(f"{color_code}{message}\033[0m")

def parse_log_file(log_path):
    """解析日志文件，返回错误信息和事件数"""
    try:
        with open(log_path, "r",errors="ignore") as log_file:
        # with open(log_path, "r") as log_file:
            log_content = log_file.read()

        errors = []
        event_count = 0

        # 检查 "There was a crash."
        if "There was a crash." in log_content:
            errors.append(("red", f"Error: 'There was a crash.' found in {log_path}"))
            return errors, event_count

        # 检查 "words_left_in_file" 错误
        if "words_left_in_file" in log_content:
            errors.append(("blue", f"Error: 'words_left_in_file' found in {log_path}"))
        
        # 提取事件数量
        evt_lines = [line for line in log_content.splitlines() if "Nevents" in line]
        if not evt_lines:
            errors.append(("yellow", f"Error: No events in file {log_path}"))
        else:
            for line in evt_lines:
                try:
                    event_count += int(line.split(":")[1].strip())
                except ValueError:
                    errors.append(("red", f"Error parsing event count in {log_path}"))

        return errors, event_count
    except FileNotFoundError:
        return [("red", f"Error: Log file not found {log_path} ")], 0
    except Exception as e:
        # exit(1)
        return [("red", f"Error reading log file {log_path}: {e}")], 0

# 定义错误严重度的顺序
severity_order = {
    "red": 1,
    "yellow": 2,
    "blue": 3
}

# 错误字典
all_errors = []

for he_version in he_versions:
    part_of_errors = []
    list_file_path = list_of_runs_path_template.format(he_version=he_version)
    res_file_path = os.path.join(res_file_dir, res_file_name_template).format(he_version=he_version, extra_info=extra_info)
    # input(f"he_version =  {he_version}, list_file_path = {list_file_path},\nres_file_path = {res_file_path}")
    with open(list_file_path, "r") as list_file:
        for file in list_file:
            file = file.strip()
            if not file:
                continue

            my_file = file.replace("run_", "Run")
            print(my_file)
            # total_files = 0
            # good_files = 0
            # evt_total = 0

            # 读取对应的 individual 文件列表
            individual_list_path = os.path.join(list_of_evio_files, f"list_Run{my_file}")

            # 检查 individual_list_path 文件是否存在
            if not os.path.exists(individual_list_path):
                error_message = f"Error: {individual_list_path} does not exist."
                print_error(error_message, "\033[1;31m")
                all_errors.append(("red", error_message))
                has_error = True
                continue

            # 如果该文件为空，给出提示并添加错误
            if os.path.getsize(individual_list_path) == 0:
                error_message = f"Error: The directory {individual_list_path} is empty."
                print_error(error_message, "red")
                all_errors.append(("red", error_message))
                has_error = True
                continue

            # 打开文件进行处理
            with open(individual_list_path, "r") as individual_list:
                for file1 in individual_list:
                    file1 = file1.strip()
                    if not file1:
                        continue

                    my_file_log = file1.replace("ps.", "").replace("hd_rawdata", "ps").replace("evio", "log")
                    log_dir = os.path.join(log_base_dir, f"Run{my_file}")
                    if not os.path.exists(log_dir) or  not os.listdir(log_dir) :
                        error_message = f"Error: The directory {log_dir} is empty."
                        print_error(error_message,"red")
                        all_errors.append(("red", error_message))
                        part_of_errors.append(error_message)
                        break
                    log_path = os.path.join(log_base_dir, f"Run{my_file}", my_file_log)

                    # total_files += 1

                    # 检查日志文件
                    errors, event_count = parse_log_file(log_path)

                    # 输出所有错误
                    if errors:
                        has_error = True
                        for color, error in errors:
                            all_errors.append((color, error))
                            print_error(error, color)
                            part_of_errors.append(error)
                    else:
                        # evt_total += event_count
                        # good_files += 1
                        pass

            # 在每个运行结束后输出空行
            print()  # 打印一个空行以分隔不同的运行
            part_of_errors.append("")
            all_errors.append((" ", " "))

        # 如果有错误，则对错误进行排序并写入文件
        if len(part_of_errors) > 0:
            # 根据严重程度对所有错误进行排序
            # sorted_errors = sorted(all_errors, key=lambda error: severity_order[error[0]])
            sorted_errors = part_of_errors  
            # 将排序后的错误写入 res.txt
            with open(res_file_path, "w") as res:
                for error in sorted_errors:
                    if error == " ":
                        if empty_line_printed:
                            pass
                        else:
                            res.write(error + "\n")
                            empty_line_printed = True
                    else:
                        res.write(error + "\n")
                        empty_line_printed = False
                
