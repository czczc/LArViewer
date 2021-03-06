#include "Gui3DController.h"
#include "MCEvent.h"

#include "TEveManager.h"
#include "TEveGeoNode.h"
#include "TEveVSDStructs.h"
#include "TEvePathMark.h"
#include "TEveTrack.h"
#include "TEveTrackPropagator.h"
#include "TEveBrowser.h"
#include "TEveWindow.h"
#include "TEveViewer.h"

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
#include "TSystem.h"

#include <iostream>
using namespace std;

Gui3DController::Gui3DController()
{
    dbPDG = new TDatabasePDG();
    list = 0;
    recoTrackList = 0;
    baseDir = baseDir + gSystem->DirName(__FILE__) + "/../..";

    TEveManager::Create();
    InitGeometry();
    InitNavigationFrame();
    InitEvent();
    AddProjectionView();
}


Gui3DController::~Gui3DController()
{
}


void Gui3DController::InitGeometry()
{
    TGeoManager::Import( (baseDir + "/geometry/microboone.root").Data() );
    gGeoManager->DefaultColors();

    // customize the visibility of elements
    TGeoNode* world = gGeoManager->GetTopNode();
    TGeoNode* enclosure = world->GetDaughter(5);
    int enclosure_daughters = enclosure->GetNdaughters();
    for (int i = 0; i < enclosure_daughters; ++i) {
        TGeoNode *node = enclosure->GetDaughter(i);
        TString name(node->GetName());
        if (   name.Contains("Insulation")
            || name.Contains("Column")
            || name.Contains("Walkway")
            ) {
            node->SetInvisible();
            node->SetAllInvisible();
        }
    }

    TGeoNode* cryo = enclosure->GetDaughter(0);

    TGeoNode* tpc = cryo->GetDaughter(3);
    int tpc_daughters = tpc->GetNdaughters();
    for (int i=0; i<tpc_daughters; i++) {
        TGeoNode *node = tpc->GetDaughter(i);
        TString name(node->GetName());
        // cout << name << endl;
        if (name.Contains("Cathode")) {
            node->GetVolume()->SetLineColor(kCyan);
            node->GetVolume()->SetTransparency(70);
        }
        else if (name.Contains("Active")) {
            node->GetVolume()->SetLineColor(kWhite);
            node->GetVolume()->SetTransparency(60);
        }
        else {
            node->SetInvisible();
            node->SetAllInvisible();
        }

    }
    tpc->SetInvisible();
    tpc->SetAllInvisible();

    TEveGeoTopNode* top = new TEveGeoTopNode(gGeoManager, world);
    gEve->AddGlobalElement(top);
}

void Gui3DController::AddProjectionView()
{
    TEveViewer            *f3DView;
    TEveViewer            *fXYView;
    TEveViewer            *fXZView;   
    TEveViewer            *fZYView;   

    // frames
    TEveWindowSlot* slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    TEveWindowPack* packH = slot->MakePack();
    packH->SetElementName("Projections");
    packH->SetHorizontal();
    packH->SetShowTitleBar(kFALSE);

    slot = packH->NewSlot();
    TEveWindowPack* pack0 = slot->MakePack();
    pack0->SetShowTitleBar(kFALSE);
    TEveWindowSlot*  slotLeftTop   = pack0->NewSlot();
    TEveWindowSlot* slotLeftBottom = pack0->NewSlot();

    slot = packH->NewSlot();
    TEveWindowPack* pack1 = slot->MakePack();
    pack1->SetShowTitleBar(kFALSE);
    TEveWindowSlot* slotRightTop    = pack1->NewSlot();
    TEveWindowSlot* slotRightBottom = pack1->NewSlot();

    // scenes
    slotLeftTop->MakeCurrent();
    f3DView = gEve->SpawnNewViewer("3D View", "");
    f3DView->AddScene(gEve->GetGlobalScene());
    f3DView->AddScene(gEve->GetEventScene());
    // f3DView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraPerspYOZ);
    f3DView->GetGLViewer()->SetGuideState(1, true, false, 0);
    // f3DView->GetGLViewer()->CurrentCamera().RotateRad(-0.1, -3.14*1/4);
    f3DView->GetGLViewer()->DoDraw();

    slotRightTop->MakeCurrent();
    fXYView = gEve->SpawnNewViewer("XY View", "");
    fXYView->AddScene(gEve->GetGlobalScene());
    fXYView->AddScene(gEve->GetEventScene());
    fXYView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    fXYView->GetGLViewer()->SetGuideState(1, true, false, 0);

    slotRightBottom->MakeCurrent();
    fXZView = gEve->SpawnNewViewer("XZ View", "");
    fXZView->AddScene(gEve->GetGlobalScene());
    fXZView->AddScene(gEve->GetEventScene());
    fXZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOZ);
    fXZView->GetGLViewer()->SetGuideState(1, true, false, 0);

    slotLeftBottom->MakeCurrent();
    fZYView = gEve->SpawnNewViewer("ZY View", "");
    fZYView->AddScene(gEve->GetGlobalScene());
    fZYView->AddScene(gEve->GetEventScene());
    fZYView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoZnOY);
    fZYView->GetGLViewer()->SetGuideState(1, true, false, 0);

}

