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
    currentEventEntry = -1;  // don't load event at initialization

    raw_wfADC = new std::vector<std::vector<int> >;
    raw_wfTDC = new std::vector<std::vector<int> >;
    calib_wfADC = new std::vector<std::vector<int> >;
    calib_wfTDC = new std::vector<std::vector<int> >;
    mc_daughters = new std::vector<std::vector<int> >;  // daughters id of this track; vector

    for (int i=0; i<MAX_TRACKER; i++) {
        trk_nHit[i] = new std::vector<int>;
        trk_length[i] = new std::vector<double>;
        trk_start_xyz[i] = new std::vector<std::vector<double> >;
        trk_end_xyz[i] = new std::vector<std::vector<double> >;
        trk_start_dxyz[i] = new std::vector<std::vector<double> >;
        trk_end_dxyz[i] = new std::vector<std::vector<double> >;
        trk_points_x[i] = new std::vector<std::vector<double> >;
        trk_points_y[i] = new std::vector<std::vector<double> >;
        trk_points_z[i] = new std::vector<std::vector<double> >;
        for (int j=0; j<3; j++) {
            trk_calo_KE[i][j] = new std::vector<double>;
            trk_calo_range[i][j] = new std::vector<double>;
            trk_calo_nHit[i][j] = new std::vector<int>;
            trk_calo_dedx[i][j] = new std::vector<std::vector<double> >;
            trk_calo_dqdx[i][j] = new std::vector<std::vector<double> >;
            trk_calo_resRange[i][j] = new std::vector<std::vector<double> >;

        }
    }

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
    geom =  &MCGeometry::GetInstance();

    optionDisplay = kRAW;      // default display raw signal
    optionInductionSignal = 1; // default draw positive signal only

    InitBranchAddress();
    InitHistograms();

}

//----------------------------------------------------------------
MCEvent::~MCEvent()
{
    for (int i=0; i<3; i++) {
        delete hPixel[i];  // delete histogram, then close file
    }

    delete raw_wfADC;
    delete raw_wfTDC;
    delete calib_wfADC;
    delete calib_wfTDC;
    delete mc_daughters;

    for (int i=0; i<MAX_TRACKER; i++) {
        delete trk_nHit[i];
        delete trk_length[i];
        delete trk_start_xyz[i];
        delete trk_end_xyz[i];
        delete trk_start_dxyz[i];
        delete trk_end_dxyz[i];
        delete trk_points_x[i];
        delete trk_points_y[i];
        delete trk_points_z[i];
        for (int j=0; j<3; j++) {
            delete trk_calo_KE[i][j];
            delete trk_calo_range[i][j];
            delete trk_calo_nHit[i][j];
            delete trk_calo_dedx[i][j];
            delete trk_calo_dqdx[i][j];
            delete trk_calo_resRange[i][j];

        }
    }

    rootFile->Close();
    delete rootFile;
    
}

