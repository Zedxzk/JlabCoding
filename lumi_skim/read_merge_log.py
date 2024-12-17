import openpyxl  # type: ignore
import re
from collections import defaultdict

log_file = "merge_log_he10_real.txt"
column_to_write = 7



# 打开 Excel 文件
workbook = openpyxl.load_workbook("template.xlsx")
worksheet = workbook['Sheet1']

# 使用 defaultdict 来确保键不存在时会初始化为 0
run_id_count = defaultdict(int)

# 读取并分析日志文件
with open(log_file, "r") as file:
    for line in file.readlines():
        if "hadd" in line:
            continue
        if "ps" in line and ".root" in line and len(line) <= 20:
            # 使用正则表达式按 '_' 和 '.' 分割字符串
            parts = re.split(r'[_\.]', line.strip())  # 按 "_" 和 "." 分割
            run_id = parts[1]  # 获取 ps_ 后面的第一个数字，作为 run_id

            # 更新字典中的 run_id 计数
            run_id_count[run_id] += 1

# 输出字典内容（每个 run_id 对应的文件数量）
for run_id, count in run_id_count.items():
    print(f"Run ID: {run_id}, Number of files: {count}")

# 将统计结果写入 Excel 文件的第 8 列
for i in range(1, worksheet.max_row + 1):
    run_id = worksheet.cell(i, 1).value  # 获取第 i 行的 run_id（假设它在第一列）
    
    # 只处理非空的 run_id
    if run_id in run_id_count:
        worksheet.cell(i, column_to_write, value=f"{run_id_count[run_id]}")  # 将计数结果写入第 8 列
    if str(run_id) in run_id_count:
        run_id = str(run_id)
        worksheet.cell(i, column_to_write, value=f"{run_id_count[run_id]}")  # 将计数结果写入第 8 列

# 保存修改后的 Excel 文件
workbook.save("template_modified.xlsx")
