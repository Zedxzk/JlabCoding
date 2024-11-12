#include <TCanvas.h>
#include <TH2D.h>
#include <TFile.h>
#include <TImage.h>
#include <TString.h>
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPalette.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunPlotStyle/zhikunPlotConfig.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/figureExtraction.h"

using namespace std;


void colFiguresExtraction() {
    string inputFile = "hd_root.root";
    type = "column"; // 可以切换为 "column"
    Int_t lengthX = 40; // 设置X轴的分段长度
    splitTH2DE(inputFile, lengthX);
    splitTH2DT(inputFile, lengthX);
    return ;
}


