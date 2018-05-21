#include "Painter.h"
#include "Display/Font/Font.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    


//----------------------------------------------------------------------------------------------------------------------------------------------------
extern void CalculateCurrentColor();

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawFormText(int x, int y, Color color, pString text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    return Text::DrawText(x, y, buffer, color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawStringInCenterRect(int eX, int eY, int width, int eHeight, const char *text, Color color)
{
    SetColor(color);
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return Text::DrawText(x, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder, 
                                                  Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = DrawStringInCenterRect(x, y, width, height, text, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    FillRegion(eX - lenght - widthBorder, y - widthBorder + 1, w, h);
    DrawStringInCenterRect(x, y, width, height, text, colorText);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill)
{
    DrawRectangle(x, y, width, height, colorFill);
    FillRegion(x + 1, y + 1, width - 2, height - 2, colorBackground);
    SetColor(colorFill);
    return DrawStringInCenterRect(x, y, width, height, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2)
{
    Text::DrawChar(x, y, symbol1, color1);
    Text::DrawChar(x, y, symbol2, color2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 5; i++)
    {
        Text::DrawChar(x + 8 * i, y, eChar + i);
        Text::DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}
