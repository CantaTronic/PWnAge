
#include <iostream>
#include <fstream>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TString.h>
#include <TAxis.h>
#include <cstdlib>

using namespace std;

class Parse {
    std::ifstream ifs;
    int thr, thr0;
    double time,func_time;
    void ReadLine();
    //   void PushGraph(TGraph * g, double y);
    //   void PushGraphs();
    void DoParse();
    //   void Draw();
    //   void DrawGraph(TGraph * g, const char * yAxis);
    //   double ReadNum();
    TGraph * gtime, * gspup, * geff;  //graphs for progr call
    TGraph * gtime_func, * gspup_func, * geff_func;   //graphs for func call
public:
  Parse(const TString & s);
  ~Parse();
  static void SetStyle();
};
int main() {
    std::ifstream datfile;
    TString fname = "log3.dat";
    datfile.open(fname);
    if(!datfile.is_open()) {
    std::cerr<<"Cannot open file "<<fname<<" for reading! Exiting..."<<std::endl;
    return 0;
    }
    double thr;
    TString str;
    while (!datfile.eof()){
        datfile>>str;
        cout<<"str = "<<str<<endl;
    }
    datfile.close();
    
    
//   Parse::SetStyle();
//   const TString NAME = "log3";   
//   Parse p(NAME+".dat");
//   Draw();
}
Parse::Parse(const TString & s){
  ifs.open(s);
  if(!ifs.is_open()) {
    std::cerr<<"Cannot open file "<<s<<" for reading! Exiting..."<<std::endl;
    return;
  }
  
  //graphs for total time
  
  gtime = new TGraph;
  gtime->SetName("gtime");
  gtime->SetMarkerStyle(20);
  gspup = new TGraph;
  gspup->SetName("gspup");
  gspup->SetMarkerStyle(20);
  geff = new TGraph;
  geff->SetName("geff");
  geff->SetMarkerStyle(20);
  
  //graphs for function call
  
  gtime_func = new TGraph;
  gtime_func->SetName("gtime_func");
  gtime_func->SetMarkerStyle(20);
  gspup_func = new TGraph;
  gspup_func->SetName("gspup_func");
  gspup_func->SetMarkerStyle(20);
  geff_func = new TGraph;
  geff_func->SetName("geff_func");
  geff_func->SetMarkerStyle(20);
  
  thr0 = 0;
  
  DoParse();
}
void Parse::DoParse() {
  while(!ifs.eof())
    ReadLine();
//   PushGraphs(); // last point
}
// double Parse::ReadNum() {
//   TString s;
//   double d;
//   do {
//     ifs>>s;
//     d = atof(s.Data());
//   } while (d == 0 && !ifs.eof());
//   return d;
// }
void Parse::ReadLine() {
    cin>>thr;
    cin>>time;
    cin>>func_time;
    cout<<"thr = "<<thr<<endl;
}
// void Parse::PushGraph(TGraph * g, double y) {
// //   y /= 60.;
// //   std::cout<<' '<<g<<std::flush;
// //   std::cout<<' '<<g->GetN()<<' '<<thr<<' '<<y<<std::endl;
//   g->SetPoint(g->GetN(), thr, y);
// }
// void Parse::PushGraphs() {
//   PushGraph(gtime, tot);
// //   PushGraph(gio, io);
// //   PushGraph(gcalc, calc);
//   PushGraph(gspup, tot0/tot);
//   PushGraph(geff, (tot0*thr0)/(tot*thr));
// }
// void Parse::DrawGraph(TGraph * g, const char * yAxis) {
//   g->SetMarkerSize(2);
//   g->SetLineWidth(2);
//   g->GetXaxis()->SetTitle("#threads");
//   g->GetYaxis()->SetTitle(yAxis);
//   g->GetXaxis()->SetNdivisions(50210);
//   g->GetYaxis()->SetNdivisions(510);
//   g->SetLineColor(kGreen+2);
//   g->SetMarkerColor(kCyan+2);
//   g->GetXaxis()->SetLabelColor(kOrange+2);
//   g->GetXaxis()->SetTitleColor(kRed+2);
//   g->GetYaxis()->SetLabelColor(kOrange+2);
//   g->GetYaxis()->SetTitleColor(kRed+2);
//   g->Draw("alp");
//   g->SetMinimum(0);
// }
// void Parse::Draw() {
//   TCanvas * c = new TCanvas("c", "c", 1000, 700);
//   DrawGraph(gtime, "Time [sec]");
//   c->Update();
//   c->SaveAs("img_time_"+NAME+".pdf");
// //   gio->Draw("lp");
// //   gcalc->Draw("lp");
//   DrawGraph(gspup, "Speedup");
//   c->Update();
//   c->SaveAs("img_spup_"+NAME+".pdf");
//   DrawGraph(geff, "Efficiency");
//   c->Update();
//   c->SaveAs("img_eff_"+NAME+".pdf");
//   c->Close();
// }
Parse::~Parse() {
    //graphs for progr call
    delete gtime; 
    delete gspup; 
    delete geff; 
    //graphs for func call
    delete gtime_func; 
    delete gspup_func; 
    delete geff_func;   
}
void Parse::SetStyle() {
  /// style
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetPadColor(kWhite);
  gStyle->SetPadBorderMode(0);
  gStyle->SetFrameFillColor(kWhite);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetTitleFillColor(kWhite);
  gStyle->SetTitleFont(42);
  gStyle->SetLabelFont(42);
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111);
  gStyle->SetStatColor(kWhite);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPadLeftMargin(0.13/*0.12*/);
  gStyle->SetPadRightMargin(0.02/*0.13*/);
  gStyle->SetPadBottomMargin(0.15/*0.14*/);
  gStyle->SetPadTopMargin(0.03/*0.06*/);
  gStyle->SetStatFont(42);
  gStyle->SetTitleFont(42,"");
  gStyle->SetTitleFont(42,"X");
  gStyle->SetTitleFont(42,"Y");
  gStyle->SetTitleFont(42,"Z");
  gStyle->SetLabelFont(42,"X");
  gStyle->SetLabelFont(42,"Y");
  gStyle->SetLabelFont(42,"Z");
  gStyle->SetTitleSize(0.08, "X");
  gStyle->SetTitleSize(0.08, "Y");
  gStyle->SetTitleSize(0.08, "Z");
  gStyle->SetLabelSize(0.06, "X");
  gStyle->SetLabelSize(0.06, "Y");
  gStyle->SetLabelSize(0.06, "Z");
  gStyle->SetTitleOffset(0.8, "X");
  gStyle->SetTitleOffset(0.7, "Y");
  /// ===============================
}