//-----------------------------------------------------------------
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

//-----------------------------------------------------------------
void Gui3DController::InitEvent()
{
    const char *filetypes[] = {"ROOT files", "*.root", 0, 0};
    TString dir(baseDir + "/data");
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

//-----------------------------------------------------------------
void Gui3DController::Prev()
{
    int maxEvent = event->nEvents-1;

    if (currentEvent == 0) {
        currentEvent = maxEvent;
    }
    else {
        currentEvent--;
    }
    cout << "Prev Clicked: current event: " << currentEvent << endl;
    Reload();
}

//-----------------------------------------------------------------
void Gui3DController::Next()
{
    int maxEvent = event->nEvents-1;

    if (currentEvent == maxEvent) {
        currentEvent = 0;
    }
    else {
        currentEvent++;
    }
    cout << "Next Clicked: current event: " << currentEvent << endl;
    Reload();
}

//-----------------------------------------------------------------
void Gui3DController::Reload()
{
    eventEntry->SetNumber(currentEvent);
    event->GetEntry(currentEvent);
    event->PrintInfo(1);
    // event->PrintInfo();
    AddTracks();
    AddRecoTracks();
    gEve->DoRedraw3D();
}

//-----------------------------------------------------------------
void Gui3DController::AddTracks()
{
    if (list) {
        list->RemoveElements();
    }
    else {
        list = new TEveTrackList();
        list->GetPropagator()->SetStepper(TEveTrackPropagator::kRungeKutta);
        list->GetPropagator()->SetMaxR(1e4);
        list->GetPropagator()->SetMaxZ(1e4);
        gEve->AddElement(list);
    }    
    TString name;
    name.Form("%i: MC Particles", currentEvent);
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
        track->SetLineWidth( 3 );
        pm = new TEvePathMarkD(TEvePathMarkD::kReference);
        pm->fV.Set(event->mc_endXYZT[i][0], event->mc_endXYZT[i][1], event->mc_endXYZT[i][2]);
        track->AddPathMark(*pm);
        list->AddElement(track);
        track->MakeTrack();
        nTrackShowed++;
    }

}

//-----------------------------------------------------------------
void Gui3DController::AddRecoTracks()
{
    int trackerIdx = 0;
    if (recoTrackList) {
        recoTrackList->RemoveElements();
    }
    else {
        recoTrackList = new TEveTrackList();
        recoTrackList->GetPropagator()->SetStepper(TEveTrackPropagator::kRungeKutta);
        recoTrackList->GetPropagator()->SetMaxR(1e4);
        recoTrackList->GetPropagator()->SetMaxZ(1e4);
        gEve->AddElement(recoTrackList);
    }    
    TString name;
    name.Form("%i: Reco Tracks", currentEvent);
    recoTrackList->SetName(name.Data());

    int nTrack = event->trk_nTrack[trackerIdx];  // number of tracks in MC
    TEveRecTrackD *rc = 0;
    TEveTrack* track = 0;
    TEvePathMarkD* pm = 0;
    int colors[6] = {kMagenta, kGreen, kYellow, kRed, kCyan, kWhite};
    int nTrackShowed = 0;
    for (int i=0; i<nTrack; i++) {
        rc = new TEveRecTrackD();
        rc->fV.Set((*event->trk_points_x[trackerIdx]).at(i).at(0), 
                   (*event->trk_points_y[trackerIdx]).at(i).at(0), 
                   (*event->trk_points_z[trackerIdx]).at(i).at(0));
        track = new TEveTrack(rc, list->GetPropagator());
        TString s;
        s.Form("track %i", i);
        track->SetName(s.Data());
        track->SetLineColor( colors[nTrackShowed % 6] );
        track->SetLineWidth( 2 );
        for (int ipm = 1; ipm<(*event->trk_nHit[trackerIdx]).at(i); ipm++) {
            pm = new TEvePathMarkD(TEvePathMarkD::kReference);
            pm->fV.Set((*event->trk_points_x[trackerIdx]).at(i).at(ipm), 
                       (*event->trk_points_y[trackerIdx]).at(i).at(ipm), 
                       (*event->trk_points_z[trackerIdx]).at(i).at(ipm));
            track->AddPathMark(*pm);
        }

        recoTrackList->AddElement(track);
        track->MakeTrack();
        nTrackShowed++;
    }

}

//-----------------------------------------------------------------
void Gui3DController::Run()
{
    gEve->FullRedraw3D(kTRUE);
    TGLViewer *v = gEve->GetDefaultGLViewer();
    v->CurrentCamera().RotateRad(-0.1, -3.14*1/3);
    // v->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);
    v->SetGuideState(1, true, false, 0);
    v->DoDraw();
}

