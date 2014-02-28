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

#include "TCanvas.h"
#include "TPad.h"
#include "TFrame.h"
#include "TColor.h"
#include "TH2F.h"
#include "TMath.h"

#include <iostream>
using namespace std;

GuiController::GuiController(const TGWindow *p, int w,int h)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;
    can = vw->can;

    event = new MCEvent("../data/sample.root");
    geom = event->geom;
    currentEvent = 0;
    currentPalette = 1;
    
    Reload();

    InitConnections();

}

GuiController::~GuiController()
{
}

void GuiController::InitConnections()
{
    mw->fMenuFile->Connect("Activated(int)", "GuiController", this, "HandleMenu(int)");

    cw->prevButton->Connect("Clicked()", "GuiController", this, "Prev()");
    cw->nextButton->Connect("Clicked()", "GuiController", this, "Next()");
    cw->xrangeButton->Connect("Clicked()", "GuiController", this, "SyncXaxis()");
    cw->paletteButtonGroup->Connect("Clicked(int)", "GuiController", this, "UpdatePalette(int)");
}

void GuiController::SyncXaxis()
{   
    double x1 = cw->tdcMinEntry->GetNumber();
    double x2 = cw->tdcMaxEntry->GetNumber();
    
    event->hPixelZT->GetXaxis()->SetRangeUser(x1, x2);
    event->hPixelUT->GetXaxis()->SetRangeUser(x1, x2);
    event->hPixelVT->GetXaxis()->SetRangeUser(x1, x2);

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
    can->GetPad(1)->Update();
    can->GetPad(2)->Modified();
    can->GetPad(2)->Update();
    can->GetPad(3)->Modified();
    can->GetPad(3)->Update();
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
    // event->PrintInfo(1);
    event->PrintInfo();
    DrawPixels();
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
                static TString dir("../../data");
                TGFileInfo fi;
                fi.fFileTypes = filetypes;
                fi.fIniDir    = StrDup(dir);
                new TGFileDialog(gClient->GetRoot(), mw, kFDOpen, &fi);
                dir = fi.fIniDir;
                if (fi.fFilename) {
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
