#ifndef _ZHIKUNPLOTCONFIG_H_
#define _ZHIKUNPLOTCONFIG_H_


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
}
#endif