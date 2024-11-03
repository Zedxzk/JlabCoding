// $Id$
//
//    File: JEventProcessor_cosmicRayTest.cc
// Created: Mon Oct 14 11:54:33 AM EDT 2024
// Creator: zhikun (on Linux ifarm2401.jlab.org 5.14.0-427.28.1.el9_4.x86_64 x86_64)
//

#include "JEventProcessor_cosmicRayTest.h"
#include "namespaceFucction.h"
using namespace jana;
using namespace std;

// Routine used to create our JEventProcessor
#include <JANA/JApplication.h>
#include <JANA/JFactory.h>
#include <TDirectory.h>
#include <TSystem.h>


using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::seconds;
using std::chrono::duration;
using std::chrono::system_clock;

extern "C"{
void InitPlugin(JApplication *app){
	InitJANAPlugin(app);
	app->AddProcessor(new JEventProcessor_cosmicRayTest());
}
} // "C"


//------------------
// JEventProcessor_cosmicRayTest (Constructor)
//------------------
JEventProcessor_cosmicRayTest::JEventProcessor_cosmicRayTest()
{

}

//------------------
// ~JEventProcessor_cosmicRayTest (Destructor)
//------------------
JEventProcessor_cosmicRayTest::~JEventProcessor_cosmicRayTest()
{

}

//------------------
// init
//------------------
jerror_t JEventProcessor_cosmicRayTest::init(void)
{
	TDirectory *main = gDirectory; // 保存当前的目录
	gDirectory->mkdir("cosmicRay")->cd(); // 创建一个名为 "fcal" 的子目录并切换到该目录

	startTime = std::chrono::high_resolution_clock::now();
	startTimeString = getTimeString(startTime);
	string fileNameTemplates = "coordinates";
	static string fileName; 
	newestFileName = fileNameTemplates + startTimeString ;
	fileName = newestFileName + ".txt";
	// outputFile.open(fileName);
	// if(outputFile.is_open()){
	// 	cout<<"Output file opened successfully!\n\n\n";
	// }
	// else{
	// 	cout << "Output file is not opened!";	
	// }

	// 初始化，需要传递 hist 指针

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
	cout<< "ALL PLOTS INITIALIZED SUCCESSFULLY\n\n\n\n\n";
	// hitsDistrubution = new TH2Poly();
	// hitsDistrubutionCanvas = initHitsDistribution(hitsDistrubution);
	main->cd();
	return NOERROR;
}

//------------------
// brun
//------------------
jerror_t JEventProcessor_cosmicRayTest::brun(JEventLoop *eventLoop, int32_t runnumber)
{
	// This is called whenever the run number changes
	return NOERROR;
}

//------------------
// evnt
//------------------
jerror_t JEventProcessor_cosmicRayTest::evnt(JEventLoop *loop, uint64_t eventnumber)
{
	// if (eventNo == 1){
	// 	outputFile << "LOOP BEGINNING!!!!" << endl << endl;
	// }
	vector<const DECALHit *>ecalhits;
	loop->Get(ecalhits);
	vector<const DFCALHit *>fcalhits;
	loop->Get(fcalhits);
	int col, row;
	Double_t energy, time;
	// outputFile.flush();
	// outputFile << "FCAL TRACK : ";
	for(unsigned int i = 0; i < fcalhits.size(); i++){
		// outputFile
		// << "( " 
		// << std::setw(2) << std::setfill(' ') << std::right << fcalhits[i]->row << " , " 
		// << std::setw(2) << std::setfill(' ') << std::right << fcalhits[i]->column 
		// << " )"
		// << "   ->  ";
		col = fcalhits[i]->column;
		row = fcalhits[i]->row;
		energy = fcalhits[i]->E;
		time = fcalhits[i]->t;
		FcalEnergyRecordMatrix[col][row] += energy;
		FcalTimeRecordMatrix[col][row] += time;
		FcalHitsRecordMatrix[col][row] += 1;
	}
	// outputFile << '\n' << "ECAL TRACK : " ;
	for (unsigned int i=0;i<ecalhits.size();i++){
	// As an example:
	// outputFile 
	// 	<< "( " 
	// 	<< std::setw(2) << std::setfill(' ') << std::right << ecalhits[i]->row << " , " 
	// 	<< std::setw(2) << std::setfill(' ') << std::right << ecalhits[i]->column 
	// 	<< " )"
	// 	<< "   ->  ";
		col = ecalhits[i]->column;
		row = ecalhits[i]->row;
		energy = ecalhits[i]->E;
		time = ecalhits[i]->t;
		EcalEnergyRecordMatrix[col][row] += energy;
		EcalTimeRecordMatrix[col][row] += time;
		EcalHitsRecordMatrix[col][row] += 1;

	}
	// outputFile << endl << endl;
	// outputFile << "ENEVT LOOPED = " << eventNo << endl << endl;
	// eventNo += 1;
	return NOERROR;
}

