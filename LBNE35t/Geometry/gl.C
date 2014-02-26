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
  TGeoManager::Import("lbne35t4apa.gdml");

  TGeoNode* node = gGeoManager->FindNode(); // find top node
  TEveGeoTopNode* its = new TEveGeoTopNode(gGeoManager, node);
  gEve->AddGlobalElement(its);
  gEve->Redraw3D(kTRUE);

}