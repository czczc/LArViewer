#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"
#include "TGButtonGroup.h"

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
    eventEntry->SetDefaultSize(40, 20);
    fNavigationFrame->AddFrame(eventEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    nextButton = new TGTextButton(fNavigationFrame, "Next >");
    fNavigationFrame->AddFrame(nextButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));
    
    AddFrame(fNavigationFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

    // XaxisFrame Frame
    fXaxisFrame = new TGHorizontalFrame(this, w, 100);

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

    AddFrame(fXaxisFrame, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));

    // Palette Selection
    paletteButtonGroup = new TGButtonGroup(this, "Palette", kVerticalFrame); 
    rainbowPaletteButton = new TGRadioButton(paletteButtonGroup, "Rainbow"); 
    grayPaletteButton = new TGRadioButton(paletteButtonGroup, "Gray"); 
    rainbowPaletteButton->SetState(kButtonDown);
    AddFrame(paletteButtonGroup, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 2, 2, 2, 2));
}

ControlWindow::~ControlWindow()
{
    // delete prev;
    // delete eventEntry;
    // delete next;
    // delete fNavigationFrame;
    // delete fTracksFrame;
}