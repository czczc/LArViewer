#ifndef GUI_CONTROLLER_H
#define GUI_CONTROLLER_H

#include "TGFrame.h"
#include "RQ_OBJECT.h"

class MainWindow;
class ViewWindow;
class ControlWindow;
class MCEvent;

class GuiController
{
    RQ_OBJECT("MyMainFrame")
public:
    GuiController(const TGWindow *p, int w, int h);
    virtual ~GuiController();

    void Prev();
    void Next();
    void Reload();
    void SetConnections();

    MainWindow *mw;
    ViewWindow *vw;
    ControlWindow *cw;
    MCEvent *event;

    int currentEvent;

};

#endif