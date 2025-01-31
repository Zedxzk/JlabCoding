// $Id$
//
//    File: JEventProcessor_cosmicRayTestEvio.cc
// Created: Wed Nov  6 10:26:16 PM EST 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#include "JEventProcessor_cosmicRayTestEvio.h"
using namespace jana;
using namespace cutsConstants;

// const bool printAllAcceptedEventsAfterCuts = true;
const bool printAllAcceptedEventsAfterCuts = false;
Int_t event_id = 0;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
#include <TTree.h>
#include <TSystem.h>
#include <set>

Double_t useless_for_test = 0;
Int_t plotIndex = 1;
Int_t MinEcalSizeToAccept = 8;
Double_t Threshold = 4;
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
	if(printAllAcceptedEventsAfterCuts){
		const char* dirPath = "figures";
		// 检查目录是否存在
		if (gSystem->AccessPathName(dirPath) == 0) {
			std::cout << "Directory exists. Deleting all files inside..." << std::endl;
			
			// 使用系统命令删除目录中的所有文件
			gSystem->Exec(Form("rm -rf %s/*", dirPath));
			std::cout << "All files deleted inside '" << dirPath << "'." << std::endl;
		} else {
			std::cout << "Directory does not exist. Creating it..." << std::endl;
		}

		// 创建目录
		gSystem->Exec(Form("mkdir -p %s", dirPath));
		std::cout << "Directory '" << dirPath << "' is ready for use." << std::endl;
	}



	ecalHitsTree = new TTree("EcalHits", "EcalHits"       );
	ecalHitsTree->Branch("Event_ID"                                   , & event_id               );
	ecalHitsTree->Branch("EcalChannelNo"                              , & channelNoByColumn      );
	ecalHitsTree->Branch("EcalEnergyDistributionByColumn"             , & energyBranchVar        );
	ecalHitsTree->Branch("EcalTimeDistributionByColumn"               , & timeBranchVar          );
	ecalDigitHitsTree = new TTree("EcalDigitHits", "EcalDigitHits" );
	ecalDigitHitsTree->Branch("EcalChannelNo"                              , & digiChannelNoByColumn  );
	ecalDigitHitsTree->Branch("Event_ID"                                   , & event_id    	          );
	ecalDigitHitsTree->Branch("EcalpulsePeakDistributionByColumn"          , & pulsePeakBranchVar     );
	ecalDigitHitsTree->Branch("EcalpulseIntegralDistributionByColumn"      , & pulseIntegralBranchVar );
	ecalDigitHitsTree->Branch("EcalpulseTimeDistributionByColumn"          , & pulseTimeBranchVar     );
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
	// setPlotStyleOfHits1D(EcalTimeColumnPlot1D);
    // setPlotStyleOfHits1D(EcalEnergyColumnPlot1D);
    // setPlotStyleOfHits1D(FcalTimeColumnPlot1D);
    // setPlotStyleOfHits1D(FcalEnergyColumnPlot1D);
    // setPlotStyleOfHits1D(EcalTimeRowPlot1D);
    // setPlotStyleOfHits1D(EcalEnergyRowPlot1D);
    // setPlotStyleOfHits1D(FcalTimeRowPlot1D);
    // setPlotStyleOfHits1D(FcalEnergyRowPlot1D);

	// initPlotStyleOfHits2D(hitsEnergy2D);
	// initPlotStyleOfHits2D(hitsTime2D);
	// initPlotStyleOfHits2D(hitsCount2D);

	// initPlotStyleOfHits2D(rowChannelEnergy2D);
	// initPlotStyleOfHits2D(columnChannelEnergy2D);
	// initPlotStyleOfHits2D(rowChannelTime2D);
	// initPlotStyleOfHits2D(columnChannelTime2D);
	
	// initPlotStyleOfHits2D(rowChannelDigitalPeak2D);
	// initPlotStyleOfHits2D(columnChannelDigitalPeak2D);
	// initPlotStyleOfHits2D(rowChannelDigitalIntegral2D);
	// initPlotStyleOfHits2D(columnChannelDigitalIntegral2D);
	// initPlotStyleOfHits2D(rowChannelDigitalTime2D);
	// initPlotStyleOfHits2D(columnChannelDigitalTime2D);
    gStyle->SetOptStat(0);
  
	gStyle->SetTitleFontSize(0.05);
  
	gStyle->SetTitleFont(130,"xy");
	gStyle->SetLabelFont(130,"xy");
	
	gStyle->SetLineWidth(1.5);
	gStyle->SetTextSize(1.2);
	
	
	gStyle->SetLabelSize(0.04,"xy");
	gStyle->SetTitleSize(0.03,"xy");
	
	
	gStyle->SetTitleOffset(0.5,"x");
	gStyle->SetTitleOffset(0.2,"y");
	
	gStyle->SetTitleBorderSize(0);
	
	
	plot->GetXaxis()->SetTitle("Column");
	plot->GetYaxis()->SetTitle("Row");
	return NOERROR;

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
	vector<Int_t> col_ECAL_digiHits;
	vector<Int_t> row_ECAL_digiHits;
	vector<Double_t> time_ECAL_digiHits;

	vector<const DECALHit *> ecalHits;
	vector<Int_t> col_ECAL_Hits;
	vector<Int_t> row_ECAL_Hits;
	vector<Double_t> time_ECAL_Hits;


	loop->Get(ecalDigitHits);
	loop->Get(ecalHits);

	for(int i = 0; i < Int_t(ecalDigitHits.size()); i++){
		col_ECAL_digiHits.push_back(ecalDigitHits[i]->column);
		row_ECAL_digiHits.push_back(ecalDigitHits[i]->row);
		time_ECAL_digiHits.push_back(ecalDigitHits[i]->pulse_time);
	}

	for(int i = 0; i < Int_t(ecalHits.size()); i++){
		col_ECAL_Hits.push_back(ecalHits[i]->column);
		row_ECAL_Hits.push_back(ecalHits[i]->row);
		time_ECAL_Hits.push_back(ecalHits[i]->t);
	}

	// vector<const DFCALHit *>fcalhits;
	// loop->Get(fcalhits);

	int nGood  = 0;
	int nGoodD = 0;

	int col, row;
	// Double_t pulse_integral, pulse_time, pulse_peak, pedestal ;
	std::vector<bool> goodChannelEvent(ecalHits.size(), true);
	std::vector<bool> hasNeighbor   (ecalHits.size(), false);
	std::vector<bool> multiNeighbor (ecalHits.size(), false);
	std::vector<bool> insideNarrowTrack   (ecalHits.size(), false);

	std::vector<bool> goodChannelEventD(ecalDigitHits.size(), true);
	std::vector<bool> hasNeighborD  (ecalDigitHits.size(), false);
	std::vector<bool> multiNeighborD(ecalDigitHits.size(), false);
	std::vector<bool> insideNarrowTrackD   (ecalDigitHits.size(), false);

	// ************************* Start Cutting condition *****************************
	// cout<<__LINE__<endl;
	// cout << "Event ID = " << plotIndex << endl;
	if(addCuts){
		if(addTimeCuts){
			timeWindowCut(time_ECAL_Hits,cutsConstants:: HitsPeakPosLowerLimit, cutsConstants::HitsPeakPosUpperLimit, goodChannelEvent);
			timeWindowCut(time_ECAL_digiHits, cutsConstants::digiHitsPeakPosLowerLimit, cutsConstants::digiHitsPeakPosUpperLimit, goodChannelEventD);
		}

		// // // neighborCut(col_ECAL_digiHits, row_ECAL_digiHits, hasNeighborD, multiNeighborD);
		// // // neighborCut(col_ECAL_Hits, row_ECAL_Hits, hasNeighbor, multiNeighbor);
		if(addNarrowTracnCut){
			goodTrackIn5Columns(col_ECAL_Hits,goodChannelEvent);
			goodTrackIn5Columns(col_ECAL_digiHits,  goodChannelEventD);
		}

		}

		for(size_t i = 0; i < ecalHits.size(); i++){
			if(goodChannelEvent[i]) nGood ++;
		}
		// cout << "goodChannelEvent = " << goodChannelEvent << endl;

		for(size_t i = 0; i < ecalDigitHits.size(); i++){
			if(goodChannelEventD[i]) nGoodD ++;
		}
	
	// **************************   END cut condition  ************************************
	// ************************************************************************************


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
	// cout << "ecalHits Size = " << ecalHits.size() << endl;
	// cout << "ecalDigitHits Size = " << ecalDigitHits.size() << endl;


	//   *******************        Start to Fill Tree  **********************
	//   *******************      Ecal Hits Tree       ***********************
	for (unsigned int i=0;i<ecalHits.size();i++){
		// cout << "TEST";
	//if no enough good channel in a single event, or the channels that passed cuts are not enough, discard the trigger
		if(Int_t(ecalHits.size()) < MinEcalSizeToAccept) break;
		// cout << "condition 1";
		if(nGood < MinEcalSizeToAccept) break;
		// cout << "condition 2";
	// 	if accept the trigger, then go on
	// 	if the channel did not pass cuts, then it is background, ignore this channel
		if(!goodChannelEvent[i]) continue;
		// cout << "condition 3";
	// 	if it is signal, then fill the tree
		col = ecalHits[i]->column;
		row = ecalHits[i]->row;
		channelNoByColumn      = channelMapByCol[col][row];
		energyBranchVar        = ecalHits[i] -> E;
		timeBranchVar          = ecalHits[i] -> t;
		// cout 
		// << "timeBranchVar = " << timeBranchVar << endl 
		// << "energyBranchVar = " << energyBranchVar <<endl;

		ecalHitsTree->Fill();
		numberGoodChannelEvents ++;
		plot->Fill(col,row,energyBranchVar);
	}

	if(printAllAcceptedEventsAfterCuts){
		my_canvas->cd();
		gPad->SetGrid(1);
		my_canvas->Update();
		if(Int_t(ecalHits.size()) >= MinEcalSizeToAccept && nGood >= MinEcalSizeToAccept){
			// for(int i = 0; i < ecalHits.size(); i++ ){
			// 	i
			// }
			plot->Draw("zcol");
			my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d.pdf", plotIndex));
			// my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d.png", plotIndex));
			my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d.svg", plotIndex));
		}
		my_canvas->Clear();
		plot->Reset();
	}

	// Do the same with Ecal Digit Hit
	//   ******************     ECAL Digit Hits Tree      ***************************
	for (unsigned int i=0;i<ecalDigitHits.size();i++){
		//if no enough good channel in a single event, or the channels that passed cuts are not enough, discard the trigger
		if(Int_t(ecalDigitHits.size()) < MinEcalSizeToAccept) break;
		if(nGoodD < MinEcalSizeToAccept) break;
		// 	if accept the trigger, then go on
		// 	if the channel did not pass cuts, then it is background, ignore this channel
		if(!goodChannelEventD[i]) continue;
		col = ecalDigitHits[i]->column;
		row = ecalDigitHits[i]->row;
		// cout << "if good channel in a single event, process next step" <<endl;
		pulseIntegralBranchVar = ecalDigitHits[i]->pulse_integral;
		pulseTimeBranchVar     = ecalDigitHits[i]->pulse_time    ;
		pulsePeakBranchVar     = ecalDigitHits[i]->pulse_peak - ecalDigitHits[i]->pedestal / 4.0;
		// if(pulsePeakBranchVar < Threshold) continue;
		digiChannelNoByColumn  = channelMapByCol[col][row];
		// if(digiChannelNoByColumn > 800){
		// 	cout << "(col, row) =  "<< col << " , " << row  << endl;
			
		// }

		ecalDigitHitsTree->Fill();
		numberGoodChannelEvents ++;
		plot->Fill(col,row,pulsePeakBranchVar);
	}
	// getchar();
	//    ********************** End filling Tree   ********************************
	if(printAllAcceptedEventsAfterCuts){
		my_canvas->cd();
		gPad->SetGrid(1);
		my_canvas->Update();
		if(Int_t(ecalHits.size()) >= MinEcalSizeToAccept && nGoodD >= MinEcalSizeToAccept){
			plot->Draw("zcol");
			my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d_Digit.pdf", plotIndex));
			// my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d_Digit.png", plotIndex));
			my_canvas->Print(TString::Format("./figures/EcalHitsEventNo_%05d_Digit.svg", plotIndex));
		}
		plotIndex ++;
		my_canvas->Clear();
		plot->Reset();
	}
	// outputFile << endl << endl;
	// outputFile << "ENEVT LOOPED = " << eventNo << endl << endl;
	// eventNo += 1;

	// cout << numberGoodChannelEvents << endl;
	// getchar();
    event_id ++;
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
	cout 
	<< endl << "Entries that passed cuts in total  =  "
	<< numberGoodChannelEvents << endl
	<< "****************************\n\n\n"
	;


	return NOERROR;
}

