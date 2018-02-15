#include "Painter.h"
#include "PainterText.h"
#include "Font/Font.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool ByteFontNotEmpty(int eChar, int byte)
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
static int DrawSpaces(int x, int y, const char *text, int *numSymbols)
{
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = Painter_DrawChar(x, y, *text);
        text++;
        (*numSymbols)++;
    }
    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int DrawSubString(int x, int y, const char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Painter_DrawChar(x, y, *text);
        numSymbols++;
        text++;
        x++;
    }
    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static int GetLenghtSubString(const char *text)
{
    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font_GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static bool BitInFontIsExist(int eChar, int numByte, int bit)
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
int Painter_DrawBigChar(int eX, int eY, int size, char symbol)
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
                            Painter_SetPoint(x + i, y + j);
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
void Painter_DrawBigText(int eX, int eY, int size, const char *text)
{
    int numSymbols = (int)strlen(text);

    int x = eX;

    for (int i = 0; i < numSymbols; i++)
    {
        x = Painter_DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter_DrawTextRelativelyRight(int xRight, int y, const char *text)
{
    int lenght = Font_GetLengthText(text);
    Painter_DrawText(xRight - lenght, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter_DrawTextInRect(int x, int y, int width, const char *text)
{
    int xStart = x;
    int xEnd = xStart + width;

    while (*text != 0)
    {
        int length = GetLenghtSubString(text);
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font_GetHeightSymbol(*text);
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter_DrawChar(int eX, int eY, char symbol)
{
    int8 width = font->symbol[symbol].width;
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
                            Painter_SetPoint(x + i, y + j);
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
int Painter_DrawText(int x, int y, const char *text)
{
    int numSymbols = strlen(text);
    for (int i = 0; i < numSymbols; ++i)
    {
        x = Painter_DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Painter_DrawTextC(int x, int y, const char *text, Color color)
{
    Painter_SetColor(color);
    return Painter_DrawText(x, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter_Draw4SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 2; i++)
    {
        Painter_DrawChar(x + 8 * i, y, eChar + i);
        Painter_DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Painter_SetFont(TypeFont typeFont)
{
    font = fonts[typeFont];
}
