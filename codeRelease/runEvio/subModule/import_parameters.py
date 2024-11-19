
import os
import importlib.util
import sys

def import_parameters():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    parent_dir = os.path.dirname(script_dir)
    current_dir = os.getcwd()
    parameters_file = os.path.join(current_dir, "parameters.py")
    
    if os.path.exists(parameters_file):
        pass
    else:
        # 如果当前工作目录中不存在parameters.py，则从脚本目录导入
        parameters_file = os.path.join(parent_dir, "parameters.py")
        print(parent_dir)
    spec = importlib.util.spec_from_file_location("parameters", parameters_file)
    parameters = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(parameters)
    
    # 将模块中的变量导入全局命名空间
    globals().update({k: getattr(parameters, k) for k in dir(parameters) if not k.startswith("__")})

import_parameters()
