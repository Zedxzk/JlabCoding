#include <TFile.h>
#include <iostream>
#include <fstream>
#include <TString.h>
#include <TH2D.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <RooRealVar.h>
#include <RooLandau.h>
#include <RooChebychev.h>
#include <RooAddPdf.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooChi2Var.h>
#include <TLatex.h>
#include <RooFit.h>
#include <string>
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunConstants.h"
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPlotConfig.h"
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPalette.h"

using namespace std;
const bool needOverView = true;
// const bool needOverView = false;
const bool needColumnView = true;
// const bool needColumnView = false;
const bool needChannelView = true;
//  const bool needChannelView = false;
// const bool addFit = true;
const bool addFit = false;
const bool convolveGaussian = true;
// const bool convolveGaussian = false;
// const bool addBkg = false;
// #define addBkg
const bool addBkg = true;
// const bool addBkg = false;

const char perfix[] = "PulseAmplitude";
const char varName[] = "Amplitude/ADC Counts";
const char anotherAxisName[] = "Events / %.1f ADC Counts";
const char outputFileTitle[] = "// index column row  mean  error sigma   chi2/ndf width\n";
const char warnFileTitle[]   = "// index column row  mean  error sigma   chi2/ndf width  status\n";
string dirPath = "./columnView/";
string columnViewName =  dirPath + "column_%02d";
//
const Double_t varLowerLimit       = 0;
const Double_t varUpperLimit       = 30;
const Int_t    varBinsInThisFile   = 30;
const Int_t    binsForFit          = 30;
const Double_t varAcceptLowerLimit = 7;
const Double_t varAcceptUpperLimit = 10;
const Int_t minColumnIndex = 0;
const Int_t maxColumnIndex = 1600;
const Int_t binsColumn     = 1600;
const Double_t maxZ        = 80;
const Int_t    cutOffThreshold     = 0;
const Double_t initialMeanInAMP  = 10;
const Double_t initialMeanOutAMP =  8;
const Double_t energyLowerLimit = 0;
const Double_t energyUpperLimit = 100;
const Int_t      energyBinsInThisFile = 100;
const Double_t energyAcceptLowerLimit = 20;
const Double_t energyAcceptUpperLimit = 25;
const Double_t chi2_ndfAcceptMaximum = 50.0;
float channelMapByCol[sizeOfEcal][sizeOfEcal] = {0,};



const TString fileName = "outputRootFiles/allRuns.root";       // The ROOT file to open
// const TString fileName = "hd_root.root";       // The ROOT file to open
const TString treeName = "EcalDigitHits";       // The ROOT file to open


const TString branNameChannelNo = "EcalChannelNo";
const TString branNameVar    = "EcalpulsePeakDistributionByColumn";
const TString outputDir = "./DigitHits/";        // Create a directory for results

const TString textOutName = "res.txt";        
const TString textOutWarnName = "possible_bad_res.txt";   
const TString textOut = outputDir + textOutName;             // Output file recording the position of peaks and chi2
const TString textWarn = outputDir + textOutWarnName;

enum dataType {   
    Energy = 1,
    Time   = 2
};


Double_t estimateFWHM(RooLandau& landau, RooRealVar& mean, RooRealVar& sigma) {  
    // Define a TF1 function for the Landau distribution
    TF1 landauFunc("landauFunc", "[0] * TMath::Landau(x, [1], [2])", varLowerLimit, varUpperLimit);
    Double_t normValue = landau.getParameters(RooArgSet(mean, sigma))->getRealValue("norm");
    landauFunc.SetParameters(normValue, mean.getVal(), sigma.getVal());  // Set parameters

    // Find the maximum of the Landau function
    Double_t peak = landauFunc.GetMaximum();
    Double_t halfMax = peak / 2;

    // Find the points where the function is equal to half of the peak
    Double_t leftX = landauFunc.GetX(halfMax, varLowerLimit, mean.getVal());  // Left half-maximum point
    Double_t rightX = landauFunc.GetX(halfMax, mean.getVal(), varUpperLimit); // Right half-maximum point

    // Return the FWHM (difference between the two points)
    return rightX - leftX;
}


