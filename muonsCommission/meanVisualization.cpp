#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1F.h"
#include <fstream>
#include <vector>
#include <string>
#include "TString.h"
#include "/w/halld-scshelf2101/home/zhikun/zhikunTemplates/zhikunConstants.h"



const char xLabel[] = "Row";
const char yLabel[] = "Peak Amplitude Mean/ADC Counts";
const char title[]  = "Column %02d";

const int channels = 800;    // should be the size of "res.txt"
const int amplifierRigon[2][2] = {{10, 29}, {10, 29}};
const Double_t markerSize = 3;
const Double_t errorSize = 2;
const Double_t lineWidth = 2;


std::string outputPattern = "./columnView/meanViewCol_%02d";

void SetGraphErrors(TGraphErrors* graph, const std::vector<double>& x, const std::vector<double>& yMeans, const std::vector<double>& yErrors) {
    for (int i = 0; i < yErrors.size(); i++) {
        // std::cout << "Setting point: (" << x[i] << ", " << yMeans[i] << ")" << std::endl;
        // getchar();
        graph->SetPoint(i, x[i], yMeans[i]);
        graph->SetPointError(i, 0.0, yErrors[i]);
    }
}

void meanVisualization() {
    TGraphErrors *graph = new TGraphErrors();
    TCanvas* canvas = new TCanvas("", "", 2400, 1600);
    Double_t index, col, row, mean, meanErr;
    std::vector<Double_t> indexVector, colVector, rowVector, meanVector, meanErrVector;
    std::vector<Double_t> indexVectorA, colVectorA, rowVectorA, meanVectorA, meanErrVectorA;
    
    std::ifstream inputFile("./DigitHits/res.txt");
    std::string line;

    // 检查文件是否成功打开
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    for (int i = 0; i < channels; i++) {
        std::getline(inputFile, line);
        std::stringstream ss(line);
        if (ss >> index >> col >> row >> mean >> meanErr) {
            if (col < amplifierRigon[0][0] || col > amplifierRigon[0][1]) {
                indexVector.push_back(index);
                colVector.push_back(col);
                rowVector.push_back(row);
                meanVector.push_back(mean);
                meanErrVector.push_back(meanErr);
            } 
            else {
                if (row < amplifierRigon[0][0] || row > amplifierRigon[0][1]) {
                    indexVector.push_back(index);
                    colVector.push_back(col);
                    rowVector.push_back(row);
                    meanVector.push_back(mean);
                    meanErrVector.push_back(meanErr);
                } 
                else {
                    indexVectorA.push_back(index);
                    colVectorA.push_back(col);
                    rowVectorA.push_back(row);
                    meanVectorA.push_back(mean);
                    meanErrVectorA.push_back(meanErr);
                }
            }
        } 
        else {
            std::cerr << "Failed to parse line: " << line << std::endl;
        }
        
        if (colVectorA.size() + colVector.size()  == 40)  {
            // 如果已经有数据，绘制图形
            if (!colVectorA.empty()) {
                TCanvas  * canvas = new TCanvas("", "", 1600, 1200);
                TGraphErrors* graph = new TGraphErrors;
                canvas->cd();
                canvas->Clear();
                graph->Clear();
                SetGraphErrors(graph, rowVectorA, meanVectorA, meanErrVectorA);
                graph->GetXaxis()->SetTitle(xLabel);
                graph->GetYaxis()->SetTitle(yLabel);
                graph->GetXaxis()->CenterTitle();
                graph->GetYaxis()->CenterTitle();
                canvas->SetGrid();
                graph->SetTitle(Form(title, int(colVectorA[0])));
                graph->SetMarkerStyle(21);
                graph->SetMarkerSize(3);
                graph->SetLineWidth(lineWidth);
                // graph->SetErrorSize(errorSize);
                graph->SetMarkerColor(kBlue);
                // graph->GetYaxis()->SetRangeUser(graph->GetMinimum() - 2, graph->GetMaximum() + 5);
                graph->Draw("AP");
                canvas->Update();
                canvas->Print(Form((outputPattern+"_AMP.pdf").c_str(), int(colVectorA[0])));
                canvas->Print(Form((outputPattern+"_AMP.png").c_str(), int(colVectorA[0])));
                // for(int i = 0; i < colVectorA.size();i++){
                //     cout << "(col ,row) = " << colVectorA[i] <<"  " << rowVectorA[i] << endl;
                // }
                // getchar();
                colVectorA.clear();
                rowVectorA.clear();
                meanVectorA.clear();
                meanErrVectorA.clear();
                delete canvas;
                delete graph;
            }
            TCanvas  * canvas = new TCanvas("", "", 1600, 1200);
            TGraphErrors* graph = new TGraphErrors;
            canvas->cd();
            canvas->Clear();
            graph->Clear();
            
            SetGraphErrors(graph, rowVector, meanVector, meanErrVector);
            graph->GetXaxis()->SetTitle(xLabel);
            graph->GetYaxis()->SetTitle(yLabel);            
            graph->GetXaxis()->CenterTitle();
            graph->GetYaxis()->CenterTitle();
            // graph->GetYaxis()->SetRangeUser(0, 40);
            // graph->GetXaxis()->SetNdivisions(20, false);  // 设置 X 轴的分区数量，使得间隔为 1
            // graph->GetYaxis()->SetNdivisions(10, false);  // 可以根据需要设置 Y 轴的分区
            canvas->SetGrid();
            graph->SetTitle(Form(title, int(colVector[0])));
            graph->SetMarkerStyle(21);
            graph->SetMarkerSize(3);
            graph->SetMarkerColor(kBlue);
            graph->SetLineWidth(lineWidth);
            graph->GetXaxis()->SetRangeUser(-1, 40);
            cout << graph->GetMinimum() << endl;
            // graph->SetErrorSize(errorSize);
            graph->Draw("AP");
            canvas->Update();
            canvas->Print(Form((outputPattern + ".pdf").c_str(), int(colVector[0])));
            canvas->Print(Form((outputPattern + ".png").c_str(), int(colVector[0])));
            colVector.clear();
            rowVector.clear();
            meanVector.clear();
            meanErrVector.clear();

        }
    }

    canvas->Update();
}
