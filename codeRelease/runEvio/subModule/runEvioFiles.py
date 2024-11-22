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
#common import




import re
import fnmatch
from pprint import pprint

def runAction():
    # setup_paths
    # list all the files in the `inputDir`
    all_files = os.listdir(inputDir)
    # print('parameters.py' in os.getcwd())
    
    # find .evio files
    evioFiles = [file for file in all_files if fnmatch.fnmatch(file, "*" + fileType)]
    evioFiles.sort()
    # print  .evio files found
    pprint(f"Found {fileType} files:" )
    pprint(evioFiles)
    
    if evioFiles == []:
        print(Fore.RED + f"Can't find any {fileType} file! Please check your directory!\nSuspensed!" + Style.RESET_ALL)
        exit(1)
    else:
        print("Do you want to keep on?  "+ Fore.GREEN + " (y/n)" + Style.RESET_ALL)
        user_input = input().strip().lower()
        if user_input == 'y':
            print("Continuing...")
            checkRunCommand(evioFiles)
            return 0 
        else:
            print("Operation cancelled.")
            exit(1)


def checkRunCommand(evioFiles):
    if(runCommand) == ['']:
        print(Fore.RED + "Run command Empty! Check run command in parameters.py!" + Style.RESET_ALL)
    else:
        print(Fore.GREEN + f"Your command is going to be:\n{runCommand}\nAre you sure you gonna continue?(y/n)"+Style.RESET_ALL)
        user_input = input().strip().lower()
        if user_input == 'y':
            print("Continuing...")
            runAllFiles(evioFiles)
        else:
            print("Operation cancelled.")
            exit(1)

def runAllFiles(evioFiles):
    for evioFile in evioFiles:
        fileName = evioFile[0: - len(fileType)]
        print(fileName)
        # input()
        subprocess.run(['bash', '-c', f'{runCommand} {inputDir + evioFile}'])
        subprocess.run(['bash', '-c', f'cp hd_root.root {outputDir + fileName}.root'])
    return 0

