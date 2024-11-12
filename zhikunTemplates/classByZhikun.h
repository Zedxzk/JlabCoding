#ifndef _CLASSBYZHIKUN_H_
#define _CLASSBYZHIKUN_H_
#include <iostream>
#include <string>
#include <TH2.h>
#include <TH2Poly.h>
#include <TH2D.h>
#include <TCanvas.h>

using namespace std;
namespace classByZhikun{
    class plotEcalHits2D {
    string xAxisName;
    string yAxisName;
    string nameOfFig;
    string titleOfFig;
    TCanvas* canvas;
    TH2* hist2D;  // 使用TH2基类指针

    public:
        // 默认构造函数
        plotEcalHits2D() : xAxisName(""), yAxisName(""), nameOfFig(""), titleOfFig("") {
            canvas = new TCanvas();
            hist2D = nullptr;
        }

        // 带参数的构造函数
        plotEcalHits2D(string xAxis, string yAxis, string name, string title, TH2D* hist2d)
            : xAxisName(xAxis), yAxisName(yAxis), nameOfFig(name), titleOfFig(title), hist2D(hist2d){
            canvas = new TCanvas();
        }

        plotEcalHits2D(string xAxis, string yAxis, string name, string title)
            : xAxisName(xAxis), yAxisName(yAxis), nameOfFig(name), titleOfFig(title) {
            canvas = new TCanvas();
            hist2D = new TH2Poly();
        }
        // 默认析构函数
        ~plotEcalHits2D() {
            delete canvas;
        }

        // 修改变量的函数
        void setXAxisName(const string& xAxis) { xAxisName = xAxis; }
        void setYAxisName(const string& yAxis) { yAxisName = yAxis; }
        void setNameOfFig(const string& name) { nameOfFig = name; }
        void setTitleOfFig(const string& title) { titleOfFig = title; }
        void setCanvas(TCanvas* c) { canvas = c; }
        void setHist2D(TH2* p) { hist2D = p; }

        // 访问器函数
        string& getXAxisName() { return xAxisName; }
        string& getYAxisName() { return yAxisName; }
        string& getNameOfFig() { return nameOfFig; }
        string& getTitleOfFig() { return titleOfFig; }
        TCanvas* getCanvas() { return canvas; }
        TH2* getHist2D() { return hist2D; }

        // 打印函数用于显示对象信息
        void printDetails() {
            cout << "X Axis Name: " << xAxisName << endl;
            cout << "Y Axis Name: " << yAxisName << endl;
            cout << "Figure Name: " << nameOfFig << endl;
            cout << "Figure Title: " << titleOfFig << endl;
        }
    };


    class plotEcalHits1D {
        string xAxisName;
        string yAxisName;
        string nameOfFig;
        string titleOfFig;
        TCanvas* canvas;
        TH1D* hist1D;  // 使用TH1D指针

    public:
        // 默认构造函数
        plotEcalHits1D() 
            : xAxisName(""), yAxisName(""), nameOfFig(""), titleOfFig(""), hist1D(nullptr) {
            canvas = new TCanvas();
        }

        // 带参数的构造函数
        plotEcalHits1D(TH1D* hist1d)
            : xAxisName(""), yAxisName(""), nameOfFig(""), titleOfFig(""), hist1D(hist1d) {
            canvas = new TCanvas();
        }

        // 带参数的构造函数
        plotEcalHits1D(string xAxis, string yAxis, string name, string title, TH1D* hist1d)
            : xAxisName(xAxis), yAxisName(""), nameOfFig(name), titleOfFig(title), hist1D(hist1d) {
            canvas = new TCanvas();
        }

        // 另一个构造函数，创建一个新的 TH1D
        plotEcalHits1D(string xAxis, string name, string title, int bins, double min, double max)
            : xAxisName(xAxis), nameOfFig(name), titleOfFig(title) {
            canvas = new TCanvas();
            hist1D = new TH1D(name.c_str(), title.c_str(), bins, min, max);
        }

        // 默认析构函数
        ~plotEcalHits1D() {
            delete canvas;
            // delete hist1D;  // 释放动态分配的内存
        }

        // 修改变量的函数
        void setXAxisName(const string& xAxis) { xAxisName = xAxis; }
        void setNameOfFig(const string& name) { nameOfFig = name; }
        void setTitleOfFig(const string& title) { titleOfFig = title; }
        void setCanvas(TCanvas* c) { canvas = c; }
        void setHist1D(TH1D* p) { hist1D = p; }

        // 访问器函数
        string& getXAxisName() { return xAxisName; }
        string& getYAxisName() { return yAxisName; }
        string& getNameOfFig() { return nameOfFig; }
        string& getTitleOfFig() { return titleOfFig; }
        TCanvas* getCanvas() { return canvas; }
        TH1D* getHist() { return hist1D; }

        // 打印函数用于显示对象信息
        void printDetails() {
            cout << "X Axis Name: " << xAxisName << endl;
            cout << "Figure Name: " << nameOfFig << endl;
            cout << "Figure Title: " << titleOfFig << endl;
        }
    };

}
using namespace classByZhikun;
#endif