#ifndef _namespaceFucction_cpp_
#define _namespaceFucction_cpp_
#include "TPaletteAxis.h"
#include "TLatex.h"

const Int_t minEventsPer5Channel = 20;

namespace figuresInCosmicRaysTest {
    void initPlotStyleOfHits2D(plotEcalHits2D& obj);
    void setPlotStyleOfHits1D(plotEcalHits1D& hist1D, bool ecal);
    void setPalatteStyleV1();
    void fillHits1DInCorrectPlaceAndPrint(plotEcalHits1D& obj, vector<vector<Double_t>>& arrayToFill, bool ecal, bool displayInColumn);
    void setPlotStyleOfHits2D(plotEcalHits2D& obj);
    string fileNameTemplates;
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    std::chrono::duration<long> durationTime;
    string startTimeString;
    string endTimeString;
    string durationTimeString;

    
    void testFunction(){
        std :: cout << "test" << endl;
        cout << "test" << endl;
        return ;
    }

void initPlotStyleOfHits2D(plotEcalHits2D& obj) {
    TH2* hist = obj.getHist2D(); 
    TH2Poly* poly = dynamic_cast<TH2Poly*>(hist); 
    TH2D* hist2d = dynamic_cast<TH2D*>(hist); 
    TCanvas* canvas = obj.getCanvas();
    if(poly){
        string title;
        canvas->SetCanvasSize(1200, 1200);
        if (strlen(poly->GetTitle()) == 0 || std::string(poly->GetTitle()) == "NoTitle") {
                title = obj.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : obj.getTitleOfFig();
                obj.setTitleOfFig(title);
                poly->SetTitle(title.c_str());
            }

        if (poly) {
            if (strlen(poly->GetName()) == 0 || std::string(poly->GetName()) == "NoName") {
                poly->SetName(obj.getNameOfFig().c_str());
            }
            if (strlen(poly->GetTitle()) == 0) {
                title = obj.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : obj.getTitleOfFig();
                obj.setTitleOfFig(title);
                poly->SetTitle(title.c_str());
            }
            title = obj.getXAxisName().empty() ? "column" : obj.getXAxisName();
            obj.setXAxisName(title);
            obj.getYAxisName().empty() ? "row" : obj.getYAxisName();
            obj.setXAxisName(title);
            poly->GetXaxis()->SetTitle(obj.getXAxisName().c_str());
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
        else if (hist2d) {
            canvas->SetCanvasSize(1200, 1000);
            string title;
            if (strlen(hist2d->GetName()) == 0 || std::string(hist2d->GetName()) == "NoName") {
                hist2d->SetName(obj.getNameOfFig().c_str());
            }
            if (strlen(hist2d->GetTitle()) == 0) {
                title = obj.getTitleOfFig().empty() ? "DefaultTitle_" + std::to_string(rand()) : obj.getTitleOfFig();
                obj.setTitleOfFig(title);
                hist2d->SetTitle(title.c_str());
            }
            title = obj.getXAxisName().empty() ? "column" : obj.getXAxisName();
            obj.setXAxisName(title);
            title = obj.getYAxisName().empty() ? "row" : obj.getYAxisName();
            obj.setYAxisName(title);
            hist2d->GetXaxis()->SetTitle(obj.getXAxisName().c_str());
            hist2d->GetYaxis()->SetTitle(obj.getYAxisName().c_str());
            hist2d->SetStats(0);

            setPlotStyleOfHits2D(obj);
            hist2d->Draw("COLZ L");
        }
        else {
            std::cerr << "Error: Not a TH2D or TH2Poly instance!" << std::endl;
        }

        
    }

    void setPalatteStyleV1(){
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
    }

    void setPlotStyleOfHits2D(plotEcalHits2D& obj) {
        TH2* hist = obj.getHist2D(); 
        TH2Poly* poly = dynamic_cast<TH2Poly*>(hist); 
        TH2D* hist2d = dynamic_cast<TH2D*>(hist); 
        TCanvas* canvas = obj.getCanvas();
        if(poly){
            Double_t maxVal = poly->GetMaximum();
            poly->SetMinimum(maxVal / 10000);
            gStyle->SetHistMinimumZero(kWhite);
            gStyle->SetHistFillColor(kWhite);
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
            ylabel->DrawLatex(0.1, 0.5, obj.getYAxisName().c_str()); 
            canvas->Modified();
            canvas->Update();
        }
        else if(hist2d){
            gStyle->SetHistMinimumZero(kWhite);
            gStyle->SetHistFillColor(kWhite);
            gPad->SetRightMargin(0.15);
            gPad->SetLeftMargin(0.10);
            gPad->SetBottomMargin(0.15);
            gPad->SetTopMargin(0.10);
            hist2d->GetXaxis()->SetTickLength(0);
            hist2d->GetYaxis()->SetTickLength(0);
            hist2d->GetXaxis()->SetRangeUser(0, EcalChannelNums);
            // gStyle->SetPalette(1);
            gPad->Update();
            // 旋转 y 轴标题
            TLatex *ylabel = new TLatex(); 
            ylabel->SetTextAngle(90); 
            ylabel->SetTextSize(0.04); 
            ylabel->DrawLatex(0.1, 0.5, obj.getYAxisName().c_str()); 
            canvas->Modified();
            canvas->Update();
        }
        else{
        }
        gPad->Update();
        setPalatteStyleV1();
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
            setPalatteStyleV1();
        }
    }



    void fillHits1DInCorrectPlaceAndPrint(plotEcalHits1D& obj, vector<vector<Double_t>>& arrayToFill, bool ecal = false, bool displayInColumn = false) {
        float x = 0.5;
        TH1D* hist = obj.getHist();
        obj.getCanvas()->cd();
        string dir = (ecal ?  "/w/halld-scshelf2101/home/zhikun/geometryFiles/binsECAL.txt"
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
        obj.getCanvas()->Print(("../figures" + endTimeString + "/" +   hist->GetName() + ".pdf").c_str());
        obj.getCanvas()->Print(("./" + endTimeString + "/" +   hist->GetName() + ".pdf").c_str());
        hist->Write();
    }
    
    void fillHits2DInCorrectPlaceAndPrint(plotEcalHits2D&  obj, std::vector<std::vector<Double_t>> EcalArray , std::vector<std::vector<Double_t>> FcalArray ){
        TH2* hist2D = obj.getHist2D(); 
        TH2Poly* poly = dynamic_cast<TH2Poly*>(hist2D); 
        TH2D* hist2d = dynamic_cast<TH2D*>(hist2D); 
        TCanvas* canvas = obj.getCanvas();
        if (poly) {

            // 成功转换为TH2Poly，你现在可以使用poly作为TH2Poly 
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
            obj.getCanvas()->Print(("../figures/" + endTimeString + "/" + poly->GetName() + ".pdf").c_str());
            obj.getCanvas()->Print(("./" + endTimeString + "/" + poly->GetName() + ".pdf").c_str());
            poly->Write();
            } 
            else if (hist2d) {
                // 处理TH2D的情况
                canvas->cd();
                setPlotStyleOfHits2D(obj);
                hist2d->Draw("COLZ L");
                obj.getCanvas()->Print(("../figures/" + endTimeString + "/" + hist2d->GetName() + ".pdf").c_str());
                obj.getCanvas()->Print(("./" + endTimeString + "/" + hist2d->GetName() + ".pdf").c_str());
                hist2d->Write();
            } 
            else { 
                // 该对象不是TH2Poly类型 
                std::cerr << "Error: Not a TH2D or TH2Poly instance!" << std::endl; 
            }
       
       
    }
}

namespace cutsFunctions{
    
    void neighborCut(vector<Int_t> column, vector<Int_t> row, vector<bool> & hasNeighbor, vector<bool> & multiNeighbor, vector<bool> & goodChannelEvent){
            // 假设坐标数组 coords 格式为 {{x1, y1}, {x2, y2}, ...}
            // cut those events without neighbor
            if(column.size() != row.size()){
                cout 
                << "Invalid input of cutsFunctions::neighborCuts()"<< endl;
                exit(1);
            }
            vector<pair<int, int>> coordinatesEcal (column.size(),{100, 100});
            for(size_t i = 0; i < column.size(); i++){
                coordinatesEcal[i].first =  column[0];
                coordinatesEcal[i].second = row[1];
            }
            //  define the neighbros that we want to accept
            vector<pair<int, int>> offsets = {
                {-1,  3}, {0,  3}, { 1,  3},
                {-1,  2}, {0,  2}, { 1,  2},
                {-1,  1}, {0,  1}, { 1,  1},
                {-1,  0},          { 1,  0}, 
                {-1, -1}, {0, -1}, { 1, -1},
                {-1, -2}, {0, -2}, { 1, -2},
                {-1, -3}, {0, -3}, { 1, -3}
            };
            // looking for a neighbor
            // use set to speed up searching
            set<pair<int, int>> coords_set (coordinatesEcal.begin() , coordinatesEcal.end());
            vector<pair<int, int>> result;
            // 使用基于索引的循环遍历所有的坐标
            for (size_t i = 0; i < coordinatesEcal.size(); ++i) {
                // if(hasNeighbor[i]) continue;
                int x = coordinatesEcal[i].first;
                int y = coordinatesEcal[i].second;
                for (auto &offset : offsets) {
                    int dx = offset.first;
                    int dy = offset.second;
                    pair<int, int> neighbor = {x + dx, y + dy};
                    auto it = coords_set.find(neighbor);
                    if (it != coords_set.end()) {
                        size_t neighborIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), neighbor));
                        hasNeighbor[i] = true;
                        hasNeighbor[neighborIndex] = true;
                        // 检查特定邻居 (x-1, y) 和 (x+1, y)
                        pair<int, int> leftNeighbor = {x - 1, y};  // (x-1, y)
                        pair<int, int> rightNeighbor = {x + 1, y};  // (x+1, y)

                        bool foundLeft = (coords_set.find(leftNeighbor) != coords_set.end());
                        bool foundRight = (coords_set.find(rightNeighbor) != coords_set.end());

                        // 如果都找到了，修改 multiNeighbor 标志
                        // if (foundLeft && foundRight) {
                        // 	size_t leftIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), leftNeighbor));
                        // 	size_t rightIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), rightNeighbor));
                        // 	multiNeighbor[i] = true;
                        // 	multiNeighbor[leftIndex] = true;  // 修改 (x-1, y) 对应的 multiNeighbor
                        // 	multiNeighbor[rightIndex] = true;  // 修改 (x+1, y) 对应的 multiNeighbor
                        // }

                        if (foundRight) {
                            size_t leftIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), leftNeighbor));
                            multiNeighbor[i] = true;
                            multiNeighbor[leftIndex] = true;  // 修改 (x-1, y) 对应的 multiNeighbor
                        }

