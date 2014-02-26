{
    gROOT->Reset();
    gROOT->ProcessLine(".x loadClasses.C" );

    new GuiController(gClient->GetRoot(), 800, 600);
}