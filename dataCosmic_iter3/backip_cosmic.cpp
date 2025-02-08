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
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunConstants.h"
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPlotConfig.h"
#include "/work/halld/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPalette.h"

const bool addFit = true;
// const bool addFit = false;
// const bool needOverView = true;
const bool needOverView = false;
const char perfix[] = "PulseAmplitude";

//
const Double_t varLowerLimit       = 0;
const Double_t varUpperLimit       = 40;
const Int_t    varBinsInThisFile   = 40;
const Int_t    binsForFit          = 40;
const Double_t varAcceptLowerLimit = 20;
const Double_t varAcceptUpperLimit = 25;
const Int_t minColumnIndex = 0;
const Int_t maxColumnIndex = 800;
const Int_t binsColumn     = 800;

const Double_t energyLowerLimit = 0;
const Double_t energyUpperLimit = 100;
const Int_t      energyBinsInThisFile = 100;
const Double_t energyAcceptLowerLimit = 20;
const Double_t energyAcceptUpperLimit = 25;
const Double_t chi2_ndfAcceptMaximum = 50.0;
float channelMapByCol[sizeOfEcal][sizeOfEcal] = {0,};



const TString fileName = "allRuns.root";       // The ROOT file to open
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
    landauFunc.SetParameters(1.0, mean.getVal(), sigma.getVal());  // Set parameters

    // Find the maximum of the Landau function
    Double_t peak = landauFunc.GetMaximum();
    Double_t halfMax = peak / 2;

    // Find the points where the function is equal to half of the peak
    Double_t leftX = landauFunc.GetX(halfMax, varLowerLimit, varUpperLimit);  // Left half-maximum point
    Double_t rightX = landauFunc.GetX(halfMax, varLowerLimit, varUpperLimit); // Right half-maximum point

    // Return the FWHM (difference between the two points)
    return rightX - leftX;
}


