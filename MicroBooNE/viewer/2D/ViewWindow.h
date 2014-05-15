#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H

#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"

class TPad;

class ViewWindow: public TRootEmbeddedCanvas
{
public:
    ViewWindow(const TGWindow *p, int w,int h);
    virtual ~ViewWindow();

    void InitCanvas();
    void InitStyle();
    void PaletteNightRainbow();
    void PaletteNightGray();
    void PaletteDayRainbow();
    void PaletteDayGray();
    void SetTheme(int theme);
    
    TCanvas* can;
    int currentTheme; 

    enum THEME {
        kNightRainbow = 0,
        kNightGray,
        kDayRainbow,
        kDayGray,
    };

    ClassDef(ViewWindow, 0)
};

#endif
