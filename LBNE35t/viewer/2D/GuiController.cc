#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "MCEvent.h"
#include "MCGeometry.h"
#include "MCChannel.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TApplication.h"
#include "TGMenu.h"
#include "TGFileDialog.h"
#include "TGListBox.h"
#include "TGString.h"

#include "TCanvas.h"
#include "TPad.h"
#include "TFrame.h"
#include "TColor.h"
#include "TH2F.h"
#include "TMath.h"
#include "TLorentzVector.h"
#include "TLine.h"
#include "TMarker.h"

#include <exception>
#include <iostream>
using namespace std;

GuiController::GuiController(const TGWindow *p, int w,int h)
{
    InitPDGMap();
    trackLine = new TLine(0,0,0,0);
    trackLine->SetLineColor(kRed);
    trackLine->SetLineWidth(2);
    trackStartPoint = new TMarker(0,0,24);
    trackStartPoint->SetMarkerColor(kWhite);
    trackStartPoint->SetMarkerSize(1.0);

    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;
    can = vw->can;

    event = new MCEvent("../data/sample.root");
    geom = event->geom;
    currentEvent = 0;
    currentPalette = 1;
    xMin_now = 0; 
    xMax_now = 3200;
    Reload();

    InitConnections();


}

GuiController::~GuiController()
{
    delete trackLine;
}

void GuiController::InitConnections()
{
    mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    cw->prevButton->Connect("Clicked()", "GuiController", this, "Prev()");
    cw->nextButton->Connect("Clicked()", "GuiController", this, "Next()");
    cw->autoZoomButton->Connect("Clicked()", "GuiController", this, "AutoZoom()");
    cw->unZoomButton->Connect("Clicked()", "GuiController", this, "UnZoom()");
    cw->xrangeButton->Connect("Clicked()", "GuiController", this, "SyncXaxis()");
    cw->paletteButtonGroup->Connect("Clicked(int)", "GuiController", this, "UpdatePalette(int)");
    cw->fSiblingTracksListBox->Connect("Selected(int)", "GuiController", this, "SiblingSelected(int)");
    cw->fDaughterTracksListBox->Connect("Selected(int)", "GuiController", this, "ParentOrDaughterSelected(int)");
    cw->fParentTracksListBox->Connect("Selected(int)", "GuiController", this, "ParentOrDaughterSelected(int)");

    can->GetPad(1)->Connect("RangeChanged()", "GuiController", this, "SyncRangeZT()");
    can->GetPad(2)->Connect("RangeChanged()", "GuiController", this, "SyncRangeUT()");
    can->GetPad(3)->Connect("RangeChanged()", "GuiController", this, "SyncRangeVT()");

}

void GuiController::AutoZoom()
{    
    AutoZoom(event->hPixelZT);
    AutoZoom(event->hPixelUT, false);
    AutoZoom(event->hPixelVT, false);
    Modified();

}

void GuiController::UnZoom(bool redraw)
{    
    xMin_now = 1;
    xMax_now = 3200;
    event->hPixelZT->GetXaxis()->SetRange(xMin_now, xMax_now); 
    event->hPixelUT->GetXaxis()->SetRange(xMin_now, xMax_now); 
    event->hPixelVT->GetXaxis()->SetRange(xMin_now, xMax_now);
    event->hPixelZT->GetYaxis()->SetRange(1, 343); 
    event->hPixelUT->GetYaxis()->SetRange(1, 400); 
    event->hPixelVT->GetYaxis()->SetRange(1, 400);
    cw->tdcMinEntry->SetIntNumber(xMin_now);
    cw->tdcMaxEntry->SetIntNumber(xMax_now);
    if (redraw) Modified();

}

void GuiController::SyncRangeZT()
{
    xMin_now = event->hPixelZT->GetXaxis()->GetFirst();
    xMax_now = event->hPixelZT->GetXaxis()->GetLast();
    event->hPixelUT->GetXaxis()->SetRange(xMin_now, xMax_now); 
    event->hPixelVT->GetXaxis()->SetRange(xMin_now, xMax_now);
    cw->tdcMinEntry->SetIntNumber(xMin_now);
    cw->tdcMaxEntry->SetIntNumber(xMax_now);
    cout << "Range Changed: " << xMin_now << ", " << xMax_now << endl;
    Modified();
}

void GuiController::SyncRangeUT()
{
    xMin_now = event->hPixelUT->GetXaxis()->GetFirst();
    xMax_now = event->hPixelUT->GetXaxis()->GetLast();
    event->hPixelZT->GetXaxis()->SetRange(xMin_now, xMax_now); 
    event->hPixelVT->GetXaxis()->SetRange(xMin_now, xMax_now);
    cw->tdcMinEntry->SetIntNumber(xMin_now);
    cw->tdcMaxEntry->SetIntNumber(xMax_now);
    cout << "Range Changed: " << xMin_now << ", " << xMax_now << endl;
    Modified();
}

