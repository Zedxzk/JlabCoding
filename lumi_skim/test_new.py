import os

# 文件路径定义
list_file_path = "list_of_runs_primex3/list_of_runs_he3"
individual_dir = "lumi_primex3/individual"
log_base_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual/log/"
res_file = "res_new.txt"

# 初始化标志变量
has_error = False

# 清空或创建 res.txt 文件
open(res_file, "w").close()

def print_error(message, color_code):
    """打印带颜色的错误消息"""
    print(f"{color_code}{message}\033[0m")

def parse_log_file(log_path):
    """解析日志文件，返回错误信息和事件数"""
    try:
        with open(log_path, "r", errors="ignore") as log_file:
            log_content = log_file.read()

        errors = []
        event_count = 0

        # 检查 "words_left_in_file" 错误
        if "words_left_in_file" in log_content:
            errors.append(("blue", f"Error: 'words_left_in_file' found in {log_path}"))
        
        # 检查 "There was a crash."
        if "There was a crash." in log_content:
            errors.append(("red", f"Error: 'There was a crash.' found in {log_path}"))

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
        return [("red", f"Error: Log file {log_path} not found")], 0
    except Exception as e:
        return [("red", f"Error reading log file {log_path}: {e}")], 0

# 定义错误严重度的顺序
severity_order = {
    "red": 1,
    "yellow": 2,
    "blue": 3
}

# 错误字典
all_errors = []

# 遍历文件列表
with open(list_file_path, "r") as list_file:
    for file in list_file:
        file = file.strip()
        if not file:
            continue

        my_file = file.replace("run_", "Run0")

        total_files = 0
        good_files = 0
        evt_total = 0

        # 读取对应的 individual 文件列表
        individual_list_path = os.path.join(individual_dir, f"list_Run{my_file}")
        if not os.path.exists(individual_list_path):
            error_message = f"Error: {individual_list_path} does not exist."
            print_error(error_message, "\033[1;31m")
            all_errors.append(("red", error_message))
            has_error = True
            continue

        with open(individual_list_path, "r") as individual_list:
            for file1 in individual_list:
                file1 = file1.strip()
                if not file1:
                    continue

                my_file_log = file1.replace("ps.", "").replace("hd_rawdata", "ps").replace("evio", "log")
                log_path = os.path.join(log_base_dir, f"Run{my_file}", my_file_log)

                total_files += 1

                # 检查日志文件
                errors, event_count = parse_log_file(log_path)

                if errors:
                    has_error = True
                    for color, error in errors:
                        all_errors.append((color, error))
                        print_error(error, "\033[1;31m" if color == "red" else "\033[1;33m" if color == "yellow" else "\033[1;34m")
                else:
                    evt_total += event_count
                    good_files += 1

# 如果有错误，则对错误进行排序并写入文件
if has_error:
    # 根据严重程度对所有错误进行排序
    # sorted_errors = sorted(all_errors, key=lambda error: severity_order[error[0]])
    sorted_errors = all_errors
    # 将排序后的错误写入 res.txt
    with open(res_file, "w") as res:
        for color, error in sorted_errors:
            res.write(error + "\n")

# 如果没有错误，删除 res.txt 文件
if not has_error:
    os.remove(res_file)
