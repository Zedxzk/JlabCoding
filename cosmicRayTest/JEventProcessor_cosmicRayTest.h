// $Id$
//
//    File: JEventProcessor_cosmicRayTest.h
// Created: Mon Oct 14 11:54:33 AM EDT 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#ifndef _JEventProcessor_cosmicRayTest_
#define _JEventProcessor_cosmicRayTest_

#include <JANA/JEventProcessor.h>
#include <ECAL/DECALHit.h>
#include <FCAL/DFCALHit.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "TCanvas.h"
#include "TH2Poly.h"
#include "TRandom.h"
#include "TStyle.h"
#include <fstream>
#include <fstream>
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/classByZhikun.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/commonFunctions.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunConstants.h"

using namespace classByZhikun;



std::vector<std::vector<Double_t>> uselessVector(sizeOfEcal, vector<Double_t>(sizeOfEcal, 0));

namespace globalVariables {
    // 导入 std::chrono 内的常用类型
    string fileNameTemplates;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::chrono::duration<long> durationTime;
    string startTimeString;
    string endTimeString;
    string durationTimeString;
    // std::ofstream outputFile;
    int eventLooped = 0;
    string newestFileName;
    plotEcalHits2D hitsCount;
	bool showInColumn = true;

	Double_t lookingForMaxEnergy = 0;
	Double_t lookingForMaxTime = 0;
	void findBigger(Double_t & initialValue, Double_t comparedValue){
		if(comparedValue > initialValue){
			initialValue = comparedValue;
		}
	}
	// int eventNo = 1;
/*
plotEcalHits2D(string xAxis, string yAxis, string name, string title, TCanvas* c, TH2Poly* p)
plotEcalHits2D(string xAxis, string yAxis, string name, string title)
*/
    plotEcalHits2D hitsEnergy2D(
		"",        "", "EnergyDistribution2D"     , "Energy Distribution"
	//	string xAxis , string yAxis, string name,  string title
	);
    plotEcalHits2D hitsTime2D(
		"",        "", "TimeDistribution2D"     , "Time Distribution"
	//	string xAxis , string yAxis, string name,  string title
	);
	plotEcalHits2D hitsCount2D(
		"",        "", "HitsDistribution2D"     , "Hits Distribution"
	//	string xAxis , string yAxis, string name,  string title
	);
	// plotEcalHits2D rowChannelEnergy2D(
	// 	"Channel Number By Row", " Energy/GeV", "rowChannelEnergy2D", "Energy Distribution VS Channels by row",
	// 	"rowChannelEnergy2D","Energy Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy
	// );
	// plotEcalHits2D columnChannelEnergy2D(
	// 	"Channel Number By Col", " Energy/GeV", "columnChannelEnergy2D", "Energy Distribution VS Channels by col",
	// 	"columnChannelEnergy2D","Energy Distribution VS Channels by col", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy
	// );
	// plotEcalHits2D rowChannelTime2D(
	// 	"Channel Number By Row", " Time/ns", "rowChannelTime2D", "Time Distribution VS Channels by row",
	// 	"rowChannelTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime
	// );
	// plotEcalHits2D columnChannelTime2D(
	// 	"Channel Number By Col", " Time/ns", "columnChannelTime2D", "Time Distribution VS Channels by col",
	// 	"columnChannelTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime
	// );

