#ifndef MCEVENT_H
#define MCEVENT_H

#include "TTree.h"

class TFile;
class MCGeometry;

class MCEvent {
public:
    TFile *rootFile;
    TTree *simTree;
    MCGeometry *geom;

    MCEvent();
    MCEvent(const char* filename);
    virtual ~MCEvent();

    //  methods
    TTree* Tree() { return simTree; }
    void GetEntry(int i) { simTree->GetEntry(i); }
    void PrintInfo();
};

#endif