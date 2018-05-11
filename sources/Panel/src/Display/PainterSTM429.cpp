#include "Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Utils/Math.h"
#include "Colors.h"
#include "Settings/Settings.h"
#include "Display/Display.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Color currentColor = Color::NUMBER;


namespace Painter
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BeginScene(Color col)
{   
    if(col != Color::NUMBER)
    {
        SetColor(col);
    }

    for (int x = 0; x < BUFFER_WIDTH; ++x)
    {
        for (int y = 0; y < BUFFER_HEIGHT; ++y)
        {
            SetPoint(x, y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SetColorValue(Color color, col_val value)
{
    COLOR(color.value) = value;

    LoadPalette();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void LoadPalette()
{
    LTDC_::SetColors(&COLOR(0), Color::NUMBER.value);
    
    Color::InitGlobalColors();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void EndScene(void)
{
    LTDC_::ToggleBuffers();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawHLine(int y, int x0, int x1, Color col)
{
    if (col != Color::NUMBER)
    {
        SetColor(col);
    }

    for (int x = x0; x <= x1; ++x)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawLine(int x1, int y1, int x2, int y2, Color col)
{
    SetColor(col);

    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)fabsf((float)(x2 - x1));
    int dy = (int)fabsf((float)(y2 - y1));
    int s1 = Sign(x2 - x1);
    int s2 = Sign(y2 - y1);
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
        SetPoint(x, y);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawRectangle(int x, int y, int width, int height, Color col)
{
    SetColor(col);

    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void DrawVLine(int x, int y0, int y1, Color col)
{
    SetColor(col);

    for (int y = y0; y <= y1; ++y)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void FillRegion(int x, int y, int width, int height, Color col)
{
    SetColor(col);

    for (int i = y; i <= y + height; ++i)
    {
        DrawHLine(i, x, x + width);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void RunDisplay(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SetColor(Color color)
{
    if (color.value != Color::NUMBER.value)
    {
        currentColor = color;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateCurrentColor(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SetPoint(int x, int y)
{
    if (x >= 0 && x < BUFFER_WIDTH && y >= 0 && y < BUFFER_HEIGHT)
    {
        *(Display::GetBuffer() + y * BUFFER_WIDTH + x) = currentColor.value;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SetPalette(Color)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
col_val ReduceBrightness(col_val colorValue, float newBrightness)
{
    int red = (int)(R_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(red, 0, 0xff);
    int green = (int)(G_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(green, 0, 0xff);
    int blue = (int)(B_FROM_COLOR(colorValue) * newBrightness);
    LIMITATION(blue, 0, 0xff);
    return MAKE_COLOR(red, green, blue);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Color GetColor()
{
    return currentColor;
}

}
