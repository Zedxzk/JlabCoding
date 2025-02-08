#common import
from pprint import pprint
import sys
import argparse
import subprocess
import importlib.util
import os
import re
sys.path.append(os.path.dirname((os.path.abspath(__file__))))
from import_parameters import *
from colorama import Fore, Back, Style # type: ignore
reset = Style.RESET_ALL
green = Fore.GREEN
yellow = Fore.YELLOW 
red = Fore.RED   
# common import
from datetime import datetime
import re

linesToWrite =[]

__run_hv_aimADCSettings_dict__ = {}
__run_hv_alphaSettings_dict__ = {}
__run_hv_hvCapSettings_dict__ = {}
__run_hv_hvBotSettings_dict__ = {}

nums_proceeded = 0

def __get_item_numbers__(line):
    match = re.search(r"hv:(-?\d+):(-?\d+)", line)
    if match:
        return int(match.group(1)), int(match.group(2))
    exit(line)


def __findTemplate__():
    if hvTemplateFile in os.listdir(os.getcwd()):
        pass
    else:
        print(f"Can't find hv Template : {hvTemplateFile}, please check `parameters.py` or {hvTemplateFile}")
        exit(1)

def __calculateVoltage__(currentVoltage, currentADC, aimADC=aimADC, hvCap=hvCap, hvBot=hvBot, alpha=alpha):
    if currentADC <= 0.5:
        return False
    __newVoltage__ = currentVoltage * (aimADC/currentADC) ** (1. / alpha)
    __newVoltage__ = __newVoltage__ if __newVoltage__ <= hvCap else hvCap
    __newVoltage__ = __newVoltage__ if __newVoltage__ >= hvBot else hvBot 
    # print(currentVoltage, currentADC, new
    # print(currentVoltage, currentADC, newVoltage)
    return __newVoltage__


def readResTable(fileIn):
    lines = fileIn.readlines()   

    # 找到表头并提取列索引
    headers = lines[0].strip().split()
    headers = headers[1:] if headers[0] == "//" else headers
    index_col = headers.index('index')
    col_col = headers.index('column')
    row_col = headers.index('row')
    mean_col = headers.index('mean')

    # 提取数据（跳过表头）
    data = []
    for line in lines[1:]:
        # 跳过空行或者注释行（假设以'//'开头的是注释）
        if line.startswith('//') or not line.strip():
            continue
        values = line.strip().split()
        
        # 获取相应的列值
        index = values[index_col]
        column = values[col_col]
        row = values[row_col]
        mean = values[mean_col]
        
        # print(index_col,col_col, row_col, mean_col)
        
        # 将读取的数据存入数据列表
        data.append({
            'index' : int(index),
            'column': int(column),
            'row'   : int(row   ),
            'mean'  : float(mean  ) 
        })
    data = {int(entry['index']): entry for entry in data}
    # input()
    return data

def __readAllSettingFile__():
    global __hv_config_comparison__
    try:
        if(aimADCSettings != None):
            with open(aimADCSettings, "r") as f:
                lines = f.readlines()
                for line in lines[1:]:
                    values = line.strip().split()
                    if values[0] == '#' or values[0] == '//':
                        continue
                    index = int(values[0])
                    column = int(values[1])
                    row = int(values[2])
                    value = float(values[3])
                    __run_hv_aimADCSettings_dict__[index] = {'index': index, 'column': column, 'row' : row, 'value': value}
        if(alphaSettings != None):
            with open(alphaSettings, "r") as f:
                lines = f.readlines()
                for line in lines[1:]:
                    values = line.strip().split()
                    if values[0] == '#' or values[0] == '//':
                        continue
                    index = int(values[0])
                    column = int(values[1])
                    row = int(values[2])
                    value = float(values[3])
                    __run_hv_alphaSettings_dict__[index] = {'index': index, 'column': column, 'row' : row, 'value': value}
        if(hvCapSettings != None):
            with open(hvCapSettings, "r") as f:
                lines = f.readlines()
                for line in lines[1:]:
                    values = line.strip().split()
                    if values[0] == '#' or values[0] == '//':
                        continue
                    index = int(values[0])
                    column = int(values[1])
                    row = int(values[2])
                    value = float(values[3])
                    __run_hv_hvCapSettings_dict__[index] = {'index': index, 'column': column, 'row' : row, 'value': value}
        if(hvBotSettings != None):  
            with open(hvBotSettings, "r") as f:
                lines = f.readlines()
                for line in lines[1:]:
                    values = line.strip().split()
                    if values[0] == '#' or values[0] == '//':
                        continue
                    index = int(values[0])
                    column = int(values[1])
                    row = int(values[2])
                    value = float(values[3])
                    __run_hv_hvBotSettings_dict__[index] = {'index': index, 'column': column, 'row' : row, 'value': value}
    except Exception as e:
        print(yellow + f"Error: {e}")
        print("Operation cancelled." + reset)
        exit(1)

 