void GuiController::SyncRangeVT()
{
    xMin_now = event->hPixelVT->GetXaxis()->GetFirst();
    xMax_now = event->hPixelVT->GetXaxis()->GetLast();
    event->hPixelZT->GetXaxis()->SetRange(xMin_now, xMax_now); 
    event->hPixelUT->GetXaxis()->SetRange(xMin_now, xMax_now);
    cw->tdcMinEntry->SetIntNumber(xMin_now);
    cw->tdcMaxEntry->SetIntNumber(xMax_now);
    cout << "Range Changed: " << xMin_now << ", " << xMax_now << endl;
    Modified();
}

void GuiController::SyncXaxis()
{   
    double x1 = cw->tdcMinEntry->GetNumber();
    double x2 = cw->tdcMaxEntry->GetNumber();
    
    event->hPixelZT->GetXaxis()->SetRange(x1, x2);
    event->hPixelUT->GetXaxis()->SetRange(x1, x2);
    event->hPixelVT->GetXaxis()->SetRange(x1, x2);

    Modified();
}

void GuiController::UpdatePalette(int id)
{
    if (id == currentPalette) return; 
    if (id == 1) vw->PaletteRainbow();
    else if (id == 2) vw->PaletteGray();
    currentPalette = id;
    Modified();
}

void GuiController::Modified()
{   
    can->GetPad(1)->Modified();
    // can->GetPad(1)->Update();
    can->GetPad(2)->Modified();
    // can->GetPad(2)->Update();
    can->GetPad(3)->Modified();
    // can->GetPad(3)->Update();
    can->Update();
}


void GuiController::DrawPixels()
{
    can->cd(1);
    event->FillPixel(2, -1); // ZT
    event->hPixelZT->Draw("colz");

    can->cd(2);
    event->FillPixel(0, -1); // UT
    event->hPixelUT->Draw("colz");

    can->cd(3);
    event->FillPixel(1, -1); // VT
    event->hPixelVT->Draw("colz");

    Modified();
}


void GuiController::DrawTrack(int id) 
{
    int i = event->trackIndex[id];
    // cout << i << " " << event->mc_startXYZT[i][0] << endl;
    trackLine->SetX1(event->mc_startXYZT[i][0]);
    trackLine->SetY1(event->mc_startXYZT[i][2]);
    trackLine->SetX2(event->mc_endXYZT[i][0]);
    trackLine->SetY2(event->mc_endXYZT[i][2]);

    trackStartPoint->SetX(event->mc_startXYZT[i][0]);
    trackStartPoint->SetY(event->mc_startXYZT[i][2]);

    can->cd(1);
    trackLine->Draw();
    trackStartPoint->Draw();
    Modified();
}


void GuiController::Prev()
{
    if (currentEvent==0) {
        currentEvent = event->nEvents-1;
    }
    else {
        currentEvent--;
    }
    Reload();
}


void GuiController::Next()
{
    if (currentEvent==event->nEvents-1) {
        currentEvent = 0;
    }
    else {
        currentEvent++;
    }
    Reload();
}


void GuiController::Reload()
{
    cw->eventEntry->SetNumber(currentEvent);
    event->GetEntry(currentEvent);
    event->PrintInfo(1);
    // event->PrintInfo();
 
    InitTracksList();
    DrawPixels();
}


void GuiController::InitTracksList()
{   

    cw->fSiblingTracksListBox->RemoveAll();
    TGString name;
    name.Form("%s (%.1f)", PDGName(event->mc_pdg[0]).Data(), KE(event->mc_startMomentum[0])*1000);
    int id = event->mc_id[0];
    cw->fSiblingTracksListBox->AddEntry(name, id);
    SiblingSelected(id);
}

void GuiController::SiblingSelected(int id)
{
    cw->fSiblingTracksListBox->Select(id);

    cw->fDaughterTracksListBox->RemoveAll();
    int i = event->trackIndex[id];
    int nDaughter = (*(event->mc_daughters)).at(i).size();
    for (int j=0; j<nDaughter; j++) {
        int idx = event->trackChildren.at(i).at(j);
        TGString name;
        name.Form("%s (%.1f)", PDGName(event->mc_pdg[idx]).Data(), KE(event->mc_startMomentum[idx])*1000);
        cw->fDaughterTracksListBox->AddEntry(name, event->mc_id[idx]);
        // cout << name.Data() << event->mc_id[idx] << endl;
    }
    cw->fDaughterTracksListBox->Layout();

    cw->fParentTracksListBox->RemoveAll();
    int nParent = event->trackParents.at(i).size();
    for (int j=0; j<nParent; j++) {
        int idx = event->trackParents.at(i).at(j);
        TGString name;
        name.Form("%s (%.1f)", PDGName(event->mc_pdg[idx]).Data(), KE(event->mc_startMomentum[idx])*1000);
        cw->fParentTracksListBox->AddEntry(name, event->mc_id[idx]);
        // cout << name.Data() << event->mc_id[idx] << endl;
    }
    cw->fParentTracksListBox->Layout();
    DrawTrack(id);
    cout << "sibling selected, id: " << id << ", daughters: " << nDaughter << endl;
}


