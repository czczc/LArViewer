#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"
#include "TGString.h"
#include <vector>
#include <map>

class MainWindow;
class ViewWindow;
class InfoWindow;
class ControlWindow;
class MCEvent;
class MCGeometry;
class TCanvas;
class TH2F;
class TLine;
class TMarker;
class TDatabasePDG;

class GuiController
{
    RQ_OBJECT("EVDGuiController")
public:
    GuiController(const TGWindow *p, int w, int h);
    virtual ~GuiController();

    void InitEvent(const char* filename);
    void Reload();
    void InitConnections();
    void DrawPixels();
    void Modified();

    // slots
    void HandleFileMenu(int id);
    void MenuOpen();
    void Prev();
    void Next();
    void Jump();
    void AutoZoom();
    void UnZoom(bool redraw=true);
    // void SyncRangeZT();
    // void SyncRangeUT();
    // void SyncRangeVT();
    // void SyncXaxis();
    // void UpdateZaxis();
    // void UpdateInductionSig(int id);
    // void UpdateAPA(int id);
    void UpdateShowMC();
    // void UpdatePalette(int id);
    // void UpdateDisplayOption(int id);
    void MCTrackSelected(int id);
    void ProcessCanvasEvent(Int_t ev, Int_t x, Int_t y, TObject *selected);

    void AutoZoom(TH2F* hist, bool zoomY=true);
    void DrawTrack(int id);
    void HideTrack();
    void SetTheme(int theme);

    // utilities
    double KE(float* momentum);  // KE
    TGString PDGName(int pdg);

    // member variables
    MainWindow    *mw;
    ViewWindow    *vw;
    ControlWindow *cw;
    InfoWindow    *iw;
    MCEvent       *event;
    // MCGeometry *geom;

    TCanvas *can;
    TString baseDir;
    TDatabasePDG *dbPDG;

    int yMin_now, yMax_now;

    TLine* trackLine[3];
    TMarker* trackStartPoint[3];

    int currentEventEntry;
    int currentTrackId;

    int currentPalette;
    int currentDisplayOption;
    int currentTheme;
    int currentInductionSig;
    bool currentShowMC;

};

#endif
