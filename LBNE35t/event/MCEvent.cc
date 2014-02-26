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
    nEvents = simTree->GetEntries();

    geom = new MCGeometry();
    InitBranchAddress();
    // InitGeometry();
}

MCEvent::~MCEvent()
{
}


void MCEvent::InitBranchAddress()
{
    simTree->SetBranchAddress("eventNo" , &eventNo);
    simTree->SetBranchAddress("runNo"   , &runNo);
    simTree->SetBranchAddress("subRunNo", &subRunNo);

    simTree->SetBranchAddress("raw_Nhit"     , &raw_Nhit);
    simTree->SetBranchAddress("raw_channelId", &raw_channelId);
    simTree->SetBranchAddress("raw_charge"   , &raw_charge);
    simTree->SetBranchAddress("raw_time"     , &raw_time);
    simTree->SetBranchAddress("raw_wfADC"    , &raw_wfADC);
    simTree->SetBranchAddress("raw_wfTDC"    , &raw_wfTDC);
}


void MCEvent::PrintGeomInfo()
{
    geom->PrintInfo();
}

void MCEvent::PrintInfo(int level)
{
    cout << "run/subRun/event (total) : " 
        << runNo << "/" 
        << subRunNo << "/" 
        << eventNo-1 << " ("
        << nEvents << ")"
        << endl;

    cout << "raw_Nhit: " << raw_Nhit << endl;

    if (level > 0) {
        cout << "raw_channelId: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_channelId[i] << " ";
        }
        cout << endl;
        cout << "raw_charge: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_charge[i] << " ";
        }
        cout << endl;
        cout << "raw_time: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_time[i] << " ";
        }
        cout << endl;

        cout << "first hit channel non zero samples: ";
        cout << (*raw_wfADC)[0].size() << endl;
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfADC)[0].at(i) << " ";
        }
        cout << endl;
        cout << "tdc samples: ";
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfTDC)[0].at(i) << " ";
        }
        cout << endl;
    }

}

