#ifndef GUI3D_CONTROLLER_H
#define GUI3D_CONTROLLER_H

#include "RQ_OBJECT.h"

class Gui3DController
{
    RQ_OBJECT("3DCotroller")
public:
    Gui3DController();
    virtual ~Gui3DController();

    void Run();
    void AddTracks();

    void InitGeometry();
    void InitNavigationFrame();
};

#endif