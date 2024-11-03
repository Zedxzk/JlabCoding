#ifndef _namespaceFucction_cpp_
#define _namespaceFucction_cpp_
#include "JEventProcessor_cosmicRayTest.h"
#include "TPaletteAxis.h"
#include "TLatex.h"

namespace figuresInCosmicRaysTest {

    void testFunction(){
        std :: cout << "test" << endl;
        cout << "test" << endl;
        return ;
    }

void initPlotStyleOfHits2D(plotEcalHits2D& hist2D) {
    string title;
    TCanvas* canvas = hist2D.getCanvas();
    canvas->SetCanvasSize(1200, 1200);
    TH2Poly* poly = hist2D.getPoly();

    if (strlen(poly->GetTitle()) == 0 || std::string(poly->GetTitle()) == "NoTitle") {
            title = hist2D.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : hist2D.getTitleOfFig();
            hist2D.setTitleOfFig(title);
            poly->SetTitle(title.c_str());
        }

    if (poly) {
        if (strlen(poly->GetName()) == 0 || std::string(poly->GetName()) == "NoName") {
            poly->SetName(hist2D.getNameOfFig().c_str());
        }
        if (strlen(poly->GetTitle()) == 0) {
            title = hist2D.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : hist2D.getTitleOfFig();
            hist2D.setTitleOfFig(title);
            poly->SetTitle(title.c_str());
        }
        title = hist2D.getXAxisName().empty() ? "column" : hist2D.getXAxisName();
        hist2D.setXAxisName(title);
        hist2D.getYAxisName().empty() ? "row" : hist2D.getYAxisName();
        hist2D.setXAxisName(title);
        poly->GetXaxis()->SetTitle(hist2D.getXAxisName().c_str());
        poly->SetStats(0);
    

        std::ifstream binFile1("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt");
        std::ifstream binFile2("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsFCAL.txt");
        std::string line;

        while (std::getline(binFile1, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            double  col, row, xmin, xmax, ymin, ymax;
            if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; }
            poly->AddBin(xmin, ymin, xmax, ymax);
        }

        while (std::getline(binFile2, line)) {
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            double xmin, ymin, xmax, ymax, col, row;
            if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; }
            poly->AddBin(xmin, ymin, xmax, ymax);
        }
    }
    }

    void setPlotStyleOfHits2D(plotEcalHits2D& hist2D) {
        TCanvas* canvas = hist2D.getCanvas();
        TH2Poly* poly = hist2D.getPoly();
        Double_t maxVal = poly->GetMaximum();
        poly->SetMinimum(maxVal / 10000);

        gStyle->SetHistMinimumZero(kWhite);
        gStyle->SetHistFillColor(kWhite);

        // const Int_t NRGBs = 3;
        // const Int_t NCont = 255;
        // Double_t stops[NRGBs] = {0.00, 0.50, 1.00};
        // Double_t red[NRGBs]   = {1.00, 0.50, 0.00};   // 浅蓝色接近白色
        // Double_t green[NRGBs] = {1.00, 0.70, 0.00};   // 浅蓝色接近白色
        // Double_t blue[NRGBs]  = {0.00, 0.00, 0.00};   // 从蓝色到白色
                // 定义预设颜色索引

        // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        // gStyle->SetNumberContours(NCont);
        // 定义颜色过渡点
        const Int_t NRGBs = 5;  // 现在有五个颜色过渡点
        const Int_t NCont = 255;  // 用于细化颜色条的颗粒度

        // 颜色过渡点
        Double_t stops[NRGBs] = {0.00, 0.25, 0.50, 0.75, 1.00};

        // 颜色分量：增加绿色和橙色过渡
        // Double_t red[NRGBs]   = {0.80, 0.00, 1.00, 1.00, 1.00};
        // Double_t green[NRGBs] = {0.90, 1.00, 1.00, 0.55, 0.00};
        // Double_t blue[NRGBs]  = {1.00, 0.00, 0.00, 0.00, 0.00};

        Double_t red[NRGBs]   = {0.80, 0.30, 1.00, 1.00, 1.00};
        Double_t green[NRGBs] = {0.90, 1.00, 1.00, 0.45, 0.00};
        Double_t blue[NRGBs]  = {1.00, 0.00, 0.00, 0.00, 0.00};

        // 创建渐变颜色表
        TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);

        // 设置颜色条的颗粒度
        gStyle->SetNumberContours(NCont);

        // Int_t colors[] = {0, 7, 3, 5, 2};
        // Int_t NCont = 5; // 细化颜色数量
        // Int_t NRGBs = sizeof(colors) / sizeof(Int_t);
        // const Int_t Number = 5; // 定义关键色数量
        // Double_t Red[Number]   = {0.0, 0.0, 0.7, 0.3, 0.2};  // 红色分量
        // Double_t Green[Number] = {0.0, 0.7, 0.3, 0.5, 0.2};  // 绿色分量
        // Double_t Blue[Number]  = {0.0, 0.3, 0.5, 0.2, 0.2};  // 蓝色分量
        // Double_t Length[Number] = {0.00, 0.25, 0.50, 0.75, 1.00};  // 颜色位置


        // const Int_t Number = 5; // 定义关键色数量
        // Double_t Red[Number]   = {0.0, 0.0, 0.7, 0.3, 0.2};  // 红色分量
        // Double_t Green[Number] = {0.0, 0.7, 0.3, 0.5, 0.2};  // 绿色分量
        // Double_t Blue[Number]  = {0.0, 0.3, 0.5, 0.2, 0.2};  // 蓝色分量
        // Double_t Length[Number] = {0.00, 0.25, 0.50, 0.75, 1.00};  // 颜色位置

        // 创建自定义调色板（255 色）
        // const Int_t NCont = 255;
        // Int_t palette[NCont];
        // TColor::CreateGradientColorTable(Number, Length, Red, Green, Blue, NCont);
        // gStyle->SetNumberContours(NCont);



        // 创建渐变颜色表
        // TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
        gStyle->SetNumberContours(NCont);
        gPad->SetRightMargin(0.15);
        gPad->SetLeftMargin(0.10);
        gPad->SetBottomMargin(0.15);
        gPad->SetTopMargin(0.10);
        poly->GetXaxis()->SetTickLength(0);
        poly->GetYaxis()->SetTickLength(0);
        // gStyle->SetPalette(1);
        gPad->Update();
        
        // 旋转 y 轴标题
        TLatex *ylabel = new TLatex(); 
        ylabel->SetTextAngle(90); 
        ylabel->SetTextSize(0.04); 
        ylabel->DrawLatex(0.1, 0.5, hist2D.getYAxisName().c_str()); 
        // TPaletteAxis *palette = (TPaletteAxis*)poly->GetListOfFunctions()->FindObject("palette");
        // if (palette) {
        //     palette->SetX1NDC(0.85); // 调整颜色条位置和大小
        //     palette->SetX2NDC(0.98);
        //     palette->SetY1NDC(0.1);
        //     palette->SetY2NDC(0.9);
        // }    
        // else{
        //     for(int i = 1; i < 20 ; i++){
        //         cout << "SETTING PALLATE ERROT!!!!\n";
        //     }
        // }
        canvas->Modified();
        canvas->Update();
    }

    void setPlotStyleOfHits1D(plotEcalHits1D& hist1D, bool ecal = false) {
        // 获取1D直方图
        TH1D* hist = hist1D.getHist();

        if (hist) {
            // 设置图表名称
            // hist->SetName(hist1D.getNameOfFig().c_str());

            // 检查并设置标题
            if (strlen(hist->GetTitle()) == 0) {
                std::string title = hist1D.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : hist1D.getTitleOfFig();
                hist1D.setTitleOfFig(title);
                hist->SetTitle(title.c_str());
            }

            // 设置 X 轴标题
            hist->GetXaxis()->SetTitle(hist1D.getXAxisName().c_str());
            hist->GetXaxis()->CenterTitle(true);  // X轴标题居中
            hist->GetXaxis()->SetTitleSize(0.04); // X轴标题字体大小
            hist->GetXaxis()->SetTitleOffset(1.2); // X轴标题偏移

            // 设置 Y 轴标题
            hist->GetYaxis()->SetTitle(hist1D.getYAxisName().c_str());
            hist->GetYaxis()->CenterTitle(true);  // Y轴标题居中
            hist->GetYaxis()->SetTitleSize(0.04); // Y轴标题字体大小
            hist->GetYaxis()->SetTitleOffset(0.2); // Y轴标题偏移

            // 隐藏统计框
            hist->SetStats(0);

            // 设置填充颜色，背景为空白
            hist->SetFillColor(kBlue - 10);  // 设置填充色为浅蓝色
            hist->SetLineColor(kBlue);       // 边线为深蓝色
            hist->SetLineWidth(2);           // 边线宽度
        }
    }



    void fillHits1DInCorrectPlaceAndPrint(plotEcalHits1D& obj, vector<vector<Double_t>>& arrayToFill, bool ecal = false, bool displayInColumn = false) {
        float x = 0.5;
        TH1D* hist = obj.getHist();
        obj.getCanvas()->cd();
        string dir = (ecal ? "/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt"
                                   : "/w/halld-scshelf2101/home/zhikun/geometryFiles/binsFCAL.txt");
        // cout << dir << endl;
        std::ifstream binFile(ecal ? "/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt"
                                   : "/w/halld-scshelf2101/home/zhikun/geometryFiles/binsFCAL.txt");
        setPlotStyleOfHits1D(obj, ecal);
        std::string line;
        int col, row, col_index, row_index;
        Float_t temp1, temp2, temp3, temp4;
        std::ofstream outputFile("test.txt",std::ios::app);
        // outputFile << "TEST" << endl;
        int rounds = 0;
        while (std::getline(binFile, line)) {
            rounds ++;
            // cout  << rounds << endl;
            if (line.empty() || line[0] == '#') continue;
            std::istringstream iss(line);
            if (!(iss >> col >> row >> temp1 >> temp2 >>temp3 >> temp4)) { break; }
            if (displayInColumn) {
                if (ecal) {
                    col_index = sizeOfEcal - row - 1;
                    row_index = sizeOfEcal - col - 1;
                } else {
                    col_index = sizeOfFcal - row - 1;
                    row_index = sizeOfEcal - col - 1;
                }
            } else {
                col_index = col;
                row_index = row;
            }
            hist->Fill(x, arrayToFill[col_index][row_index]);
            x++;
        }
        hist->Draw();
        obj.getCanvas()->Print(("./" + endTimeString + "/" +   hist->GetName() + ".pdf").c_str());
        hist->Write();
    }
    
    void fillHits2DInCorrectPlaceAndPrint(plotEcalHits2D&  obj, std::vector<std::vector<Double_t>> EcalArray , std::vector<std::vector<Double_t>> FcalArray ){
        TH2Poly* poly = obj.getPoly();
        TCanvas* canvas = obj.getCanvas();
        canvas->cd();
        std::ifstream binFile1("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt");
        std::ifstream binFile2("/w/halld-scshelf2101/home/zhikun/geometryFiles/binsFCAL.txt");
        std::string line;
        // Fill Ecal bins;
         while (std::getline(binFile1, line)) {
                if (line.empty() || line[0] == '#') continue;
                std::istringstream iss(line);
                double  col, row, xmin, xmax, ymin, ymax;
                if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; }
                poly->Fill(xmin +  EcalLength /2, ymin + FcalLength / 2, EcalArray[col][row]);
            }
        //Fill Fcal bins;
            while (std::getline(binFile2, line)) {
                if (line.empty() || line[0] == '#') continue;
                std::istringstream iss(line);
                double xmin, ymin, xmax, ymax, col, row;
                if (!(iss >> col >> row >> xmin >> ymin >> xmax >> ymax)) { break; }
                poly->Fill(xmin +  FcalLength /2, ymin + FcalLength / 2, FcalArray[col][row]);
            }
        binFile1.close();
        binFile2.close();
        setPlotStyleOfHits2D(obj);
        poly->Draw("COLZ L");        
        obj.getCanvas()->Print(("./" + endTimeString + "/" + poly->GetName() + ".pdf").c_str());
        poly->Write();
    }
}



#endif