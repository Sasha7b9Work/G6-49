#include "Editor/Editor.h"
#include "Editor/Form.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/History.h"


void init()
{
    FreeConsole();

    Frame *frame = new Frame("Редактор сигналов Г6-49"); //-V2511

    TheCanvas = new Canvas(frame); //-V2511

    TheForm = new Form(); //-V2511

	History::Add(TheForm);
}

void update()
{
    TheCanvas->Draw();
}