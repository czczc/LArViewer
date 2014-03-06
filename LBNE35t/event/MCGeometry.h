#ifndef MCGEOMETRY_H
#define MCGEOMETRY_H

#include "MCChannel.h"

#define NCHANNELS 1992

class TFile;
class TTree;
class TString;

class MCGeometry {
public:
    // TFile *rootFile;
    // TTree *geoTree;

    MCChannel fChannels[NCHANNELS];

    MCGeometry(const char* mapFileName="../Geometry/ChannelWireMap.txt");
    virtual ~MCGeometry();

    //  methods
    void ReadChanneleWireMap(const char* mapFileName);
    void PrintInfo();
    double ProjectionZ(int tpc, int wire); // projection to Z coordinate for collection plane
    double ProjectionU(int tpc, int wire); // projection to perpendicular coordinate of U plane
    double ProjectionV(int tpc, int wire); // projection to perpendicular coordinate of V plane
    double ProjectionX(int tpc, int tdc);  // projection to X coordinate for drifting
};

#endif