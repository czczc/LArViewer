#include "ViewWindow.h"

#include "TRootEmbeddedCanvas.h"

#include "TROOT.h"
#include "TStyle.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TColor.h"

#include <iostream>
using namespace std;


ViewWindow::ViewWindow(const TGWindow *p, int w,int h)
    :TRootEmbeddedCanvas("ECanvas", p, w, h)
{
    can = GetCanvas();
    InitCanvas();
    InitStyle();
}

ViewWindow::~ViewWindow()
{
}


void ViewWindow::InitCanvas()
{
    can->cd();
    can->Divide(1, 3, 0.005, 0.005);
    for (int i=1; i<=3; i++) {
        can->cd(i);
        gPad->SetFillColor(TColor::GetColor(222,186,135));
        gPad->SetGridx();
        gPad->SetGridy();
    }
}

void ViewWindow::InitStyle()
{
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetPalette(1);
    gStyle->SetTitleStyle(0);
    gStyle->SetTitleBorderSize(0);
    gStyle->SetTitleOffset(1.0, "x");
    gStyle->SetTitleOffset(1.0, "y");
    gStyle->SetTitleXSize(0.05);
    gStyle->SetTitleYSize(0.05);
    gStyle->SetLabelSize(0.06, "x");
    gStyle->SetLabelSize(0.06, "y");
    gStyle->SetHistLineWidth(2);
    gStyle->SetLegendBorderSize(0);
    
    gStyle->SetPadLeftMargin(0.08);
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetMarkerSize(0.3);

    PaletteRainbow();

    gROOT->ForceStyle();
}


void ViewWindow::PaletteRainbow()
{
    // http://diana.parno.net/thoughts/?p=28
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;
    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
}

void ViewWindow::PaletteGray()
{
    // gray scale
    const UInt_t Number = 2;
    Int_t nb=50;
    Double_t Red[Number]   = { 0.10, 1.00};
    Double_t Green[Number] = { 0.10, 1.00};
    Double_t Blue[Number]  = { 0.10, 1.00};
    Double_t Stops[Number] = { 0.10, 1.00};
    TColor::CreateGradientColorTable(Number,Stops,Red,Green,Blue,nb);
    gStyle->SetNumberContours(nb);
}
