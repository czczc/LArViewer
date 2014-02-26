#include "GuiController.h"
#include "MainWindow.h"
#include "ViewWindow.h"
#include "ControlWindow.h"
#include "MCEvent.h"

GuiController::GuiController(const TGWindow *p, int w,int h)
{
    mw = new MainWindow(p, w, h);
    event = new MCEvent("../data/sample.root");
}

GuiController::~GuiController()
{
}