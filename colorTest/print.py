def log_result(file_path):
    # 定义颜色代码
    RED = "\033[91m"    # 红色
    GREEN = "\033[92m"  # 绿色
    RESET = "\033[0m"   # 重置颜色

    try:
        with open(file_path, 'r') as file:
            in_error_block = False  # 标识是否在错误块内
            error_lines = []         # 用于存储错误行

            for line in file:
                line = line.strip()  # 去除前后空白

                if line.startswith("[ERROR]") and line.endswith("****************************************"):
                    if in_error_block:  # 如果已经在错误块中，输出之前存储的行
                        # 输出收集的行，包括星号
                        print(RED + "\n".join(error_lines) + RESET)
                        error_lines.clear()  # 清空存储
                    in_error_block = True  # 开始新的错误块
                    error_lines.append(line)  # 添加星号行

                elif in_error_block:
                    if line.strip() == "****************************************":
                        # 到达错误块结束，输出最后收集的错误块
                        error_lines.append(line)  # 添加结束星号行
                        print(RED + "\n".join(error_lines) + RESET)  # 红色输出整个错误块
                        error_lines.clear()  # 清空存储
                        in_error_block = False  # 结束错误块
                    else:
                        error_lines.append(line)  # 继续收集错误信息

                elif "SUCCESS" in line:  # 如果包含 "SUCCESS"
                    print(GREEN + line + RESET)  # 绿色输出
                else:
                    print(line)  # 无特殊颜色，正常输出

    except FileNotFoundError:
        print(f"{RED}Error: File '{file_path}' not found.{RESET}")

if __name__ == "__main__":
    destination_res_dir = "/lustre24/expphy/volatile/halld/home/test_lumi/copy_and_replace_res.txt"
    log_result(destination_res_dir)