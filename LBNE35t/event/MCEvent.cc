#include "MCEvent.h"
#include "MCGeometry.h"

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

using namespace std;

MCEvent::MCEvent(){}

MCEvent::MCEvent(const char* filename)
{
    rootFile = new TFile(filename);
    simTree = (TTree*)rootFile->Get("/Event/Sim");
    
    geom = new MCGeometry();
    // InitBranchAddress();
    // InitGeometry();
}

MCEvent::~MCEvent()
{
}

void MCEvent::PrintInfo()
{
    geom->PrintInfo();
}