#ECAL:hv:-10:-10:i0set 1 5.000000000000000e+02
def processAimConfig(line, dataDict):
    global nums_proceeded
    __readAllSettingFile__()
    # 使用正则表达式匹配：ECAL:hv:<数字>:<数字>:<itemToConfig> <科学计数法的数字>
    pattern = re.compile(rf"ECAL:hv:(-?\d+):(-?\d+):({itemToConfig})([+-]?\d*(\.\d+)?([eE][+-]?\d+)?)")
    # 查找匹配
    match = pattern.search(line)

    if match:
        # 如果匹配成功，提取匹配的内容
        col = int(match.group(1))  # 第一个数字 (-10)
        row = int(match.group(2))  # 第二个数字 (-10)
        config = match.group(3)  # 字符串 (i0set)
        currentVoltage = float(match.group(4))  # 科学计数法数字部分 (例如 5.000000000000000e+02)
        col_2 = col + 20 if col < 0 else col + 19
        row_2 = row + 20 if row < 0 else row + 19
        index = 40 * (39 - col_2) + (39 - row_2) 
        # print(f"Match Success: hv1={col}, hv2={row}, index={index}, config={config}, scientific_number={currentVoltage}")
        if index in dataDict:
            nums_proceeded += 1
            if nums_proceeded % 100 == 0:
                print(green + f"Proceeded {nums_proceeded} items" +reset)
            if nums_proceeded == 1596:
                print(green + f"Proceeded {nums_proceeded} items" +reset)
            # 获取对应的配置数据
            current_config = dataDict[index]
            # print(f"Found Config for index {index}: {current_config}")
            # 假设 column 和 row 是你要对比的目标值
            target_column = dataDict[index]['column']
            target_row = dataDict[index]['row']
            currentADC = dataDict[index]['mean']
            channel_index = dataDict[index]['index']
            # 对比 col_2 和 row_2 是否与 target_column 和 target_row 匹配
            if col_2 == target_column and row_2 == target_row:
                # print(index)
                # print(f"成果比对: 在 index {index} 下，col_2={col_2} 和 row_2={row_2} 与目标值 column={target_column} 和 row={target_row} 匹配。")
                # print(len(__run_hv_aimADCSettings_dict__))
                # input()
                __run_hv_aimADC__ = __run_hv_aimADCSettings_dict__[index]['value'] if aimADCSettings != None else aimADC
                __run_hv_alpha__  = __run_hv_alphaSettings_dict__[index]['value'] if alphaSettings != None else alpha
                __run_hv_hvCap__  = __run_hv_hvCapSettings_dict__[index]['value'] if hvCapSettings != None else hvCap
                __run_hv_hvBot__  = __run_hv_hvBotSettings_dict__[index]['value'] if hvBotSettings != None else hvBot
                newHV = __calculateVoltage__(currentVoltage, currentADC,aimADC= __run_hv_aimADC__, hvCap=__run_hv_hvCap__, hvBot=__run_hv_hvBot__, alpha=__run_hv_alpha__)
                # print(f"(column, row) = ({col_2}, {row_2})")
                if(newHV == False):
                    print(f"(column, row) = ({col_2}, {row_2}) or index = {index}")
                    print(f"Illigal fitting result of mean! Return current voltage setting of this channel. Current ADC = {currentADC}")
                    input("Please check manually! Type Enter to continue")
                    newHV = currentVoltage if currentVoltage <= __run_hv_hvCap__ else currentVoltage if currentVoltage > __run_hv_hvBot__ else __run_hv_hvBot__
                __hv_config_comparison__.write(f'{col_2:02d}      {row_2:02d}     {currentVoltage:>8.3f}         {newHV:>8.3f}         {("+" if newHV - currentVoltage >= 0 else "-")}{abs(newHV - currentVoltage):7.3f}\n')

                return f"ECAL:hv:{col}:{row}:{itemToConfig}{newHV:.3f}\n"
            else:
                # print(red + f"成果比对失败: 在 index {index} 下，col={col} 和 row={row} ,col_2={col_2} 和 row_2={row_2} 不匹配目标值 column={target_column} 和 row={target_row}。")
                print(f"Index does not match! Check the index in {fitResDir} and index in {hvTemplateFile}" + Style.RESET_ALL)
                exit(1)                

        else:
            print(yellow + f"Warnning!: No config found for index {index}!" +Style.RESET_ALL)
            try:
                return line
            except Exception as e:
                exit(f"{e}")


    else:
        print(red + f"Error Happend!!! Please Check the Line:\n {line}" +Style.RESET_ALL)
        exit(1)



def processConfiguration():
    current_datetime = datetime.now()
    formatted_datetime = current_datetime.strftime("%d_%b_%Y_%H_%M_%S")
    fileOut = "ECAL_HV_" + formatted_datetime + ".snap"

    

    
    # 打开输入文件并读取内容
    with open(hvTemplateFile, 'r') as fileIn:
        lines = fileIn.readlines()  # 读取所有行

    with open(fitResDir, 'r') as fileIn:
        currentRes = readResTable(fileIn)  # 读取所有行
    # for key, value in currentRes.items():
    #     print(key,value)
        
    # input()

    # 打开输出文件进行写入
    with open(fileOut, 'w') as fileOut:

        # 遍历每一行并处理
        for line in lines:
            if not (f"{itemToConfig}" in line):  # 判断行是否包含 itemToConfig
                # 对包含匹配项的行进行特殊处理（例如修改格式或增加内容）
                fileOut.write(line)
            else:
                lineToWrite = processAimConfig(line, currentRes)
                # print(lineToWrite)
                linesToWrite.append(lineToWrite)
                # fileOut.write(lineToWrite)
                
                # 不匹配的行直接写入输出文件
  
        sorted_lines = sorted(linesToWrite, key=__get_item_numbers__)
        for line in sorted_lines:
            fileOut.write(line)
    print(f"All res Successfully proceeded!New Configuration File: {fileOut}")



def hvConfigureAction():
    # try:
    global __hv_config_comparison__
    __hv_config_compare_file_name__ = "HV_Comparison.txt"
    with  open(__hv_config_compare_file_name__, "w") as __hv_config_comparison__:
        __hv_config_comparison__.write("column  row  current_HV_Setting  new_HV_Setting  Voltage_change\n")
        processConfiguration()
    # except Exception as e:
    #     print(red + f"Error Occurred! \n{e}" + Style.RESET_ALL)
    #     exit(1)
