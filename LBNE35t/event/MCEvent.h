#ifndef MCEVENT_H
#define MCEVENT_H

#include "TTree.h"
#include <vector>

class MCGeometry;
class TFile;
class TH2F;

class MCEvent {
public:
    TFile *rootFile;
    TTree *simTree;
    MCGeometry *geom;

    enum LIMITS {
        MAX_CHANNEL = 1992,
    };

    enum VIEW {
        kU = 0,
        kV = 1, 
        kZ = 2,
        kT = -1,
    };

    int nEvents;

    // simTree Leafs
    int eventNo;
    int runNo;
    int subRunNo;

    int raw_Nhit;
    int raw_channelId[MAX_CHANNEL];  // hit channel id; size == raw_Nhit
    int raw_charge[MAX_CHANNEL];  // hit channel charge (simple alg); size == raw_Nhit
    int raw_time[MAX_CHANNEL];  // hit channel time (simple alg); size == raw_Nhit
    std::vector<std::vector<int> > *raw_wfADC;
    std::vector<std::vector<int> > *raw_wfTDC;
    int raw_NZchannels;
    int raw_NUchannels;
    int raw_NVchannels;
    vector<int> raw_ZchannelId;
    vector<int> raw_UchannelId;
    vector<int> raw_VchannelId;

    // histograms
    TH2F *hPixelZT;
    TH2F *hPixelUT;
    TH2F *hPixelVT;


    //-------------------------------------
    MCEvent();
    MCEvent(const char* filename);
    virtual ~MCEvent();

    //  methods
    TTree* Tree() { return simTree; }
    void GetEntry(int i);
    void PrintGeomInfo();
    void PrintInfo(int level=0);  // print the current event(entry) info
    void InitBranchAddress();
    void InitHistograms();

    void FillPixel(int yView, int xView);  // T=-1, U=0, V=1, Z=2
};

#endif