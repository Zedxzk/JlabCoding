#ifndef _ZHIKUNPALETTE_H_
#define _ZHIKUNPALETTE_H_ 

#include "TPaletteAxis.h"

namespace zhikunPalette{
    void setPaletteStyleV1(TH2* hist){
        gPad->Update();
        const Int_t NRGBs = 5;  // 现在有五个颜色过渡点
        const Int_t NCont = 255;  // 用于细化颜色条的颗粒度

        // 颜色过渡点
        Double_t stops[NRGBs] = {0.00, 0.25, 0.50, 0.75, 1.00};

        Double_t red[NRGBs]   = {0.80, 0.30, 1.00, 1.00, 1.00};
        Double_t green[NRGBs] = {0.90, 1.00, 1.00, 0.45, 0.00};
        Double_t blue[NRGBs]  = {1.00, 0.00, 0.00, 0.00, 0.00};
        TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        gStyle->SetNumberContours(NCont);
        
        gPad->Update();
        TPaletteAxis *palette = (TPaletteAxis*)hist->GetListOfFunctions()->FindObject("palette");
        if (palette) {
            palette->SetX1NDC(0.89);
            palette->SetX2NDC(0.94);
            palette->SetY1NDC(0.10);
            palette->SetY2NDC(0.90);
        }
        Double_t maxVal = hist->GetMaximum();
        hist->SetMinimum(maxVal / 100);
        gStyle->SetHistMinimumZero(kWhite);
        gStyle->SetHistFillColor(kWhite);
        gPad->Update();
    }
}
#endif