void GuiController::ParentOrDaughterSelected(int id)
{
    cw->fSiblingTracksListBox->RemoveAll();
    int i = event->trackIndex[id];
    TGString name;
    name.Form("%s (%.1f)", PDGName(event->mc_pdg[i]).Data(), KE(event->mc_startMomentum[i])*1000);
    cw->fSiblingTracksListBox->AddEntry(name, event->mc_id[i]);
    SiblingSelected(id);

    cout << "parent or daughter selected, id: " << id << endl;
}


void GuiController::Open(const char* filename)
{
    if (event) delete event;
    event = new MCEvent(filename);
    geom = event->geom;
    currentEvent = 0;
    Reload();
}


void GuiController::HandleMenu(int id)
{
    const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    switch (id) {
        case M_FILE_OPEN:
            {
                static TString dir("../data");
                TGFileInfo fi;
                fi.fFileTypes = filetypes;
                fi.fIniDir    = StrDup(dir);
                new TGFileDialog(gClient->GetRoot(), mw, kFDOpen, &fi);
                dir = fi.fIniDir;
                if (fi.fFilename) {
                    UnZoom();
                    cout << "open file: " << fi.fFilename << endl;
                    Open(fi.fFilename);
                }
            }
            break;

        case M_FILE_EXIT:
            gApplication->Terminate(0);
            break;
    }
}

void GuiController::AutoZoom(TH2F* hist, bool zoomX)
{
    // zooms in on where the action is i.e., where there is counts
    Int_t xMin = hist->GetNbinsX();
    Int_t xMax = 0;
    Int_t yMin = hist->GetNbinsY();
    Int_t yMax = 0;

    for(Int_t i = 1; i <= hist->GetNbinsX(); i++){
        for(Int_t j = 1; j <= hist->GetNbinsY(); j++){
      
            if(hist->GetBinContent(i, j) <= 1) continue;

            if(i < xMin) xMin = i;
            if(j < yMin) yMin = j;
            if(i > xMax) xMax = i;
            if(j > yMax) yMax = j;
        }
    }

    hist->GetYaxis()->SetRange(yMin, yMax);
    if (zoomX) {
        hist->GetXaxis()->SetRange(xMin, xMax);
        xMin_now = xMin;
        xMax_now = xMax;
    }
    else {
        hist->GetXaxis()->SetRange(xMin_now, xMax_now);
    }
}

double GuiController::KE(float* momentum)
{
    TLorentzVector particle(momentum);
    return particle.E()-particle.M();
}


void GuiController::InitPDGMap()
{
    pdgMap[321]   = "K+";
    pdgMap[-321]  = "K-";
    pdgMap[130]   = "K0long";
    pdgMap[310]   = "K0short";
    pdgMap[310]   = "K0short";

    pdgMap[221]   = "eta";
    pdgMap[2212]  = "p";
    pdgMap[-2212] = "~p";
    pdgMap[2112]  = "n";
    pdgMap[-2112] = "~n";

    pdgMap[113]   = "rho";
    pdgMap[211]   = "pi+";
    pdgMap[-211]  = "pi-";
    pdgMap[111]   = "pi0";
    
    pdgMap[22]    = "gamma";
    pdgMap[-11]   = "e+";
    pdgMap[11]    = "e-";
    pdgMap[-13]   = "mu+";
    pdgMap[13]    = "mu-";
    pdgMap[-15]   = "tau+";
    pdgMap[15]    = "tau-";
 
    pdgMap[12]    = "nu_e";
    pdgMap[-12]   = "~nu_e";
    pdgMap[14]    = "nu_mu";
    pdgMap[-14]   = "~nu_mu";

}


TGString GuiController::PDGName(int pdg)
{   
    try {
        return pdgMap.at(pdg);
    } catch (exception& e) {
        if (pdg>1e9) {
            int z = (pdg - 1e9) / 10000;
            int a = (pdg - 1e9 - z*1e4) / 10;
            TGString name;
            if (z == 18) name = "Ar";   
            else if (z == 17) name = "Cl";
            else if (z == 16) name = "S";
            else if (z == 15) name = "P";
            else if (z == 19) name = "Ca";
            else if (z == 1) name = "H";
            else if (z == 2) name = "He";
            else return pdg;
            return Form("%s-%i", name.Data(), a);
        }
        return pdg;
    }
    
}


