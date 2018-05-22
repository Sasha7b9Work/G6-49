#include "Text.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Text::upperCase = false;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Text::SetFont(TypeFont typeFont)
{
    font = fonts[typeFont];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::SetUpperCase(bool upper)
{
    upperCase = upper;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::Draw4SymbolsInRect(int x, int y, char eChar, Color color)
{
    Painter::SetColor(color);
    for (char i = 0; i < 2; i++)
    {
        DrawChar(x + 8 * i, y, eChar + i);
        DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawTextInBoundedRectWithTransfers(int x, int y, int width, const char *text, Color colorBackground, Color colorFill)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, text, &height);

    Painter::DrawRectangle(x, y, width, height, colorFill);
    Painter::FillRegion(x + 1, y + 1, width - 2, height - 2, colorBackground);
    DrawTextInRectWithTransfers(x + 3, y + 3, width - 8, text, colorFill);
    return y + height;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawChar(int eX, int eY, char symbol, Color color)
{
    Painter::SetColor(color);

    if (upperCase)
    {
        symbol = SU::ToUpper(symbol);
    }

    int8 width = (int8)font->symbol[symbol].width;
    int8 height = (int8)font->height;

    for (int b = 0; b < height; b++)
    {
        if (ByteFontNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (BitInFontIsExist(symbol, b, bit))
                {
                    Painter::SetPoint(x, y);
                }
                x++;
            }
        }
    }

    return eX + width;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawText(int x, int y, const char *text, Color color)
{
    Painter::SetColor(color);

    uint numSymbols = strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::ByteFontNotEmpty(int eChar, int byte)
{
    static const uint8 *bytes = 0;
    static int prevChar = -1;
    if (eChar != prevChar)
    {
        prevChar = eChar;
        bytes = font->symbol[prevChar].bytes;
    }
    return bytes[byte];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::BitInFontIsExist(int eChar, int numByte, int bit)
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
bool Text::GetHeightTextWithTransfers(int left, int top, int right, const char *text, int *height)
{
    char buffer[20];
    int numSymbols = (int)strlen(text);

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x += Font::GetLengthSymbol(symbol);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, false);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x += Font::GetLengthText(word);
                }
            }
        }
        x = left;
        y += 9;
    }

    *height = y - top + 4;
    LIMITATION(*height, 0, 239);

    return curSymbol == numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Text::GetWord(const char *firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (IsLetter(*firstSymbol))
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::IsLetter(char symbol)
{
    static const bool isLetter[256] =
    {     //  0x00   0x01   0x02   0x03   0x04   0x05   0x06   0x07   0x08   0x09   0x0a   0x0b   0x0c   0x0d   0x0e   0x0f
 /* 0x00 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0x10 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0x20 */  false, true,  true,  false, false, false, false, false, true,  true,  false, false, false, false, true,  false,
 /* 0x30 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false, false,
 /* 0x40 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
 /* 0x50 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false,
 /* 0x60 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
 /* 0x70 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false,
 /* 0x80 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0x90 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0xa0 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0xb0 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0xc0 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
 /* 0xd0 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
 /* 0xe0 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,
 /* 0xf0 */  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  true
    };

    return isLetter[(uint8)symbol];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawPartWord(char *word, int x, int y, int xRight, bool draw)
{
    int8 *lengthSyllables = BreakWord(word);
    int numSyllabels = 0;
    char buffer[30];
    for (int i = 0; i < 10; i++)
    {
        if (lengthSyllables[i] == 0)
        {
            numSyllabels = i;
            break;
        }
    }

    for (int i = numSyllabels - 2; i >= 0; i--)
    {
        char *subString = PartWordForTransfer(word, lengthSyllables, i, buffer);
        int length = Font::GetLengthText(subString);
        if (xRight - x > length - 5)
        {
            if (draw)
            {
                Text::DrawText(x, y, subString);
            }
            return (int)strlen(subString) - 1;
        }
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawTextInRectWithTransfers(const int left, const int top, const int width, pString text, const Color color)
{
    Painter::SetColor(color);

    int right = left + width;

    char buffer[20];
    int numSymbols = (int)strlen(text);

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = GetWord(text + curSymbol, &length, buffer);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = text[curSymbol++];
                if (symbol == '\n')
                {
                    x = right;
                    continue;
                }
                if (symbol == ' ' && x == left)
                {
                    continue;
                }
                x = DrawChar(x, y, symbol);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = DrawText(x, y, word) + 1;
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 *Text::BreakWord(char *word)
{
    int num = 0;
    static int8 lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;
    if (strcmp(word, "структуру") == 0)
    {
        int8 lengths[] = {5, 2, 2, 0};
        memcpy(lengthSyllables, lengths, 4);
    }
    else if (strcmp(word, "соответствующей") == 0)
    {
        int8 lenghts[] = {4, 3, 4, 5, 3, 0};
        memcpy(lengthSyllables, lenghts, 6);
    }
    return lengthSyllables;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Text::PartWordForTransfer(char *word, int8 *lengthSyllables, int numSyllable, char buffer[30])
{
    size_t length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += (size_t)lengthSyllables[i];
    }
    memcpy((void *)buffer, (void *)word, length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::FindNextTransfer(const char *letters, int8 *lettersInSyllable)
{

#define VOWEL       0   // Гласная
#define CONSONANT   1   // Согласная

    * lettersInSyllable = (int8)strlen(letters);
    if (strlen(letters) <= 3)
    {
        return false;
    }

    static const bool template1[3] = {false, true, true};               //     011     2   // После второго символа перенос
    static const bool template2[4] = {true, false, true, false};        //     1010    2
    static const bool template3[4] = {false, true, false, true};        //     0101    3
    static const bool template4[4] = {true, false, true, true};         //     1011    3
    static const bool template5[4] = {false, true, false, false};       //     0100    3
    static const bool template6[4] = {true, false, true, true};         //     1011    3
    static const bool template7[5] = {true, true, false, true, false};  //     11010   3
    static const bool template8[6] = {true, true, false, true, true};   //     11011   4

    bool consonant[20];

    int size = (int)strlen(letters);
    for (int i = 0; i < size; i++)
    {
        consonant[i] = IsConsonant(letters[i]);
    }

    if (CompareArrays(template1, consonant, 3))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (CompareArrays(template2, consonant, 4))
    {
        *lettersInSyllable = 2;
        return true;
    }
    if (strlen(letters) < 5)
    {
        return false;
    }
    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) ||
        CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (strlen(letters) < 6)
    {
        return false;
    }
    if (CompareArrays(template7, consonant, 5))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (CompareArrays(template8, consonant, 5))
    {
        *lettersInSyllable = 4;
        return true;
    }
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::IsConsonant(char symbol)
{
    static const bool isConsonat[256] =
    {
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false,
        false, true, true, true, true, false, true, true, false, false, true, true, true, true, false, true,
        true, true, true, false, true, true, true, true, true, true, true, false, true, false, false, false
    };

    return isConsonat[(uint8)symbol];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Text::CompareArrays(const bool *array1, const bool *array2, int numElems)
{
    for (int i = 0; i < numElems; i++)
    {
        if (array1[i] != array2[i])
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawBigChar(int eX, int eY, int size, char symbol)
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
                            Painter::SetPoint(x + i, y + j);
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
void Text::DrawBigText(int eX, int eY, int size, const char *text, Color color)
{
    Painter::SetColor(color);

    uint numSymbols = strlen(text);

    int x = eX;

    for (uint i = 0; i < numSymbols; i++)
    {
        x = DrawBigChar(x, eY, size, text[i]);
        x += size;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::DrawTextRelativelyRight(int xRight, int y, const char *text, Color color)
{
    int lenght = Font::GetLengthText(text);
    DrawText(xRight - lenght, y, text, color);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::DrawTextInRect(int x, int y, int width, const char *text)
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
int Text::DrawSubString(int x, int y, const char *text)
{
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = DrawChar(x, y, *text) + 1;
        numSymbols++;
        text++;
    }
    return numSymbols;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::GetLenghtSubString(const char *text)
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
int Text::DrawSpaces(int x, int y, const char *text, int *numSymbols)
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
int Text::DrawTextInCenterRect(int eX, int eY, int width, int eHeight, const char *text, Color color)
{
    Painter::SetColor(color);
    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;
    return DrawText(x, y, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawFormatTextInRectWithTransfers(int x, int y, int width, pString text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);

    return DrawTextInRectWithTransfers(x, y, width, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawFormatTextInCenterRect(int x, int y, int width, int height, const char *text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);

    return DrawTextInCenterRect(x, y, width, height, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawFormatText(int x, int y, pString text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    va_list args;
    va_start(args, text);
    vsprintf(buffer, text, args);
    va_end(args);
    return DrawText(x, y, buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::DrawStringInCenterRectOnBackgroundC(int x, int y, int width, int height, const char *text, Color colorText, int widthBorder,
                                                  Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = DrawTextInCenterRect(x, y, width, height, text, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Painter::FillRegion(eX - lenght - widthBorder, y - widthBorder + 1, w, h);
    DrawTextInCenterRect(x, y, width, height, text, colorText);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Text::DrawStringInCenterRectAndBoundItC(int x, int y, int width, int height, const char *text, Color colorBackground, Color colorFill)
{
    Painter::DrawRectangle(x, y, width, height, colorFill);
    Painter::FillRegion(x + 1, y + 1, width - 2, height - 2, colorBackground);
    Painter::SetColor(colorFill);
    return DrawTextInCenterRect(x, y, width, height, text);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::Draw2SymbolsC(int x, int y, char symbol1, char symbol2, Color color1, Color color2)
{
    DrawChar(x, y, symbol1, color1);
    DrawChar(x, y, symbol2, color2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Text::Draw10SymbolsInRect(int x, int y, char eChar)
{
    for (char i = 0; i < 5; i++)
    {
        DrawChar(x + 8 * i, y, eChar + i);
        DrawChar(x + 8 * i, y + 8, eChar + i + 16);
    }
}
