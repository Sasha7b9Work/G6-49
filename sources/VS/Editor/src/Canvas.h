#pragma once
#pragma warning(push, 0)
#include <wx/display.h>
#pragma warning(pop)
#include "Colors.h"


class Canvas
{
public:
    Canvas(wxWindow *parent, int width, int height);
    void BeginScene();
    void EndScene();
    void Resize(const wxSize &size);
    void SetColor(Color &color = Color::NUMBER);
};


extern Canvas *TheCanvas;