void channelsFit(TH2D* hist2D, dataType type) {
    // std :: cout << energyUpperLimit - varLowerLimit << "           "  << energyBinsWidth << std :: endl;
    if (type == Energy) {
        std::ofstream outFile ("useless.txt",std::ios::out);
        std::ofstream warnFile("useless.txt",std::ios::out);
        if(addFit){
                // 使用构造函数初始化文件流对象
            std::ofstream outFileInner(textOut.Data(),std::ios::out);
            std::ofstream warnFileInner(textWarn.Data(),std::ios::out);
            outFile  = std::move(outFileInner);
            warnFile = std::move(warnFileInner);
            outFile<< outputFileTitle;
            warnFile<< warnFileTitle;
        }
        // for (int i = 0; i < 50; i++) {
        for (int i = 0; i < binsColumn; i++) {
            // zhikunPlotConfig::setFontTimesNewRoman();
            TCanvas *c = new TCanvas("c", "c", 1200, 800);
            int col = 39 - i / 40;
            // if(col <= 0)  col --;
            int row = 39 - i % 40;
            // if(row <= 0) row--;
            TString histName = TString::Format("col_row_%02d_%02d_", col, row) + perfix;
            TH1D *hist1D = new TH1D(histName, "Channel Number", binsForFit, varLowerLimit, varUpperLimit);
            hist1D->GetXaxis()->SetRangeUser(varLowerLimit, varUpperLimit);
            
            // 临时投影以获取数据
            TH1D *tempHist = hist2D->ProjectionY(Form("tempHist_%d", i), i + 1, i + 1);

            // 将投影的数据逐条复制到自定义的 TH1D
            for (int j = 1; j <= tempHist->GetNbinsX(); ++j) {
                double binContent = tempHist->GetBinContent(j);
                double binCenter = tempHist->GetBinCenter(j);
                hist1D->Fill(binCenter, binContent);
            }

            // 手动设置 total entries
            hist1D->SetEntries(tempHist->GetEntries());
            gStyle->SetOptStat("e");  // e: entries, m: mean, r: rms (均值和标准差)
            hist1D->SetStats(kTRUE);  // 启用统计框     
            // // 从hist2D中填充hist1D
            // for (int j = 1; j <= energyBinsInThisFile; j++) {
            //     hist1D->SetBinContent(j, hist2D->GetBinContent(i + 1 , j));
            // }
            hist1D->GetXaxis()->SetTitle("Peak Position/");
            hist1D->GetYaxis()->SetTitle(TString::Format(anotherAxisName,  hist1D->GetXaxis()->GetBinWidth(1)));

            if(addFit){
            Double_t initialMean;
            // 判断是否在中心区域
            if (col >= 10 && col <= 29 && row >= 10 && row <= 29) {
                initialMean = initialMeanInAMP;
            } else {
                initialMean = initialMeanOutAMP;
            }
            RooRealVar mean("mean", "mean", initialMean, 5, varUpperLimit);
            RooRealVar sigma("sigma", "sigma", 3, 1e-4, 5);
            RooRealVar a0("a0", "a0", 0.0, -1.0, 1.0); // 切比雪夫多项式的一阶系数
            RooRealVar a1("a1", "a1", 0.5, -1.0, 1.0); // 切比雪夫多项式的一阶系数
            RooRealVar x("x", "x", varLowerLimit, varUpperLimit);

            // RooRealVar gaussMean("gaussMean", "Mean of Gaussian", 0, -5, 5);
            RooRealVar gaussMean("gaussMean", "Mean of Gaussian", 0, -1, 1);
            RooRealVar gaussSigma("gaussSigma", "Width of Gaussian", 1, 1e-5, 10);
            RooGaussian gauss("gauss", "Gaussian PDF", x, gaussMean, gaussSigma);

            RooLandau landau("landau", "landau", x, mean, sigma);
            RooFFTConvPdf landauGauss("landauGauss", "Landau (X) Gaussian", x, landau, gauss);
            RooChebychev cheb("cheb", "cheb", x, RooArgList(a0));

            RooAbsPdf* signalModel = nullptr;

            if (convolveGaussian) {
                // 使用卷积模型
                signalModel = new RooFFTConvPdf("landauGauss", "Landau (X) Gaussian", x, landau, gauss);
                // std::cout << "Using convoluted Landau + Gaussian model." << std::endl;
            } else {
                // 直接使用Landau模型
                signalModel = &landau;
                // std::cout << "Using Landau model without convolution." << std::endl;
            }

            RooRealVar frac("frac", "frac", 1.0, 0.0, 1.0);  // 加权参数
            RooDataHist data("data", "data", x, hist1D);
            // RooAddPdf model("model", "landau + cheb", RooArgList(landau, cheb), RooArgList(frac));
            RooAddPdf model("model", "signal + cheb", RooArgList(*signalModel, cheb), RooArgList(frac));

            model.fitTo(data);

            if(!addBkg){
                frac.setConstant();
            }
            // model.fitTo(data,RooFit::Extended(kTRUE));
            RooFitResult* result = model.fitTo(data,RooFit::Save(kTRUE),RooFit::Extended(kTRUE));

            // model.fitTo(data);

            RooPlot* frame = x.frame();
            frame->SetTitle(TString::Format("Channel %4d   (col, row) = (%2d, %2d)", i, col , row).Data());
            frame->SetTitleSize(0.05);   // 设置标题的字体大小（根据需要调整）
            frame->SetTitleOffset(1.0);  // 控制标题与图形的距离
            data.plotOn(frame);
            model.plotOn(frame);
            model.plotOn(frame, RooFit::Components("cheb"), RooFit::LineStyle(kDashed));
            // if (frame->GetHist()) {
            //     frame->GetHist()->SetStats(kTRUE);  // 显示统计信息
            //     gStyle->SetOptStat("e");  // 选择显示条目、均值和标准差
            // }

            // // 获取绘制的对象
            // TH1* hist = dynamic_cast<TH1*>(frame->getObject(0));  // getObject(0) 返回第一个绘制对象

            // // 如果对象是 TH1 类型，设置统计信息
            // if (hist) {
            //     hist->SetStats(kTRUE);  // 启用统计框
            // }
            int dof = 32;  // 这里假设你已经知道自由度的数量
            RooChi2Var chi2("chi2", "chi2", model, data);

            // 计算 chi2/dof
            double chi2_dof = frame->chiSquare(3);  // 这里的3是模型参数的个数

            // 创建文本框来显示 chi²/dof 和条目数
            TPaveText *pt = new TPaveText(0.60, 0.70, 0.9, 0.80, "BRNDC");
            pt->SetBorderSize(0);
            pt->SetFillColor(0);
            pt->SetTextAlign(12);  // 文本居中
            pt->SetTextSize(0.04);
            pt->SetTextFont(132);

            // 获取数据集的条目数
            int entries = tempHist->GetEntries();

            // 显示 chi²/dof 和条目数
            pt->AddText(TString::Format("#chi^{2}/dof = %2.2f/%d = %2.1f", chi2_dof * dof, dof, chi2_dof));
            pt->AddText(TString::Format("Entries = %d", entries));

            model.paramOn(frame, RooFit::Parameters(RooArgSet(mean,sigma)), RooFit::Layout(0.6, 0.9, 0.7));

            zhikunPlotConfig::setRooFitPlotStyleV1(frame);
            frame->SetXTitle(varName);            
            frame->SetYTitle(TString::Format(anotherAxisName,  hist1D->GetXaxis()->GetBinWidth(1)));
            frame->Draw();
            c->Update();
            pt->Draw();
            if(convolveGaussian){
                c->SaveAs(outputDir + histName.Data() + "_FIT_Convolved_Gaussian.pdf");
                c->SaveAs(outputDir + histName.Data() + "_FIT_Convolved_Gaussian.png");
            }
            else{
                c->SaveAs(outputDir + histName.Data() + "_FIT.pdf");
                c->SaveAs(outputDir + histName.Data() + "_FIT.png");
            }

            
            Double_t width = estimateFWHM(landau, mean, sigma);
            // std::cout<<__LINE__<<endl;
            bool warn = false;
            warn = mean.getVal() < varAcceptLowerLimit || mean.getVal() > varAcceptUpperLimit || (chi2_dof >= chi2_ndfAcceptMaximum);
            // std::cout<<__LINE__<<endl;
            warn = warn || width >=12 || (result == nullptr) || (result->status() != 0);
            if(warn){
                warnFile 
                << std::setw(4) << std::setfill(' ') << i 
                << std::setw(5) << std::setfill(' ') << col 
                << std::setw(5) << std::setfill(' ') << row 
                << TString::Format("%8.3f   ",mean.getVal()).Data()
                << TString::Format("%6.3f   ",mean.getError()).Data()
                << TString::Format("%6.3f   ",sigma.getVal()).Data()
                << TString::Format("%6.3f   ",chi2_dof).Data()
                << TString::Format("%7.3f   ",width).Data() 
                << ((result == nullptr )? "nullptr" : ((result->status() != 0) ? "SUCCESS" : "FAILED" ))
                << std:: endl;
            }
            // std::cout<<__LINE__<<endl;

            outFile 
            << std::setw(4) << std::setfill(' ') << i 
            << std::setw(5) << std::setfill(' ') << col 
            << std::setw(5) << std::setfill(' ') << row 
            << TString::Format("%8.3f   ",mean.getVal()).Data()
            << TString::Format("%8.3f   ",mean.getError()).Data()
            << TString::Format("%6.3f   ",sigma.getVal()).Data()
            << TString::Format("%6.3f   ",chi2_dof).Data()
            << TString::Format("%7.3f   ",width).Data() << std:: endl;
            delete c;
            delete hist1D;
            
            }
            else{
                gPad->SetGrid(0);

                hist1D->GetXaxis()->CenterTitle();
                hist1D->GetYaxis()->CenterTitle();

                // 设置统计框显示内容
                hist1D->SetTitle(Form("Channel %4d   (col, row) = (%2d, %2d)", i, col, row));
                // 启用统计框
                hist1D->SetStats(kTRUE);
                // hist1D->SetErrorOption("e"); // "e" 表示显示误差棒
                hist1D->Draw("HIST");
                c->Print(outputDir + histName.Data() + ".pdf");
                c->Print(outputDir + histName.Data() + ".png");
                delete c;
                delete hist1D;
            }
            }
            outFile.close();
            warnFile.close();

    }
}

