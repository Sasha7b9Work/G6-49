#include "defines.h"
#include "Editor/Form.h"
#include "Editor/Selector.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Painter.h"


struct Cursor
{
    static int delta;                       // Дельта для расчёта пересечения с курсором
    static Cursor *grabbing;                // Указатель на "заграбленный" курсор
    int x = -1;                             // Координата x курсора
    void Set(int mouseX);                   // Установить курсор в позицию мыши mouseX
    void Disable()   { x = -1; }            // Выключить курсор
    bool IsEnabled() { return x >= 0; }
    bool OverMouseX(int mouseX);            // Возвращает true, если курсор находится поверх точки с координатой x
    void Draw();
    bool IsGrabbing()         { return grabbing == this; }
    static void EndGrabbing() { grabbing = nullptr; }
    int GetMouseX()           { return Point::PointToMouseX(x); }
};

int Cursor::delta = 5;
Cursor *Cursor::grabbing = nullptr;


static Cursor cursors[2];

#define cursor1 cursors[0]
#define cursor2 cursors[1]


void Selector::BeginGrab(int mouseX)
{
    if (cursor2.OverMouseX(mouseX))
    {
        cursor2.Set(mouseX);
    }
    else if (cursor1.OverMouseX(mouseX))
    {
        cursor1.Set(mouseX);
    }
    else if(!cursor1.IsEnabled())
    {
        cursor1.Set(mouseX);
        cursor2.Set(mouseX);
    }
}


void Selector::MoveBorder(int mouseX)
{
    if (cursor1.IsGrabbing())
    {
        cursor1.Set(mouseX);
    }
    else if (cursor2.IsGrabbing())
    {
        cursor2.Set(mouseX);
    }
}


void Selector::EndGrab()
{ 
    Cursor::EndGrabbing();
}


void Selector::DrawCursors()
{
    cursor1.Draw();
    cursor2.Draw();
}


void Selector::DrawRegion()
{
    int x1 = cursor1.GetMouseX();
    int x2 = cursor2.GetMouseX();

    if (x2 < x1)
    {
        Math::Swap(&x1, &x2);
    }

    Painter::FillRegion(x1, Grid::Y(), x2 - x1, Grid::Bottom(), Color::GREEN_1F);
}


bool Selector::CursorOverBorder(int mouseX)
{
    return cursor1.OverMouseX(mouseX) || cursor2.OverMouseX(mouseX);
}


void Cursor::Draw()
{
    if (IsEnabled())
    {
        int coord = Point::PointToMouseX(x);

        Painter::DrawLine(coord, Grid::Y(), coord, Grid::Bottom(), Color::GREEN);
    }
}


bool Cursor::OverMouseX(int mouseX)
{
    if (!IsEnabled())
    {
        return false;
    }

    int point = Point::PointToMouseX(x);

    return Math::InBoundaries(point, mouseX - delta, mouseX + delta);
}


void Cursor::Set(int mouseX)
{
    x = Point::MouseToPointX(mouseX);
    grabbing = this;
}
