# JlabCoding
## Pluggins Manual

|   Direcrory Name   |    Usage  |  Extra Info    |
| :--: | :--: | :--: |
|    cosmicRayTest   |  Draw Some plots without ANY CUTS, it was first designed using MC event        |   Call ECAL hits and FCAL hits, save some useful information and plots. Detailed information can be found in the `README.md` in the directory             |
|    cosmicRayTestEvio   |  Developed on the basis of `cosmicRayTest`, add some cuts condition, and can select whether to call FCAL(Because FCAL code is not working)       |                |
|    muonsCommission  |   Extract the Energy deposition and Time distribution plots of individual column by the 2D histograms saved in the `.root` file. To extract the plots you should run `cosmicRayTest` or `cosmicRayTestEvio` First   |      |


## Update Log
### 2024.11.2
Debugged some error and produce figures in row and column.

Created some header files used to provide coherent standard for plots. They are in `/zhikunTemplate/*`

If Want to remove add history, just type in 
```
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch inputFiles/muons_smeared.hddm' --prune-empty --tag-name-filter cat -- --all
```

### 2024.11.3

Error occur when over 120 plots are printed in a single root command line. Some settings you did will not be properly implemented by root.


### 2024.11.5

For Next step, I need to develop some code for fitting the energy deposition in all the channels. In principle all the channels should behave in the same way. But before high voltage configuration, this migh not be true.

To achieve this goal, `muonsCommission/muonsCommission.cpp` is developed.