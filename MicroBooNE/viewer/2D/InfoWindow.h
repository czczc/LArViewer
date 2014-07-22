#ifndef INFO_WINDOW_H
#define INFO_WINDOW_H

#include "TRootEmbeddedCanvas.h"

#include <vector>

#define MAXCOLORS 6

class MCEvent;
class TObject;
class TH1F;
class TH2F;

class InfoWindow: public TRootEmbeddedCanvas
{
public:
    InfoWindow(const TGWindow *p, int w,int h);
    virtual ~InfoWindow();

    void InitCanvas();
    void ClearCanvas();
    void SetTheme(int i);
    void UpdateCanvas();


    void DrawEventInfo(MCEvent *ev);
    void DrawWire(int channelId, MCEvent *ev, int wirehash=0);
    void AutoZoom(TH2F* hh, TH1F* h, int thresh=12);
    
    TCanvas* can; 
    vector<TObject*> listOfDrawables;
    int theme;
    int colors[MAXCOLORS];

    ClassDef(InfoWindow, 0)
};

#endif
