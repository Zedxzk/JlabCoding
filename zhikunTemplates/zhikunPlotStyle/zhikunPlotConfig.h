#ifndef _ZHIKUNPLOTCONFIG_H_
#define _ZHIKUNPLOTCONFIG_H_

#include <RooPlot.h>

namespace zhikunPlotConfig{
    void setPlotConfigV1(){
        gPad->SetRightMargin(0.12);
        gPad->SetLeftMargin(0.12);
        gPad->SetBottomMargin(0.15);
        gPad->SetTopMargin(0.10);
    }

    void setRooFitPlotStyleV1(RooPlot* frame){
        frame->GetXaxis()->CenterTitle();
        frame->GetYaxis()->CenterTitle();
    }

    void setFontTimesNewRoman(){
        // Set ALL fonts as Times New Roman 
        gStyle->SetTextFont(3);        // 设置全局文本字体为 Times New Roman
        gStyle->SetLabelFont(3);       // 设置轴标签字体为 Times New Roman
        gStyle->SetTitleFont(3);       // 设置标题字体为 Times New Roman
        }
}


#endif