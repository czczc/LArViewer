#include "MCGeometry.h"
#include "MCChannel.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

using namespace std;

MCGeometry::MCGeometry(const char* mapFileName)
{
    // rootFile = new TFile(rootFileName);
    // geoTree = (TTree*)rootFile->Get("/Detector/Geometry");

    ReadChanneleWireMap(mapFileName);

  // InitBranchAddress();
  // InitGeometry();
}

MCGeometry::~MCGeometry()
{
}

void MCGeometry::ReadChanneleWireMap(const char* mapFileName)
{
    ifstream infile(mapFileName);
    string line;
    // skip header
    getline(infile, line);
    getline(infile, line);

    string tmp;
    for (int i=0; i<NCHANNELS; i++) {
        MCChannel *channel = &fChannels[i];
        infile >> tmp >> channel->channelNo;
        infile >> tmp >> channel->Nwires;
        infile >> tmp;
        for (int j=0; j<channel->Nwires; j++) {
            int value;
            infile >> value;
            channel->tpcs.push_back(value);
        }
        infile >> tmp;
        for (int j=0; j<channel->Nwires; j++) {
            int value;
            infile >> value;
            channel->planes.push_back(value);
        }
        infile >> tmp;
        for (int j=0; j<channel->Nwires; j++) {
            int value;
            infile >> value;
            channel->wires.push_back(value);
        }
    }
    infile.close();
}

double MCGeometry::ProjectionZ(int tpc, int wire)
{
    // all numbers are in cm;
    double start = 0.2325;  // starting z for the 0 wire in 0 tpc
    double pitch = 0.45; 
    double gap = 2.175;  // gap between APA's
    // const int wiresPerAPA = 111;
    const double tpc1End = 49.7325;  // 0.2325 + 0.45*110
    const double tpc3End = 101.4075;   // 49.7325 + 2.175 + 0.45*110

    double z = -10000; // init to some unphysical region
    if (tpc == 1 || tpc == 0) {
        z = start + pitch * wire;
    }
    else if (tpc == 3 || tpc == 5 || tpc == 2 || tpc == 4) {
        z = tpc1End + gap + pitch * wire;
    }
    else if (tpc == 7 || tpc == 6) {
        z = tpc3End + gap + pitch * wire;
    }

    return z;
}


double MCGeometry::ProjectionU(int tpc, int wire)
{
    // const double angle = TMath::Pi()/4;
    // const double s = TMath::Sin(angle);
    // const double c = TMath::Cos(angle);

    // all numbers are in cm;
    double pitch = 0.4888;  // vertical distance between wires
    const double tpc1start = -0.48617;  // -(0.932758-0.245198)*s
    const double tpc3start = -211.531;  // -(113.234+85.244+100.917-0.2452)*s
    const double tpc5start = -37.0259;  // -(52.6078-0.245198)*s
    const double tpc7start = -73.5658;  // -(104.283-0.245198)*s
    const double  tpc1dist = 79.8951;  // distance of tpc1 top left corner to world (0,0) u wire

    double u = -10000; // init to some unphysical region
    if (tpc == 1 || tpc == 0) {
        u = tpc1start - pitch * wire;
    }
    else if (tpc == 3 || tpc == 2) {
        u = tpc3start + pitch * wire;
    }
    else if (tpc == 5 || tpc == 4) {
        u = tpc5start - pitch * wire;
    }
    else if (tpc == 7 || tpc == 6) {
        u = tpc7start - pitch * wire;
    }
    u = u + tpc1dist;
    return u;
}


double MCGeometry::ProjectionV(int tpc, int wire)
{
    // const double angle = TMath::Pi()/4;
    // const double s = TMath::Sin(angle);
    // const double c = TMath::Cos(angle);

    // all numbers are in cm;
    double pitch = 0.4888;  // vertical distance between wires
    const double tpc1start = -73.5662;  // -(153.282-49.2436)*s
    const double tpc3start = -211.540;  // -(113.243+85.2438+153.282-52.6064)*s
    const double tpc5start = -37.0262;  // -(153.282-100.919)*s
    const double tpc7start = -0.48649;  // -(153.282-152.594)*s
    const double  tpc7dist = 188.4616;  // (113.243+153.282)*s (distance of tpc7 top right corner to world (0,0) v wire)

    double v = -10000; // init to some unphysical region
    if (tpc == 1 || tpc == 0) {
        v = tpc1start - pitch * wire;
    }
    else if (tpc == 3 || tpc == 2) {
        v = tpc3start + pitch * wire;
    }
    else if (tpc == 5 || tpc == 4) {
        v = tpc5start - pitch * wire;
    }
    else if (tpc == 7 || tpc == 6) {
        v = tpc7start - pitch * wire;
    }
    v = v + tpc7dist;
    return v;
}


double MCGeometry::ProjectionX(int tpc, int tdc)
{
    // all numbers are in cm;
    const double xPerTDC = 0.0775; // TDC = 2MHZ; drift = 1.55e6 mm/s
    double start = -0.967;  // starting z for the 0 wire in 0 tpc

    double x = -10000; // init to some unphysical region
    if (tpc % 2 == 1) {
        x = start + xPerTDC * tdc;
    }
    else {
        x = start + xPerTDC * tdc;  // keep same for now
    }

    return x;
}


void MCGeometry::PrintInfo()
{
    for (int i=0; i<NCHANNELS; i++) {
        fChannels[i].PrintInfo();
    }

    for (int tpc=1; tpc<=7; tpc++) {
        if (tpc%2 == 0) continue;
        int Nwires = 0;
        // if (tpc == 1 || tpc == 7 ) Nwires = 357;
        // else if (tpc == 3) Nwires = 193;
        // else if (tpc == 5) Nwires = 234;
        if (tpc == 1 || tpc == 7 ) Nwires = 343;
        else if (tpc == 3) Nwires = 187;
        else if (tpc == 5) Nwires = 226;

        for (int wire=0; wire<Nwires; wire++) {
            // cout << tpc << "\t" << wire << "\t" << ProjectionZ(tpc, wire) << endl;
            // cout << tpc << "\t" << wire << "\t" << ProjectionU(tpc, wire) << endl;
            cout << tpc << "\t" << wire << "\t" << ProjectionV(tpc, wire) << endl;
        }
    }
}