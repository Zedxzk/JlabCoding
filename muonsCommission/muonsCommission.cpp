#include <TFile.h>
#include <iostream>
#include <fstream>
#include <string>   
#include <RooRealVar.h>
#include <RooGaussian.h>
#include <RooAddPdf.h>
#include <TText.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2D.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooFitResult.h>
#include <RooPlot.h>
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooFFTConvPdf.h>
#include <RooFormulaVar.h>
#include <RooChi2Var.h>
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPalette.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPlotConfig.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/figureExtraction.h"

//  *******************   Parameters input **********************
const string fileDir  = "../inputFiles/"
const string fileName = "hd_root.root";
const strinf fileIn   = fileDir + fileName;
const string outputDir = "./muonCommision/"


void muonCommision(){
    TFile* rootIn = 
    
}