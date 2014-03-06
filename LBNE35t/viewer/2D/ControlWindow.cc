#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"
#include "TGListBox.h"
#include "TGLabel.h"

#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGVerticalFrame(p, w, h)
{
    // Navigation Frame
    fNavigationFrame = new TGHorizontalFrame(this, w, 100, kFixedWidth);

    prevButton = new TGTextButton(fNavigationFrame, "< Prev");
    fNavigationFrame->AddFrame(prevButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    eventEntry = new TGNumberEntryField(fNavigationFrame, -1, 0, TGNumberFormat::kNESInteger);
    eventEntry->SetDefaultSize(80, 20);
    fNavigationFrame->AddFrame(eventEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    nextButton = new TGTextButton(fNavigationFrame, "Next >");
    fNavigationFrame->AddFrame(nextButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    
    AddFrame(fNavigationFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // zoom control group frame  
    fZoomControlFrame = new TGGroupFrame(this, "Zoom", kVerticalFrame);
    fZoomControlFrame->SetTitlePos(TGGroupFrame::kLeft);

    autoZoomButton = new TGTextButton(fZoomControlFrame, "Auto Zoom");
    fZoomControlFrame->AddFrame(autoZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    unZoomButton = new TGTextButton(fZoomControlFrame, "UnZoom");
    fZoomControlFrame->AddFrame(unZoomButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // XaxisFrame Frame
    fXaxisFrame = new TGHorizontalFrame(fZoomControlFrame, w, 100);

    tdcMinEntry = new TGNumberEntryField(fXaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    tdcMinEntry->SetDefaultSize(40, 20);
    tdcMinEntry->SetIntNumber(0);
    fXaxisFrame->AddFrame(tdcMinEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    xrangeButton = new TGTextButton(fXaxisFrame, "Sync x-axis");
    fXaxisFrame->AddFrame(xrangeButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    tdcMaxEntry = new TGNumberEntryField(fXaxisFrame, -1, 0, TGNumberFormat::kNESInteger);
    tdcMaxEntry->SetDefaultSize(50, 20);
    tdcMaxEntry->SetIntNumber(3200);
    fXaxisFrame->AddFrame(tdcMaxEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    fZoomControlFrame->AddFrame(fXaxisFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    AddFrame(fZoomControlFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // Palette Selection
    paletteButtonGroup = new TGButtonGroup(this, "Palette", kVerticalFrame); 
    rainbowPaletteButton = new TGRadioButton(paletteButtonGroup, "Rainbow"); 
    grayPaletteButton = new TGRadioButton(paletteButtonGroup, "Gray"); 
    rainbowPaletteButton->SetState(kButtonDown);
    AddFrame(paletteButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // MC group frame
    fMCGroupFrame = new TGGroupFrame(this, "Monte Carlo Truth", kVerticalFrame);
    int hTracks = 300;
    fTracksFrame = new TGHorizontalFrame(fMCGroupFrame, w, hTracks);
    fDaughterTracksFrame = new TGVerticalFrame(fTracksFrame, w, hTracks);
    fParentSiblingTracksFrame = new TGVerticalFrame(fTracksFrame, w, hTracks);
    
    TGLabel *labelDaughterTracks = new TGLabel(fDaughterTracksFrame, "Children (MeV)");
    fDaughterTracksFrame->AddFrame(labelDaughterTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    
    fDaughterTracksListBox = new TGListBox(fDaughterTracksFrame);
    fDaughterTracksFrame->AddFrame(fDaughterTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    fDaughterTracksListBox->Resize(w/2, hTracks-40);

    TGLabel *labelParentTracks = new TGLabel(fParentSiblingTracksFrame, "Parents (MeV)");
    fParentSiblingTracksFrame->AddFrame(labelParentTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    
    fParentTracksListBox = new TGListBox(fParentSiblingTracksFrame);
    fParentSiblingTracksFrame->AddFrame(fParentTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    fParentTracksListBox->Resize(w/2, 40);


    TGLabel *labelSiblingTracks = new TGLabel(fParentSiblingTracksFrame, "Siblings (MeV)");
    fParentSiblingTracksFrame->AddFrame(labelSiblingTracks, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    
    fSiblingTracksListBox = new TGListBox(fParentSiblingTracksFrame);
    fParentSiblingTracksFrame->AddFrame(fSiblingTracksListBox, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    fSiblingTracksListBox->Resize(w/2, hTracks-100);

    fTracksFrame->AddFrame(fParentSiblingTracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    fTracksFrame->AddFrame(fDaughterTracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    fMCGroupFrame->AddFrame(fTracksFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
    AddFrame(fMCGroupFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

}

ControlWindow::~ControlWindow()
{
    // delete prev;
    // delete eventEntry;
    // delete next;
    // delete fNavigationFrame;
    // delete fTracksFrame;
}