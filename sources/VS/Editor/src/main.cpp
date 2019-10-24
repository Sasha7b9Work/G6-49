#include "Application.h"
#include "Canvas.h"


void init()
{
    Frame *frame = new Frame("");

    frame->Show(true);
}

void update()
{
    TheCanvas->BeginScene();
    TheCanvas->EndScene();
}