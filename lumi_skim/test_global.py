key = "initial_key"
dict = {}

def add_bad_run(key_var ,value, dict):
    global key
    key = str(key_var)  # 修改全局变量 key
    value = str(value)
    if key in dict:
        dict[key].append(value)
    else:
        dict[key] = [value]

add_bad_run("test", "test", dict)


print(key)  # 输出新的 key 值
print(dict)  # dict 被修改了，输出 { 'initial_key': ['some_value'] }
