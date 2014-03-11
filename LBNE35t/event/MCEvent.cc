#include "MCEvent.h"
#include "MCGeometry.h"
#include "MCChannel.h"

#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH2F.h"

using namespace std;

MCEvent::MCEvent(){}

MCEvent::MCEvent(const char* filename)
{
    rootFile = new TFile(filename);
    simTree = (TTree*)rootFile->Get("/Event/Sim");
    nEvents = simTree->GetEntries();

    geom = new MCGeometry();
    InitBranchAddress();
    InitHistograms();
}

MCEvent::~MCEvent()
{
    rootFile->Close();
    delete rootFile;

    delete geom;
    
    delete hPixelZT;
    delete hPixelUT;
    delete hPixelVT;    
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

    simTree->SetBranchAddress("mc_Ntrack"       , &mc_Ntrack);
    simTree->SetBranchAddress("mc_id"           , &mc_id);
    simTree->SetBranchAddress("mc_pdg"          , &mc_pdg);
    simTree->SetBranchAddress("mc_mother"       , &mc_mother);
    simTree->SetBranchAddress("mc_daughters"    , &mc_daughters);
    simTree->SetBranchAddress("mc_startXYZT"    , &mc_startXYZT);
    simTree->SetBranchAddress("mc_endXYZT"      , &mc_endXYZT);
    simTree->SetBranchAddress("mc_startMomentum", &mc_startMomentum);
    simTree->SetBranchAddress("mc_endMomentum"  , &mc_endMomentum);

}


