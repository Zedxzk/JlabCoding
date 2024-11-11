// $Id$
//
//    File: JEventProcessor_cosmicRayTestEvio.cc
// Created: Wed Nov  6 10:26:16 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#include "JEventProcessor_cosmicRayTestEvio.h"
using namespace jana;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
#include <TTree.h>
#include <TSystem.h>
extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_cosmicRayTestEvio());
}
} // "C"


//------------------
// JEventProcessor_cosmicRayTestEvio (Constructor)
//------------------
JEventProcessor_cosmicRayTestEvio::JEventProcessor_cosmicRayTestEvio()
{

}

//------------------
// ~JEventProcessor_cosmicRayTestEvio (Destructor)
//------------------
JEventProcessor_cosmicRayTestEvio::~JEventProcessor_cosmicRayTestEvio()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_cosmicRayTestEvio::init(void)
{
	// This is called once at program startup. 
	for(int i = 0;i < 40 ; i++){
		for(int j = 0; j < 40; j++){
			channelMapByCol[j][i] = (39 - j) * 40 + (39 - i) + 0.5;
			channelMapByRow[j][i] = i * 40 + j + 0.5;
		}
	}
	//record start time and convert it to string
	startTime = std::chrono::high_resolution_clock::now();
	startTimeString = commonFunctions::getTimeString(startTime);
	setPlotStyleOfHits1D(EcalTimeColumnPlot1D);
    setPlotStyleOfHits1D(EcalEnergyColumnPlot1D);
    setPlotStyleOfHits1D(FcalTimeColumnPlot1D);
    setPlotStyleOfHits1D(FcalEnergyColumnPlot1D);
    setPlotStyleOfHits1D(EcalTimeRowPlot1D);
    setPlotStyleOfHits1D(EcalEnergyRowPlot1D);
    setPlotStyleOfHits1D(FcalTimeRowPlot1D);
    setPlotStyleOfHits1D(FcalEnergyRowPlot1D);

	initPlotStyleOfHits2D(hitsEnergy2D);
	initPlotStyleOfHits2D(hitsTime2D);
	initPlotStyleOfHits2D(hitsCount2D);

	initPlotStyleOfHits2D(rowChannelEnergy2D);
	initPlotStyleOfHits2D(columnChannelEnergy2D);
	initPlotStyleOfHits2D(rowChannelTime2D);
	initPlotStyleOfHits2D(columnChannelTime2D);
	
	initPlotStyleOfHits2D(rowChannelDigitalPeak2D);
	initPlotStyleOfHits2D(columnChannelDigitalPeak2D);
	initPlotStyleOfHits2D(rowChannelDigitalIntegral2D);
	initPlotStyleOfHits2D(columnChannelDigitalIntegral2D);
	initPlotStyleOfHits2D(rowChannelDigitalTime2D);
	initPlotStyleOfHits2D(columnChannelDigitalTime2D);
	cout<< "ALL PLOTS INITIALIZED SUCCESSFULLY\n\n\n\n\n";



	// hitsDistrubution = new TH2Poly();

	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_cosmicRayTestEvio::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_cosmicRayTestEvio::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// This is called for every event. Use of common resources like writing
	// to a file or filling a histogram should be mutex protected. Using
	// loop->Get(...) to get reconstructed objects (and thereby activating the
	// reconstruction algorithm) should be done outside of any mutex lock
	// since multiple threads may call this method at the same time.
	// Here's an example:
	//
	// vector<const MyDataClass*> mydataclasses;
	// loop->Get(mydataclasses);
	//
	// japp->RootFillLock(this);
	//  ... fill historgrams or trees ...
	// japp->RootFillUnLock(this);
	vector<const DECALDigiHit *> ecalDigitHits;
	loop->Get(ecalDigitHits);
	// vector<const DFCALHit *>fcalhits;
	// loop->Get(fcalhits);
	int col, row;
	Double_t pulse_integral, pulse_time, pulse_peak;
	std::vector<bool> goodEvent(ecalDigitHits.size(), true);

	// 	for(unsigned int i = 0; i < ecalDigitHits.size(); i++){
	// 	col = fcalhits[i]->column;
	// 	row = fcalhits[i]->row;
	// 	// energy = fcalhits[i]->E;
	// 	// time = fcalhits[i]->t;
	// 	FcalEnergyRecordMatrix[col][row] += energy;
	// 	FcalTimeRecordMatrix[col][row] += time;
	// 	FcalHitsRecordMatrix[col][row] += 1;
	// 	// findBigger(lookingForMaxEnergy,energy);
	// 	// findBigger(lookingForMaxTime,time);
	// }
	// outputFile << '\n' << "ECAL TRACK : " ;

	//for first cut, accepting col >= 20, time in time window
	for (unsigned int i=0;i<ecalDigitHits.size();i++){
		col = ecalDigitHits[i]->column;
		row = ecalDigitHits[i]->row;
		pulse_integral = ecalDigitHits[i]->pulse_integral;
		pulse_time     = ecalDigitHits[i]->pulse_time    ;
		pulse_peak     = ecalDigitHits[i]->pulse_peak    ;
		goodEvent[i-] = (col > 20) && (pause_peak > digiHitsPeakPosLowerLimit) && (pause_peak < digiHitsPeakPosUpperLimit)

		
		//Needs modification
		if(goodEvent[i]){
			EcalEnergyRecordMatrix[col][row] += energy;
			EcalTimeRecordMatrix[col][row] += time;
			EcalHitsRecordMatrix[col][row] += 1;
			rowChannelEnergy2D.getHist2D()->Fill(channelMapByRow[col][row], energy);
			columnChannelEnergy2D.getHist2D()->Fill(channelMapByCol[col][row], energy);
			rowChannelTime2D.getHist2D()>Fill(channelMapByRow[col][row], time);
			columnChannelTime2D.getHist2D()->Fill(channelMapByCol[col][row], time);
		}


		else{
			continue;
		}

	}
	// outputFile << endl << endl;
	// outputFile << "ENEVT LOOPED = " << eventNo << endl << endl;
	// eventNo += 1;

	

	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_cosmicRayTestEvio::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_cosmicRayTestEvio::fini(void)
{
	// Called before program exit after event processing is finished.
	return NOERROR;
}