//----------------------------------------------------------------
void MCEvent::InitBranchAddress()
{
    simTree->SetBranchAddress("eventNo" , &eventNo);
    simTree->SetBranchAddress("runNo"   , &runNo);
    simTree->SetBranchAddress("subRunNo", &subRunNo);

    simTree->SetBranchAddress("raw_Nhit"     , &raw_Nhit);
    simTree->SetBranchAddress("raw_channelId", &raw_channelId);
    simTree->SetBranchAddress("raw_baseline" , &raw_baseline);
    simTree->SetBranchAddress("raw_charge"   , &raw_charge);
    simTree->SetBranchAddress("raw_time"     , &raw_time);
    simTree->SetBranchAddress("raw_wfADC"    , &raw_wfADC);
    simTree->SetBranchAddress("raw_wfTDC"    , &raw_wfTDC);

    simTree->SetBranchAddress("calib_Nhit"     , &calib_Nhit);
    simTree->SetBranchAddress("calib_channelId", &calib_channelId);
    simTree->SetBranchAddress("calib_wfADC"    , &calib_wfADC);
    simTree->SetBranchAddress("calib_wfTDC"    , &calib_wfTDC);

    simTree->SetBranchAddress("mc_Ntrack"       , &mc_Ntrack);
    simTree->SetBranchAddress("mc_id"           , &mc_id);
    simTree->SetBranchAddress("mc_pdg"          , &mc_pdg);
    simTree->SetBranchAddress("mc_process"      , &mc_process);
    simTree->SetBranchAddress("mc_mother"       , &mc_mother);
    simTree->SetBranchAddress("mc_daughters"    , &mc_daughters);
    simTree->SetBranchAddress("mc_startXYZT"    , &mc_startXYZT);
    simTree->SetBranchAddress("mc_endXYZT"      , &mc_endXYZT);
    simTree->SetBranchAddress("mc_startMomentum", &mc_startMomentum);
    simTree->SetBranchAddress("mc_endMomentum"  , &mc_endMomentum);


    simTree->SetBranchAddress("mc_isnu"  , &mc_isnu);
    simTree->SetBranchAddress("mc_nGeniePrimaries"  , &mc_nGeniePrimaries);
    simTree->SetBranchAddress("mc_nu_pdg"  , &mc_nu_pdg);
    simTree->SetBranchAddress("mc_nu_ccnc"  , &mc_nu_ccnc);
    simTree->SetBranchAddress("mc_nu_mode"  , &mc_nu_mode);
    simTree->SetBranchAddress("mc_nu_intType"  , &mc_nu_intType);
    simTree->SetBranchAddress("mc_nu_target"  , &mc_nu_target);
    simTree->SetBranchAddress("mc_hitnuc"  , &mc_hitnuc);
    simTree->SetBranchAddress("mc_hitquark"  , &mc_hitquark);
    simTree->SetBranchAddress("mc_nu_Q2"  , &mc_nu_Q2);
    simTree->SetBranchAddress("mc_nu_W"  , &mc_nu_W);
    simTree->SetBranchAddress("mc_nu_X"  , &mc_nu_X);
    simTree->SetBranchAddress("mc_nu_Y"  , &mc_nu_Y);
    simTree->SetBranchAddress("mc_nu_Pt"  , &mc_nu_Pt);
    simTree->SetBranchAddress("mc_nu_X"  , &mc_nu_X);
    simTree->SetBranchAddress("mc_nu_pos"  , &mc_nu_pos);
    simTree->SetBranchAddress("mc_nu_mom"  , &mc_nu_mom);

    simTree->SetBranchAddress("no_hits"    , &no_hits);
    simTree->SetBranchAddress("hit_channel", &hit_channel);
    simTree->SetBranchAddress("hit_plane"  , &hit_plane);
    simTree->SetBranchAddress("hit_peakT"  , &hit_peakT);
    simTree->SetBranchAddress("hit_charge" , &hit_charge);

    for (size_t i=0; i<1; i++) {
        TString name = "trackkalmanhit";
        simTree->SetBranchAddress((name+"_nTrack"    ).Data(), &trk_nTrack[i]);
        simTree->SetBranchAddress((name+"_nHit"      ).Data(), &trk_nHit[i]);
        simTree->SetBranchAddress((name+"_length"    ).Data(), &trk_length[i]);
        simTree->SetBranchAddress((name+"_start_xyz" ).Data(), &trk_start_xyz[i]);
        simTree->SetBranchAddress((name+"_end_xyz"   ).Data(), &trk_end_xyz[i]);
        simTree->SetBranchAddress((name+"_start_dxyz").Data(), &trk_start_dxyz[i]);
        simTree->SetBranchAddress((name+"_end_dxyz"  ).Data(), &trk_end_dxyz[i]);

        simTree->SetBranchAddress((name+"_points_x" ).Data(), &trk_points_x[i]);
        simTree->SetBranchAddress((name+"_points_y" ).Data(), &trk_points_y[i]);
        simTree->SetBranchAddress((name+"_points_z" ).Data(), &trk_points_z[i]);

        for (int j=0; j<3; j++) {
            TString caloName;
            caloName.Form("%s_calo%i_KE", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_KE[i][j]);
            caloName.Form("%s_calo%i_range", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_range[i][j]);
            caloName.Form("%s_calo%i_nHit", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_nHit[i][j]);
            caloName.Form("%s_calo%i_dedx", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_dedx[i][j]);
            caloName.Form("%s_calo%i_dqdx", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_dqdx[i][j]);
            caloName.Form("%s_calo%i_resRange", name.Data(), j);
            simTree->SetBranchAddress(caloName.Data(), &trk_calo_resRange[i][j]);
        }
        
    }

}

//----------------------------------------------------------------
void MCEvent::InitHistograms()
{
    // const double xPerTDC = 0.0775;
    const double xPerTDC = 0.0803;
    const int nTDC = 3200*3;
    double x_start = -0.6-3200*xPerTDC;
    hPixel[0] = new TH2F("hPixel_0", "X (drift distance) vs U", 2398, -101, -101+2398*0.3, nTDC, x_start, x_start+nTDC*xPerTDC);
    hPixel[1] = new TH2F("hPixel_1", "X (drift distance) vs V", 2398,  -57,  -57+2398*0.3, nTDC, x_start, x_start+nTDC*xPerTDC);
    hPixel[2] = new TH2F("hPixel_2", "X (drift distance) vs Z", 3455,  0.15,  0.15+3455*0.3, nTDC, x_start, x_start+nTDC*xPerTDC);

    hPixel[0]->GetYaxis()->SetTitle("x [cm]");
    hPixel[1]->GetYaxis()->SetTitle("x [cm]");
    hPixel[2]->GetYaxis()->SetTitle("x [cm]");

    hPixel[0]->GetXaxis()->SetTitle("u [cm]");
    hPixel[1]->GetXaxis()->SetTitle("v [cm]");
    hPixel[2]->GetXaxis()->SetTitle("z [cm]");

}

