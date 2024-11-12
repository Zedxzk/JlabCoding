// $Id$
//
//    File: JEventProcessor_cosmicRayTestEvio.cc
// Created: Wed Nov  6 10:26:16 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#include "JEventProcessor_cosmicRayTestEvio.h"
using namespace jana;
using namespace cutsConstants;


// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
#include <TTree.h>
#include <TSystem.h>
#include <set>
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
	cosmicRayTree = new TTree("cosmicRayDistributions", "Cosmic Rays Tree Of Distribution"       );
	cosmicRayTree->Branch("EcalChannelNo"                              , &channelNoByColumn       );
	cosmicRayTree->Branch("EcalEnergyDistributionByColumn"             , &energyBranchVar        );
	cosmicRayTree->Branch("EcalTimeDistributionByColumn"               , &timeBranchVar          );
	cosmicRayTree->Branch("EcalpulsePeakDistributionByColumn"          , &pulsePeakBranchVar     );
	cosmicRayTree->Branch("EcalpulseIntegralDistributionByColumn"      , &pulseIntegralBranchVar );
	cosmicRayTree->Branch("EcalpulseTimeDistributionByColumn"          , &pulseTimeBranchVar     );
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
	vector<const DECALHit *> ecalHits;
	loop->Get(ecalDigitHits);
	loop->Get(ecalHits);
	// vector<const DFCALHit *>fcalhits;
	// loop->Get(fcalhits);
	int col, row;
	Double_t pulse_integral, pulse_time, pulse_peak, energy, time;
	std::vector<bool> goodChannelEvent(ecalHits.size(), true);
	std::vector<bool> hasNeighbor(ecalHits.size(), false);
	// cout<<__LINE__<endl;
	if(addCuts){
		// 假设坐标数组 coords 格式为 {{x1, y1}, {x2, y2}, ...}
		// cut those events without neighbor
		vector<pair<int, int>> coordinatesEcal(ecalHits.size(),{100, 100});
		for(size_t i = 0; i < ecalHits.size(); i++){
			coordinatesEcal[i].first = ecalHits[i] -> column;
			coordinatesEcal[i].second = ecalHits[i] -> row;
		}
		//  define the neighbros that we want to accept
		vector<pair<int, int>> offsets = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
		// use set for fast scan
		set<pair<int, int>> coords_set(coordinatesEcal.begin(), coordinatesEcal.end());
		// looking for a neighbor
		vector<pair<int, int>> result;
		for(size_t i = 0; i < ecalHits.size(); i++){
			coordinatesEcal[i].first  = ecalHits[i] -> column;
			coordinatesEcal[i].second = ecalHits[i] -> row;
			// cout<<__LINE__<endl;
		}
		// 使用基于索引的循环遍历所有的坐标
		for (size_t i = 0; i < coordinatesEcal.size(); ++i) {
			if(hasNeighbor[i]) continue;
			int x = coordinatesEcal[i].first;
			int y = coordinatesEcal[i].second;
			// 检查8个相邻位置
			// cout<<__LINE__<endl;
			for (auto &offset : offsets) {
				int dx = offset.first;
				int dy = offset.second;
				pair<int, int> neighbor = {x + dx, y + dy};
				auto it = coords_set.find(neighbor);
				if (it != coords_set.end()) {
					size_t neighborIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), neighbor));
					hasNeighbor[i] = true;
					hasNeighbor[neighborIndex] = true;
					result.push_back(coordinatesEcal[i]);
					break;
				}
			}
		}
		// cout<<__LINE__<endl;

		for(size_t i = 0; i < ecalHits.size(); i++){
			goodChannelEvent[i] = goodChannelEvent[i] && (ecalHits[i] -> column >= 20);
			// goodChannelEvent[i] = goodChannelEvent[i] && (ecalDigitHits[i] -> pulse_time >= cutsConstants::digiHitsPeakPosLowerLimit );
			// goodChannelEvent[i] = goodChannelEvent[i] && (ecalDigitHits[i] -> pulse_time <= cutsConstants::digiHitsPeakPosUpperLimit);
			goodChannelEvent[i] = goodChannelEvent[i] && (hasNeighbor[i]);
		}
	}
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
	cout << "ecalHits Size = " << ecalHits.size() << endl;
	cout << "ecalDigitHits Size = " << ecalDigitHits.size() << endl;
	for (unsigned int i=0;i<ecalHits.size();i++){
	//if good channel in a single event, process next step
		if(!goodChannelEvent[i]) continue;
		col = ecalHits[i]->column;
		row = ecalHits[i]->row;
		cout << "if good channel in a single event, process next step" <<endl;
		pulse_integral = ecalDigitHits[i]->pulse_integral;
		pulse_time     = ecalDigitHits[i]->pulse_time    ;
		pulse_peak     = ecalDigitHits[i]->pulse_peak    ;
		channelNoByColumn      = channelMapByCol[col][row];
		energyBranchVar        = ecalHits[i] -> E;
		timeBranchVar          = ecalHits[i] -> t;
		pulsePeakBranchVar     = ecalDigitHits[i] -> pulse_peak    ;
		pulseIntegralBranchVar = ecalDigitHits[i] -> pulse_integral;
		pulseTimeBranchVar     = ecalDigitHits[i] -> pulse_time    ;
		cosmicRayTree->Fill();
		numberGoodChannelEvents ++;
	}
	// outputFile << endl << endl;
	// outputFile << "ENEVT LOOPED = " << eventNo << endl << endl;
	// eventNo += 1;

	cout << numberGoodChannelEvents << endl;
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
	cout << "****************************\n"<< numberGoodChannelEvents << endl;
	// Called before program exit after event processing is finished.
	cout << numberGoodChannelEvents << endl;
	cout << numberGoodChannelEvents << endl;
	cout << numberGoodChannelEvents << endl;
	cout << numberGoodChannelEvents << endl;
	cout << numberGoodChannelEvents << endl;

	return NOERROR;
}

