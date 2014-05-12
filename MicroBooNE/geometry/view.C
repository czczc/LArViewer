{
  TGeoManager::Import("microboone.root");
  // gGeoManager->GetTopVolume()->Draw("ogl"); // opengl viewer
  TBrowser b("b", gGeoManager);

  gGeoManager->GetTopVolume()->Draw();
}
