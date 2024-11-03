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

const int sizeOfEcal = 40;
const int sizeOfFcal = 59;
const int FcalChannelNums = 2364;
const int EcalChannelNums = 1600;
const int maxTime = 200;
const int timeResolution = 4;
const int timeBins = maxTime * 10;
const int maxEnergy = 1;
const int minEnergy = 0;
const int energyBins = maxEnergy * 1000;
const Float_t EcalLength = 2.1;
const Float_t FcalLength = 4;
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

	// TCanvas*  initHitsDistribution(TH2Poly* h2poly){
	// 	// 读取分bin规则
	// 	std::ifstream binFile1("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt");
	// 	std::ifstream binFile2("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsFCAL.txt");
	// 	std::string line;
	// 	while (std::getline(binFile1, line)) {
	// 		// 忽略注释行
	// 		if (line.empty() || line[0] == '#') continue;
			
	// 		std::istringstream iss(line);
	// 		double xmin, ymin, xmax, ymax, col, row;
	// 		if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; } // 读取失败则退出
	// 		h2poly->AddBin(xmin, ymin, xmax, ymax);
	// 	}
	// 	while (std::getline(binFile2, line)) {
	// 		// 忽略注释行
	// 		if (line.empty() || line[0] == '#') continue;
			
	// 		std::istringstream iss(line);
	// 		double xmin, ymin, xmax, ymax, col, row;
	// 		if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; } // 读取失败则退出
	// 		h2poly->AddBin(xmin, ymin, xmax, ymax);
	// 	}

	// 	h2poly->Draw("COLZ L");
	// 	// 调整画布的边距，确保右边的颜色条不影响正方形区域
	// 	gPad->SetRightMargin(0.15);  // 适当调整右边的空白区域，可以根据需要调整这个值
	// 	gPad->SetLeftMargin(0.15);   // 确保左边的留白均衡
	// 	gPad->SetBottomMargin(0.15); // 确保下方留白均衡
	// 	gPad->SetTopMargin(0.10);    // 顶部留白
	// 	// 隐藏 X 轴和 Y 轴的刻度标记
	// 	h2poly->GetXaxis()->SetTickLength(0);  // 隐藏 X 轴刻度
	// 	h2poly->GetYaxis()->SetTickLength(0);  // 隐藏 Y 轴刻度
	// 	// 更新画布
	// 	c1->Update();
	// 	return c1;
	// }

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
	void setHitsDiatributionPlotStyle(TH2Poly* h2poly){
		h2poly->SetName("cosmicRayHits"); // 更新对象名称
		h2poly->SetTitle("cosmicRayHits"); // 更新对象标题

		// Hide Stat
		gStyle->SetOptStat(0);
		// Get Maximun
		Double_t maxVal = h2poly->GetMaximum();

		// 设置 Z 轴最小值为最大值的 1/10000
		h2poly->SetMinimum(maxVal / 10000);

		// 创建调色板，使用预定义颜色
		gStyle->SetHistMinimumZero(kWhite);  // 允许直方图的空 bin 显示为背景色
		gStyle->SetHistFillColor(kWhite);   // 设置背景色为白色


		// 自定义颜色渐变，最低为浅蓝色，最高为红色
		const Int_t NRGBs = 3;
		const Int_t NCont = 255;  // 用于细化颜色条的颗粒度
		Double_t stops[NRGBs] = {0.00, 0.50, 1.00};  // 颜色过渡点
		Double_t red[NRGBs]   = {0.70, 1.00, 1.00};  // 红色分量，最低值有一些红色分量用于浅蓝色
		Double_t green[NRGBs] = {0.90, 1.00, 0.00};  // 绿色分量，浅蓝色有较高的绿分量
		Double_t blue[NRGBs]  = {1.00, 0.00, 0.00};  // 蓝色分量，最低的为浅蓝色，最高的为红色
		TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
		gStyle->SetNumberContours(NCont);  // 细化颜色条
	}
	void fillHits1DInCorrectPlaceAndPrint(plotEcalHits1D&  obj, Double_t* arrayToFill, bool, bool );
	void fillHits2DInCorrectPlaceAndPrint(plotEcalHits2D&  obj, std::vector<std::vector<Double_t>>, std::vector<std::vector<Double_t>>);
}


namespace cosMicRayFunctions {
    // 获取当前时间的字符串格式
    std::string getTimeString(std::chrono::system_clock::time_point time) {
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(time);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y_%m_%d_%H.%M.%S");
        return oss.str();
    }

};


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

using namespace cosMicRayFunctions;
using namespace globalVariables;
using namespace figuresInCosmicRaysTest;

#endif // _JEventProcessor_cosmicRayTest_

