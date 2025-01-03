import re
import os
import sys

def add_bad_run(key, value, dict):
    key = str(key)
    value = str(value)
    if key in dict.keys():
        dict[key].append(value)
    else:
        dict[key] = [value]  # Use list notation to create a new list with the value


def log(msg, is_error=False):
    if is_error:
        formatted_msg = "[ERROR] " + msg  # 添加 [ERROR] 标志
        # logging.info(formatted_msg)  # 写入日志文件
        print("\033[91m" + formatted_msg + "\033[0m")  # 红色输出
    else:
        formatted_msg = "[SUCCESS] " + msg  # 添加 [SUCCESS] 标志
        # logging.info(formatted_msg)  # 写入日志文件
        print("\033[92m" + formatted_msg + "\033[0m")  # 绿色输出

    # quick_view_messages.append(formatted_msg)  # 收集快速浏览信息


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
    # print()
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
            # log(f"\nError: {e}")
            sys.exit(1)

    return all_results

def check_all(res_path_1, res_path_2):
    """检查所有结果。"""
    try:
        res_1 = read_res(res_path_1)
        res_2 = read_res(res_path_2)
        errors_1 = process_error_messages(res_1) if res_1 else {}
        errors_2 = process_error_messages(res_2) if res_2 else {}
        return errors_1, errors_2
    except Exception as e:
        log(f"\n Error :\n{e}", is_error=True)
        return {}, {}
    

def compare_result(errors_1, errors_2):
    bad_files = {}
    for key, value in errors_1.items():
        run_id = key[0]  
        run_dir = f"Run{run_id}"  
        if key[1] == "*":
            if key in errors_2:
                bad_files[key] = f"{value} \n{errors_2[key]}"
            else:
                matching_errors = {k: f"{value}\n{v}" for k, v in errors_2.items() if k[0] == run_id}
                bad_files.update(matching_errors)
            
        else:
            if key in errors_2.keys():
                # 如果直接找到 key，则用 key 访问
                msg_key = key

            else:
                # 如果没有找到，尝试找到与 key[0] 相同且第二个元素为 '*' 的键
                matching_key = next(((k, errors_2[k]) for k in errors_2.keys() if k[0] == key[0] and k[1] == '*'), (None, None))
                msg_key = matching_key[0]  # 获取找到的键，如果没有找到则为 None

            if msg_key is not None:
                # 生成错误消息
                bad_files[key] = f"{value}\n{errors_2[msg_key]}"
    return bad_files
# res1, res2  = check_all()