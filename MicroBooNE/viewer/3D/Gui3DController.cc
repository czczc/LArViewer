#include "Gui3DController.h"
// #include "MCEvent.h"

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

#include <iostream>
using namespace std;

Gui3DController::Gui3DController()
{
    // dbPDG = new TDatabasePDG();
    // list = 0;

    TEveManager::Create();
    InitGeometry();
    // InitNavigationFrame();
    // InitEvent();

    // AddTracks();

    ProjectionView();
}


Gui3DController::~Gui3DController()
{
}


void Gui3DController::InitGeometry()
{
    // TGeoManager::Import("../Geometry/lbne35t4apa.gdml");
    TGeoManager::Import("../Geometry/microboone.root");
    gGeoManager->DefaultColors();
    TGeoNode* world = gGeoManager->GetTopNode();
    TGeoNode* cyro = world->GetDaughter(5)->GetDaughter(0);
    TGeoNode* tpc = cyro->GetDaughter(3);
    int nDaughters = tpc->GetNdaughters();
    for (int i=0; i<nDaughters; i++) {
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

    TEveGeoTopNode* top = new TEveGeoTopNode(gGeoManager, tpc);
    gEve->AddGlobalElement(top);
}

void Gui3DController::ProjectionView()
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

void Gui3DController::Run()
{
    gEve->FullRedraw3D(kTRUE);
    TGLViewer *v = gEve->GetDefaultGLViewer();
    v->CurrentCamera().RotateRad(-0.1, -3.14*1/3);
    // v->SetCurrentCamera(TGLViewer::kCameraPerspXOZ);
    v->SetGuideState(1, true, false, 0);
    v->DoDraw();
}