void channelsFit(TH2D* hist2D, dataType type) {
    if (type == Energy) {
        std::ofstream outFile ("useless.txt",std::ios::out);
        std::ofstream warnFile("useless.txt",std::ios::out);
        if(addFit){
            // Initialize file stream objects using constructor
            std::ofstream outFileInner(textOut.Data(),std::ios::out);
            std::ofstream warnFileInner(textWarn.Data(),std::ios::out);
            outFile  = std::move(outFileInner);
            warnFile = std::move(warnFileInner);
        }
        for (int i = 0; i < binsColumn; i++) {
            TCanvas *c = new TCanvas("c", "c", 2400, 1600);
            int col = 39 - i / 40;
            int row = 39 - i % 40;
            TString histName = TString::Format("col_row_%02d_%02d_", col, row) + perfix;
            TH1D *hist1D = new TH1D(histName, "Channel Number", binsForFit, varLowerLimit, varUpperLimit);
            hist1D->GetXaxis()->SetRangeUser(varLowerLimit, varUpperLimit);
            
            // Temporary projection to get data
            TH1D *tempHist = hist2D->ProjectionY(Form("tempHist_%d", i), i + 1, i + 1);

            // Copy the data from the projection to the custom TH1D
            for (int j = 1; j <= tempHist->GetNbinsX(); ++j) {
                double binContent = tempHist->GetBinContent(j);
                double binCenter = tempHist->GetBinCenter(j);
                hist1D->Fill(binCenter, binContent);
            }

            // Manually set total entries
            hist1D->SetEntries(tempHist->GetEntries());
            gStyle->SetOptStat("e");  // e: entries, m: mean, r: rms (mean and standard deviation)
            hist1D->SetStats(kTRUE);  // Enable statistics box     
            hist1D->GetXaxis()->SetTitle("Energy deposition/MeV");
            hist1D->GetYaxis()->SetTitle(TString::Format("Events / %.1fMeV/c^{2}",  hist1D->GetXaxis()->GetBinWidth(1)));
            if(addFit){
                RooRealVar mean("mean", "mean", 8, varLowerLimit, varUpperLimit);
                RooRealVar sigma("sigma", "sigma", 3, 1e-4, 5);
                RooRealVar a0("a0", "a0", 0.0, -1.0, 1.0); // Chebyshev polynomial coefficient
                RooRealVar a1("a1", "a1", 0.5, -1.0, 1.0); // Chebyshev polynomial coefficient
                RooRealVar x("x", "x", varLowerLimit, varUpperLimit);

                RooLandau landau("landau", "landau", x, mean, sigma);
                RooChebychev cheb("cheb", "cheb", x, RooArgList(a0));

                RooRealVar frac("frac", "frac", 0.5, 0.0, 1.0);  // Weight parameter
                RooAddPdf model("model", "landau + cheb", RooArgList(landau, cheb), RooArgList(frac));

                RooDataHist data("data", "data", x, hist1D);
                model.fitTo(data);

                RooPlot* frame = x.frame();
                frame->SetTitle(TString::Format("Channel %4d   (col, row) = (%2d, %2d)", i, col , row).Data());
                frame->SetTitleSize(0.05);   // Set title font size (adjust as needed)
                frame->SetTitleOffset(1.0);  // Control the distance between title and plot
                data.plotOn(frame);
                model.plotOn(frame);
                model.plotOn(frame, RooFit::Components("cheb"), RooFit::LineStyle(kDashed));
                int dof = 32;  // Assuming you know the degrees of freedom
                RooChi2Var chi2("chi2", "chi2", model, data);

                // Calculate chi2/dof
                double chi2_dof = frame->chiSquare(3);  // 3 is the number of model parameters

                // Create a text box to display chi2/dof
                TPaveText *pt = new TPaveText(0.60, 0.70, 0.89, 0.89, "BRNDC");
                pt->SetBorderSize(0);
                pt->SetFillColor(0);
                pt->SetTextAlign(12);  // Center text
                pt->SetTextSize(0.03);
                pt->SetTextFont(42);
                // Display chi2/dof
                pt->AddText(TString::Format("#chi^{2}/dof=%2.2f/%d=%2.1f", chi2_dof * dof, dof, chi2_dof));
                model.paramOn(frame, RooFit::Parameters(RooArgSet(mean,sigma)), RooFit::Layout(0.6, 0.9,0.7));
                                        
                zhikunPlotConfig::setRooFitPlotStyleV1(frame);
                frame->SetXTitle("Energy deposition/MeV");            
                frame->SetYTitle(TString::Format("Events / %.1fMeV/c^{2}",  hist1D->GetXaxis()->GetBinWidth(1)));
                frame->Draw();
                c->Update();
                pt->Draw();
                c->SaveAs(outputDir + histName.Data() + "_FIT.pdf");
                c->SaveAs(outputDir + histName.Data() + "_FIT.png");
                
                Double_t width = estimateFWHM(landau, mean, sigma);
                bool warn = false;
                warn = mean.getVal() < varAcceptLowerLimit || mean.getVal() > varAcceptUpperLimit || (chi2_dof >= chi2_ndfAcceptMaximum);
                if(warn){
                    warnFile << std::setw(4) << std::setfill(' ') << i <<
                    "    " << "(" 
                    << std::setw(3) << std::setfill(' ') << col <<" , "
                    << std::setw(3) << std::setfill(' ') << row << ")    " 
                    << TString::Format("%6.3f MeV    ",mean.getVal())
                    << chi2_dof << std::endl;}

                outFile 
                << "Channel :"
                << std::setw(4) << std::setfill(' ') << i <<
                "    " << "(" 
                << std::setw(3) << std::setfill(' ') << col <<" , "
                << std::setw(3) << std::setfill(' ') << row << ")    " 
                << TString::Format("MEAN: %6.3f MeV    ",mean.getVal())
                << "chi2/dof = " << chi2_dof << std::endl
                << "Width =  " << width << std:: endl;
                delete c;
                delete hist1D;
                
            }
            else{
                gPad->SetGrid(0);

                hist1D->GetXaxis()->CenterTitle();
                hist1D->GetYaxis()->CenterTitle();

                // Set statistics box display content
                hist1D->SetTitle(Form("Channel %4d   (col, row) = (%2d, %2d)", i, col, row));
                // Enable statistics box
                hist1D->SetStats(kTRUE);
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

    // Associate tree branches with variables
    tree->SetBranchAddress(branNameChannelNo.Data(), &channelIndex);
    tree->SetBranchAddress(branNameVar.Data(), &energy);

    TH2D* hist2d = new TH2D("hist2d", "", binsColumn, minColumnIndex, maxColumnIndex, varBinsInThisFile, varLowerLimit, varUpperLimit);
    hist2d->GetXaxis()->SetTitle("Channel Number");
    hist2d->GetYaxis()->SetTitle("Energy deposition/MeV");
    hist2d->GetXaxis()->CenterTitle();
    hist2d->GetYaxis()->CenterTitle();
    tree->Project("hist2d", Form("%s:%s", branNameVar.Data(), branNameChannelNo.Data()));

    if(needOverView){
        TH2D* overview = new TH2D("overview", "", sizeOfEcal, 0, sizeOfEcal, sizeOfEcal, 0, sizeOfEcal);
        TCanvas* tempCanvas = new TCanvas("", "", 2400, 1600);
        // Set palette (using default palette here, you can choose others as needed)
        gStyle->SetPalette(kBird);
        // Limit Z axis range
        hist2d->SetMinimum(0);
        hist2d->SetMaximum(20);
        hist2d->Draw("COLZ");
        tempCanvas->Print("test.pdf");
        tempCanvas->Print("test.png");
        // Iterate over X axis range, outputting an image every 40 bins
        for (int i = 0; i < 800; i += 40) {
            // Set user range, limit x and y axis display range
            hist2d->GetXaxis()->SetRangeUser(i, i + 40);

            // Create temporary canvas and draw histogram
            TCanvas* tempCanvas = new TCanvas("", "", 2400, 2400);
            hist2d->Draw("COLZ");

            // Set axis labels and titles
            hist2d->GetXaxis()->SetTitle("Channel Number");
            hist2d->GetYaxis()->SetTitle("Energy deposition/MeV");
            hist2d->GetXaxis()->CenterTitle();
            hist2d->GetYaxis()->CenterTitle();
            hist2d->SetTitle(Form("column %02d", 39 - i / 40));

            // Save image
            tempCanvas->Print(Form("./columnView/column_%02d.png", 39 - i / 40));
            tempCanvas->Print(Form("./columnView/column_%02d.pdf", 39 - i / 40));

            // Clean up temporary canvas
            delete tempCanvas;
        }
        TCanvas* tempCanvas2 = new TCanvas("", "", 2400, 1600);
        // Loop through all entries
        const char* dirPath = "columnView";

        // Check if directory exists
        if (gSystem->AccessPathName(dirPath) == 0) {
            std::cout << "Directory exists. Deleting all files inside..." << std::endl;
            
            // Use system command to delete all files in the directory
            gSystem->Exec(Form("rm -rf %s/*", dirPath));
            std::cout << "All files deleted inside '" << dirPath << "'." << std::endl;
        } else {
            std::cout << "Directory does not exist. Creating it..." << std::endl;
        }

        // Create directory
        gSystem->Exec(Form("mkdir -p %s", dirPath));
        std::cout << "Directory '" << dirPath << "' is ready for use." << std::endl;
        Long64_t nEntries = tree->GetEntries();
        for (Long64_t i = 0; i < nEntries; ++i) {
            tree->GetEntry(i);
            col = 39 - channelIndex / 40;
            row = 39 - channelIndex % 40;
            overview->Fill(col, row);
        }
        tempCanvas2->cd();
        gPad->SetGrid();
        zhikunPalette::setPaletteStyleV1(overview);
        overview->GetXaxis()->SetTitle("column");
        overview->GetYaxis()->SetTitle("row");
        overview->GetZaxis()->SetTitle("entries");
        overview->GetXaxis()->CenterTitle();
        overview->GetYaxis()->CenterTitle();
        overview->GetZaxis()->CenterTitle();
        overview->Draw("COLZ");
        tempCanvas2->Print("overview.pdf");
        tempCanvas2->Print("overview.png");
    }
    
    channelsFit(hist2d, Energy);

    fileInPtr->Close();
    delete fileInPtr;
}
