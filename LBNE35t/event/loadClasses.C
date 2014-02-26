{
    TString prefix = ".";
    TString include = ".include ";
    TString load = ".L ";

    gROOT->ProcessLine( include + prefix );

    gROOT->ProcessLine( load + prefix + "/MCChannel.cc+" );
    gROOT->ProcessLine( load + prefix + "/MCGeometry.cc+" );
    gROOT->ProcessLine( load + prefix + "/MCEvent.cc+" );

}