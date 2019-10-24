#pragma once
#pragma warning(push, 0)
#include <wx/display.h>
#pragma warning(pop)
#include "Colors.h"


class Canvas
{
public:
    Canvas(wxWindow *parent, int width, int height);
    void Draw();
    void Resize(const wxSize &size);
    const wxSize GetSize() const;
    void SetColor(const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, const Color &color = Color::NUMBER);
    void SetPoint(int x, int y, int size, const Color &color = Color::NUMBER);

private:
    void BeginScene();
    void EndScene();
};


extern Canvas *TheCanvas;
