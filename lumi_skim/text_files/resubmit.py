import re
import os
import subprocess
from pprint import pprint

num_not_accessible = 0
script_dir = "/work/halld3/home/somov/lumi_skim/lumi_primex3/individual/scripts/Run{run_id}"
script_name_template = "run_ps_skims_ps_{run_id}_{file_id}.evio.csh"

# 打开并读取文件
with open("/work/halld/home/zhikun/lumi_skim/files_need_to_be_reprocessed_after_resubmission.txt", "r", encoding="utf-8") as f:
    content = f.read()

# 正则表达式提取数据
pattern = r"(\d+)\n\[(.*?)\]"
matches = re.findall(pattern, content)

# 解析为结构化数据
results = []
for match in matches:
    number = match[0]
    list_values = match[1].split(", ")
    list_values = [value.strip("'") for value in list_values]  # 去掉单引号
    results.append((number, list_values))
pprint(results)
# results = [ ('112000', ['056'])]
# results = []

# 输出结果
for result in results:
    for file_id in result[1]:
        script_name = script_name_template
        script_path = os.path.join(script_dir, script_name).format(run_id=result[0], file_id=file_id)
        command = ""



        # print(command)

        if not os.access(script_path, os.X_OK):
            num_not_accessible += 1
            print(f"script not accessible, skip: {script_path}")
            continue
print(f"Total scripts not accessible = {num_not_accessible}\n")
        # # 执行脚本
        # try:
        #     process = subprocess.run(
        #         [script_path],
        #         check=True,  # 如果命令失败，抛出异常
        #         stdout=subprocess.PIPE,  # 捕获输出
        #         stderr=subprocess.PIPE,  # 捕获错误信息
        #         text=True  # 输出为字符串而不是字节
        #     )
        #     print(f"成功运行脚本: {script_path}")
        #     print(f"标准输出: {process.stdout}")
        # except subprocess.CalledProcessError as e:
        #     print(f"运行脚本失败: {script_path}")
        #     print(f"错误输出: {e.stderr}")
