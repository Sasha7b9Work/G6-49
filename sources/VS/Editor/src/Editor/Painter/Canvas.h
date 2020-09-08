#pragma once
#include "Colors.h"

#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)


struct Grid
{
    static int X();
    static int Y();
    static int Width();
    static int Height();
    static int Bottom();
    static int Right();
    static void Draw();
    static void ChangeTypeGrid() { typeIsPercents = !typeIsPercents; }
    // Возвращает true, если линии сетки выводятся в процентах
    static bool TypeIsPercents() { return typeIsPercents; }

private:
    static bool typeIsPercents;
};


class Canvas : public wxPanel
{
public:

    Canvas(wxWindow *parent);

    ~Canvas();

    void Draw();

    void Redraw();

private:
    
    bool mouseIsDown = false;       // true, если ЛКМ находится в нажатом положении
    int mouseX = 0;
    int mouseY = 0;
    wxWindow *parent;

    void BeginScene();

    void EndScene();

    void OnPaint(wxPaintEvent &);

    void OnResize(wxSizeEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    // Установить необходимую форму курсора мыши
    void SetMouseCursor();
};


extern Canvas *TheCanvas;
