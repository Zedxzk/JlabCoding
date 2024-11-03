#ifndef  _FIGURE_EXTRACTION_H_
#define _FIGURE_EXTRACTION_H_

string type;
const double dpi = 400;

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
    hist->GetYaxis()->SetRangeUser(0.005, 0.04); // 设置Y轴范围为0到0.04

    for (Int_t i = 0; i < lengthX; ++i) {
        TCanvas *canvas = new TCanvas(TString::Format("%s_%d.pdf", histName.c_str(), i), TString::Format("%s_%d.pdf", histName.c_str(), i), 1200, 800);
        Int_t startBinX = i * 40 + 1;
        Int_t endBinX = (i + 1) * 40 ;
        hist->GetXaxis()->SetRange(startBinX, endBinX);
        zhikunPalette::setPaletteStyleV1(hist);
        Double_t maxVal = hist->GetMaximum();
        hist->SetMinimum(maxVal / 10000);
        gStyle->SetHistMinimumZero(kWhite);
        gStyle->SetHistFillColor(kWhite);
        zhikunPlotConfig::setPlotConfigV1();
        hist->Draw("COLZ L");
        TString outputFileName1 = TString::Format("../figures/channelScan/%s_%02d.pdf", histName.c_str(), i);
        TString outputFileName2 = TString::Format("./channelScan/%s_%02d.pdf", histName.c_str(), i);        
        TString outputFileName3 = TString::Format("../figures/channelScan/%s_%02d.png", histName.c_str(), i);
        TString outputFileName4 = TString::Format("./channelScan/%s_%02d.png", histName.c_str(), i);
        canvas->Print(outputFileName1.Data());
        canvas->Print(outputFileName2.Data());        
        canvas->Print(outputFileName3.Data());
        canvas->Print(outputFileName4.Data());
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
    hist->GetYaxis()->SetRangeUser(2, 10); // 设置Y轴范围为0到0.04

    for (Int_t i = 0; i < lengthX; ++i) {
        TCanvas *canvas = new TCanvas(TString::Format("%s_%d.pdf", histName.c_str(), i), TString::Format("%s_%d.pdf", histName.c_str(), i), 1200, 800);
        Int_t startBinX = i * 40 + 1;
        Int_t endBinX = (i + 1) * 40 ;
        hist->GetXaxis()->SetRange(startBinX, endBinX);
        Double_t maxVal = hist->GetMaximum();
        hist->SetMinimum(maxVal / 10000);
        gStyle->SetHistMinimumZero(kWhite);
        gStyle->SetHistFillColor(kWhite);
        zhikunPalette::setPaletteStyleV1(hist);
        zhikunPlotConfig::setPlotConfigV1();
        hist->Draw("COLZ L");
        TString outputFileName1 = TString::Format("../figures/channelScan/%s_%02d.pdf", histName.c_str(), i);
        TString outputFileName2 = TString::Format("./channelScan/%s_%02d.pdf", histName.c_str(), i);        
        TString outputFileName3 = TString::Format("../figures/channelScan/%s_%02d.png", histName.c_str(), i);
        TString outputFileName4 = TString::Format("./channelScan/%s_%02d.png", histName.c_str(), i);
        canvas->Print(outputFileName1.Data());
        canvas->Print(outputFileName2.Data());        
        canvas->Print(outputFileName3.Data());
        canvas->Print(outputFileName4.Data());
        delete canvas;
    }
    file->Close();
}

#endif