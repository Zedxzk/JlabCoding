# JlabCoding
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