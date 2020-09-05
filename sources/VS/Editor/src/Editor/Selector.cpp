#include "defines.h"
#include "Editor/Form.h"
#include "Editor/Selector.h"
#include "Editor/Painter/Canvas.h"


static int startPoint = -1;     // Первая точка выделенного участка
static int endPoint = -1;       // Последняя точка выделенного участка
static const int delta = 5;     // Дельта для расчёта пересечения с курсором


void Selector::BeginSelect(int mouseX)
{
    int x = Point::MouseToPointX(mouseX);

    if (CursorOverPointX(mouseX, endPoint))
    {
        endPoint = x;
    }
    else if (CursorOverPointX(mouseX, startPoint))
    {
        startPoint = x;
    }
    else
    {
        if (startPoint >= 0)
        {
            endPoint = x;
        }
        else
        {
            startPoint = endPoint = x;
        }
    }
}


void Selector::MoveSelect(int mouseX)
{
    int x = Point::MouseToPointX(mouseX);

    if (CursorOverPointX(mouseX, endPoint))
    {
        endPoint = x;
    }
    else if (CursorOverPointX(mouseX, startPoint))
    {
        startPoint = x;
    }
    else
    {
        endPoint = x;
    }
}


void Selector::EndSelect(int /*mouseX*/, int /*mouseY*/)
{

}


void Selector::Draw()
{
    if (startPoint == -1)
    {
        return;
    }

    int start = Point::PointToMouseX(startPoint);
    int end = Point::PointToMouseX(endPoint);


    TheCanvas->DrawLine(start, 0, start, TheCanvas->GetSize().y, Color::GREEN);
    TheCanvas->DrawLine(end, 0, end, TheCanvas->GetSize().y);
}


bool Selector::CursorOverBorder(int mouseX)
{
    return CursorOverPointX(mouseX, startPoint) || CursorOverPointX(mouseX, endPoint);
}


bool Selector::CursorOverPointX(int mouseX, int x)
{
    if (x < 0)
    {
        return false;
    }

    int point = Point::PointToMouseX(x);

    return Math::InBoundaries(point, mouseX - delta, mouseX + delta);
}
