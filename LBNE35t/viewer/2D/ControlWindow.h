#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;
class TGButtonGroup;
class TGRadioButton;
class TGListBox;

class ControlWindow: public TGVerticalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGHorizontalFrame  *fNavigationFrame; 
    TGTextButton       *prevButton, *nextButton;
    TGNumberEntryField *eventEntry; 

    TGGroupFrame       *fZoomControlFrame;
    TGHorizontalFrame  *fXaxisFrame;
    TGTextButton       *xrangeButton;
    TGNumberEntryField *tdcMinEntry, *tdcMaxEntry;
    TGTextButton       *autoZoomButton;
    TGTextButton       *unZoomButton;

    TGButtonGroup *paletteButtonGroup;
    TGRadioButton *rainbowPaletteButton; 
    TGRadioButton *grayPaletteButton;

    TGGroupFrame       *fMCGroupFrame;
    TGHorizontalFrame  *fTracksFrame;
    TGVerticalFrame    *fDaughterTracksFrame;
    TGVerticalFrame    *fParentSiblingTracksFrame;

    TGListBox          *fDaughterTracksListBox;
    TGListBox          *fParentTracksListBox;
    TGListBox          *fSiblingTracksListBox;


    ClassDef(ControlWindow, 0)
};

#endif