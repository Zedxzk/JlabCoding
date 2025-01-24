#common import
from pprint import pprint
import sys
import argparse
import subprocess
import importlib.util
import os
import re
from generateSettingFiles import __get_variable_name__
sys.path.append(os.path.dirname((os.path.abspath(__file__))))
from import_parameters import *
from colorama import Fore, Back, Style # type: ignore
reset = Style.RESET_ALL
green = Fore.GREEN
yellow = Fore.YELLOW 
blue  = Fore.BLUE
red = Fore.RED   
# common import
from datetime import datetime
import re

__setHV_items_dict__ = {}
__set_hv_items_linesToWrite__ = []
nums_proceeded = 0
from hvConfig import __get_item_numbers__

def __readOneSettingFile__(settingFile = itemSettings):
    if itemSettings is not None:
        with open(itemSettings) as f:
            lines = f.readlines()
            for line in lines[1:]:
                values = line.strip().split()
                if values[0] == '#' or values[0] == '//':
                    continue
                index = int(values[0])
                column = int(values[1])
                row = int(values[2])
                value = float(values[3])
                __setHV_items_dict__[index] = {'index': index, 'column': column, 'row' : row, 'value': value}
    else:
        __action__ = input(blue + f"itemSettings is None, set all values of ``{itemToConfig.split()[0]}`` to {itemValue}, press enter to continue, any key else to quit.\n" + reset)
        if __action__.strip() == "":
            pass
        else:
            exit(yellow + "Operation Cancelled" + reset)

def __set_item_value__(line, dataDict):
    global nums_proceeded
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
        nums_proceeded += 1
        if nums_proceeded % 100 == 0:
            print(green + f"Proceeded {nums_proceeded} items" +reset)
        if index in dataDict:
        #    input(index)
           return f"ECAL:hv:{col}:{row}:{itemToConfig}{dataDict[index]['value']:.3f}\n"
        else:
            return f"ECAL:hv:{col}:{row}:{itemToConfig}{itemValue:.3f}\n"



def __setHV_items__():
    print(blue + f"The item ``{itemToConfig.split()[0]}`` is going to be set. Please make sure this item exists. Press enter to continue, any key else to cancel" + reset)
    __action__ = input().strip()
    if __action__ != "":
        exit(yellow + "Operation canceled\n" + reset)
    __readOneSettingFile__()
    current_datetime = datetime.now()
    formatted_datetime = current_datetime.strftime("%d_%b_%Y_%H_%M_%S")
    fileOut = "ECAL_HV_" + formatted_datetime + ".snap"
    
    # 打开输入文件并读取内容
    with open(hvTemplateFile, 'r') as fileIn:
        lines = fileIn.readlines()  # 读取所有行

            # 打开输出文件进行写入
    with open(fileOut, 'w') as fileOut:
        # 遍历每一行并处理
        for line in lines:
            if not (f"{itemToConfig}" in line):  # 判断行是否包含 itemToConfig
                # 对包含匹配项的行进行特殊处理（例如修改格式或增加内容）
                fileOut.write(line)
            else:
                lineToWrite = __set_item_value__(line, __setHV_items_dict__)
                # print(lineToWrite)
                __set_hv_items_linesToWrite__.append(lineToWrite)
                # fileOut.write(lineToWrite)
        sorted_lines = sorted(__set_hv_items_linesToWrite__, key=__get_item_numbers__)
        for line in sorted_lines:
            fileOut.write(line)
        print(f"All res Successfully proceeded!New Configuration File: {fileOut}")
                # 不匹配的行直接写入输出文件