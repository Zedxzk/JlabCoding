#common import
import sys
import argparse
import subprocess
import importlib.util
import os
from colorama import Fore, Back, Style # type: ignore
sys.path.append(os.path.dirname((os.path.abspath(__file__)+"/../")))
from parameters import runCommand, inputDir, outputDir, fileType # type: ignore
reset = Style.RESET_ALL
green = Fore.GREEN
yellow = Fore.YELLOW 
#common import





def createPara():
    current_file_path = os.path.dirname(__file__)
    paraFile = current_file_path +  "/../parameters.py"
    paraCopy = subprocess.run(['bash', '-c', f'cp {paraFile} .'], capture_output=True)
    if paraCopy.returncode == 0:
        print(green + "parameters.py successfully created" + reset)
    else:
        print(yellow +"Failed to create parameters.py, please check your permission!" + reset)

def createInputFolder():
    try:
        os.mkdir(inputDir)
        print(Fore.GREEN  + f"{inputDir} successfully created" + Style.RESET_ALL)
    except PermissionError:
        print(yellow + "Permission Denied, check your permmision!" + reset)
    except Exception as e:
        print(yellow + f"Other Error: {e}" + reset)

def createOutputFolder():
    try:
        os.mkdir(outputDir)
        print(green + f"{outputDir} successfully created" + reset)
    except PermissionError:
        print(yellow + "Permission Denied, check your permmision!" + reset)
    except Exception as e:
        print(yellow + f"Other Error: {e}" +  reset)


def initialization():
        # if True:
        # print(runCommand)
        print("\nEmpty folder, initialzing!")
        # print(current_file_path)
        # print(paraFile)
        # 执行 cp 命令
        createPara()
        createInputFolder()
        createOutputFolder()

def checkInitializationStatus():
    # importParas()
    goodInitialization = False
    allFiles = os.listdir(".")
    for i in range(len(allFiles)):
        if(os.path.isdir(allFiles[i])):
            allFiles[i] = "./" + allFiles[i] + "/"
    print("Current folder has following items:")
    print(allFiles)
    if allFiles == []:
        initialization()
    else:
        print( yellow + "\nNot empty folder, please mind data security!" + reset)
        if "parameters.py" in allFiles:
            print(yellow + "parameters.py already exist, please check the command to run" + reset)
        else:
            createPara()

        if inputDir in allFiles:
            print(yellow + f"{inputDir} already exists, make sure all the files that you want to run are included" + reset)
        else:
            createInputFolder()

        if outputDir in allFiles:
            print(yellow + f"{outputDir} already exists, do backup data!!!!!!" + reset)
        else:
            createOutputFolder()

def importParas():
    try:
        current_working_dir = os.getcwd()
        parameters_file = os.path.join(current_working_dir, "parameters.py")
        spec = importlib.util.spec_from_file_location("parameters", parameters_file)
        parameters = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(parameters)
        globals().update({k: getattr(parameters, k) for k in dir(parameters) if not k.startswith("__")})
    # or, use template
    except Exception as e:
        print(Fore.RED + f"Error:\n {e} \noccurred when loading parameters.py, Exit!" + Style.RESET_ALL)
        exit(1)
