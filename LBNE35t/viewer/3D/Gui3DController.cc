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

#include "TDatabasePDG.h"
#include "TParticlePDG.h"
#include "TString.h"
#include "TColor.h"

#include <iostream>
using namespace std;

Gui3DController::Gui3DController()
{
    dbPDG = new TDatabasePDG();

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
    static TString dir("../data");
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
    TGeoManager::Import("../Geometry/lbne35t4apa.gdml");
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

    TEveGeoTopNode* top = new TEveGeoTopNode(gGeoManager, world);
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
    if (list) {
        list->RemoveElements();
    }
    else {
        list = new TEveTrackList();
        list->GetPropagator()->SetStepper(TEveTrackPropagator::kRungeKutta);
        gEve->AddElement(list);
    }    
    TString name;
    name.Form("Event: %i", currentEvent);
    list->SetName(name.Data());

    int mc_Ntrack = event->mc_Ntrack;  // number of tracks in MC
    TEveRecTrackD *rc = 0;
    TEveTrack* track = 0;
    TEvePathMarkD* pm = 0;
    int colors[6] = {kMagenta, kGreen, kYellow, kRed, kCyan, kWhite};
    int nTrackShowed = 0;
    for (int i=0; i<mc_Ntrack; i++) {
        int mc_id = event->mc_id[i];
        int mc_pdg = event->mc_pdg[i];
        TParticlePDG *p = dbPDG->GetParticle(mc_pdg);
        if (p == 0) {
            continue; // skip unknown particles
        }
        if ( fabs(p->Charge()) < 1e-2 ) {
            continue;  // skip neutral particles
        }
        rc = new TEveRecTrackD();
        rc->fV.Set(event->mc_startXYZT[i][0], event->mc_startXYZT[i][1], event->mc_startXYZT[i][2]);
        track = new TEveTrack(rc, list->GetPropagator());
        TString s;
        s.Form("%i: %s", mc_id, p->GetName());
        track->SetName(s.Data());
        track->SetLineColor( colors[nTrackShowed % 6] );
        pm = new TEvePathMarkD(TEvePathMarkD::kReference);
        pm->fV.Set(event->mc_endXYZT[i][0], event->mc_endXYZT[i][1], event->mc_endXYZT[i][2]);
        track->AddPathMark(*pm);
        list->AddElement(track);
        track->MakeTrack();
        nTrackShowed++;
    }

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
    AddTracks();
    gEve->DoRedraw3D();
}
