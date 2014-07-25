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
        MAX_TRACKER = 10,
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

    int calib_Nhit;  // number of hit channels
    int calib_channelId[MAX_CHANNEL];  // hit channel id; size == raw_Nhit
    std::vector<std::vector<int> > *calib_wfADC;
    std::vector<std::vector<int> > *calib_wfTDC;

    int mc_Ntrack;  // number of tracks in MC
    int mc_id[MAX_TRACKS];  // track id; size == mc_Ntrack
    int mc_pdg[MAX_TRACKS];  // track particle pdg; size == mc_Ntrack
    int mc_process[MAX_TRACKS];  // track generation process code; size == mc_Ntrack
    int mc_mother[MAX_TRACKS];  // mother id of this track; size == mc_Ntrack
    float mc_startXYZT[MAX_TRACKS][4];  // start position of this track; size == mc_Ntrack
    float mc_endXYZT[MAX_TRACKS][4];  // end position of this track; size == mc_Ntrack
    float mc_startMomentum[MAX_TRACKS][4];  // start momentum of this track; size == mc_Ntrack
    float mc_endMomentum[MAX_TRACKS][4];  // end momentum of this track; size == mc_Ntrack
    std::vector<std::vector<int> > *mc_daughters;  // daughters id of this track; vector

    int mc_isnu; // is neutrino interaction
    int mc_nGeniePrimaries; // number of Genie primaries
    int mc_nu_pdg; // pdg code of neutrino
    int mc_nu_ccnc; // cc or nc
    int mc_nu_mode; // mode: http://nusoft.fnal.gov/larsoft/doxsvn/html/MCNeutrino_8h_source.html
    int mc_nu_intType; // interaction type
    int mc_nu_target; // target interaction
    int mc_hitnuc; // hit nucleon
    int mc_hitquark; // hit quark
    double mc_nu_Q2; // Q^2
    double mc_nu_W; // W
    double mc_nu_X; // X
    double mc_nu_Y; // Y
    double mc_nu_Pt; // Pt
    double mc_nu_Theta; // angle relative to lepton
    float mc_nu_pos[4];  // interaction position of nu
    float mc_nu_mom[4];  // interaction momentum of nu

    int    no_hits;                  //number of hits
    int    hit_channel[MAX_HITS];    //channel ID
    int    hit_plane[MAX_HITS];    //channel ID
    float  hit_peakT[MAX_HITS];      //peak time
    float  hit_charge[MAX_HITS];     //charge (area)

    int trk_nTrack[MAX_TRACKER]; // no. of tracks of each tracker.
    std::vector<int>* trk_nHit[MAX_TRACKER];  // no. of hits of each track
    std::vector<double>* trk_length[MAX_TRACKER];  // length of each track
    std::vector<std::vector<double> >* trk_start_xyz[MAX_TRACKER];  // position of start vertex
    std::vector<std::vector<double> >* trk_end_xyz[MAX_TRACKER];  // position of end vertex
    std::vector<std::vector<double> >* trk_start_dxyz[MAX_TRACKER];  // direction of start vertex
    std::vector<std::vector<double> >* trk_end_dxyz[MAX_TRACKER];  // direction of end vertex

    std::vector<std::vector<double> >* trk_points_x[MAX_TRACKER];  // position of all points on trajectory
    std::vector<std::vector<double> >* trk_points_y[MAX_TRACKER];  // position of all points on trajectory
    std::vector<std::vector<double> >* trk_points_z[MAX_TRACKER];  // position of all points on trajectory

    std::vector<double>* trk_calo_KE[MAX_TRACKER][3];  // KE of calorimetry from each plan of each trk
    std::vector<double>* trk_calo_range[MAX_TRACKER][3];  // Range of calorimetry from each plan of each trk
    std::vector<int>* trk_calo_nHit[MAX_TRACKER][3];  // hits of calorimetry from each plan of each trk
    std::vector<std::vector<double> >* trk_calo_dedx[MAX_TRACKER][3];  // dedx of calorimetry from each plan of each trk
    std::vector<std::vector<double> >* trk_calo_dqdx[MAX_TRACKER][3];  // dqdx of calorimetry from each plan of each trk
    std::vector<std::vector<double> >* trk_calo_resRange[MAX_TRACKER][3];  // residual range of calorimetry from each plan of each trk


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