                        if (foundLeft) {
                            size_t rightIndex = distance(coordinatesEcal.begin(), find(coordinatesEcal.begin(), coordinatesEcal.end(), rightNeighbor));
                            multiNeighbor[i] = true;
                            multiNeighbor[rightIndex] = true;  // 修改 (x+1, y) 对应的 multiNeighbor
                        }
                        break;
                    }
                }
            }
            return ;
    }

    void goodTrackIn5Columns(vector<Int_t> column, vector<Int_t> row, vector<bool> & insideNarrowTrack, vector<bool> & goodChannelEvent){
        if(column.size() != insideNarrowTrack.size() || goodChannelEvent.size() != column.size()){
            cout<< "Size of insideNarrowTrack = " << insideNarrowTrack.size() << "; size of Column = " << column.size() << endl;
            cout << "Invalid Input of cutsFunctions::goodTrackIn5Columns(vector<Int_t> column, vector<Int_t> row, vector<bool> & insideNarrowTrack)" << endl;
            exit(1);
        }
        vector<Int_t> eventsPerColumn(sizeOfEcal, 0);
        vector<bool>  goodColumn(sizeOfEcal, false);
        for(Int_t i = 0; i < Int_t(column.size()); i++){
            if(goodChannelEvent[i]){
                eventsPerColumn[i] += 1;
            }
        }
        for(int i = 0; i < sizeOfEcal; i++){
            Int_t sum = eventsPerColumn[i] + eventsPerColumn[i + 1] + eventsPerColumn[i + 2] + eventsPerColumn[i + 3] + eventsPerColumn[i + 4] ;
            if(sum >= minEventsPer5Channel){
                goodColumn[i] = true;
                goodColumn[i + 1] = true;
                goodColumn[i + 2] = true;
                goodColumn[i + 3] = true;
                goodColumn[i + 4] = true;
                // cout 
                // <<"******************" << " i = "  << i  << "******************" << endl
                // << "column " << i << ", events = " << eventsPerColumn[i] << endl
                // << "column " << i + 1 << ", events = " << eventsPerColumn[i + 1] << endl
                // << "column " << i + 2 << ", events = " << eventsPerColumn[i + 2] << endl
                // << "column " << i + 3 << ", events = " << eventsPerColumn[i + 3] << endl
                // << "column " << i + 4 << ", events = " << eventsPerColumn[i + 4] << endl
                // <<"******************" << " i = "  << i  << "******************" << endl
                // ;
            }
        }
        for (int i = 0; i < Int_t(column.size()); i++) {
            if (goodColumn[column[i]]) {
                insideNarrowTrack[i] = true;  // 标记满足条件的列
                goodChannelEvent[i] = goodChannelEvent[i] && insideNarrowTrack[i];
            }
        }
    }

    void goodTrackIn5Columns(vector<Int_t> column,  vector<bool> & goodChannelEvent){
        // cout << sizeOfEcal <<endl;
        if(column.size() !=  goodChannelEvent.size()){
            cout<< "Size of goodChannelEvent = " << goodChannelEvent.size() << "; size of Column = " << column.size() << endl;
            cout << "Invalid Input of cutsFunctions::goodTrackIn5Columns(vector<Int_t> column, vector<Int_t> row, vector<bool> & insideNarrowTrack)" << endl;
            exit(1);
        }
        vector<Int_t> eventsPerColumn(sizeOfEcal, 0);
        vector<bool>  goodColumn(sizeOfEcal, false);

        for(Int_t i = 0; i < Int_t(column.size()); i++){
            if(goodChannelEvent[i]){
                eventsPerColumn[column[i]] += 1;
            }
        }
        for(int i = 0; i < sizeOfEcal; i++){
            Int_t sum = eventsPerColumn[i] + eventsPerColumn[i + 1] + eventsPerColumn[i + 2] + eventsPerColumn[i + 3] + eventsPerColumn[i + 4] ;
            if(sum >= minEventsPer5Channel){
                goodColumn[i] = true;
                goodColumn[i + 1] = true;
                goodColumn[i + 2] = true;
                goodColumn[i + 3] = true;
                goodColumn[i + 4] = true;
                // cout 
                // <<"******************" << " i = "  << i  << "******************" << endl
                // << "column " << i << ", events = " << eventsPerColumn[i] << endl
                // << "column " << i + 1 << ", events = " << eventsPerColumn[i + 1] << endl
                // << "column " << i + 2 << ", events = " << eventsPerColumn[i + 2] << endl
                // << "column " << i + 3 << ", events = " << eventsPerColumn[i + 3] << endl
                // << "column " << i + 4 << ", events = " << eventsPerColumn[i + 4] << endl
                // <<"******************" << " i = "  << i  << "******************" << endl
                // ;
            }
        }
        for (int i = 0; i < Int_t(column.size()); i++) {
            if (goodColumn[column[i]]) {
                goodChannelEvent[i] = goodChannelEvent[i] && true;
            }
            else{
                goodChannelEvent[i] = goodChannelEvent[i] && false;
            }
        }
    }


    void timeWindowCut(vector<Double_t> time, Double_t time1, Double_t time2, vector<bool> & goodChannelEvent ){
            if(time.size() != goodChannelEvent.size()){
                cout 
                << "Invalid input of cutsFunctions::timeWindowCut()"<< endl;
                exit(1);
            }
            for(Int_t i = 0; i < Int_t(time.size()); i++){
                if(goodChannelEvent[i]){
                    if(time[i] < time1){
                        goodChannelEvent[i] = false;
                    }
                    if(time[i] > time2){
                        goodChannelEvent[i] = false;
                    }
                }
            }
    }
}
using namespace figuresInCosmicRaysTest;
using namespace cutsFunctions;
#endif