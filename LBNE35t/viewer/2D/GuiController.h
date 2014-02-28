#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"
#include "TMarker.h"
#include <vector>

class MainWindow;
class ViewWindow;
class ControlWindow;
class MCEvent;
class MCGeometry;
class TCanvas;
class TH2F;

class GuiController
{
    RQ_OBJECT("MyMainFrame")
public:
    GuiController(const TGWindow *p, int w, int h);
    virtual ~GuiController();

    void Open(const char* filename);
    void Reload();
    void InitConnections();
    void DrawPixels();
    void Modified();
    
    // slots
    void Prev();
    void Next();
    void SyncXaxis();
    void UpdatePalette(int id);
    void HandleMenu(int id);

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    MCEvent *event;
    MCGeometry *geom;


    TCanvas *can;
    vector<TMarker> pixels_ZT;
    vector<TMarker> pixels_UT;
    vector<TMarker> pixels_VT;

    int currentEvent;
    int currentPalette;
};

#endif