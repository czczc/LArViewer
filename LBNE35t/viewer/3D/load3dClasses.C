{
    TString include = ".include ";
    TString load = ".L ";

    TString prefix = "../../event";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/MCEvent.cc+" );

    TString prefix = ".";
    gROOT->ProcessLine( include + prefix );
    gROOT->ProcessLine( load + prefix + "/Gui3DController.cc+" );

}