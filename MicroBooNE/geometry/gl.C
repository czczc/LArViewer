#include "TEveManager.h"
#include "TEveGeoNode.h"

#include "TGeoManager.h"
#include "TGeoNode.h"
#include "TGeoVolume.h"
#include "TGeoMedium.h"

void gl()
{
  gSystem->IgnoreSignal(kSigSegmentationViolation, true);
  TEveManager::Create();
  TGeoManager::Import("microboone.root");

  TGeoNode* world = gGeoManager->GetTopNode();
  TEveGeoTopNode* top = new TEveGeoTopNode(gGeoManager, world);
  gEve->AddGlobalElement(top);
  // gEve->Redraw3D(kTRUE);

  gEve->FullRedraw3D(kTRUE);
  TGLViewer *v = gEve->GetDefaultGLViewer();
  v->CurrentCamera().RotateRad(-0.1, -3.14*1/3);
  v->SetGuideState(1, true, false, 0);
  v->DoDraw();

}