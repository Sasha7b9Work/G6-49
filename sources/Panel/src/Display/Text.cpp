#include "defines.h"
#include "Display/Colors.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>


using namespace Primitives;


int Char::Draw(int eX, int eY, Color color)
{
    color.SetAsCurrent();

    if (Font::IsUpperCase())
    {
        symbol = SU::ToUpper(symbol);
    }

    uint8 s = static_cast<uint8>(symbol);

    int8 width = Font::Width(s);
    int8 height = Font::Height();

    if(Font::IsAdvanced())
    {
        for(int row = 0; row < height; row++)
        {
            if(Font::RowNotEmpty(s, row))
            {
                for(int col = 0; col < width; col++)
                {
                    if(Font::BitIsExist(s, row, col))
                    {
                        Point().Draw(eX + col, eY + row);
                    }
                }
            }
        }
    }
    else
    {
        for(int b = 0; b < height; b++)
        {
            if(Font::RowNotEmpty(s, b))
            {
                int x = eX;
                int y = eY + b + 9 - height;
                int endBit = 8 - width;
                for(int bit = 7; bit >= endBit; bit--)
                {
                    if(Font::BitIsExist(s, b, bit))
                    {
                        Point().Draw(x, y);
                    }
                    x++;
                }
            }
        }
    }

    return eX + width;
}


void Char::Draw4InRect(int x, int y, Color color)
{
    color.SetAsCurrent();

    for (char i = 0; i < 2; i++)
    {
        Char(symbol + i).Draw(x + 8 * i, y);
        Char(symbol + i + 16).Draw(x + 8 * i, y + 8);
    }
}



void Char::Draw2Horizontal(int x, int y, Color color)
{
    color.SetAsCurrent();

    Char(symbol).Draw(x, y);
    Char(symbol + 1).Draw(x + 8, y);
}


char *Text::GetWord(const char *firstSymbol, int *length, char buffer[20])
{
    int pointer = 0;
    *length = 0;

    while (Char(*firstSymbol).IsLetter())
    {
        buffer[pointer] = *firstSymbol;
        pointer++;
        firstSymbol++;
        (*length)++;
    }
    buffer[pointer] = '\0';

    return buffer;
}


bool Char::IsLetter()
{
    static const bool isLetter[256] =
    {     //  0x00   0x01   0x02   0x03   0x04   0x05   0x06   0x07   0x08   0x09   0x0a   0x0b   0x0c   0x0d   0x0e   0x0f
 /* 0x00 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0x10 */  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false,
 /* 0x20 */  false, true,  true,  false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  false,
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

    return isLetter[static_cast<uint8>(symbol)];
}


int Text::DrawPartWord(char *word, int x, int y, int xRight, bool draw)
{
    uint *lengthSyllables = BreakWord(word);
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
                String(subString).Draw(x, y);
            }
            return static_cast<int>(std::strlen(subString)) - 1;
        }
    }

    return 0;
}


int String::DrawInColumnWithTransfersDiffColors(const int left, const int top, const int width, const Color colorDif, const Color color) //-V801
{
    bool inverse = false;               // Установленное в true значение означает, что сейчас идёт вывод инверсным цветом

    color.SetAsCurrent();

    Color colorDraw = Color::GetCurrent();

    int right = left + width;

    char buf[20];
    int numSymbols = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = Text::GetWord(text + curSymbol, &length, buf);

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
                x = Char(symbol).Draw(x, y);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = Text::DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;

                    uint size = std::strlen(word);

                    for (uint i = 0; i < size; ++i)
                    {
                        if(inverse && word[i] == '\"')
                        {
                            colorDraw.SetAsCurrent();
                            inverse = false;
                            x = Char(word[i]).Draw(x, y);
                        }
                        else
                        {
                            x = Char(word[i]).Draw(x, y);
                            if (!inverse && word[i] == '\"')
                            {
                                colorDif.SetAsCurrent();
                                inverse = true;
                            }
                        }
                        ++x;
                    }
                }
            }
        }
        x = left;
        y += 9;
    }

    color.SetAsCurrent();

    return y;
}


uint *Text::BreakWord(char *word)
{
    int num = 0;
    static uint lengthSyllables[10];
    char *position = word;
    while (FindNextTransfer(position, &(lengthSyllables[num])))
    {
        position += lengthSyllables[num];
        num++;
    }
    lengthSyllables[num + 1] = 0;
    if (std::strcmp(word, "структуру") == 0)
    {
        int8 lengths[] = {5, 2, 2, 0};
        std::memcpy(lengthSyllables, lengths, 4);
    }
    else if (std::strcmp(word, "соответствующей") == 0)
    {
        int8 lenghts[] = {4, 3, 4, 5, 3, 0};
        std::memcpy(lengthSyllables, lenghts, 6);
    }
    else
    {
        // ничего делать не нужно
    }
    return lengthSyllables;
}


