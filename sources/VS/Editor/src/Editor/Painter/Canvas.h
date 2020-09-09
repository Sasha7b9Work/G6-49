#pragma once
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Colors.h"

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
    static void ChangeTypeGrid();
    
    // Возвращает true, если линии сетки выводятся в процентах
    static bool TypeIsPercents() { return typeIsPercents; }
    
    // Возвращает расстояние между вертикальными линиями в точках или процентах
    static wxString GetScale();

private:
    static bool typeIsPercents;
    static wxString sScale;
    static int deltaPoints;
    static int deltaPercents;
    static void DrawTypePercents();
    static void DrawTypePoints();
    static void DrawHorizontalLines();

    // Рассчитывает дельту сетки в точках
    static void CalculateDeltaPoints();

    // Рассчитывает дельту сетки в процентах. Каждая единица - одна тысячная процента. 1% == 1000
    static void CalculateDeltaPercents();

    // 
    static void CalculateScale();
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

    void OnMouseWheel(wxMouseEvent &);

    void OnMouseMove(wxMouseEvent &);

    void OnMouseLeftDown(wxMouseEvent &);

    void OnMouseRightDown(wxMouseEvent &);

    void OnMouseLeftUp(wxMouseEvent &);

    void OnMouseRightUp(wxMouseEvent &);

    // Установить необходимую форму курсора мыши
    void SetMouseCursor();
};


extern Canvas *TheCanvas;
