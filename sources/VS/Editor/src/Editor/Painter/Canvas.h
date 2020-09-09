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
    
    // Возвращает расстояние между вертикальными линиями в точках или процентах
    static wxString GetScale();

private:
    static bool typeIsPercents;
    static wxString sScale;
    static void DrawTypePercents();
    static void DrawTypePoints();
    static void DrawHorizontalLines();

    // Возвращает дльту сетки в точках
    static int GetDeltaPoints();

    // Возвращает дельту сетки в процентах. Каждая единица - одна тысячная процента. 1% == 1000
    static int GetDeltaPercents();
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
