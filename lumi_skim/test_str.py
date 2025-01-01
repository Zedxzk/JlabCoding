import re

# 定义要匹配的文本
text = "110737 ['000', '002', '123', '456']"

# 定义正则表达式模式
# 匹配六位数字和方括号内被单引号包围的三位数字
pattern = r"(\d{6})\s+\[('(\d{3})'(?:,\s*'(\d{3})')*)\]"

# 查找匹配项
matches = re.findall(pattern, text)

# 打印匹配结果
if matches:
    print(f"匹配到的六位数字：{matches[0][0]}")
    print("匹配到的三位数字：")
    print(matches[0][3])
    # matches[0][1] 是整个方括号内的内容，我们需要进一步处理以提取单独的数字字符串
    numbers = matches[0][1].split(",")  # 分割字符串
    for num in numbers:
        if num:
            print(num.strip().strip("'"))
else:
    print("未找到匹配项")