#ifndef MCGEOMETRY_H
#define MCGEOMETRY_H

#include "MCChannel.h"
#include <map>
#include "TString.h"

#define NCHANNELS 8254

class MCGeometry {
private: 
    MCGeometry();
    MCGeometry(MCGeometry const&) {};
    void operator=(MCGeometry const&) {};

public:

    MCChannel channels[NCHANNELS];
    TString mapFileName;
    std::map<int, int> wireToChannel; // hash map to find channel no. given wire hash

    virtual ~MCGeometry();

    // singleton method
    static MCGeometry& GetInstance() { static MCGeometry geom; return geom; }

    //  methods
    void ReadChanneleWireMap();
    void PrintInfo();
    double Projection(int plane, int wire);
    // double ProjectionZ(int wire); // projection to Z coordinate for collection plane
    // double ProjectionU(int wire); // projection to perpendicular coordinate of U plane
    // double ProjectionV(int wire); // projection to perpendicular coordinate of V plane
    // double ProjectionX(int tdc);  // projection to X coordinate for drifting

    enum VIEW {
        kU = 0,
        kV = 1, 
        kZ = 2,   // collection
        kX = 3,   // drift
    };
};

#endif