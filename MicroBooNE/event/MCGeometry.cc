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

MCGeometry::~MCGeometry()
{
}

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

void MCGeometry::PrintInfo()
{
    cout << "channel-wire map: " << mapFileName << endl;
    cout << "total channels: " << NCHANNELS << endl;
    cout << "channel\tplane\twire\thash" << endl;
    for (int i=0; i<NCHANNELS; i++) {
        channels[i].PrintInfo();
    }
}
