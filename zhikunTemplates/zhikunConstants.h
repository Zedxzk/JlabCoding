#ifndef _ZHIKUNCONSTANTS_H_
#define _ZHIKUNCONSTANTS_H_

namespace zhikunConstants{
    const Int_t sizeOfEcal = 40;
    const Int_t sizeOfFcal = 59;
    const Int_t FcalChannelNums = 2364;
    const Int_t EcalChannelNums = 1600;
    const Int_t maxTime = 16;
    const Int_t timeResolution = 4;                      // A sample width equals to 4 ns
    const Double_t timeBinsWidth = 1.0 / 10;           // bin width : 0.1 ns
    const Int_t timeBins = int(maxTime / timeBinsWidth);
    const Int_t maxEnergy = 0.1;
    const Int_t minEnergy = 0;
    const Double_t energyBinsWidth = 1.0 / 1000;         // bin width : 1 MeV
    const Int_t energyBins = int(maxEnergy * 1000);      // bin nums , actually 1000
    const Float_t EcalLength = 2.1;                    // unit: centimeter
    const Float_t FcalLength = 4.0;                    // unit: centimeter  
    
    const Double_t digiHitsPeakPosLowerLimit = 0.;     // define peak position time cuts limit
    const Double_t digiHitsPeakPosUpperLimit = 430.;   // define peak position time cuts limit
    
    // Some constant related to digital pulse
    const Int_t maxSample = 4096;


}

using namespace zhikunConstants;
#endif 