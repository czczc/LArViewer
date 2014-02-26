#ifndef MCGEOMETRY_H
#define MCGEOMETRY_H

#include "MCChannel.h"

#define NCHANNELS 1992

class TFile;
class TTree;
class TString;

class MCGeometry {
public:
    TFile *rootFile;
    TTree *geoTree;

    MCChannel fChannels[NCHANNELS];

    MCGeometry(const char* mapFileName="../Geometry/ChannelWireMap.txt", const char* rootFileName="../data/sample.root");
    virtual ~MCGeometry();

    //  methods
    void ReadChanneleWireMap(const char* mapFileName);
    void PrintInfo();
};

#endif