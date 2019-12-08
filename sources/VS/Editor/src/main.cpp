#include "Editor.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"


void init()
{
    Frame *frame = new Frame("");

    TheCanvas = new Canvas(frame);

    TheForm = new Form();

	History::Add(TheForm);
}

void update()
{
    TheCanvas->Draw();
}