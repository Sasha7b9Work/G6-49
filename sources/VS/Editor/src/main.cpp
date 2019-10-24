#include "Editor.h"
#include "Canvas.h"
#include "Form.h"


void init()
{
    Frame *frame = new Frame("");

    TheCanvas = new Canvas(frame, 640, 480);

    TheForm = new Form();
}

void update()
{
    TheCanvas->Draw();
}