char *Text::PartWordForTransfer(const char *word, const uint *lengthSyllables, int numSyllable, char buffer[30])
{
    uint length = 0;
    for (int i = 0; i <= numSyllable; i++)
    {
        length += static_cast<uint>(lengthSyllables[i]);
    }
    std::memcpy(buffer, word, length);
    buffer[length] = '-';
    buffer[length + 1] = '\0';
    return buffer;
}


bool Text::FindNextTransfer(const char *letters, uint *lettersInSyllable)
{

#define VOWEL       0   // Гласная
#define CONSONANT   1   // Согласная

    * lettersInSyllable = std::strlen(letters);
    if (std::strlen(letters) <= 3)
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

    uint size = std::strlen(letters);
    for (uint i = 0; i < size; i++)
    {
        consonant[i] = Char(letters[i]).IsConsonant();
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
    if (std::strlen(letters) < 5)
    {
        return false;
    }
    if (CompareArrays(template3, consonant, 4) || CompareArrays(template4, consonant, 4) || CompareArrays(template5, consonant, 4) ||
        CompareArrays(template6, consonant, 4))
    {
        *lettersInSyllable = 3;
        return true;
    }
    if (std::strlen(letters) < 6)
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


bool Char::IsConsonant()
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

    return isConsonat[static_cast<uint8>(symbol)];
}


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


int BigChar::Draw(int eX, int eY)
{
    int8 width = Font::Width(symbol);
    int8 height = Font::Height();

    for (int b = 0; b < height; b++)
    {
        if (Font::RowNotEmpty(symbol, b))
        {
            int x = eX;
            int y = eY + b * size + 9 - height;
            int endBit = 8 - width;
            for (int bit = 7; bit >= endBit; bit--)
            {
                if (Font::BitIsExist(symbol, b, bit))
                {
                    for (int i = 0; i < size; i++)
                    {
                        for (int j = 0; j < size; j++)
                        {
                            Point().Draw(x + i, y + j);
                        }
                    }
                }
                x += size;
            }
        }
    }

    return eX + width * size;
}


void BigText::Draw(int eX, int eY, Color color)
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);

    int x = eX;

    for (uint i = 0; i < numSymbols; i++)
    {
        x = BigChar(text[i], size).Draw(x, eY);
        x += size;
    }
}


int Text::DrawFormatTextInColumnWithTransfersDiffColors(int x, int y, int width, Color color, pString text, ...)
{
#define SIZE_BUFFER_DRAW_FORM_TEXT 200
    char buffer[SIZE_BUFFER_DRAW_FORM_TEXT];
    std::va_list args;
    va_start(args, text); //-V2567
    std::vsprintf(buffer, text, args);
    va_end(args);

    return String(buffer).DrawInColumnWithTransfersDiffColors(x, y, width, color);
}


void Text::DrawStringInCenterRectOnBackground(int x, int y, int width, int height, pString text, Color colorText, int widthBorder,
                                                  Color colorBackground)
{
    int lenght = Font::GetLengthText(text);
    int eX = String(text).DrawInCenterRect(x, y, width, height, colorBackground);
    int w = lenght + widthBorder * 2 - 2;
    int h = 7 + widthBorder * 2 - 1;
    Rectangle(w, h).Fill(eX - lenght - widthBorder, y - widthBorder + 1);

    String(text).DrawInCenterRect(x, y, width, height, colorText);
}


int Text::DrawStringInCenterRectAndBoundIt(int x, int y, int width, int height, pString text, Color colorBackground, Color colorFill)
{
    Rectangle(width, height).Draw(x, y, colorFill);
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorBackground);
    colorFill.SetAsCurrent();

    return String(text).DrawInCenterRect(x, y, width, height);
}


String::String() : text(nullptr)
{
    Set(TypeConversionString::None, "");
}


String::String(const String &rhs) : text(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(std::strlen(rhs.c_str()) + 1))
    {
        std::strcpy(text, rhs.c_str());
    }
}


String::String(char symbol) : text(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(2))
    {
        text[0] = symbol;
        text[1] = 0;
    }
}


String::String(const char *format, ...) : text(nullptr)
{
    Set(TypeConversionString::None, "");

    if (format == nullptr)
    {
        return;
    }

    static const int SIZE = 500;
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2528
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
        std::strcpy(text, "Буфер слишком мал");
    }
    else if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(text, buf);
    }
}


void String::Set(TypeConversionString::E conv, const char *format, ...)
{
    Free();

    if (format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if (numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(text, "Буфер слишком мал");
        }
        else if (Allocate(std::strlen(buf) + 1))
        {
            std::strcpy(text, buf);
            Conversion(conv);
        }
    }
}


void String::Append(const char *str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(old.Size() + std::strlen(str) + 1);

    std::strcpy(text, old.c_str());
    std::strcat(text, str);
}


