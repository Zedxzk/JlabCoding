# muonsCommision Directory Mannual

## Source File
Currently there're 2 source file in this directory, which are
>`cosmicExtraction.cpp` and `muonsCommission.cpp`

These files are used to extract the data recorden in the root file and plot figures and even do some fitting. `muonsCommission.cpp` was initially designed to deal with `.hddm` file. After I realized that we may need do combine data of different run, I then developed the `cosmicExtraction.cpp`. So let's introduce some details.

### `cosmicExtraction.cpp`
#### Pre-defined constants

Shared constants are defined at `/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunConstants.h`. 
And some other constants are defined at the beginning of the code. Two important bool variables are:
>`const bool addFit = true;`
>
>`const bool needOverView = false;`

If you want to draw an overview of the whole detector, the switch the `needOverView` to true. **Warning: this opion will remove all the files in the directory `./columView/`, be sure to backup!**. And you will see `overview.pdf`,`overview.png`,`test.pdf`, `test.png` created in the working folder and another folder `./columView/` which includes the plots of each column.

If you want to do some fit, leave `addFit` true. And then you will find the fitting results `./cosmicCommissionOutput`, named in a way `col_row_39_39_EnergyDeposit_FIT.png` does. If no fitting need, the result will be saved without `_FIT` suffix.

#### Running the code
To run the code, you need to modify some input file names,like `const TString branNameChannelNo = "EcalChannelNo";` or whatever. The code requires the `input.root` file has a `TTree` and at least two branches which include `columnIndex` and `Energy Deposition` respectively. I currently put all the files in `/w/halld-scshelf2101/home/zhikun/rootFiles`. Let's take a look at this root file:
```
 root [1] .ls
 TFile**         allRuns.root
  TFile*         allRuns.root
   KEY: TTree    cosmicRayDistributions;1        Cosmic Rays - Tree Of Distribution
   KEY: TH1F     test;1  test
   KEY: TH1F     energy;1        energy
   KEY: TH2F     Plot;1  Plot
 root [2] cosmicRayDistributions->Print()
 *************************************************************- *****************
 *Tree    :cosmicRayDistributions: Cosmic Rays Tree Of - Distribution                       *
 *Entries :    86174 : Total =         3814287 bytes  File  - Size =     979442 *
 *        :          : Tree compression factor =   3.- 90                       *
 *************************************************************- *****************
 *Br    0 :EcalChannelNo : EcalChannelNo/- I                                    *
 *Entries :    86174 : Total  Size=     346921 bytes  File - Size  =     131437 *
 *Baskets :       16 : Basket Size=      32000 bytes  - Compression=   2.63     *
 *............................................................- ................*
 *Br    1 :EcalEnergyDistributionByColumn : - EcalEnergyDistributionByColumn/D  *
 *Entries :    86174 : Total  Size=     693315 bytes  File - Size  =     174055 *
 *Baskets :       26 : Basket Size=      32000 bytes  - Compression=   3.98     *
 *............................................................- ................*
 *Br    2 :EcalTimeDistributionByColumn : - EcalTimeDistributionByColumn/D      *
 *Entries :    86174 : Total  Size=     693255 bytes  File - Size  =     166811 *
 *Baskets :       26 : Basket Size=      32000 bytes  - Compression=   4.15     *
 *............................................................- ................*
 *Br    3 :EcalpulsePeakDistributionByColumn - :                                *
 *         | EcalpulsePeakDistributionByColumn/- D                              *
 *Entries :    86174 : Total  Size=     693405 bytes  File - Size  =     146967 *
 *Baskets :       26 : Basket Size=      32000 bytes  - Compression=   4.71     *
 *............................................................- ................*
 *Br    4 :EcalpulseIntegralDistributionByColumn - :                            *
 *         | EcalpulseIntegralDistributionByColumn/- D                          *
 *Entries :    86174 : Total  Size=     693525 bytes  File - Size  =     181929 *
 *Baskets :       26 : Basket Size=      32000 bytes  - Compression=   3.81     *
 *............................................................- ................*
 *Br    5 :EcalpulseTimeDistributionByColumn - :                                *
 *         | EcalpulseTimeDistributionByColumn/- D                              *
 *Entries :    86174 : Total  Size=     693405 bytes  File - Size  =     176087 *
 *Baskets :       26 : Basket Size=      32000 bytes  - Compression=   3.93     *
 *............................................................................*
 ```

 In this file we include these parameters. Every entry, we read it's channel index and then convert it into column and number and then associate them with the values that we want to fill in the `TH2D` or `TH2Poly` objects.