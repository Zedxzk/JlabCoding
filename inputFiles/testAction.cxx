#include <TCanvas.h>
#include <TH1D.h>

void testAction() {
    // 创建一个1D直方图
    TH1D *hist = new TH1D("hist", "Bin Edge Test", 10, 0, 10);  // 10个bin，范围是0到10

    // 填充直方图
    hist->Fill(0);
    hist->Fill(1);
    hist->Fill(2);
    hist->Fill(9);

    // 设置范围
    hist->GetXaxis()->SetRange(0, 5);  // 这将展示范围在0到1之间的内容，包括右边界1

    // 创建画布
    TCanvas *c1 = new TCanvas("c1", "Histogram Canvas", 800, 600);

    // 绘制直方图
    hist->Draw();

    // 在画布上显示
    c1->SaveAs("hist_set_range.pdf");
}


