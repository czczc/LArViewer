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

void MCGeometry::PrintInfo()
{
    for (int i=0; i<NCHANNELS; i++) {
        fChannels[i].PrintInfo();
    }
}
