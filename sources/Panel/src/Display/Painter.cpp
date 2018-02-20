#include "Display.h"
#include "Painter.h"
#include "globals.h"
#include "Font/Font.h"
#include "Hardware/CPU.h"
#include "Utils/Math.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint8 Painter::color = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::BeginScene(Color col)
{
    color = col;

    for (int x = 0; x < 320; ++x)
    {
        for (int y = 0; y < 240; ++y)
        {
            SetPoint(x, y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawHLine(int y, int x0, int x1)
{
    for(int x = x0; x <= x1; ++x)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawLine(int x1, int y1, int x2, int y2)
{
    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = x1;
    int y = y1;
    int dx = (int)fabsf((float)(x2 - x1));
    int dy = (int)fabsf((float)(y2 - y1));
    int s1 = Math_Sign(x2 - x1);
    int s2 = Math_Sign(y2 - y1);
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
        Painter::SetPoint(x, y);
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
void Painter::DrawRectangle(int x, int y, int width, int height)
{
    DrawHLine(y, x, x + width);
    DrawHLine(y + height, x, x + width);
    DrawVLine(x, y, y + height);
    DrawVLine(x + width, y, y + height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawVLine(int x, int y0, int y1)
{
    for(int y = y0; y <= y1; ++y)
    {
        SetPoint(x, y);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene(void)
{
    CPU::_LTDC_::ToggleBuffers();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegion(int x, int y, int width, int height)
{
    for (int i = y; i <= y + height; ++i)
    {
        DrawHLine(i, x, x + width);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::RunDisplay(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetColor(Color col)
{
    color = col;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CalculateCurrentColor(void)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetPoint(int x, int y)
{
    if(x >= 0 && x < 320 && y >= 0 && y < 240)
    {
        *(Display::GetBuffer() + y * 320 + x) = color;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::FillRegionC(int x, int y, int width, int height, Color col)
{
    Painter::SetColor(col);
    Painter::FillRegion(x, y, width, height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawRectangleC(int x, int y, int width, int height, Color col)
{
    Painter::SetColor(col);
    Painter::DrawRectangle(x, y, width, height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawText(int x, int y, const char *text)
{
    uint numSymbols = strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(TypeFont typeFont)
{
    font = fonts[typeFont];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawChar(int eX, int eY, char symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    int size = 1;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Painter::ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte]; //-V108
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawBigChar(int eX, int eY, int size, char symbol)
{
    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            SetPoint(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Painter::BitInFontIsExist(int eChar, int numByte, int bit)
{
    static uint8 prevByte = 0;      /// \todo здесь точно статики нужны?
    static int prevChar = -1;
    static int prevNumByte = -1;
    if (prevNumByte != numByte || prevChar != eChar)
    {
        prevByte = font->symbol[eChar].bytes[numByte];
        prevChar = eChar;
        prevNumByte = numByte;
    }
    return prevByte & (1 << bit);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTextInRect(int x, int y, int width, const char *text)
{
    int xStart = x;
    int xEnd = xStart + width;

    while (*text != 0)
    {
        int length = GetLenghtSubString(text);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol(*text);
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawSpaces(int x, int y, const char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = DrawChar(x, y, *text);
        text++;
        (*numSymbols)++;
    }
    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawSubString(int x, int y, const char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = DrawChar(x, y, *text);
        numSymbols++;
        text++;
        x++;
    }
    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::GetLenghtSubString(const char *text)
{
    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTextRelativelyRight(int xRight, int y, const char *text)
{
    int lenght = Font::GetLengthText(text);
    DrawText(xRight - lenght, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw4SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 2; i++)
    {
        DrawChar(x + 8 * i, y, eChar + i);
        DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawBigText(int eX, int eY, int size, const char *text)
{
    int numSymbols = (int)strlen(text);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawTextC(int x, int y, const char *text, Color col)
{
    SetColor(col);
    return DrawText(x, y, text);
}
