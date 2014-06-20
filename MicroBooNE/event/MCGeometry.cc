#include "MCGeometry.h"
#include "MCChannel.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

#include "TSystem.h"
#include "TMath.h"

using namespace std;

MCGeometry::MCGeometry()
{
    mapFileName = mapFileName + gSystem->DirName(__FILE__) + "/../geometry/ChannelWireMap.txt";
    if (gSystem->AccessPathName(mapFileName.Data())) { // note the bizarre return convention!
        cout << mapFileName << " Does NOT exist! exiting ..." << endl;
        exit(1);
    }
    ReadChanneleWireMap();
}

//-------------------------------------------------
MCGeometry::~MCGeometry()
{
}

//-------------------------------------------------
void MCGeometry::ReadChanneleWireMap()
{
    ifstream infile(mapFileName);
    string line;
    // skip header
    getline(infile, line);
    getline(infile, line);
    int c, p, w; 
    for (int i=0; i<NCHANNELS; i++) {
        infile >> c >> p >> w;
        channels[i].SetChannel(c, p, w);
    }
    infile.close();
}

//-------------------------------------------------
void MCGeometry::PrintInfo()
{
    cout << "channel-wire map: " << mapFileName << endl;
    cout << "total channels: " << NCHANNELS << endl;
    cout << "channel\tplane\twire\thash" << endl;
    for (int i=0; i<NCHANNELS; i++) {
        channels[i].PrintInfo();
    }
}

//-------------------------------------------------
double MCGeometry::Projection(int plane, int wire)
{
    // all numbers are in cm;
    double distance = -10000; // init to some unphysical region
    if (plane == kX) {  // drift axis
        double start = -0.6-3200*0.0775;  // x of the collection plane
        const double xPerTDC = 0.0775; // TDC = 2MHZ; drift = 1.55e6 mm/s
        distance = start + xPerTDC * wire;  // here input is actually tdc number
    }
    else if (plane == kU) {
        double start = -201.42; // - (117.021 + 115.524)*cos(30./180*3.14)
        double pitch = 0.3;
        distance = start + pitch * wire;
    }
    else if (plane == kV) {
        double start = 0.293; // (0.5935 - 0.0065)*sin(30./180*3.14)
        double pitch = 0.3;
        distance = start + pitch * wire;
    }
    else if (plane == kZ) {  // collection plane
        double start = 0.3;  // starting z for the 0 wire in 0 tpc
        double pitch = 0.3;
        distance = start + pitch * wire;
    }
    return distance;
}