void MCEvent::InitHistograms()
{
    // hPixelZT = new TH2F("hPixelZT", "Z vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    // hPixelUT = new TH2F("hPixelUT", "U vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    // hPixelVT = new TH2F("hPixelVT", "V vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);

    hPixelZT = new TH2F("hPixelZT", "Z (|_ collection Y wire) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 343, 0, 0+343*0.45);
    hPixelUT = new TH2F("hPixelUT", "V (|_  induction U wire ) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 510, -168, -168+510*0.4888);
    hPixelVT = new TH2F("hPixelVT", "U (|_  induction V wire ) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 495, -53, -53+495*0.4888);

    hPixelZT->GetXaxis()->SetTitle("x [cm]");
    hPixelUT->GetXaxis()->SetTitle("x [cm]");
    hPixelVT->GetXaxis()->SetTitle("x [cm]");

    hPixelZT->GetYaxis()->SetTitle("z [cm]");
    hPixelUT->GetYaxis()->SetTitle("v [cm]");
    hPixelVT->GetYaxis()->SetTitle("u [cm]");
}

void MCEvent::GetEntry(int entry)
{
    Reset();
    simTree->GetEntry(entry);
    ProcessTracks();
    ProcessChannels();
}

void MCEvent::Reset()
{
    trackIndex.clear();
    trackParents.clear();
    trackChildren.clear();

    raw_NZchannels = 0;
    raw_NUchannels = 0;
    raw_NVchannels = 0;
    raw_ZchannelId.clear();
    raw_UchannelId.clear();
    raw_VchannelId.clear();
}

void MCEvent::ProcessChannels()
{
    for (int i=0; i<raw_Nhit; i++) {
        int channelId = raw_channelId[i];
        MCChannel channel = geom->fChannels[channelId];
        int plane = channel.planes[0];
        if (plane == 0) {
            raw_NUchannels++;
            raw_UchannelId.push_back(channelId);
        }
        else if (plane == 1) {
            raw_NVchannels++;
            raw_VchannelId.push_back(channelId);
        }
        else {
            raw_NZchannels++;
            raw_ZchannelId.push_back(channelId);
        }
    }
}

void MCEvent::FillPixel(int yView, int xView)
{
    vector<int>& channels = raw_ZchannelId;
    TH2F *h = 0;
    int nChannels = 0;
    if (yView == kZ && xView == kT) {
        h = hPixelZT;
        nChannels = raw_NZchannels;
    }
    else if (yView == kU && xView == kT) {
        channels = raw_UchannelId;
        h = hPixelUT;
        nChannels = raw_NUchannels;
    }
    else if (yView == kV && xView == kT) {
        channels = raw_VchannelId;
        h = hPixelVT;
        nChannels = raw_NVchannels;
    }
    else {
        cout << "no such combination view: " << yView << " vs " << xView;
        return;
    }

    h->Reset();
    for (int i=0; i<nChannels; i++) {
        MCChannel channel = geom->fChannels[channels[i]];
        // cout << yView << " View channel " << channels[i] << ": " << endl;
        for (int j=0; j<channel.Nwires; j++) {
            int wire = channel.wires[j];
            int tpc = channel.tpcs[j];
            // int plane = channel.planes[j];

            // skip short drift chamber
            if (tpc % 2 == 0) continue; 

            double y = (double)wire;
            if (yView == kZ) {
                // y = (double)wire;
                y = geom->ProjectionZ(tpc, wire);
                // cout << tpc << " " << wire << " " << y << endl;
            }
            else if (yView == kU) {
                y = geom->ProjectionU(tpc, wire);
            }
            else if (yView == kV) {
                y = geom->ProjectionV(tpc, wire);
            }

            int id = TMath::BinarySearch(raw_Nhit, raw_channelId, channel.channelNo);
            vector<int>& tdcs = (*raw_wfTDC).at(id);
            vector<int>& adcs = (*raw_wfADC).at(id);
            int size_tdc = tdcs.size();
            for (int i_tdc=0; i_tdc<size_tdc; i_tdc++) {
                // double x = tdcs[i_tdc];
                double x = geom->ProjectionX(tpc, tdcs[i_tdc]);
                // cout << tpc << " " << tdcs[i_tdc] << " " << x << endl;
                int weight = adcs[i_tdc];
                if (weight>0) {
                    h->Fill(x, y, weight);
                }
            }

            // cout << raw_time[id] << " ";
            // cout << tpc << " ";
            // cout << plane << " ";

        }
        // cout << endl;
    }
    
}


void MCEvent::PrintGeomInfo()
{
    geom->PrintInfo();
}


void MCEvent::ProcessTracks()
{
    for (int i=0; i<mc_Ntrack; i++) {
        trackIndex[mc_id[i]] = i;
    }
    for (int i=0; i<mc_Ntrack; i++) {
        vector<int> parents;
        if (mc_mother[i]!=0) {
            parents.push_back(trackIndex[mc_mother[i]]);  // for single gen, parent size == 1;
        }
        trackParents.push_back(parents);

        vector<int> children;
        int nChildren = (*mc_daughters).at(i).size();
        for (int j=0; j<nChildren; j++) {
            children.push_back(trackIndex[(*mc_daughters).at(i).at(j)]);
        }
        trackChildren.push_back(children);
    }

}

void MCEvent::PrintInfo(int level)
{
    cout << "run/subRun/event (total) : " 
        << runNo << "/" 
        << subRunNo << "/" 
        << eventNo-1 << " ("
        << nEvents << ")"
        << endl;

    cout << "Hit channels: " << raw_Nhit << endl;
    cout << "Z channels: " << raw_NZchannels << endl;
    cout << "U channels: " << raw_NUchannels << endl;
    cout << "V channels: " << raw_NVchannels << endl;

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
        cout << "raw_charge: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_charge[i] << " ";
        }
        cout << endl << endl;
        cout << "raw_time: ";
        for (int i=0; i<raw_Nhit; i++) {
            cout << raw_time[i] << " ";
        }
        cout << endl << endl;

        cout << "Z channels: ";
        for (int i=0; i<raw_NZchannels; i++) {
            cout << raw_ZchannelId[i] << " ";
        }
        cout << endl << endl;
        cout << "U channels: ";
        for (int i=0; i<raw_NUchannels; i++) {
            cout << raw_UchannelId[i] << " ";
        }
        cout << endl << endl;
        cout << "V channels: ";
        for (int i=0; i<raw_NVchannels; i++) {
            cout << raw_VchannelId[i] << " ";
        }
        cout << endl << endl;

        cout << "first hit channel non zero samples: ";
        cout << (*raw_wfADC)[0].size() << endl;
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfADC)[0].at(i) << " ";
        }
        cout << endl << endl;
        cout << "tdc samples: ";
        for (size_t i=0; i<(*raw_wfADC)[0].size(); i++) {
            cout << (*raw_wfTDC)[0].at(i) << " ";
        }
        cout << endl << endl;
    }

}

