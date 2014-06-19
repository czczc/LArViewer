#ifndef CONTROL_WINDOW_H
#define CONTROL_WINDOW_H

#include "TGFrame.h"

class InfoWindow;
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

    InfoWindow         *fInfoWindow;

    TGTextButton       *fPrevButton, *fNextButton;
    TGNumberEntryField *fEventEntry; 

    // TGHorizontalFrame  *fDisplayFrame; 
    // TGButtonGroup      *displayButtonGroup;
    // TGRadioButton      *rawButton; 
    // TGRadioButton      *calibButton; 
    // TGRadioButton      *hitsButton;

    // TGGroupFrame       *fZoomControlFrame;
    // TGHorizontalFrame  *fZoomButtonFrame;
    TGTextButton       *fAutoZoomButton;
    TGTextButton       *fUnZoomButton;
    // TGHorizontalFrame  *fXaxisFrame;
    // TGTextButton       *xrangeButton;
    // TGNumberEntryField *tdcMinEntry, *tdcMaxEntry;
    // TGHorizontalFrame  *fZaxisFrame;
    // TGTextButton       *zrangeButton;
    // TGNumberEntryField *ztColorEntry, *utColorEntry, *vtColorEntry;
    // TGButtonGroup      *inductionSigButtonGroup;
    // TGRadioButton      *inductionSigButton_pos; 
    // TGRadioButton      *inductionSigButton_neg;
    // TGRadioButton      *inductionSigButton_both;



    // TGHorizontalFrame  *fColorScaleFrame; 
    // TGButtonGroup      *paletteButtonGroup;
    // TGRadioButton      *rainbowPaletteButton; 
    // TGRadioButton      *grayPaletteButton;
    // TGRadioButton      *summerPaletteButton;
    // TGRadioButton      *grayinvPaletteButton;

    // TGGroupFrame       *fMCGroupFrame;
    // TGHorizontalFrame  *fTracksFrame;
    // TGVerticalFrame    *fDaughterTracksFrame;
    // TGVerticalFrame    *fParentSiblingTracksFrame;

    TGListBox          *fDaughterTracksListBox;
    TGListBox          *fParentTracksListBox;
    TGListBox          *fSiblingTracksListBox;
    TGCheckButton      *fShowMCButton;

    
    ClassDef(ControlWindow, 0)
};

#endif
