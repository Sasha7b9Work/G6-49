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
    void Resize(const wxSize &size);
};


extern Canvas *TheCanvas;
