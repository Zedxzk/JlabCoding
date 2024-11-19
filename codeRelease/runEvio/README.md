# A python3 based script
Hello, welcome to use this scrip. This is a script developed by Zhikun Xi, an undergraduate from Wuhan University, at the end of 2024. This is a python3 based scripts. It is to run all the `.evio` (or whatever kinds, just modify the parameters in `parameters.py`) file with certain plugins and save the root file to a certain folder. And you can then use `hadd allRuns.root *root` to combine all the files for further analysis.

Suggesstions: You should make a new folder and run this command again in order to keep you woking space clean and managable. If you insist not creating a new folder, please backup all the data before getting started.

<u>**Most Important Thing: Check the parameters in `parameters.py`**<u>

To run the script, you should install `colorama` first(pip install colorama). Then, go to an empty folder, and type 
`/w/halld-scshelf2101/home/zhikun/codeRelease/runEvio/main.py -init `
Then check the file `parameter.py`, put all the files that you want to run in the `inputDir`, then run. After that you will see the root file in `outputDir`

For help, use `-h` option.

## Details For Each File:
### Quick View For All
Every File is in charge of a specific kind of `Action`:
|File|Action|Usage|
|:--:|:--:|:--:|
|main.py|-h|Define the basic procedure, and accept the command, the most important skeleton|
|subModule/setupEnvironment.py|-init|Initialize the environments. Specifically, to copy some pre-defined template and create some directories|
|submodule/import_parameters||Import all the parameters in `parameters.py`. If the working directory doesn't have a `parameters.py` (when the directory has not been initialized yet), use `parameters.py` template located at `/w/halld-scshelf2101/home/zhikun/codeRelease/runEvio/parameters.py`. If|
|subModule/runEvioFiles.py|-run|To run all the `.evio` files or `.hddm` files in a command and see all the results|
|subModule/hvConfig.py|-hvConfig|Used for High Voltage Configuration. Pleased Check the parameters in `parameters.py` |



### Main.py
This file is in