	plotEcalHits2D rowChannelEnergy2D(
		"Channel Number By Row", " Energy/GeV", "rowChannelEnergy2D", "Energy Distribution VS Channels by row",
		new TH2D("rowChannelEnergy2D","Energy Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy)
	);
	plotEcalHits2D columnChannelEnergy2D(
		"Channel Number By Col", " Energy/GeV", "columnChannelEnergy2D", "Energy Distribution VS Channels by col",
		new TH2D("columnChannelEnergy2D","Energy Distribution VS Channels by col", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy)
	);
	plotEcalHits2D rowChannelTime2D(
		"Channel Number By Row", " Time/ns", "rowChannelTime2D", "Time Distribution VS Channels by row",
		new TH2D("rowChannelTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
	);
	plotEcalHits2D columnChannelTime2D(
		"Channel Number By Col", " Time/ns", "columnChannelTime2D", "Time Distribution VS Channels by col",
		new TH2D("columnChannelTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
	);



	plotEcalHits1D EcalTimeColumnPlot1D = 
	plotEcalHits1D(new TH1D("EcalTimeColumPlot1D", "Time Distribution of Ecal in column", EcalChannelNums, 0, EcalChannelNums));

    plotEcalHits1D EcalEnergyColumnPlot1D = 
	plotEcalHits1D(new TH1D("EcalEnergyColumPlot1D", "Energy Distribution of Ecal in column", EcalChannelNums, 0, EcalChannelNums)); 
	// 初始化，需要传递 hist 指针

	plotEcalHits1D FcalTimeColumnPlot1D = 
	plotEcalHits1D(new TH1D("FcalTimeColumPlot1D", "Time Distribution of Fcal in column", FcalChannelNums, 0, FcalChannelNums));

    plotEcalHits1D FcalEnergyColumnPlot1D = 
	plotEcalHits1D(new TH1D("FcalEnergyColumPlot1D", "Energy Distribution of Fcal in column", FcalChannelNums, 0, FcalChannelNums)); 
	// 初始化，需要传递 hist 指针
	
	plotEcalHits1D EcalTimeRowPlot1D = 
	plotEcalHits1D(new TH1D("EcalTimeRowPlot1D", "Time Distribution of Ecal", EcalChannelNums, 0, EcalChannelNums));

    plotEcalHits1D EcalEnergyRowPlot1D = 
	plotEcalHits1D(new TH1D("EcalEnergyRowPlot1D", "Energy Distribution of Ecal", EcalChannelNums, 0, EcalChannelNums)); 


	plotEcalHits1D FcalTimeRowPlot1D = 
	plotEcalHits1D(new TH1D("FcalTimeRowPlot1D", "Time Distribution of Fcal", FcalChannelNums, 0, FcalChannelNums));

    plotEcalHits1D FcalEnergyRowPlot1D = 
	plotEcalHits1D(new TH1D("FcalEnergyRowPlot1D", "Energy Distribution of Fcal", FcalChannelNums, 0, FcalChannelNums)); 

	std::vector<std::vector<Double_t>> EcalEnergyRecordMatrix(sizeOfEcal, std::vector<Double_t>(sizeOfEcal, 0));
	std::vector<std::vector<Double_t>> EcalTimeRecordMatrix(sizeOfEcal, std::vector<Double_t>(sizeOfEcal, 0));
	std::vector<std::vector<Double_t>> FcalEnergyRecordMatrix(sizeOfFcal, std::vector<Double_t>(sizeOfFcal, 0));
	std::vector<std::vector<Double_t>> FcalTimeRecordMatrix(sizeOfFcal, std::vector<Double_t>(sizeOfFcal, 0));
	std::vector<std::vector<Double_t>> EcalHitsRecordMatrix(sizeOfEcal, std::vector<Double_t>(sizeOfEcal, 0));
	std::vector<std::vector<Double_t>> FcalHitsRecordMatrix(sizeOfFcal, std::vector<Double_t>(sizeOfFcal, 0));
	float channelMapByRow[sizeOfEcal][sizeOfEcal] = {0,};
	float channelMapByCol[sizeOfEcal][sizeOfEcal] = {0,};


}

namespace figuresInCosmicRaysTest{	
	void initPlotStyleOfHits2D(plotEcalHits2D& );
	void initPlotStyleOfHits1D(plotEcalHits1D& ,bool ecal);
	void setPlotStyleOfHits2D(plotEcalHits2D& );
	void setPlotStyleOfHits1D(plotEcalHits1D& ,bool ecal);
	void setPalatteStyleV1();

	void fillHitsDistribution(int hitColum, int hitRow, TH2Poly* h2poly ,TCanvas* canvas, bool ecal = false) {
		Double_t x, y;
		if(!ecal){
			x = hitColum * 4  - 2;
			y = hitRow * 4 - 2;
		}
		else{
			x = hitColum * 2 + 75;
			y = hitRow * 2 +75;
		}
    	// 更新画布
		h2poly->Fill(x, y);
		canvas->Update();
}

}



class JEventProcessor_cosmicRayTest:public jana::JEventProcessor{
	public:
		JEventProcessor_cosmicRayTest();
		~JEventProcessor_cosmicRayTest();
		const char* className(void){return "JEventProcessor_cosmicRayTest";}

	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.
};

using namespace globalVariables;
using namespace figuresInCosmicRaysTest;

#endif // _JEventProcessor_cosmicRayTest_

