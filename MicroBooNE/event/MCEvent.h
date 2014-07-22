#ifndef MCEVENT_H
#define MCEVENT_H

#include <map>
#include <vector>
#include "TTree.h"

class MCGeometry;
class TFile;
class TH2F;

class MCEvent {
public:
    //-------------------------------------
    MCEvent(const char* dataFileName="");
    virtual ~MCEvent();

    //  methods
    TTree* Tree() { return simTree; }
    void InitBranchAddress();
    void GetEntry(int i);
    void Reset();
    void PrintInfo(int level=0);  // print the current event(entry) info
    void InitHistograms();
    void ProcessTracks();
    // void ProcessChannels();
    bool IsPrimary(int i) { return mc_mother[i] == 0; }

    void FillPixel(int plane);  // U=0, V=1, Z=2



    TFile *rootFile;
    TTree *simTree;
    MCGeometry *geom;

    enum LIMITS {
        MAX_CHANNEL = 8254,
        MAX_TRACKS = 3000,
        MAX_HITS = 20000,
    };

    enum DISPLAY {
        kRAW = 1,
        kCALIB = 2,
        kHITS = 3,
        kTRACK = 4,
    };

    int nEvents;
    int currentEventEntry;
    
    // simTree Leafs
    int eventNo;
    int runNo;
    int subRunNo;

    int raw_Nhit;  // number of hit channels
    int raw_channelId[MAX_CHANNEL];  // hit channel id; size == raw_Nhit
    int raw_baseline[MAX_CHANNEL];  // hit channel baseline; size == raw_Nhit
    int raw_charge[MAX_CHANNEL];  // hit channel charge (simple alg); size == raw_Nhit
    int raw_time[MAX_CHANNEL];  // hit channel time (simple alg); size == raw_Nhit
    std::vector<std::vector<int> > *raw_wfADC;
    std::vector<std::vector<int> > *raw_wfTDC;

    // int calib_Nhit;  // number of hit channels
    // int calib_channelId[MAX_CHANNEL];  // hit channel id; size == raw_Nhit
    // std::vector<std::vector<int> > *calib_wfADC;
    // std::vector<std::vector<int> > *calib_wfTDC;

    int mc_Ntrack;  // number of tracks in MC
    int mc_id[MAX_TRACKS];  // track id; size == mc_Ntrack
    int mc_pdg[MAX_TRACKS];  // track particle pdg; size == mc_Ntrack
    int mc_mother[MAX_TRACKS];  // mother id of this track; size == mc_Ntrack
    float mc_startXYZT[MAX_TRACKS][4];  // start position of this track; size == mc_Ntrack
    float mc_endXYZT[MAX_TRACKS][4];  // end position of this track; size == mc_Ntrack
    float mc_startMomentum[MAX_TRACKS][4];  // start momentum of this track; size == mc_Ntrack
    float mc_endMomentum[MAX_TRACKS][4];  // end momentum of this track; size == mc_Ntrack
    std::vector<std::vector<int> > *mc_daughters;  // daughters id of this track; vector

//     int    no_hits;                  //number of hits
//     int    hit_channel[MAX_HITS];    //channel ID
//     float  hit_peakT[MAX_HITS];      //peak time
//     float  hit_charge[MAX_HITS];     //charge (area)

//     // derived variables
//     int raw_NZchannels;
//     int raw_NUchannels;
//     int raw_NVchannels;
//     std::vector<int> raw_ZchannelId;
//     std::vector<int> raw_UchannelId;
//     std::vector<int> raw_VchannelId;

//     int hit_NZchannels;
//     int hit_NUchannels;
//     int hit_NVchannels;
//     std::vector<int> hit_ZchannelId;
//     std::vector<int> hit_UchannelId;
//     std::vector<int> hit_VchannelId;

    std::map<int, int> trackIndex;
    std::vector<std::vector<int> > trackParents;
    std::vector<std::vector<int> > trackChildren;
    std::vector<std::vector<int> > trackSiblings;


    // histograms
    TH2F *hPixel[3]; // 0: XvsU; 1: XvsV; 2: XvsZ; 
    std::map<int, int> bintoWireHash[3];  // for the three histograms, maps bin number to wire hash
    double adc_thresh; // noise threshold for histogram

//     std::map<int, int> zBintoWireHash;
//     std::map<int, int> uBintoWireHash;
//     std::map<int, int> vBintoWireHash;

    int optionDisplay; 
    int optionInductionSignal;  // 1:pos; 0:both; -1:neg
//     bool showAPA[4];


};

#endif