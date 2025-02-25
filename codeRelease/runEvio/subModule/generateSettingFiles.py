#common import
import sys
import argparse
import subprocess
import importlib.util
import os
sys.path.append(os.path.dirname((os.path.abspath(__file__))))
from import_parameters import *
from colorama import Fore, Back, Style # type: ignore
# if 'parameters.py' in os.listdir("."):
#     sys.path.append(os.getcwd())
# else:
#     sys.path.append(os.path.dirname((os.path.abspath(__file__)+"/../")))
# from parameters import runCommand, inputDir, outputDir, fileType # type: ignore
reset = Style.RESET_ALL
green = Fore.GREEN
yellow = Fore.YELLOW 
blue = Fore.BLUE
red = Fore.RED
#common import

__generate_files_ECAL_channels__ = 1600
__generate_files_ECAL_size__ = 40

__file_title__ = "# index    column     row     {__variable_name__} column(-20 ~ 20)   row(-20 ~ 20)\n"
__file_lines__ = "{index:<5d}     {column:<5d}      {row:<5d}      {value:<8.2f}       {column2:<5d}      {row2:<5d}\n"


def __translate_cordinate_from_positive_to_both__(number):
    number = number - 20 if number <=19 else number - 19
    return number



def __get_variable_name__(var):
    var_id = id(var)
    for var_name, var_val in globals().items():
        if id(var_val) == var_id:
            return var_name
    for var_name, var_val in locals().items():
        if id(var_val) == var_id:
            return var_name
    return None

def __generate_setttings_contents__(f, value):
        f.write(__file_title__.format(__variable_name__=f"{__get_variable_name__(value)}"))
        for i in range(__generate_files_ECAL_channels__):
            column =  39 - i // __generate_files_ECAL_size__
            row = 39 -  i % __generate_files_ECAL_size__
            column2 = __translate_cordinate_from_positive_to_both__(column)
            row2 = __translate_cordinate_from_positive_to_both__(row)
            f.write(__file_lines__.format(index=i, column=column, row=row, value=value,column2=column2, row2=row2 ))


def generateSettingFiles():
    try:
        print(green + "The Following Setting Files are going to be generated:" + blue )
        if(aimADCSettings != None):
            print(f"aimADCSettings : {aimADCSettings}")
        if(alphaSettings != None):
            print(f"alphaSettings : {alphaSettings}")
        if(hvCapSettings != None):
            print(f"hvCapSettings : {hvCapSettings}")
        if(hvBotSettings != None):
            print(f"hvBotSettings : {hvBotSettings}")
        if(itemSettings != None):
            print(f"itemSettings : {itemSettings}")
        action = input( green + "Press Enter to continue, or quit to exit\n" + reset)
        if(action.lower() == 'q' or action.lower() == 'quit'):
            print(yellow + "Operation cancelled." + reset)
            exit(1)
        elif(action.strip() != ''):
            print(yellow + "Invalid input, operation cancelled." + reset)
            exit(1)
        else:

            if(aimADCSettings != None):
                with open(aimADCSettings, "w") as f:
                    __generate_setttings_contents__(f=f,value=aimADC)
                    f.write(__file_title__.format(__variable_name__="aimADC"))
                    # for i in range(__generate_files_ECAL_channels__):
                    #     column =  39 - i // __generate_files_ECAL_size__
                    #     row = 39 -  i % __generate_files_ECAL_size__
                    #     f.write(__file_lines__.format(index=i, column=column, row=row, value=aimADC))
                print(green + f"Generated {aimADCSettings}" + reset)


            if(alphaSettings != None):
                with open(alphaSettings, "w") as f:
                    __generate_setttings_contents__(f=f,value=alpha)
                print(green + f"Generated {alphaSettings}" + reset)
            
            if(hvCapSettings != None):
                with open(hvCapSettings, "w") as f:
                    __generate_setttings_contents__(f=f,value=hvCap)
                    # f.write(__file_title__.format(__variable_name__="hvCap"))
                    # for i in range(__generate_files_ECAL_channels__):
                    #     column =  39 - i // __generate_files_ECAL_size__
                    #     row = 39 -  i % __generate_files_ECAL_size__
                    #     f.write(__file_lines__.format(index=i, column=column, row=row, value=hvCap))
                print(green + f"Generated {hvCapSettings}" + reset)

            if(hvBotSettings != None):
                with open(hvBotSettings, "w") as f:
                    __generate_setttings_contents__(f, value=hvBot)

                    # f.write(__file_title__.format(__variable_name__="hvBot"))
                    # for i in range(__generate_files_ECAL_channels__):
                    #     column =  39 - i // __generate_files_ECAL_size__
                    #     row = 39 -  i % __generate_files_ECAL_size__
                    #     f.write(__file_lines__.format(index=i, column=column, row=row, value=hvBot))
                print(green + f"Generated {hvBotSettings}" + reset)
            if(itemSettings != None):
                with open(itemSettings, "w") as f:
                    __generate_setttings_contents__(f, value=itemValue)
                    print(green + f"Generated {itemSettings}" + reset)
    except Exception as e:
            print(yellow + f"Error: {e}")
            print("Operation cancelled." + reset)
            exit(1)