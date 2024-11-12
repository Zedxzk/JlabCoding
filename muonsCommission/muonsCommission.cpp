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
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunConstants.h"
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPlotConfig.h"

const Double_t energyLowerLimit = 0.005;
const Double_t energyUpperLimit = 0.040;
const int      energyBinsInThisFile = int ((energyUpperLimit - energyLowerLimit) / energyBinsWidth);
const Double_t energyAcceptLowerLimit = 0.020;
const Double_t energyAcceptUpperLimit = 0.025;
const Double_t chi2_ndfAcceptMaximum = 50.0;




enum dataType {   
    Energy = 1,
    Time   = 2
};

const TString fileName = "../inputFiles/hd_root.root";       // The ROOT file to open
const TString outputDir = "./muonsCommissionOutput/";        // Create a directory for results
const TString hist2DName = "columnChannelEnergy2D";          // Name of hist2D to read from root file

const TString textOutName = "res.txt";        
const TString textOutWarnName = "possible_bad_res.txt";   
const TString textOut = outputDir + textOutName;             // Output file recording the position of peaks and chi2
const TString textWarn = outputDir + textOutWarnName;

void channelsFit(TH2D* hist2D, dataType type) {
    // std :: cout << energyUpperLimit - energyLowerLimit << "           "  << energyBinsWidth << std :: endl;
    if (type == Energy) {
        std::ofstream outFile(textOut.Data());
        std::ofstream warnFile(textWarn.Data());



        // for (int i = 0; i < 50; i++) {
        for (int i = 1; i <= EcalChannelNums; i++) {
            // zhikunPlotConfig::setFontTimesNewRoman();
            TCanvas *c = new TCanvas("c", "c", 800, 600);
            int col = 39 - i / 40;
            // if(col <= 0)  col --;
            int row = 39 - i % 40;
            // if(row <= 0) row--;
            TString histName = TString::Format("col_row_%02d_%02d_EnergyDeposit", col, row);
            TH1D *hist1D = new TH1D(histName, "Channel Number", energyBinsInThisFile, energyLowerLimit, energyUpperLimit);

            // 从hist2D中填充hist1D

            for (int j = 1; j <= energyBinsInThisFile; j++) {
                hist1D->SetBinContent(j, hist2D->GetBinContent(i + 1, j + 5));
            }

            hist1D->GetXaxis()->SetRangeUser(energyLowerLimit, energyUpperLimit);

            RooRealVar mean("mean", "mean", 0.023, energyLowerLimit, energyUpperLimit);
            RooRealVar sigma("sigma", "sigma", 0.001, 1e-4, 0.1);
            RooRealVar a0("a0", "a0", 0.0, -1.0, 1.0); // 切比雪夫多项式的一阶系数
            RooRealVar a1("a1", "a1", 0.5, -1.0, 1.0); // 切比雪夫多项式的一阶系数
            RooRealVar x("x", "x", energyLowerLimit, energyUpperLimit);

            RooLandau landau("landau", "landau", x, mean, sigma);
            RooChebychev cheb("cheb", "cheb", x, RooArgList(a0));

            RooRealVar frac("frac", "frac", 0.5, 0.0, 1.0);  // 加权参数
            RooAddPdf model("model", "landau + cheb", RooArgList(landau, cheb), RooArgList(frac));

            RooDataHist data("data", "data", x, hist1D);
            model.fitTo(data);

            RooPlot* frame = x.frame();
            frame->SetTitle(TString::Format("Channel %d   (col, row) = (%2d, %2d)", i, col , row).Data());
            frame->SetTitleSize(0.05);   // 设置标题的字体大小（根据需要调整）
            frame->SetTitleOffset(1.0);  // 控制标题与图形的距离
            data.plotOn(frame);
            model.plotOn(frame);
            model.plotOn(frame, RooFit::Components("cheb"), RooFit::LineStyle(kDashed));


            int dof = 32;  // 这里假设你已经知道自由度的数量
            RooChi2Var chi2("chi2", "chi2", model, data);

            // 计算 chi2/dof
            double chi2_dof = frame->chiSquare(3);  // 这里的3是模型参数的个数

            // 创建文本框来显示 chi2/dof
            TPaveText *pt = new TPaveText(0.60, 0.70, 0.89, 0.890, "BRNDC");
            pt->SetBorderSize(0);
            pt->SetFillColor(0);
            pt->SetTextAlign(12);  // 文本居中
            pt->SetTextSize(0.03);
            pt->SetTextFont(42);
            // 显示 chi2/dofs
            pt->AddText(TString::Format("#chi^{2}/dof=%2.2f/%d=%2.1f", chi2_dof * dof, dof, chi2_dof));

            // 将文本框绘制在画布上


            
            zhikunPlotConfig::setRooFitPlotStyleV1(frame);
            frame->SetXTitle("Energy deposition/GeV");
            frame->SetYTitle(TString::Format("Events / %.1fMeV/c^{2}",  hist1D->GetYaxis()->GetBinWidth(1)));
            frame->Draw();
            c->Update();
            pt->Draw();
            c->SaveAs(outputDir + histName.Data() + ".pdf");
            
            
            bool warn = false;
            warn = mean.getVal() < energyAcceptLowerLimit || mean.getVal() > energyAcceptUpperLimit || (chi2_dof >= chi2_ndfAcceptMaximum);
            if(warn){
                warnFile << std::setw(4) << std::setfill(' ') << i <<
                "    " << "(" 
                << std::setw(3) << std::setfill(' ') << col <<" , "
                << std::setw(3) << std::setfill(' ') << row << ")    " 
                << TString::Format("%6.3f MeV    ",mean.getVal()*1000)
                << chi2_dof << std::endl;
            }

            outFile 
            << "Channel :"
            << std::setw(4) << std::setfill(' ') << i <<
            "    " << "(" 
            << std::setw(3) << std::setfill(' ') << col <<" , "
            << std::setw(3) << std::setfill(' ') << row << ")    " 
            << TString::Format("MEAN: %6.3f MeV    ",mean.getVal()*1000)
            << "chi2/dof = " << chi2_dof << std::endl;
            delete c;
            delete hist1D;
        }
        outFile.close();
        warnFile.close();
    }
}

void muonsCommission() {   
    // Initialization
    gSystem->Load("libRooFit");
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetStripDecimals(kFALSE);
    gSystem->mkdir(outputDir.Data(), kTRUE);

    TFile *fileInPtr = TFile::Open(fileName.Data());
    if (!fileInPtr || fileInPtr->IsZombie()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }

    if (fileInPtr->cd("cosmicRay")) {
        std::cout << "Successfully changed to cosmicRay directory." << std::endl;
    } else {
        std::cerr << "Failed to change to cosmicRay directory." << std::endl;
        return;
    }

    std::cout << "Attempting to get histogram: " << hist2DName << std::endl;
    TH2D *hist2D = (TH2D*)gDirectory->Get(hist2DName.Data());
    if (!hist2D) {
        std::cerr << "Failed to get histogram: " << hist2DName << std::endl;
        return;
    } else {
        std::cout << "Successfully accessed histogram: " << hist2DName << std::endl;
    }
    // int i = 1;
    // TString histName = TString::Format("channelNumber_%04d_fit", i);
    // cout << histName.Data() << std ::endl;

    channelsFit(hist2D, Energy);

    fileInPtr->Close();
    delete fileInPtr;
}
