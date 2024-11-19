import sys
import argparse
import subprocess
import os
from colorama import Fore, Back, Style # type: ignore
sys.path.append(os.path.dirname((os.path.abspath(__file__))))


from pprint import pprint
from subModule import setupEnvironment # type: ignore
from subModule import runEvioFiles # type: ignore
from subModule import hvConfig # type: ignore
import textwrap



greeting = '''
Hello, welcome to use this scrip. This is a script developed by Zhikun Xi, an undergraduate from Wuhan University, at the end of 2024. This is a python3 based scripts. It is to run all the \".evio\" (or whatever kinds, just modify the parameters in parameters.py)file with certain plugins and save the root file to a certain folder. And you can then use \"hadd allRuns.root *root\" to combine all the files for further analysis.

Suggesstions: You should make a new folder and run this command again in order to keep you woking space clean and managable. If you insist not creating a new folder, please backup all the data before getting started.

To run the script, you should install `colorama` first(pip install colorama). Then, go to an empty folder, and type 
/w/halld-scshelf2101/home/zhikun/codeRelease/runEvio/main.py -init 
Then check the file parameter.py, put all the files that you want to run in the inputDir, then run. After that you will see the root file in outputDir
'''
# greeting = textwrap.fill(greeting, width=100)
# print(greeting)

def main():
    try:
        from colorama import Fore, Back, Style # type: ignore
    except ImportError:
        print("colorama not found, installing...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", "colorama"])
        from colorama import Fore, Back, Style # type: ignore
    print(Fore.GREEN + greeting +Style.RESET_ALL)


    parser = argparse.ArgumentParser(description="runEvio script")
    parser.add_argument('-run'  , action='store_true', help='Run the program')
    parser.add_argument('-init' , action='store_true',help=' initialize the environment')
    parser.add_argument('-hvConfig' , action='store_true',help=' run High Voltage Configuration')
    args = parser.parse_args()
        # 根据传递的参数执行相应的操作

    if args.run:
        # 获取当前工作目录
        current_directory = os.getcwd()
        allFiles = os.listdir(".")
        for i in range(len(allFiles)):
            if(os.path.isdir(allFiles[i])):
                allFiles[i] = "./" + allFiles[i] + "/"
        localPara = False
        if 'parameters.py' in allFiles:
            localPara = True
        print(Fore.BLUE + "current working directory:", current_directory + Style.RESET_ALL)
        setupEnvironment.checkInitializationStatus()    # 添加命令行参数
        print("Program is running...")
        runEvioFiles.runAction()
        print("\n\n\n\n****************************")
        print("All run finised, please check results!\n****************************")
    if args.init:
        print(f"Initialization start")
        setupEnvironment.checkInitializationStatus()  
    if args.hvConfig:
        hvConfig.hvConfigureAction()




if __name__ == "__main__":
    main()