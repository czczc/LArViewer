#ifndef MCGEOMETRY_H
#define MCGEOMETRY_H

#include "MCChannel.h"
#include <map>
#include "TString.h"

#define NCHANNELS 8254

class MCGeometry {
public:

    MCChannel channels[NCHANNELS];
    TString mapFileName;
    std::map<int, int> wireToChannel; // hash map to find channel no. given wire hash

    MCGeometry();
    virtual ~MCGeometry();

    //  methods
    void ReadChanneleWireMap();
    void PrintInfo();
    // double ProjectionZ(int tpc, int wire); // projection to Z coordinate for collection plane
    // double ProjectionU(int tpc, int wire); // projection to perpendicular coordinate of U plane
    // double ProjectionV(int tpc, int wire); // projection to perpendicular coordinate of V plane
    // double ProjectionX(int tpc, int tdc);  // projection to X coordinate for drifting
};

#endif