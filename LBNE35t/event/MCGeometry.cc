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

MCGeometry::MCGeometry(const char* mapFileName, const char* rootFileName)
{
    rootFile = new TFile(rootFileName);
    geoTree = (TTree*)rootFile->Get("/Detector/Geometry");

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

    // for (int tpc=1; tpc<=7; tpc++) {
    //     if (tpc%2 == 0) continue;
    //     for (int wire=0; wire<111; wire++) {
    //         cout << tpc << "\t" << wire << "\t" << ProjectionZ(tpc, wire) << endl;
    //     }
    // }
}
