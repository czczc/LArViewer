#ifndef MCEVENT_H
#define MCEVENT_H

#include <map>
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
        MAX_TRACKS = 1000,
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

    int raw_Nhit;  // number of hit channels
    int raw_channelId[MAX_CHANNEL];  // hit channel id; size == raw_Nhit
    int raw_charge[MAX_CHANNEL];  // hit channel charge (simple alg); size == raw_Nhit
    int raw_time[MAX_CHANNEL];  // hit channel time (simple alg); size == raw_Nhit
    std::vector<std::vector<int> > *raw_wfADC;
    std::vector<std::vector<int> > *raw_wfTDC;

    int mc_Ntrack;  // number of tracks in MC
    int mc_id[MAX_TRACKS];  // track id; size == mc_Ntrack
    int mc_pdg[MAX_TRACKS];  // track particle pdg; size == mc_Ntrack
    int mc_mother[MAX_TRACKS];  // mother id of this track; size == mc_Ntrack
    float mc_startXYZT[MAX_TRACKS][4];  // start position of this track; size == mc_Ntrack
    float mc_endXYZT[MAX_TRACKS][4];  // end position of this track; size == mc_Ntrack
    float mc_startMomentum[MAX_TRACKS][4];  // start momentum of this track; size == mc_Ntrack
    float mc_endMomentum[MAX_TRACKS][4];  // end momentum of this track; size == mc_Ntrack
    std::vector<std::vector<int> > *mc_daughters;  // daughters id of this track; vector

    // derived variables
    int raw_NZchannels;
    int raw_NUchannels;
    int raw_NVchannels;
    vector<int> raw_ZchannelId;
    vector<int> raw_UchannelId;
    vector<int> raw_VchannelId;

    map<int, int> trackIndex;
    std::vector<std::vector<int> > trackParents;
    std::vector<std::vector<int> > trackChildren;


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
    void Reset();
    void PrintGeomInfo();
    void PrintInfo(int level=0);  // print the current event(entry) info
    void InitBranchAddress();
    void InitHistograms();
    void ProcessTracks();
    void ProcessChannels();
    void FillPixel(int yView, int xView);  // T=-1, U=0, V=1, Z=2

};

#endif