void cosmicExtraction() {   
    for(int i = 0;i < sizeOfEcal ; i++){
		for(int j = 0; j < sizeOfEcal; j++){
			channelMapByCol[j][i] = (39 - j) * 40 + (39 - i) + 0.5;
		}
    }
    // Initialization
    gSystem->Load("libRooFit");   // Load RooFit library
    gSystem->Load("libHist");     // Load the ROOT Hist library if necessary
    gSystem->Load("libCore");     // Load the ROOT Core library
    gSystem->Load("libGraf");     // Load the ROOT Graphics library

    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetStripDecimals(kFALSE);
    gSystem->mkdir(outputDir.Data(), kTRUE);

    TFile *fileInPtr = TFile::Open(fileName.Data());
    if (!fileInPtr || fileInPtr->IsZombie()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    TTree *tree = (TTree*)fileInPtr->Get(treeName.Data());
    if (!tree) {
        std::cerr << "Failed to get TTree: cosmicRayDistributions" << std::endl;
        return;
    }
    std::cout << "Successfully retrieved TTree: cosmicRayDistributions" << std::endl;

    Int_t channelIndex;
    double energy;
    int col, row;

    // 将树的分支关联到变量
    tree->SetBranchAddress(branNameChannelNo.Data(), &channelIndex);
    tree->SetBranchAddress(branNameVar.Data(), &energy);


    // RooRealVar channelInexEntries  (branNameChannelNo.Data(),branNameChannelNo.Data(),minColumnIndex    , maxColumnIndex      );
    // RooRealVar EnergyEntries       (branNameVar.Data()   ,branNameVar.Data()   ,varLowerLimit , energyUpperLimit     );
    // RooDataSet dataSet("dataSet", "Data Set", RooArgSet(channelIndexEntries, EnergyEntries));


    TH2D* hist2d   = new TH2D("hist2d"  ,"", binsColumn, minColumnIndex,maxColumnIndex, varBinsInThisFile, varLowerLimit, varUpperLimit );
    hist2d->GetXaxis()->SetTitle("Channel Number");
    hist2d->GetYaxis()->SetTitle(varName);
    hist2d->GetXaxis()->CenterTitle();
    hist2d->GetYaxis()->CenterTitle();
    // tree->Project("hist2d", Form("%s:%s", branNameVar.Data(), branNameChannelNo.Data()));
    tree->Project("hist2d", Form("%s:%s", branNameVar.Data(), branNameChannelNo.Data()), Form("%s>%d", branNameVar.Data(), cutOffThreshold));
    // tree->Project("hist2d", Form("%s:%s", branNameVar.Data(), branNameChannelNo.Data()), Form("%s > 4", branNameVar.Data()));
    if(needOverView){
        // 检查目录是否存在
        if (gSystem->AccessPathName(dirPath.c_str()) == 0) {
            std::cout << "Directory exists. Deleting all files inside..." << std::endl;
            
            // 使用系统命令删除目录中的所有文件
            gSystem->Exec(Form("rm -rf %s/*", dirPath.c_str()));
            std::cout << "All files deleted inside '" << dirPath << "'." << std::endl;
        } else {
            std::cout << "Directory does not exist. Creating it..." << std::endl;
        }
        // 创建目录
        gSystem->Exec(Form("mkdir -p %s", dirPath.c_str()));
        std::cout << "Directory '" << dirPath.c_str() << "' is ready for use." << std::endl;
        TH2D* overview = new TH2D("overview","", sizeOfEcal, 0, sizeOfEcal, sizeOfEcal, 0, sizeOfEcal );
        TCanvas* tempCanvas = new TCanvas("","", 2400, 1600);
        // 设置调色板（这里使用默认调色板，你可以根据需要选择其他调色板）
        gStyle->SetPalette(kBird);
        // 限制 Z 轴的范围
        hist2d->SetMinimum(0);
        hist2d->SetMaximum(maxZ);
        hist2d->Draw("COLZ");
        tempCanvas->Print("channels_Overview.pdf");
        tempCanvas->Print("channels_Overview.png");
            // 遍历 X 轴范围，每 40 个 bin 输出一个图像
        if(needColumnView){
            for (int i = 0; i < maxColumnIndex; i += 40) {
                // 设置用户范围，限制 x 轴和 y 轴的显示范围
                hist2d->GetXaxis()->SetRangeUser(i, i + 40);

                // 创建临时画布并绘制直方图
                TCanvas* tempCanvas = new TCanvas("", "", 2400, 2400);
                hist2d->Draw("COLZ");

                // 设置轴标签和标题
                hist2d->GetXaxis()->SetTitle("Channel Number");
                hist2d->GetYaxis()->SetTitle(varName);
                hist2d->GetXaxis()->CenterTitle();
                hist2d->GetYaxis()->CenterTitle();
                hist2d->SetTitle(Form("column %02d",39 -  i / 40));
                // gStyle->SetTitleAlign(23);  //
                // 保存图像
                tempCanvas->Print(Form((columnViewName+".pdf").c_str(),39 -  i / 40));
                tempCanvas->Print(Form((columnViewName+".png").c_str(),39 -  i / 40));

                // 清理临时画布
                delete tempCanvas;
            }
        }
        TCanvas* tempCanvas2 = new TCanvas("","", 2000, 1600);

        Long64_t nEntries = tree->GetEntries();
        int nums = 0;
        int num2 = 0;
        for (Long64_t i = 0; i < nEntries; ++i) {
            tree->GetEntry(i);
            col = 39 - channelIndex / 40;
            // if(col <= 0)  col --;
            row = 39 - channelIndex % 40;
            if(col == 6 && row == 21){
                num2++;
            }
            if(energy < 4) { 
                nums++;
                continue;
            }
            overview->Fill(col,row);
            // std::cout << "Entry " << i << ": Channel Index = " << channelIndex 
            // << ", col, row = " << col << ", " << row 
            // << ", Energy = " << energy << std::endl;
        }
        // cout << "Total number of entries with energy < 4: " << nums << endl;
        // cout << "Total number of entries in (6,21): " << num2 << endl;
        // cout << "Total number of entries with energy >= 4: " << nEntries - nums << endl;
        TH2D  * h2 = overview;
        h2->GetXaxis()->SetNdivisions(440); // 主刻度5个，次刻度每个主刻度分5份
        h2->GetXaxis()->SetLabelSize(0.015); // 设置刻度标签的大小
        // h2->GetXaxis()->SetLabelOffset(0.01); // 设置刻度标签的偏移
        h2->GetXaxis()->SetLimits(-0.5, 39.5); // 原始范围为 [0, 10]，向右平移 0.5 单位
    // 设置刻度线长度
        h2->GetXaxis()->SetTickLength(0.001); // 设置 X 轴刻度线长度为 0.05
        h2->LabelsOption("v");
        // 自定义 Y 轴刻度
        h2->GetYaxis()->SetNdivisions(440); // 主刻度5个，次刻度每个主刻度分10份
        h2->GetYaxis()->SetLabelSize(0.015);
        // h2->GetYaxis()->SetLabelOffset(0.01);
        h2->GetYaxis()->SetLimits(-0.5, 39.5); // 原始范围为 [0, 10]，向右平移 0.5 单位
        h2->GetYaxis()->SetTickLength(0.001); // 设置 Y 轴刻度线长度为 0.02
        
        // cout << overview->GetBinContent(6,21);
        tempCanvas2->cd();
        gPad->SetGrid();
        overview->GetXaxis()->SetTitle("column");
        overview->GetYaxis()->SetTitle("row");
        overview->GetZaxis()->SetTitle("entries");
        overview->GetXaxis()->CenterTitle();
        overview->GetYaxis()->CenterTitle();
        overview->GetZaxis()->CenterTitle();
        // tempCanvas2->SetPad(0.1, 0.1, 0.5, 0.5);  // 保持边距，不覆盖调色板
        // TPad *pad1 = new TPad("pad1", "Pad for 2D Histogram", 0.1, 0.1, 0.9, 0.9);
        // pad1->Draw();
        // pad1->cd();
        tempCanvas2->SetMargin(0.08, 0.20, 0.10, 0.05); // 左边距 0.1, 右边距 0.05, 下边距 0.1, 上边距 0.05
        overview->Draw("COLZ");
        zhikunPalette::setPaletteStyleV2(overview);

        tempCanvas2->Print("ECAL_overview.pdf");
        tempCanvas2->Print("ECAL_overview.png");
    }
    if(needChannelView){
        channelsFit(hist2d, Energy);
    }

    fileInPtr->Close();
    delete fileInPtr;
}
