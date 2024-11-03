#include <TCanvas.h>
#include <TH2D.h>
#include <TFile.h>
#include <TImage.h>
#include <TString.h>

using namespace std;

string type = "row"; // 可以切换为 "column"

void splitTH2DE(const string &fileName, Int_t lengthX) {
    TFile *file = TFile::Open(fileName.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    file->cd("cosmicRay");
    string histName = type + "ChannelEnergy2D";
    TH2D *hist = (TH2D*)gDirectory->Get(histName.c_str());

    if (!hist) {
        std::cerr << "Failed to get histogram" << std::endl;
        file->Close();
        return;
    }

    Int_t nbinsX = hist->GetNbinsX();
    Int_t nbinsPerFileX = nbinsX / lengthX;

    for (Int_t i = 0; i < lengthX; ++i) {
        TCanvas *canvas = new TCanvas(TString::Format("%s_%d.pdf", histName.c_str(), i), TString::Format("%s_%d.pdf", histName.c_str(), i), 800, 600);
        Int_t startBinX = i * nbinsPerFileX;
        Int_t endBinX = (i + 1) * nbinsPerFileX - 1;
        hist->GetXaxis()->SetRange(startBinX, endBinX);
        hist->GetYaxis()->SetRangeUser(0, 0.04); // 设置Y轴范围为0到0.04
        hist->Draw("COLZ L");
        TString outputFileName = TString::Format("channelScan/%s_%d.pdf", histName.c_str(), i);
        canvas->Print(outputFileName.Data());
        delete canvas;
    }
    file->Close();
}

void splitTH2DT(const string &fileName, Int_t lengthX) {
    TFile *file = TFile::Open(fileName.c_str(), "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Failed to open file" << std::endl;
        return;
    }

    file->cd("cosmicRay");
    string histName = type + "ChannelTime2D";
    TH2D *hist = (TH2D*)gDirectory->Get(histName.c_str());

    if (!hist) {
        std::cerr << "Failed to get histogram" << std::endl;
        file->Close();
        return;
    }

    Int_t nbinsX = hist->GetNbinsX();
    Int_t nbinsPerFileX = nbinsX / lengthX;

    for (Int_t i = 0; i < lengthX; ++i) {
        TCanvas *canvas = new TCanvas(TString::Format("%s_%d.pdf", histName.c_str(), i), TString::Format("%s_%d.pdf", histName.c_str(), i), 800, 600);
        Int_t startBinX = i * nbinsPerFileX;
        Int_t endBinX = (i + 1) * nbinsPerFileX - 1;
        hist->GetXaxis()->SetRange(startBinX, endBinX);
        hist->GetYaxis()->SetRangeUser(0, 0.04); // 设置Y轴范围为0到0.04
        hist->Draw("COLZ L");
        TString outputFileName = TString::Format("channelScan/%s_%d.pdf", histName.c_str(), i);
        canvas->Print(outputFileName.Data());
        delete canvas;
    }
    file->Close();
}

void extractFigures() {
    string inputFile = "hd_root.root";
    Int_t lengthX = 40; // 设置X轴的分段长度
    splitTH2DE(inputFile, lengthX);
    splitTH2DT(inputFile, lengthX);
    return ;
}


