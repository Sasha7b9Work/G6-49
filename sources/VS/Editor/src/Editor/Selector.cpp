#include "defines.h"
#include "Editor/Form.h"
#include "Editor/Selector.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Painter.h"


struct Cursor
{
    static int delta;                       // ������ ��� ������� ����������� � ��������
    static Cursor *grabbing;                // ��������� �� "������������" ������
    int x = -1;                             // ���������� x �������
    void Set(int mouseX);                   // ���������� ������ � ������� ���� mouseX
    void Disable()   { x = -1; }            // ��������� ������
    bool IsEnabled() { return x >= 0; }
    bool OverMouseX(int mouseX);            // ���������� true, ���� ������ ��������� ������ ����� � ����������� x
    void Draw();
    bool IsGrabbing()         { return grabbing == this; }
    static void EndGrabbing() { grabbing = nullptr; }
    int GetMouseX()           { return Point::PointToCanvasX(x); }
};

int Cursor::delta = 5;
Cursor *Cursor::grabbing = nullptr;
bool Selector::isEnabled = false;


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
        int coord = Point::PointToCanvasX(x);

        Painter::DrawLine(coord, Grid::Y(), coord, Grid::Bottom(), Color::GREEN);
    }
}


bool Cursor::OverMouseX(int mouseX)
{
    if (!IsEnabled())
    {
        return false;
    }

    int point = Point::PointToCanvasX(x);

    return Math::InBoundaries(point, mouseX - delta, mouseX + delta);
}


void Cursor::Set(int mouseX)
{
    x = Point::CanvasToPointX(mouseX);
    grabbing = this;
}


bool Selector::IsEnabled()
{
    return isEnabled;
}


void Selector::Enable()
{
    isEnabled = true;
}


void Selector::Disable()
{
    isEnabled = false;
}
