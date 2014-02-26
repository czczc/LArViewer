#include "ControlWindow.h"

#include "TGButton.h"
#include "TGNumberEntry.h"

#include <iostream>
using namespace std;

ControlWindow::ControlWindow(const TGWindow *p, int w, int h)
    :TGVerticalFrame(p, w, h)
{
    //Navigation Frame
    fNavigationFrame = new TGHorizontalFrame(this, w, 100, kFixedWidth);
    AddFrame(fNavigationFrame, new TGLayoutHints(kLHintsTop | kLHintsLeft, 2, 2, 2, 2));

    prevButton = new TGTextButton(fNavigationFrame, "< Prev");
    fNavigationFrame->AddFrame(prevButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

    eventEntry = new TGNumberEntryField(fNavigationFrame, -1, 0, TGNumberFormat::kNESInteger);
    eventEntry->SetDefaultSize(50, 20);
    fNavigationFrame->AddFrame(eventEntry, new TGLayoutHints(kLHintsTop | kLHintsCenterY, 3, 2, 2, 2));

    nextButton = new TGTextButton(fNavigationFrame, "Next >");
    fNavigationFrame->AddFrame(nextButton, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 3, 2, 2, 2));

}

ControlWindow::~ControlWindow()
{
    // delete prev;
    // delete eventEntry;
    // delete next;
    // delete fNavigationFrame;
    // delete fTracksFrame;
}