Hello, welcome to use this scrip. This is a script developed by Zhikun Xi, an undergraduate from Wuhan University, at the end of 2024. This is a python3 based scripts. It is to run all the `.evio` (or whatever kinds, just modify the parameters in `parameters.py`) file with certain plugins and save the root file to a certain folder. And you can then use `hadd allRuns.root *root` to combine all the files for further analysis.

Suggesstions: You should make a new folder and run this command again in order to keep you woking space clean and managable. If you insist not creating a new folder, please backup all the data before getting started.

To run the script, you should install `colorama` first(pip install colorama). Then, go to an empty folder, and type 
`/w/halld-scshelf2101/home/zhikun/codeRelease/runEvio/main.py -init `
Then check the file `parameter.py`, put all the files that you want to run in the `inputDir`, then run. After that you will see the root file in `outputDir`