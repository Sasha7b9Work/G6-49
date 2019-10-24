#include "Editor.h"
#include "Canvas.h"


void init()
{
    Frame *frame = new Frame("");

    TheCanvas = new Canvas(frame, 640, 480);
}

void update()
{
    TheCanvas->BeginScene();
    TheCanvas->EndScene();
}