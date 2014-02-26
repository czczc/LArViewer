#ifndef MCEVENT_H
#define MCEVENT_H

#include "TTree.h"
#include <vector>

class TFile;
class MCGeometry;

class MCEvent {
public:
    TFile *rootFile;
    TTree *simTree;
    MCGeometry *geom;

    enum State {
        MAX_CHANNEL = 1992,
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

    MCEvent();
    MCEvent(const char* filename);
    virtual ~MCEvent();

    //  methods
    TTree* Tree() { return simTree; }
    void GetEntry(int i) { simTree->GetEntry(i); }
    void PrintGeomInfo();
    void PrintInfo(int level=0);  // print the current event(entry) info
    void InitBranchAddress();
};

#endif