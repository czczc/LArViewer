#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class TGTextButton;
class TGNumberEntryField;
class TGButtonGroup;
class TGRadioButton;
class TGListBox;
class TGDoubleVSlider;
class TGCheckButton;
class TGLabel;

class ControlWindow: public TGVerticalFrame
{
public:
    ControlWindow(const TGWindow *p, int w, int h);
    virtual ~ControlWindow();

    TGHorizontalFrame  *fNavigationFrame; 
    TGTextButton       *prevButton, *nextButton;
    TGNumberEntryField *eventEntry; 

    TGGroupFrame       *fZoomControlFrame;
    TGHorizontalFrame  *fZoomButtonFrame;
    TGTextButton       *autoZoomButton;
    TGTextButton       *unZoomButton;
    TGHorizontalFrame  *fXaxisFrame;
    TGTextButton       *xrangeButton;
    TGNumberEntryField *tdcMinEntry, *tdcMaxEntry;
    TGHorizontalFrame  *fZaxisFrame;
    TGTextButton       *zrangeButton;
    TGNumberEntryField *ztColorEntry, *utColorEntry, *vtColorEntry;
    TGButtonGroup      *inductionSigButtonGroup;
    TGRadioButton      *inductionSigButton_pos; 
    TGRadioButton      *inductionSigButton_neg;
    TGRadioButton      *inductionSigButton_both;
    TGButtonGroup      *apaButtonGroup;
    TGCheckButton      *apaButton_1;
    TGCheckButton      *apaButton_2;
    TGCheckButton      *apaButton_3;
    TGCheckButton      *apaButton_4;


    TGHorizontalFrame  *fColorScaleFrame; 
    TGButtonGroup      *paletteButtonGroup;
    TGRadioButton      *rainbowPaletteButton; 
    TGRadioButton      *grayPaletteButton;
    TGRadioButton      *summerPaletteButton;
    TGRadioButton      *grayinvPaletteButton;

    TGGroupFrame       *fMCGroupFrame;
    TGHorizontalFrame  *fTracksFrame;
    TGVerticalFrame    *fDaughterTracksFrame;
    TGVerticalFrame    *fParentSiblingTracksFrame;

    TGListBox          *fDaughterTracksListBox;
    TGListBox          *fParentTracksListBox;
    TGListBox          *fSiblingTracksListBox;
    TGCheckButton      *showMCButton;

    ClassDef(ControlWindow, 0)
};

#endif