#ifndef VIEW_WINDOW_H
#define VIEW_WINDOW_H

#include "TGFrame.h"
#include "TRootEmbeddedCanvas.h"

class ViewWindow: public TRootEmbeddedCanvas
{
public:
    ViewWindow(const TGWindow *p, int w,int h);
    virtual ~ViewWindow();

    TCanvas* Canvas() { return GetCanvas(); };

    ClassDef(ViewWindow, 0)
};

#endif
