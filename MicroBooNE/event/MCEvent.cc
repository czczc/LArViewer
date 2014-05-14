#include "MCEvent.h"
#include "MCGeometry.h"
#include "MCChannel.h"

#include <iostream>
#include <vector>
#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TLine.h"

using namespace std;

MCEvent::MCEvent(const char* dataFileName)
{
    raw_wfADC = new std::vector<std::vector<int> >;
    raw_wfTDC = new std::vector<std::vector<int> >;
    // calib_wfADC = new std::vector<std::vector<int> >;
    // calib_wfTDC = new std::vector<std::vector<int> >;
    mc_daughters = new std::vector<std::vector<int> >;  // daughters id of this track; vector

    rootFile = new TFile(dataFileName);
    if (rootFile->IsZombie()) {
        cout << "Data file " << dataFileName << " Does NOT exist! exiting ..." << endl;
        exit(1);
    }
    simTree = (TTree*)rootFile->Get("/Event/Sim");
    if (!simTree) {
        cout << "TTee /Event/Sim does NOT exist! exiting ..." << endl;
        exit(1);
    }
    nEvents = simTree->GetEntries();
    cout << "total events: " << nEvents << endl;
    geom = new MCGeometry();

    // optionDisplay = kRAW;      // default display raw signal
    // optionInductionSignal = 1; // default draw positive signal only
    // for (int i=0; i<4; i++) showAPA[i] = true;

    InitBranchAddress();
    // InitHistograms();
}

//----------------------------------------------------------------
MCEvent::~MCEvent()
{
    // delete hPixelZT;  // delete histogram, then close file
    // delete hPixelUT;
    // delete hPixelVT;
    delete raw_wfADC;
    delete raw_wfTDC;
    // delete calib_wfADC;
    // delete calib_wfTDC;
    delete mc_daughters;

    rootFile->Close();
    delete rootFile;
    delete geom;
    
}

//----------------------------------------------------------------
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

    // simTree->SetBranchAddress("calib_Nhit"     , &calib_Nhit);
    // simTree->SetBranchAddress("calib_channelId", &calib_channelId);
    // simTree->SetBranchAddress("calib_wfADC"    , &calib_wfADC);
    // simTree->SetBranchAddress("calib_wfTDC"    , &calib_wfTDC);

    simTree->SetBranchAddress("mc_Ntrack"       , &mc_Ntrack);
    simTree->SetBranchAddress("mc_id"           , &mc_id);
    simTree->SetBranchAddress("mc_pdg"          , &mc_pdg);
    simTree->SetBranchAddress("mc_mother"       , &mc_mother);
    simTree->SetBranchAddress("mc_daughters"    , &mc_daughters);
    simTree->SetBranchAddress("mc_startXYZT"    , &mc_startXYZT);
    simTree->SetBranchAddress("mc_endXYZT"      , &mc_endXYZT);
    simTree->SetBranchAddress("mc_startMomentum", &mc_startMomentum);
    simTree->SetBranchAddress("mc_endMomentum"  , &mc_endMomentum);

    // simTree->SetBranchAddress("no_hits"    , &no_hits);
    // simTree->SetBranchAddress("hit_channel", &hit_channel);
    // simTree->SetBranchAddress("hit_peakT"  , &hit_peakT);
    // simTree->SetBranchAddress("hit_charge" , &hit_charge);

}

//----------------------------------------------------------------
void MCEvent::Reset()
{
    (*raw_wfADC).clear();
    (*raw_wfTDC).clear();
    // (*calib_wfADC).clear();
    // (*calib_wfTDC).clear();
    (*mc_daughters).clear();

    // trackIndex.clear();
    // trackParents.clear();
    // trackChildren.clear();
    // trackSiblings.clear();

    // zBintoWireHash.clear();
    // uBintoWireHash.clear();
    // vBintoWireHash.clear();

    // raw_NZchannels = 0;
    // raw_NUchannels = 0;
    // raw_NVchannels = 0;
    // raw_ZchannelId.clear();
    // raw_UchannelId.clear();
    // raw_VchannelId.clear();

    // hit_NZchannels = 0;
    // hit_NUchannels = 0;
    // hit_NVchannels = 0;
    // hit_ZchannelId.clear();
    // hit_UchannelId.clear();
    // hit_VchannelId.clear();
}

//----------------------------------------------------------------
void MCEvent::GetEntry(int entry)
{
    Reset();
    simTree->GetEntry(entry);
    // ProcessTracks();
    // ProcessChannels();
}

//----------------------------------------------------------------
void MCEvent::PrintInfo(int level)
{
    cout 
    << "run/subRun/event (total) : " 
    << runNo << "/" 
    << subRunNo << "/" 
    << eventNo-1 << " ("
    << nEvents << ")"
    << endl;
    cout << "raw hit channels: " << raw_Nhit << endl;
    // cout << "hit channels (raw, calib, hits): " << raw_Nhit << ", " << calib_Nhit << ", " << no_hits << endl;
    // cout << "Z channels: " << raw_NZchannels << ", " << hit_NZchannels << endl;
    // cout << "U channels: " << raw_NUchannels << ", " << hit_NUchannels << endl;
    // cout << "V channels: " << raw_NVchannels << ", " << hit_NVchannels << endl;

    // print mc info
    if (level > 0) {
        cout << "MC tracks:" << mc_Ntrack;
        for (int i=0; i<mc_Ntrack; i++) {
            cout << "\n              id: " << mc_id[i];
            cout << "\n             pdg: " << mc_pdg[i];
            cout << "\n          mother: " << mc_mother[i];
            cout << "\n      Ndaughters: " << (*mc_daughters).at(i).size();
            cout << "\n      start XYZT: (" << mc_startXYZT[i][0] << ", " << mc_startXYZT[i][1] << ", " << mc_startXYZT[i][2] << ", " << mc_startXYZT[i][3] << ")";
            cout << "\n        end XYZT: (" << mc_endXYZT[i][0] << ", " << mc_endXYZT[i][1] << ", " << mc_endXYZT[i][2] << ", " << mc_endXYZT[i][3] << ")";
            cout << "\n  start momentum: (" << mc_startMomentum[i][0] << ", " << mc_startMomentum[i][1] << ", " << mc_startMomentum[i][2] << ", " << mc_startMomentum[i][3] << ")";
            cout << "\n    end momentum: (" << mc_endMomentum[i][0] << ", " << mc_endMomentum[i][1] << ", " << mc_endMomentum[i][2] << ", " << mc_endMomentum[i][3] << ")";
            cout << endl;
        }
    }

    // print raw channel info
    if (level > 1) {
        cout << "raw_channelId: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_channelId[i] << " ";
        }
        cout << endl << endl;

        // cout << "Z channels: ";
        // for (int i=0; i<raw_NZchannels; i++) {
        //     cout << raw_ZchannelId.at(i) << " ";
        // }
        // cout << endl << endl;
        // cout << "U channels: ";
        // for (int i=0; i<raw_NUchannels; i++) {
        //     cout << raw_UchannelId.at(i) << " ";
        // }
        // cout << endl << endl;
        // cout << "V channels: ";
        // for (int i=0; i<raw_NVchannels; i++) {
        //     cout << raw_VchannelId.at(i) << " ";
        // }
        // cout << endl << endl;

        cout << "first hit channel non zero samples: ";
        cout << (*raw_wfADC)[0].size() << endl;
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfADC)[0].at(i) << " ";
        }
        cout << endl << endl;
        cout << "tdc locations: ";
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfTDC)[0].at(i) << " ";
        }
        cout << endl << endl;
    }

}
