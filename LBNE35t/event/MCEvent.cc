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
    raw_wfADC = new std::vector<std::vector<int> >;
    raw_wfTDC = new std::vector<std::vector<int> >;
    mc_daughters = new std::vector<std::vector<int> >;  // daughters id of this track; vector

    rootFile = new TFile(filename);
    simTree = (TTree*)rootFile->Get("/Event/Sim");
    nEvents = simTree->GetEntries();

    geom = new MCGeometry();
    optionDisplay = kRAW;      // default display raw signal
    optionInductionSignal = 1; // default draw positive signal only
    for (int i=0; i<4; i++) showAPA[i] = true;

    InitBranchAddress();
    InitHistograms();
}

MCEvent::~MCEvent()
{
    delete hPixelZT;  // delete histogram, then close file
    delete hPixelUT;
    delete hPixelVT;
    delete raw_wfADC;
    delete raw_wfTDC;
    delete mc_daughters;

    rootFile->Close();
    delete rootFile;
    delete geom;
    
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

    simTree->SetBranchAddress("no_hits"    , &no_hits);
    simTree->SetBranchAddress("hit_channel", &hit_channel);
    simTree->SetBranchAddress("hit_peakT"  , &hit_peakT);
    simTree->SetBranchAddress("hit_charge" , &hit_charge);

}


void MCEvent::InitHistograms()
{
    // hPixelZT = new TH2F("hPixelZT", "Z vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    // hPixelUT = new TH2F("hPixelUT", "U vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);
    // hPixelVT = new TH2F("hPixelVT", "V vs T", 3200, 0-0.5, 3200-0.5, 400, 0-0.5, 400-0.5);

    hPixelZT = new TH2F("hPixelZT", "Z (|_ collection Y wire) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 343, 0, 0+343*0.45);
    hPixelUT = new TH2F("hPixelUT", "V (|_  induction U wire ) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 510, -168, -168+510*0.4888);
    hPixelVT = new TH2F("hPixelVT", "U (|_  induction V wire ) vs X (drift axis)", 3200, -1, -1+3200*0.0775, 495, -53, -53+495*0.5012);

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
    (*raw_wfADC).clear();
    (*raw_wfTDC).clear();
    (*mc_daughters).clear();

    trackIndex.clear();
    trackParents.clear();
    trackChildren.clear();
    trackSiblings.clear();

    raw_NZchannels = 0;
    raw_NUchannels = 0;
    raw_NVchannels = 0;
    raw_ZchannelId.clear();
    raw_UchannelId.clear();
    raw_VchannelId.clear();

    hit_NZchannels = 0;
    hit_NUchannels = 0;
    hit_NVchannels = 0;
    hit_ZchannelId.clear();
    hit_UchannelId.clear();
    hit_VchannelId.clear();
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

    for (int i=0; i<no_hits; i++) {
        int channelId = hit_channel[i];
        MCChannel channel = geom->fChannels[channelId];
        int plane = channel.planes[0];
        if (plane == 0) {
            hit_NUchannels++;
            hit_UchannelId.push_back(channelId);
        }
        else if (plane == 1) {
            hit_NVchannels++;
            hit_VchannelId.push_back(channelId);
        }
        else {
            hit_NZchannels++;
            hit_ZchannelId.push_back(channelId);
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
        if (optionDisplay == kRAW) {
           channels = raw_ZchannelId;
           nChannels = raw_NZchannels; 
       }
       else if (optionDisplay == kHITS) {
           channels = hit_ZchannelId;
           nChannels = hit_NZchannels;
       }
    }
    else if (yView == kU && xView == kT) {
        h = hPixelUT;
        if (optionDisplay == kRAW) {
            channels = raw_UchannelId;
            nChannels = raw_NUchannels;
        }
        else if (optionDisplay == kHITS) {
            channels = hit_UchannelId;
            nChannels = hit_NUchannels;
        }
    }
    else if (yView == kV && xView == kT) {
        h = hPixelVT;
        if (optionDisplay == kRAW) {
            channels = raw_VchannelId;
            nChannels = raw_NVchannels;
        }
        else if (optionDisplay == kHITS) {
            channels = hit_VchannelId;
            nChannels = hit_NVchannels;
        }
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
            // only show designated APA's
            if (!showAPA[(tpc-1)/2]) continue;

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

            if (optionDisplay == kRAW) {
                int id = TMath::BinarySearch(raw_Nhit, raw_channelId, channel.channelNo);
                vector<int>& tdcs = (*raw_wfTDC).at(id);
                vector<int>& adcs = (*raw_wfADC).at(id);
                int size_tdc = tdcs.size();
                for (int i_tdc=0; i_tdc<size_tdc; i_tdc++) {
                    // double x = tdcs[i_tdc];
                    double x = geom->ProjectionX(tpc, tdcs[i_tdc]);
                    // cout << tpc << " " << tdcs[i_tdc] << " " << x << endl;
                    int weight = adcs[i_tdc];
                    if (yView == kZ) {
                        if (weight>0) h->Fill(x, y, weight);
                    }
                    else {
                        if (optionInductionSignal == 1) {
                            if (weight>0) h->Fill(x, y, weight);
                        }
                        else if (optionInductionSignal == -1) {
                            if (weight<0) h->Fill(x, y, -weight);
                        }
                        else if (optionInductionSignal == 0) {
                            h->Fill(x, y, fabs(weight));
                        }
                    }
                }
            }
            else if (optionDisplay == kHITS) {
                int id = TMath::BinarySearch(no_hits, hit_channel, channel.channelNo);
                double x = geom->ProjectionX(tpc, hit_peakT[id]);
                h->Fill(x, y, hit_charge[id]);
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
    for (int i=0; i<mc_Ntrack; i++) {
        vector<int> siblings;
        if (mc_mother[i] == 0) {
            siblings.push_back(i);
        }
        else {
            // siblings are simply children of the mother
            int mother = trackIndex[mc_mother[i]];
            int nSiblings = trackChildren.at(mother).size();
            for (int j=0; j<nSiblings; j++) {
                siblings.push_back(trackChildren.at(mother).at(j));
            }
        }
        trackSiblings.push_back(siblings);
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

    cout << "hit channels (raw, hits): " << raw_Nhit << ", " << no_hits << endl;
    cout << "Z channels: " << raw_NZchannels << ", " << hit_NZchannels << endl;
    cout << "U channels: " << raw_NUchannels << ", " << hit_NUchannels << endl;
    cout << "V channels: " << raw_NVchannels << ", " << hit_NVchannels << endl;

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

