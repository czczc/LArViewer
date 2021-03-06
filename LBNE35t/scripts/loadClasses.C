{
    TString include = ".include ";
    TString load = ".L ";

    TString prefix = "../event";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/MCChannel.cc+" );
    gROOT->ProcessLine( load + prefix + "/MCGeometry.cc+" );
    gROOT->ProcessLine( load + prefix + "/MCEvent.cc+" );

    prefix = "../viewer/2D";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/ViewWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/InfoWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/ControlWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/MainWindow.cc+" );
    gROOT->ProcessLine( load + prefix + "/GuiController.cc+" );
}
