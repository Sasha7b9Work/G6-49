#include "Editor.h"
#include "Canvas.h"
#include "Form.h"


void init()
{
    TheForm = new Form();

    Frame *frame = new Frame("");

    TheCanvas = new Canvas(frame, 640, 480);
}

void update()
{
    TheCanvas->Draw();
}