#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)
#include "Colors.h"


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    ~Canvas();

    void Draw();

    void SetColor(const Color &color = Color::NUMBER);

    void DrawPoint(int x, int y, int size, const Color &color = Color::NUMBER);

    void DrawLine(int x0, int y0, int x1, int y1, const Color &color = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, const Color &color = Color::NUMBER);

    void Redraw();

private:
    
    bool mouseIsDown = false;       // true, если ЛКМ находится в нажатом положении
    int mouseX = 0;
    int mouseY = 0;
    wxWindow *parent;

    void BeginScene();

    void EndScene();

    void DrawGrid();

    void OnPaint(wxPaintEvent &);

    void OnResize(wxSizeEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    // Установить необходимую форму курсора мыши
    void SetMouseCursor();

    wxColour MakeColour(const Color &color);
};


extern Canvas *TheCanvas;