//----------------------------------------------------------------
void MCEvent::Reset()
{
    (*raw_wfADC).clear();
    (*raw_wfTDC).clear();
    (*calib_wfADC).clear();
    (*calib_wfTDC).clear();
    (*mc_daughters).clear();
    for (int i=0; i<MAX_TRACKER; i++) {
        trk_nTrack[i] = -1;
        (*trk_nHit[i]).clear();
        (*trk_length[i]).clear();
        (*trk_start_xyz[i]).clear();
        (*trk_end_xyz[i]).clear();
        (*trk_start_dxyz[i]).clear();
        (*trk_end_dxyz[i]).clear();
        (*trk_points_x[i]).clear();
        (*trk_points_y[i]).clear();
        (*trk_points_z[i]).clear();
        for (int j=0; j<3; j++) {
            (*trk_calo_KE[i][j]).clear();
            (*trk_calo_range[i][j]).clear();
            (*trk_calo_nHit[i][j]).clear();
            (*trk_calo_dedx[i][j]).clear();
            (*trk_calo_dqdx[i][j]).clear();
            (*trk_calo_resRange[i][j]).clear();

        }
    }

    trackIndex.clear();
    trackParents.clear();
    trackChildren.clear();
    trackSiblings.clear();

    for (int i=0; i<3; i++) {
        bintoWireHash[i].clear();
    }

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
    ProcessTracks();

    currentEventEntry = entry;
}

