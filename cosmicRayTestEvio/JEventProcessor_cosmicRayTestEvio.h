// $Id$
//
//    File: JEventProcessor_cosmicRayTestEvio.h
// Created: Wed Nov  6 10:26:16 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#ifndef _JEventProcessor_cosmicRayTestEvio_
#define _JEventProcessor_cosmicRayTestEvio_

#include <JANA/JEventProcessor.h>

class JEventProcessor_cosmicRayTestEvio:public jana::JEventProcessor{
	public:
		JEventProcessor_cosmicRayTestEvio();
		~JEventProcessor_cosmicRayTestEvio();
		const char* className(void){return "JEventProcessor_cosmicRayTestEvio";}
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
	// Physical Values 2D plots variable VS chennel index
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

	// Digital Values 2D plots variable VS channel index
	plotEcalHits2D rowChannelDigitalPeak2D(
		"Channel Number By Row", " Digital Pause Peak/Digital Counts", "rowChannelDigitalPeak2D", "Digital Pause Peak VS Channels by row",
		new TH2D("rowChannelDigitalPeak2D","Digital Pause Peak VS Channels by row", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy)
	);
	plotEcalHits2D columnChannelDigitalPeak2D(
		"Channel Number By Col", " Digital Pause Peak/Digital Counts", "columnChannelDigitalPeak2D", "Digital Pause Peak VS Channels by col",
		new TH2D("columnChannelEnergy2D","Energy Distribution VS Channels by col", EcalChannelNums, 0, EcalChannelNums, energyBins, minEnergy, maxEnergy)
	);

	plotEcalHits2D rowChannelDigitalIntegral2D(
		"Channel Number By Row", " Pause Integral", "rowChannelDigitalIntegral2D", "Digital Integral VS Channels by row",
		new TH2D("rowChannelDigitalIntegral2D","Digital Integral VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
	);
	plotEcalHits2D columnChannelDigitalIntegral2D(
		"Channel Number By Col", " Pause Integral", "rowChannelDigitalIntegral2D", "Digital Integral VS Channels by col",
		new TH2D("columnChannelDigitalIntegral2D","Digital Integral VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
	);

	plotEcalHits2D rowChannelDigitalTime2D(
		"Channel Number By Row", " Digital Time/sample", "rowChannelDigitalTime2D", "Digital Time Distribution VS Channels by row",
		new TH2D("rowChannelDigitalTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
	);
	plotEcalHits2D columnChannelDigitalTime2D(
		"Channel Number By Col", "Digital Time/sample", "columnChannelDigitalTime2D", "Digital Time Distribution VS Channels by col",
		new TH2D("columnChannelDigitalDigitalTime2D","Time Distribution VS Channels by row", EcalChannelNums, 0, EcalChannelNums, timeBins, 0, maxTime)
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




	private:
		jerror_t init(void);						///< Called once at program start.
		jerror_t brun(jana::JEventLoop *eventLoop, int32_t runnumber);	///< Called everytime a new run number is detected.
		jerror_t evnt(jana::JEventLoop *eventLoop, uint64_t eventnumber);	///< Called every event.
		jerror_t erun(void);						///< Called everytime run number changes, provided brun has been called.
		jerror_t fini(void);						///< Called after last event of last event source has been processed.



};

#endif // _JEventProcessor_cosmicRayTestEvio_

