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
}


void MCEvent::InitHistograms()
{
    hPixelZT = new TH2F("hPixelZT", "Z vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    hPixelUT = new TH2F("hPixelUT", "U vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    hPixelVT = new TH2F("hPixelVT", "V vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
}

void MCEvent::GetEntry(int entry)
{
    simTree->GetEntry(entry);

    raw_NZchannels = 0;
    raw_NUchannels = 0;
    raw_NVchannels = 0;
    raw_ZchannelId.clear();
    raw_UchannelId.clear();
    raw_VchannelId.clear();

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

            int id = TMath::BinarySearch(raw_Nhit, raw_channelId, channel.channelNo);
            vector<int>& tdcs = (*raw_wfTDC).at(id);
            vector<int>& adcs = (*raw_wfADC).at(id);
            int size_tdc = tdcs.size();
            for (int i_tdc=0; i_tdc<size_tdc; i_tdc++) {
                int x = tdcs[i_tdc];
                int y = wire;
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
    h->SetMinimum(-1); // produces an "inverted picture"
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

    cout << "Hit channels: " << raw_Nhit << endl;
    cout << "Z channels: " << raw_NZchannels << endl;
    cout << "U channels: " << raw_NUchannels << endl;
    cout << "V channels: " << raw_NVchannels << endl;

    if (level > 0) {
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

