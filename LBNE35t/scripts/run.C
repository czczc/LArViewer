{
    gROOT->Reset();
    gROOT->ProcessLine(".x loadClasses.C" );

    new GuiController(gClient->GetRoot(), 900, 900);
}