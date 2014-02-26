#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"

class MainWindow;
class MCEvent;

class GuiController
{
    RQ_OBJECT("MyMainFrame")
public:
    GuiController(const TGWindow *p, int w, int h);
    virtual ~GuiController();

    MainWindow *mw;
    MCEvent *event;

};

#endif