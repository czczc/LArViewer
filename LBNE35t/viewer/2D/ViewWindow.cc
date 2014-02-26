#include "ViewWindow.h"

#include "TRootEmbeddedCanvas.h"

#include <iostream>
using namespace std;


ViewWindow::ViewWindow(const TGWindow *p, int w,int h)
    :TRootEmbeddedCanvas("ECanvas", p, w, h)
{

}

ViewWindow::~ViewWindow(){}
