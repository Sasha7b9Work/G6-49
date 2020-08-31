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
    void Redraw();

    // Заблокировать холст (после этого нельзя ставить точки)
    void Block() { isBlocking = true; }

    // Разблокировать холст
    void UnBlock() { isBlocking = false; };

    // Возвращает true, если холст заблокирован (нельзя ставить точки мышкой)
    bool IsBlocknig() { return isBlocking; };

private:
    /// true, если ЛКМ находится в нажатом положении
    bool mouseIsDown = false;
    int mouseX = 0;
    int mouseY = 0;
    wxWindow *parent;
    bool isBlocking = false;

    void BeginScene();
    void EndScene();
    void DrawGrid();
    void OnPaint(wxPaintEvent &);
    void OnResize(wxSizeEvent &);
    void OnMouseMove(wxMouseEvent &);
    void OnMouseLeftDown(wxMouseEvent &);
    void OnMouseRightDown(wxMouseEvent &);
    void OnMouseUp(wxMouseEvent &);
    /// Установить необходимую форму курсора мыши
    void SetMouseCursor();
};


extern Canvas *TheCanvas;
