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

def cleanAction():
    print(Fore.RED + "Are you sure you want to clean up the environment?(y/n)" + reset)
    if input().strip().lower() != "y":
        return
    else:
        subprocess.run(['bash', '-c', f'rm parameters.py'])
        subprocess.run(['bash', '-c', f'rm *.snap'])
        subprocess.run(['bash', '-c', f'rm -rf {inputDir}'])
        subprocess.run(['bash', '-c', f'rm -rf {outputDir}'])
        subprocess.run(['bash', '-c', f'rm -rf __pycache__'])

