#include "TCanvas.h"
#include "TGraphErrors.h"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

const char xLabel[] = "Row";
const char yLabel[] = "Peak Amplitude Mean/ADC Counts";
const char title[] = "Column %02d";

const int channels = 1600;  // Number of entries in "res.txt"
const Double_t markerSize = 3;
const Double_t lineWidth = 2;

std::string outputPattern = "./meanView/meanViewCol_%02d";

// Function to set graph points and errors
void SetGraphErrors(TGraphErrors* graph, const std::vector<double>& x, const std::vector<double>& yMeans, const std::vector<double>& yErrors) {
    for (size_t i = 0; i < yErrors.size(); i++) {
        graph->SetPoint(i, x[i], yMeans[i]);
        graph->SetPointError(i, 0.0, yErrors[i]);
    }
}

// Function to find the maximum value of (mean + meanErr) for corresponding elements
double GetMaxYFromSums(const std::vector<Double_t>& means, const std::vector<Double_t>& errors) {
    if (means.size() != errors.size()) {
        std::cerr << "Error: meanVector and meanErrVector sizes do not match!" << std::endl;
        return 0.0;
    }
    double maxSum = 0.0;
    for (size_t i = 0; i < means.size(); i++) {
        maxSum = std::max(maxSum, means[i] + errors[i]);
    }
    return maxSum;
}

void meanVisualization() {
    int lineCount = 0;
    // Create canvas and graph objects
    TCanvas* canvas = new TCanvas("", "", 1600, 1200);
    TGraphErrors* graph = new TGraphErrors();

    // Vectors to store data
    std::vector<Double_t> indexVector, colVector, rowVector, meanVector, meanErrVector;

    // Open the input file
    std::ifstream inputFile("./DigitHits/res.txt");
    if (!inputFile.is_open()) {
        std::cerr << "Unable to open file!" << std::endl;
        return;
    }

    Double_t index, col, row, mean, meanErr;
    std::string line;



    // Read the file line by line
    for (int i = 0; i < channels; i++) {
    std::string line;
        while (std::getline(inputFile, line)) {
            lineCount++;
            if (line.empty()) {
                std::cerr << "Empty line at: " << lineCount << std::endl;
                continue;
        }

        std::stringstream ss(line);
        if (!(ss >> index >> col >> row >> mean >> meanErr)) {
            std::cerr << "Failed to parse line " << lineCount << ": " << line << std::endl;
            continue;
        }
        // cout << "Lines read = " << lines_read << " ,lines = " << line << endl;
        std::cout << "Total lines read: " << lineCount << std::endl;
        // Adjust mean error values
        if (meanErr > mean) meanErr = mean;
        if (meanErr <= 0.1) meanErr = 0.1;

        // Store data in vectors
        colVector.push_back(col);
        rowVector.push_back(row);
        meanVector.push_back(mean);
        meanErrVector.push_back(meanErr);

        // Draw and save the graph for every 40 entries
        if (colVector.size() == 40) {
            canvas->Clear();
            graph->Clear();
            SetGraphErrors(graph, rowVector, meanVector, meanErrVector);

            // Configure graph appearance
            graph->GetXaxis()->SetTitle(xLabel);
            graph->GetYaxis()->SetTitle(yLabel);
            graph->GetXaxis()->CenterTitle();
            graph->GetYaxis()->CenterTitle();
            graph->SetTitle(Form(title, int(colVector[0])));
            graph->SetMarkerStyle(21);
            graph->SetMarkerSize(markerSize);
            graph->SetMarkerColor(kBlue);
            graph->SetLineWidth(lineWidth);
            graph->GetXaxis()->SetRangeUser(0, 40);

            // Get the maximum Y value and set the Y-axis range
            Double_t maxY = GetMaxYFromSums(meanVector, meanErrVector);
            graph->GetYaxis()->SetRangeUser(0, maxY *1.05);

            canvas->SetGrid();

            // Draw and save the graph
            graph->Draw("AP");
            canvas->Update();
            canvas->Print(Form((outputPattern + ".pdf").c_str(), int(colVector[0])));
            canvas->Print(Form((outputPattern + ".png").c_str(), int(colVector[0])));

            // Clear vectors for the next set of data
            colVector.clear();
            rowVector.clear();
            meanVector.clear();
            meanErrVector.clear();
        }
    }
    }

    // Clean up dynamically allocated memory
    delete canvas;
    delete graph;
}
