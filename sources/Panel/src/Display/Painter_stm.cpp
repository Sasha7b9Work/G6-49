#include "Painter.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include "Utils/Debug.h"
#include "Colors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include <cmath>
#include <cstdio>


using namespace Primitives;


void Painter::BeginScene(const Color &col)
{   
    col.SetAsCurrent();

    uint *address = (uint *)Display::GetBuffer();
    uint *end = address + (BUFFER_HEIGHT * BUFFER_WIDTH) / 4;
    uint value = (uint)col.value + (uint)(col.value << 8) + (uint)(col.value << 16) + (uint)(col.value << 24);
    while (address != end)
    {
        *address++ = value;
    }
}


void Painter::LoadPalette()
{
    HAL_LTDC::SetColors(&COLOR(0), Color::NUMBER.value);
}


void Painter::EndScene()
{
    HAL_LTDC::ToggleBuffers();
}


void Primitives::HLine::Draw(int y, int x0, int x1, const Color &col)
{
    col.SetAsCurrent();

    uint8 *address = Display::GetBuffer() + x0 + y * BUFFER_WIDTH;
    uint8 *end = Display::GetBuffer() + BUFFER_WIDTH * BUFFER_HEIGHT;

    uint8 value = Color::GetCurrent().value;

    for (int x = x0; x <= x1; ++x)
    {
        if(address < end)
        {
            *address++ = value;
        };
    }
}


void Primitives::VLine::Draw(int x, int y0, int y1, const Color &col)
{
    if(y1 < y0)
    {
        Math::Swap(&y0, &y1);
    }

    col.SetAsCurrent();

    uint8 *address = Display::GetBuffer() + x + y0 * BUFFER_WIDTH;
    uint8 *end = Display::GetBuffer() + BUFFER_WIDTH * BUFFER_HEIGHT;

    uint8 value = Color::GetCurrent().value;

    for (int y = y0; y < y1; ++y)
    {
        if(address < end)
        {
            *address = value;
        };
        address += BUFFER_WIDTH;
    }
}


void Primitives::Line::Draw(int x1, int y1, int x2, int y2, const Color &col)
{
    col.SetAsCurrent();

    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)std::fabsf((float)(x2 - x1));
    int dy = (int)std::fabsf((float)(y2 - y1));
    int s1 = Math::Sign(x2 - x1);
    int s2 = Math::Sign(y2 - y1);
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        Point().Draw(x, y);
        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}


void Point::Draw(int x, int y)
{
    uint8 *buffer = Display::GetBuffer() + y * BUFFER_WIDTH + x;

    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT)
    {
        *buffer = Color::GetCurrent().value;
    }
}


void Rectangle::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    for (int i = y; i <= y + height; ++i)
    {
        HLine::Draw(i, x, x + width);
    }
}


void Rectangle::Draw(int x, int y, const Color &col)
{
    col.SetAsCurrent();

    HLine::Draw(y, x, x + width);
    HLine::Draw(y + height, x, x + width);
    VLine::Draw(x, y, y + height);
    VLine::Draw(x + width, y, y + height);
}


void Rectangle::DrawFilled(int x, int y, Color colorFill, Color colorRect)
{
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorFill);
    Rectangle(width, height).Draw(x, y, colorRect);
}
