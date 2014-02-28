#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;
class TGButtonGroup;
class TGRadioButton;

class ControlWindow: public TGVerticalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGHorizontalFrame  *fNavigationFrame; 
    TGTextButton       *prevButton, *nextButton;
    TGNumberEntryField *eventEntry; 

    TGHorizontalFrame  *fXaxisFrame;
    TGTextButton       *xrangeButton;
    TGNumberEntryField *tdcMinEntry, *tdcMaxEntry;

    TGButtonGroup *paletteButtonGroup;
    TGRadioButton *rainbowPaletteButton; 
    TGRadioButton *grayPaletteButton;

    ClassDef(ControlWindow, 0)
};

#endif