//------------------
// erun
//------------------
jerror_t JEventProcessor_cosmicRayTest::erun(void)
{
	// This is called whenever the run number changes, before it is
	// changed to give you a chance to clean up before processing
	// events from the next run number.
	return NOERROR;
}

//------------------
// fini
//------------------
jerror_t JEventProcessor_cosmicRayTest::fini(void)
{

	// outputFile.close();
    endTime = system_clock::now();
	endTimeString = getTimeString(endTime).c_str();
    // 创建文件夹
    if (gSystem->mkdir(endTimeString.c_str(), true) == 0) {
        std::cout << "目录创建成功: " << endTimeString << std::endl;
    } 
	else {
        std::cout << "目录创建失败或已存在: " << endTimeString << std::endl;
    }
	TDirectory *main = gDirectory; // 保存当前的目录
	gDirectory->cd("cosmicRay"); // 创建一个名为 "fcal" 的子目录并切换到该目录

	std::cout << "test" << endl;

	testFunction();
	fillHits1DInCorrectPlaceAndPrint(EcalEnergyColumnPlot1D, EcalEnergyRecordMatrix,true, true);
	fillHits1DInCorrectPlaceAndPrint(EcalEnergyRowPlot1D, EcalEnergyRecordMatrix,true, false);
	fillHits1DInCorrectPlaceAndPrint(EcalTimeColumnPlot1D, EcalEnergyRecordMatrix,true, true);
	fillHits1DInCorrectPlaceAndPrint(EcalTimeRowPlot1D, EcalEnergyRecordMatrix,true, false);

	fillHits1DInCorrectPlaceAndPrint(FcalEnergyColumnPlot1D, FcalEnergyRecordMatrix,false, true);
	fillHits1DInCorrectPlaceAndPrint(FcalEnergyRowPlot1D, FcalEnergyRecordMatrix,false, false);
	fillHits1DInCorrectPlaceAndPrint(FcalTimeColumnPlot1D, FcalEnergyRecordMatrix,false, true);
	fillHits1DInCorrectPlaceAndPrint(FcalTimeRowPlot1D, FcalEnergyRecordMatrix,false, false);
	fillHits2DInCorrectPlaceAndPrint(hitsCount2D,EcalHitsRecordMatrix,FcalHitsRecordMatrix);
	fillHits2DInCorrectPlaceAndPrint(hitsTime2D,EcalTimeRecordMatrix, FcalTimeRecordMatrix);
	fillHits2DInCorrectPlaceAndPrint(hitsEnergy2D, EcalEnergyRecordMatrix, FcalEnergyRecordMatrix);
	

    durationTime = duration_cast<seconds>(endTime - startTime);
    std::string timeCost = cosMicRayFunctions::getTimeString(system_clock::now());
    std::cout << "Total Time: " << durationTime.count() << " seconds" << std::endl;

	// ofstream recordOfname("newestFile.txt");
	// recordOfname << newestFileName << endl;
	// recordOfname.close();
	// setHitsDiatributionPlotStyle(hitsDistrubution);
	// hitsDistrubutionCanvas->Print((newestFileName + ".pdf").c_str());
	// hitsDistrubution->Write();
    // 在事件处理完成后调用的函数


    return NOERROR; // 
}

