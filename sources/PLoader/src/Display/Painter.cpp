#include "defines.h"
#include "Painter.h"
#include "Display/Colors.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include <cmath>


void Painter::BeginScene(Color col)
{
    col.SetAsCurrent();

    uint *address = reinterpret_cast<uint *>(HAL_LTDC::GetBuffer());
    uint *end = address + (SCREEN_HEIGHT * SCREEN_WIDTH) / 4;
    uint value = static_cast<uint>(col.value) + static_cast<uint>(col.value << 8) + static_cast<uint>(col.value << 16) + static_cast<uint>(col.value << 24);
    while(address != end)
    {
        *address++ = value;
    }
}


void Painter::EndScene()
{
    HAL_LTDC::ToggleBuffers();
}


void Painter::DrawHLine(int y, int x0, int x1, Color col)
{
    col.SetAsCurrent();

    uint8 *address = HAL_LTDC::GetBuffer() + x0 + y * SCREEN_WIDTH;
    uint8 *end = HAL_LTDC::GetBuffer() + SCREEN_WIDTH * SCREEN_HEIGHT;

    uint8 value = Color::CurrentValue();

    for(int x = x0; x <= x1; ++x)
    {
        if(address < end)
        {
            *address++ = value;
        };
    }
}


void Painter::DrawVLine(int x, int y0, int y1, Color col)
{
    if(y1 < y0)
    {
        Math::Swap(&y0, &y1);
    }

    col.SetAsCurrent();

    uint8 *address = HAL_LTDC::GetBuffer() + x + y0 * SCREEN_WIDTH;
    uint8 *end = HAL_LTDC::GetBuffer() + SCREEN_WIDTH * SCREEN_HEIGHT;

    uint8 value = Color::CurrentValue();

    for(int y = y0; y < y1; ++y)
    {
        if(address < end)
        {
            *address = value;
        };
        address += SCREEN_WIDTH;
    }
}


void Painter::DrawLine(int x1, int y1, int x2, int y2, Color col)
{
    col.SetAsCurrent();

    if((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = static_cast<int>(std::fabsf(static_cast<float>(x2 - x1)));
    int dy = static_cast<int>(std::fabsf(static_cast<float>(y2 - y1)));
    int s1 = Math::Sign(x2 - x1);
    int s2 = Math::Sign(y2 - y1);
    int temp;
    int exchange = 0;
    if(dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for(; i <= dx; i++)
    {
        SetPoint(x, y);
        while(e >= 0)
        {
            if(exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if(exchange)
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


void Painter::DrawRectangle(int x, int y, int width, int height, Color col)
{
    col.SetAsCurrent();

    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}


void Painter::FillRegion(int x, int y, int width, int height, Color col)
{
    col.SetAsCurrent();

    for(int i = y; i <= y + height; ++i)
    {
        DrawHLine(i, x, x + width);
    }
}


void Painter::SetPoint(int x, int y)
{
    uint8 *buffer = HAL_LTDC::GetBuffer() + y * SCREEN_WIDTH + x;

    if(x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        *buffer = Color::CurrentValue();
    }
}

