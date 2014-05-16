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
    if (plane == kZ) {  // collection plane
        double start = 0.3;  // starting z for the 0 wire in 0 tpc
        double pitch = 0.3;
        distance = start + pitch * wire;
    }
    else if (plane == kX) {  // drift axis
        double start = -0.6;  // x of the collection plane
        const double xPerTDC = 0.0775; // TDC = 2MHZ; drift = 1.55e6 mm/s
        distance = start + xPerTDC * wire;  // here input is actually tdc number
    }
    return distance;

    // double gap = 2.175;  // gap between APA's
    // // const int wiresPerAPA = 111;
    // const double tpc1End = 49.7325;  // 0.2325 + 0.45*110
    // const double tpc3End = 101.4075;   // 49.7325 + 2.175 + 0.45*110

    // double z = -10000; // init to some unphysical region
    // if (tpc == 1 || tpc == 0) {
    //     z = start + pitch * wire;
    // }
    // else if (tpc == 3 || tpc == 5 || tpc == 2 || tpc == 4) {
    //     z = tpc1End + gap + pitch * wire;
    // }
    // else if (tpc == 7 || tpc == 6) {
    //     z = tpc3End + gap + pitch * wire;
    // }

    // return z;
}


// double MCGeometry::ProjectionU(int tpc, int wire)
// {
//     // const double angle = TMath::Pi()/4;
//     // const double s = TMath::Sin(angle);
//     // const double c = TMath::Cos(angle);

//     // all numbers are in cm;
//     double pitch = 0.4888;  // vertical distance between wires
//     const double tpc1start = -0.48617;  // -(0.932758-0.245198)*s
//     const double tpc3start = -211.531;  // -(113.234+85.244+100.917-0.2452)*s
//     const double tpc5start = -37.0259;  // -(52.6078-0.245198)*s
//     const double tpc7start = -73.5658;  // -(104.283-0.245198)*s
//     const double  tpc1dist = 79.8951;  // distance of tpc1 top left corner to world (0,0) u wire

//     double u = -10000; // init to some unphysical region
//     if (tpc == 1 || tpc == 0) {
//         u = tpc1start - pitch * wire;
//     }
//     else if (tpc == 3 || tpc == 2) {
//         u = tpc3start + pitch * wire;
//     }
//     else if (tpc == 5 || tpc == 4) {
//         u = tpc5start - pitch * wire;
//     }
//     else if (tpc == 7 || tpc == 6) {
//         u = tpc7start - pitch * wire;
//     }
//     u = u + tpc1dist;
//     return u;
// }


// double MCGeometry::ProjectionV(int tpc, int wire)
// {
//     // const double angle = TMath::Pi()/4;
//     // const double s = TMath::Sin(angle);
//     // const double c = TMath::Cos(angle);

//     // all numbers are in cm;
//     double pitch = 0.5012;  // vertical distance between wires
//     const double tpc1start = -73.5662;  // -(153.282-49.2436)*s
//     const double tpc3start = -211.540;  // -(113.243+85.2438+153.282-52.6064)*s
//     const double tpc5start = -37.0262;  // -(153.282-100.919)*s
//     const double tpc7start = -0.48649;  // -(153.282-152.594)*s
//     const double  tpc7dist = 188.4616;  // (113.243+153.282)*s (distance of tpc7 top right corner to world (0,0) v wire)

//     double v = -10000; // init to some unphysical region
//     if (tpc == 1 || tpc == 0) {
//         v = tpc1start - pitch * wire;
//     }
//     else if (tpc == 3 || tpc == 2) {
//         v = tpc3start + pitch * wire;
//     }
//     else if (tpc == 5 || tpc == 4) {
//         v = tpc5start - pitch * wire;
//     }
//     else if (tpc == 7 || tpc == 6) {
//         v = tpc7start - pitch * wire;
//     }
//     v = v + tpc7dist;
//     return v;
// }

