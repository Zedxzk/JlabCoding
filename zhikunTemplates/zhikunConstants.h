#ifndef _ZHIKUNCONSTANTS_H_
#define _ZHIKUNCONSTANTS_H_

namespace zhikunConstants{
    const int sizeOfEcal = 40;
    const int sizeOfFcal = 59;
    const int FcalChannelNums = 2364;
    const int EcalChannelNums = 1600;
    const int maxTime = 200;
    const int timeResolution = 4;
    const Double_t timeBinsWidth = 1.0 / 10;           // bin width : 0.1 ns
    const int timeBins = int(maxTime / timeBinsWidth);
    const int maxEnergy = 1;
    const int minEnergy = 0;
    const Double_t energyBinsWidth = 1.0 / 1000;       // bin width : 1 MeV
    const int energyBins = int(maxEnergy * 1000);      // bin nums , actually 1000
    const Float_t EcalLength = 2.1;                    // unit: centimeter
    const Float_t FcalLength = 4.0;                    // unit: centimeter


}

using namespace zhikunConstants;
#endif