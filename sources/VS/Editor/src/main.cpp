#include "Editor.h"
#include "Canvas.h"
#include "Form.h"
#include "History.h"


void init()
{
    //FreeConsole();

    Frame *frame = new Frame("Редактор сигналов Г6-49");

    TheCanvas = new Canvas(frame);

    TheForm = new Form();

	History::Add(TheForm);
}

void update()
{
    TheCanvas->Draw();
}