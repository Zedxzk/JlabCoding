import sys
import argparse
import subprocess
import os
from colorama import Fore, Back, Style # type: ignore
sys.path.append(os.path.dirname((os.path.abspath(__file__))))


from pprint import pprint
from subModule import setupEnvironment # type: ignore
from subModule import runEvioFiles # type: ignore
import textwrap



greeting = '''
Hello, welcome to use this scrip. This is a script developed by Zhikun Xi, an undergraduate from Wuhan University, at the end of 2024. This is a python3 based scripts. It is to run all the \".evio\" file with certain plugins and save the root file to a certain folder. And you can then use \"hadd allRuns.root *root\" to combine all the files for further analysis.

Suggesstions: You should make a new folder and run this command again in order to keep you woking space clean and managable. If you insist not creating a new folder, please backup all the data before getting started.

To run the script, you should install `colorama` first.
'''
# greeting = textwrap.fill(greeting, width=100)
# print(greeting)

def main():
    print(os.path.dirname(os.path.abspath(__file__)))
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
    args = parser.parse_args()
        # 根据传递的参数执行相应的操作

    if args.run:
        # 获取当前工作目录
        current_directory = os.getcwd()
        print(Fore.BLUE + "current working directory:", current_directory + Style.RESET_ALL)
        setupEnvironment.checkInitializationStatus()    # 添加命令行参数
        print("Program is running...")
        runEvioFiles.runAction()
        print("\n\n\n\n****************************")
        print("All run finised, please check results!\n****************************")
    if args.init:
        print(f"Initialization start")
        setupEnvironment.checkInitializationStatus()  




if __name__ == "__main__":
    main()