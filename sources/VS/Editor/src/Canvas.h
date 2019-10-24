#pragma once
#pragma warning(push, 0)
#include <wx/display.h>
#pragma warning(pop)


class Canvas
{
public:
    Canvas(wxWindow *parent, int width, int height);
    void BeginScene();
    void EndScene();
};


extern Canvas *TheCanvas;
