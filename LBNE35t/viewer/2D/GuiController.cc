#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "MCEvent.h"

#include "TGButton.h"
#include "TGNumberEntry.h"

GuiController::GuiController(const TGWindow *p, int w,int h)
{
    mw = new MainWindow(p, w, h);
    vw = mw->fViewWindow;
    cw = mw->fControlWindow;

    event = new MCEvent("../data/sample.root");
    currentEvent = 0;
    Reload();

    SetConnections();
}

GuiController::~GuiController()
{
}

void GuiController::SetConnections()
{
    cw->prevButton->Connect("Clicked()", "GuiController", this, "Prev()");
    cw->nextButton->Connect("Clicked()", "GuiController", this, "Next()");

}

void GuiController::Prev()
{
    if (currentEvent==0) {
        currentEvent = event->nEvents-1;
    }
    else {
        currentEvent--;
    }
    Reload();
}


void GuiController::Next()
{
    if (currentEvent==event->nEvents-1) {
        currentEvent = 0;
    }
    else {
        currentEvent++;
    }
    Reload();
}


void GuiController::Reload()
{
    cw->eventEntry->SetNumber(currentEvent);
    event->GetEntry(currentEvent);
    event->PrintInfo();
}

