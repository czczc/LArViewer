#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;
class TGButtonGroup;
class TGRadioButton;
class TGListBox;
class TGDoubleVSlider;

class ControlWindow: public TGVerticalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGHorizontalFrame  *fNavigationFrame; 
    TGTextButton       *prevButton, *nextButton;
    TGNumberEntryField *eventEntry; 

    TGGroupFrame       *fZoomControlFrame;
    TGTextButton       *autoZoomButton;
    TGTextButton       *unZoomButton;
    TGHorizontalFrame  *fXaxisFrame;
    TGTextButton       *xrangeButton;
    TGNumberEntryField *tdcMinEntry, *tdcMaxEntry;
    TGHorizontalFrame  *fZaxisFrame;
    TGTextButton       *zrangeButton;
    TGNumberEntryField *ztColorEntry, *utColorEntry, *vtColorEntry;

    TGHorizontalFrame  *fColorScaleFrame; 
    TGButtonGroup      *paletteButtonGroup;
    TGRadioButton      *rainbowPaletteButton; 
    TGRadioButton      *grayPaletteButton;
    TGRadioButton      *summerPaletteButton;
    TGRadioButton      *grayinvPaletteButton;
    // TGDoubleVSlider    *fZTSlider;


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