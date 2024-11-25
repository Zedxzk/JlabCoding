import re
import sys

# 错误信息示例
error_messages = [
    "Error: The file /w/halld-scshelf2101/home/zhikun/lumi_skim/lumi_primex3/individual/list_Run110754 is empty.",
    "Error: No events in file /work/halld/home/zhikun/lumi_skim/lumi_primex3/individual/log/Run110755/ps_110755_015.log",
    "Error: Unable to open file /path/to/some/file.ps_110756_001.log",
    "Error: File processing failed for /work/halld/home/zhikun/logs/ps_110757_002.log"
]

# 定义两种匹配模式
pattern1 = re.compile(r"(Error: The file (.+?(\d{6})) is empty.)")
# 改进的 pattern2，能够捕获描述中的空格
pattern2 = re.compile(r"(Error: (.+?) (/.+?(\d{6})_(\d{3}).log))")

def match_error(message):
    """从给定错误信息中匹配内容并返回字典"""
    result = {}

    # 尝试匹配第一种格式
    match1 = pattern1.search(message)
    if match1:
        first_number = match1.group(2)  # 第一个数字（六位）
        result[(first_number, '*')] = match1.group(1)  # 错误的完整字符串
        return result

    # 尝试匹配第二种格式
    match2 = pattern2.search(message)
    if match2:
        error_description = match2.group(2).strip()  # 捕获的错误描述（包括空格）
        first_number = match2.group(4)  # 第一个数字（六位）
        second_number = match2.group(5)  # 第二个数字（三位）
        # 将匹配到的结果添加到字典，值为完整的错误信息
        result[(first_number, second_number)] = match2.group(1)  # 错误的完整字符串
        return result

    # 若两种格式都不匹配，抛出异常
    raise ValueError(f"No matching error pattern found in:\n {message}\n")

def process_error_messages(messages):
    """处理所有错误信息并返回结果字典"""
    all_results = {}

    # 处理每个错误信息
    for message in messages:
        try:
            result = match_error(message)
            all_results.update(result)  # 更新结果字典
        except ValueError as e:
            print(f"\nError: {e}")
            sys.exit(1)  # 退出程序

    return all_results

# 调用处理函数并打印结果
if __name__ == "__main__":
    matched_results = process_error_messages(error_messages)
    print("Matched Results:", matched_results)