//----------------------------------------------------------------
void MCEvent::ProcessTracks()
{
    // map track id to track index in the array
    for (int i=0; i<mc_Ntrack; i++) {
        trackIndex[mc_id[i]] = i;
    }

    // in trackParents, trackChildren, trackSiblings vectors, store track index (not track id)
    for (int i=0; i<mc_Ntrack; i++) {
        // currently, parent size == 1;
        // for primary particle, parent id = 0;
        vector<int> parents;
        if ( !IsPrimary(i) ) {
            parents.push_back(trackIndex[mc_mother[i]]);
        }
        trackParents.push_back(parents); // primary track will have 0 parents

        vector<int> children;
        int nChildren = (*mc_daughters).at(i).size();
        for (int j=0; j<nChildren; j++) {
            children.push_back(trackIndex[(*mc_daughters).at(i).at(j)]);
        }
        trackChildren.push_back(children);

    }

    // siblings
    for (int i=0; i<mc_Ntrack; i++) {
        vector<int> siblings;
        if ( IsPrimary(i) ) {
            for (int j=0; j<mc_Ntrack; j++) {
                if( IsPrimary(j) ) {
                    siblings.push_back(j);
                }
            }
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

//----------------------------------------------------------------
void MCEvent::FillPixel(int wirePlane)
{
    adc_thresh = 1.;

    if (wirePlane >= 3 or wirePlane < 0) {
        cout << "Plane " << wirePlane << "does not exist. exiting ..." << endl;
        exit(1);
    }
    TH2F *h = hPixel[wirePlane];
    h->Reset();
    map<int, int> *m = &bintoWireHash[wirePlane];


    if (optionDisplay == kRAW || optionDisplay == kCALIB) {
        for (int i=0; i<raw_Nhit; i++) {
            int channelId = raw_channelId[i];
            MCChannel channel = geom->channels[channelId];
            if (channel.plane != wirePlane) continue; // skip other channels

            int wire = channel.wire;
            double x = geom->Projection(wirePlane, wire);
            int xbin = h->GetXaxis()->FindBin(x);
            (*m)[xbin] = channel.hash;
            int idx = 0;
            vector<int>& tdcs = (*raw_wfTDC).at(idx);
            vector<int>& adcs = (*raw_wfADC).at(idx);
            int size_tdc = tdcs.size();
            if (optionDisplay == kRAW) {
                idx = TMath::BinarySearch(raw_Nhit, raw_channelId, channel.channelNo);
                tdcs = (*raw_wfTDC).at(idx);
                adcs = (*raw_wfADC).at(idx);
                size_tdc = tdcs.size();
            }
            else {  // calib wire
            }
            for (int i_tdc=0; i_tdc<size_tdc; i_tdc++) {
                double y = geom->Projection(MCGeometry::kX, tdcs[i_tdc]);
                // cout << tpc << " " << tdcs[i_tdc] << " " << x << endl;
                // int weight = adcs[i_tdc]-400;
                int weight = adcs[i_tdc];
                if (weight>1e4) {
                    cout << weight << endl;
                }
                if (weight<1) continue;
                // if (wirePlane==0 || wirePlane==1) continue;
                if (fabs(weight) < adc_thresh) {
                    continue; // removing noise, increasing draw speed.
                }
                if (wirePlane == MCGeometry::kZ) {
                    if (weight>0) h->Fill(x, y, weight);
                }
                else {  // induction plane has negative readouts
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
            }  // tdc loops done
        }  // channel loops done
    } // raw & calib done



    //         for (int j=0; j<channel.Nwires; j++) {


    //             if (optionDisplay == kRAW) {
    //                 id = TMath::BinarySearch(raw_Nhit, raw_channelId, channel.channelNo);
    //                 tdcs = (*raw_wfTDC).at(id);
    //                 adcs = (*raw_wfADC).at(id);
    //                 size_tdc = tdcs.size();
    //             }
    //             else {  // calib wire
    //                 id = TMath::BinarySearch(calib_Nhit, calib_channelId, channel.channelNo);
    //                 if (calib_channelId[id] == channel.channelNo) {
    //                     tdcs = (*calib_wfTDC).at(id);
    //                     adcs = (*calib_wfADC).at(id);
    //                     size_tdc = tdcs.size();
    //                 }
    //                 else {
    //                     cout << "cannot find raw channel " << channel.channelNo << " in calib wire, skipping" << endl; 
    //                     size_tdc = 0;
    //                 }
    //             }
    //             for (int i_tdc=0; i_tdc<size_tdc; i_tdc++) {
    //                 // double x = tdcs[i_tdc];
    //                 double x = geom->ProjectionX(tpc, tdcs[i_tdc]);
    //                 // cout << tpc << " " << tdcs[i_tdc] << " " << x << endl;
    //                 int weight = adcs[i_tdc];
    //                 if (weight>1e4) {
    //                     cout << weight << endl;
    //                 }
    //                 if (yView == kZ) {
    //                     if (weight>0) h->Fill(x, y, weight);
    //                 }
    //                 else {
    //                     if (optionInductionSignal == 1) {
    //                         if (weight>0) h->Fill(x, y, weight);
    //                     }
    //                     else if (optionInductionSignal == -1) {
    //                         if (weight<0) h->Fill(x, y, -weight);
    //                     }
    //                     else if (optionInductionSignal == 0) {
    //                         h->Fill(x, y, fabs(weight));
    //                     }
    //                 }
    //             }
    //         }
    //     }
    // else if (optionDisplay == kHITS) {
    //     for (int i=0; i<no_hits; i++) {
    //         int channelId = hit_channel[i];
    //         MCChannel channel = geom->channels[channelId];
    //         int plane = channel.planes[0];
    //         if ( ! ((yView == kU && plane == 0) || 
    //                 (yView == kV && plane == 1) ||
    //                 (yView == kZ && plane == 2))
    //            ) {
    //             continue; // skip other channels
    //         }
    //         for (int j=0; j<channel.Nwires; j++) {
    //             int wire = channel.wires[j];
    //             int tpc = channel.tpcs[j];

    //             // skip short drift chamber
    //             if (tpc % 2 == 0) continue; 
    //             // only show designated APA's
    //             if (!showAPA[(tpc-1)/2]) continue;

    //             double x = geom->ProjectionX(tpc, hit_peakT[i]);
    //             double y = _ProjectionY(yView, tpc, wire);
    //             h->Fill(x, y, hit_charge[i]);
    //         }
    //     }
    // } // hits done
    
}

//----------------------------------------------------------------
void MCEvent::PrintInfo(int level)
{
    cout 
    << "run/subRun/event: " 
    << runNo << "/" 
    << subRunNo << "/" 
    << eventNo << " ("
    << currentEventEntry << "/"
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
        cout << (*raw_wfADC)[2].size() << endl;
        for (size_t i=0; i<(*raw_wfADC)[2].size(); i++) {
            cout << (*raw_wfADC)[2].at(i) << " ";
        }
        cout << endl << endl;
        cout << "tdc locations: ";
        for (size_t i=0; i<(*raw_wfADC)[2].size(); i++) {
            cout << (*raw_wfTDC)[2].at(i) << " ";
        }
        cout << endl << endl;
    }

}