void String::Append(const char *str, uint numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    uint size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(text, old.c_str());
    std::memcpy(text + old.Size(), str, numSymbols);
    text[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(text);
}


void String::Free()
{
    if (text)
    {
        std::free(text);
        text = nullptr;
        Set(TypeConversionString::None, "");
    }
}


char *String::c_str() const
{
    return text;
}


bool String::Allocate(uint size)
{
    std::free(text);
    text = static_cast<char *>(std::malloc(size));
    if (text)
    {
        return true;
    }

    return false;
}


int String::Draw(int x, int y, Color color) const
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = Char(text[i]).Draw(x, y);
        ++x;
    }

    return x;
}


int String::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    color.SetAsCurrent();

    int lenght = Font::GetLengthText(text);
    int height = Font::GetHeightSymbol(text[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;

    return Draw(x, y);
}


void String::DrawRelativelyRight(int xRight, int y, Color color)
{
    int lenght = Font::GetLengthText(text);
    Draw(xRight - lenght, y, color);
}


void String::DrawInColumn(int x, int y, int width)
{
    int xStart = x;
    int xEnd = xStart + width;

    const char *t = text;

    while (*t != 0)
    {
        int length = Length();
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol(*t) + 2;
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, t);
        t += numSymbols;
        x += length;
        x = DrawSpaces(x, y, t, &numSymbols);
        t += numSymbols;     // -V102
    }
}


int String::DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBack, Color colorRect, Color colorText)
{
    int height = 0;
    GetHeightTextWithTransfers(x + 3, y + 3, x + width - 8, &height);
    Rectangle(width, height).Draw(x, y, colorRect);
    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, colorBack);
    DrawInColumnWithTransfers(x + 3, y + 3, width - 8, colorText);
    return y + height;
}


int String::DrawInColumnWithTransfers(const int left, const int top, const int width, const Color color) //-V801
{
    color.SetAsCurrent();

    int right = left + width;

    char buf[20];
    int numSymbols = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = Text::GetWord(text + curSymbol, &length, buf);

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
                x = Char(symbol).Draw(x, y);
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = Text::DrawPartWord(word, x, y, right, true);
                    x = right;
                    curSymbol += numSymb;
                    continue;
                }
                else
                {
                    curSymbol += length;
                    x = String(word).Draw(x, y) + 1;
                }
            }
        }
        x = left;
        y += 9;
    }

    return y;
}


int String::Length() const
{
    const char *t = text;

    int retValue = 0;
    while (((*t) != ' ') && ((*t) != '\0'))
    {
        retValue += Font::GetLengthSymbol(*t);
        t++;
    }
    return retValue;
}


void String::Conversion(TypeConversionString::E conv)
{
    char *pointer = text;

    if (conv == TypeConversionString::FirstUpper)
    {
        if (*pointer)
        {
            *pointer = SU::ToUpper(*pointer);
            pointer++;
        }

        while (*pointer)
        {
            *pointer = SU::ToLower(*pointer);
            pointer++;
        }
    }
}


int String::DrawSubString(int x, int y, pString _t)
{
    const char *t = _t;
    int numSymbols = 0;
    while (((*t) != ' ') && ((*t) != '\0'))
    {
        x = Char(*t).Draw(x, y) + 1;
        numSymbols++;
        t++;
    }
    return numSymbols;
}


int String::DrawSpaces(int x, int y, pString _t, int *numSymbols)
{
    const char *t = _t;
    *numSymbols = 0;
    while (*t == ' ')
    {
        x = Char(*t).Draw(x, y);
        t++;
        (*numSymbols)++;
    }
    return x;
}


bool String::GetHeightTextWithTransfers(int left, int top, int right, int *height)
{
    char buf[20];
    int numSymbols = static_cast<int>(std::strlen(text));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = Text::GetWord(text + curSymbol, &length, buf);

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
                x += Font::GetLengthSymbol(SU::ToUpper(symbol));
            }
            else                                            // А здесь найдено по крайней мере два буквенных символа, т.е. найдено слово
            {
                int lengthString = Font::GetLengthText(word);
                if (x + lengthString > right + 5)
                {
                    int numSymb = Text::DrawPartWord(word, x, y, right, false);
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
    LIMITATION(*height, 0, 239);    // -V2516

    return curSymbol == numSymbols;
}


void String::RemoveFromBegin(uint numSymbols)
{
    if (std::strlen(text) == numSymbols)
    {
        Free();
    }
    else
    {
        String old(text);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(text, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if (Size() > 0)
    {
        text[Size() - 1] = '\0';
    }
}


uint String::Size() const
{
    if (text == nullptr)
    {
        return 0;
    }

    return std::strlen(text);
}


char &String::operator[](uint i)
{
    static char result = 0;

    if (text == nullptr || Size() < i)
    {
        return result;
    }

    return text[i];
}
