#include "Gui3DController.h"
#include "MCEvent.h"

#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveVSDStructs.h"
#include "TEvePathMark.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveBrowser.h"

#include "TGLViewer.h"
#include "TGeoManager.h"
#include "TGeoNode.h"

#include "TGFrame.h"
#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGFileDialog.h"

#include "TString.h"
#include "TColor.h"

#include <iostream>
using namespace std;

Gui3DController::Gui3DController()
{
    TEveManager::Create();
    InitGeometry();
    InitNavigationFrame();
    InitEvent();

    AddTracks();
}


Gui3DController::~Gui3DController()
{
}


void Gui3DController::InitEvent()
{
    const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    static TString dir("../..");
    TGFileInfo fi;
    fi.fFileTypes = filetypes;
    fi.fIniDir    = StrDup(dir);
    new TGFileDialog(gClient->GetRoot(), frmMain, kFDOpen, &fi);
    dir = fi.fIniDir;
    cout << fi.fFilename << endl;
    event = new MCEvent(fi.fFilename);
    currentEvent = 0;
    Reload();
}


void Gui3DController::InitGeometry()
{
    TGeoManager::Import("../../Geometry/lbne35t4apa.gdml");
    gGeoManager->DefaultColors();
    TGeoNode* world = gGeoManager->GetTopNode();
    TGeoNode* cyro = world->GetDaughter(0)->GetDaughter(0);
    int nDaughters = cyro->GetNdaughters();
    for (int i=0; i<nDaughters; i++) {
        TGeoNode *node = cyro->GetDaughter(i);
        TString name(node->GetName());
        if (name.Contains("Cathode")) {
            node->GetVolume()->SetLineColor(kCyan);
            node->GetVolume()->SetTransparency(70);
        }
        // else if (name.Contains("TPC")) {
        //     node->GetVolume()->SetTransparency(100);
        // }
        else if (name.Contains("APA")) {
            node->GetVolume()->SetLineColor(kRed);
            // node->GetVolume()->SetTransparency(10);
        }
        else {
            node->SetInvisible();
            node->SetAllInvisible();
        }

    }

    TEveGeoTopNode* top = new TEveGeoTopNode(gGeoManager, cyro);
    gEve->AddGlobalElement(top);
}

void Gui3DController::InitNavigationFrame()
{
   // Create minimal GUI for event navigation.

   TEveBrowser* browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);

   frmMain = new TGMainFrame(gClient->GetRoot(), 200, 600);
   frmMain->SetWindowName("Navigation GUI");
   frmMain->SetCleanup(kDeepCleanup);

   TGHorizontalFrame* hf = new TGHorizontalFrame(frmMain);
   frmMain->AddFrame(hf, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
   {
        prevButton = new TGTextButton(hf, "< Prev");
        hf->AddFrame(prevButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
        prevButton->Connect("Clicked()", "Gui3DController", this, "Prev()");

        eventEntry = new TGNumberEntryField(hf, -1, 0, TGNumberFormat::kNESInteger);
        eventEntry->SetDefaultSize(60, 20);
        hf->AddFrame(eventEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

        nextButton = new TGTextButton(hf, "Next >");
        hf->AddFrame(nextButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
        nextButton->Connect("Clicked()", "Gui3DController", this, "Next()");

   }
   

   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Events", 0);
}

void Gui3DController::AddTracks()
{
    TEveTrackList *list = new TEveTrackList();
    list->SetName("LBNE");
    list->GetPropagator()->SetStepper(TEveTrackPropagator::kRungeKutta);
    

    TEveRecTrackD *rc = new TEveRecTrackD();
    rc->fV.Set(100, 20, 20);
    rc->fP.Set(100, 20, 20);
    TEveTrack* track = new TEveTrack(rc, list->GetPropagator());

    // t = new TParticle(-13, 0, 0, 0, 0, 0, 0, 0, 1, 2, 100, 20, 200, 200);
    // TEveTrack* track = new TEveTrack(t, 1, list->GetPropagator());
    
    track->SetName("test track");
    track->SetLineColor(kMagenta);


    TEvePathMarkD* pm1 = new TEvePathMarkD(TEvePathMarkD::kReference);
    pm1->fV.Set(100, 20, 200);
    track->AddPathMark(*pm1);
    TEvePathMarkD* pm2 = new TEvePathMarkD(TEvePathMarkD::kReference);
    pm2->fV.Set(100, 100, 200);
    track->AddPathMark(*pm2);

    gEve->AddElement(list);
    list->AddElement(track);

    track->MakeTrack();

}

void Gui3DController::Run()
{
    gEve->FullRedraw3D(kTRUE);
    TGLViewer *v = gEve->GetDefaultGLViewer();
    v->CurrentCamera().RotateRad(-0.5, -2);
    v->SetGuideState(1, true, false, 0);
    v->DoDraw();
}


void Gui3DController::Prev()
{
    int maxEvent = event->nEvents-1;

    if (currentEvent == 0) {
        currentEvent = maxEvent;
    }
    else {
        currentEvent--;
    }
    cout << "prev: current event: " << currentEvent << endl;
    Reload();
}


void Gui3DController::Next()
{
    int maxEvent = event->nEvents-1;

    if (currentEvent == maxEvent) {
        currentEvent = 0;
    }
    else {
        currentEvent++;
    }
    cout << "next: current event: " << currentEvent << endl;
    Reload();
}

void Gui3DController::Reload()
{
    eventEntry->SetNumber(currentEvent);
    event->GetEntry(currentEvent);
    // event->PrintInfo(1);
    event->PrintInfo();
}
