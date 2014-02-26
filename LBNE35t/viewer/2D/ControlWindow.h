#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;

class ControlWindow: public TGVerticalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

private:
    TGHorizontalFrame *fNavigationFrame;
    TGTextButton      *prev, *next;
    TGNumberEntryField       *eventEntry;

    ClassDef(ControlWindow, 0)
};

#endif