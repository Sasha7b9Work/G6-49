#include "defines.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


using namespace Primitives;


String::String() : buffer(nullptr)
{
    Set(TypeConversionString::None, "");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(std::strlen(rhs.c_str()) + 1))
    {
        std::strcpy(buffer, rhs.c_str());
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(const char *format, ...) : buffer(nullptr)
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
        std::strcpy(buffer, "Буфер слишком мал");
    }
    else if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
}


void String::Set(TypeConversionString::E conv, const char *format, ...)
{
    Free();

    if(format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(buffer, "Буфер слишком мал");
        }
        else if(Allocate(std::strlen(buf) + 1))
        {
            std::strcpy(buffer, buf);
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

    std::strcpy(buffer, old.c_str());
    std::strcat(buffer, str);
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

    std::strcpy(buffer, old.c_str());
    std::memcpy(buffer + old.Size(), str, numSymbols);
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
        Set(TypeConversionString::None, "");
    }
}


char *String::c_str() const
{
    return buffer;
}


bool String::Allocate(uint size)
{
    std::free(buffer);
    buffer = static_cast<char *>(std::malloc(size));
    if (buffer)
    {
        return true;
    }

    return false;
}


int String::Draw(int x, int y, Color color) const
{
    color.SetAsCurrent();

    uint numSymbols = std::strlen(buffer);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = Char(buffer[i]).Draw(x, y);
        ++x;
    }

    return x;
}


int String::DrawInCenterRect(int eX, int eY, int width, int eHeight, Color color)
{
    color.SetAsCurrent();

    int lenght = Font::GetLengthText(buffer);
    int height = Font::GetHeightSymbol(buffer[0]);
    int x = eX + (width - lenght) / 2;
    int y = eY + (eHeight - height) / 2;

    return Draw(x, y);
}


void String::DrawRelativelyRight(int xRight, int y, Color color)
{
    int lenght = Font::GetLengthText(buffer);
    Draw(xRight - lenght, y, color);
}


void String::DrawInColumn(int x, int y, int width)
{
    int xStart = x;
    int xEnd = xStart + width;

    const char *text = buffer;

    while (*text != 0)
    {
        int length = Length();
        if (length + x > xEnd)
        {
            x = xStart;
            y += Font::GetHeightSymbol(*text) + 2;
        }
        int numSymbols = 0;
        numSymbols = DrawSubString(x, y, text);
        text += numSymbols;
        x += length;
        x = DrawSpaces(x, y, text, &numSymbols);
        text += numSymbols;     // -V102
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
    int numSymbols = static_cast<int>(std::strlen(buffer));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = Text::GetWord(buffer + curSymbol, &length, buf);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = buffer[curSymbol++];
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
    const char *text = buffer;

    int retValue = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        retValue += Font::GetLengthSymbol(*text);
        text++;
    }
    return retValue;
}


void String::Conversion(TypeConversionString::E conv)
{
    char *pointer = buffer;

    if(conv == TypeConversionString::FirstUpper)
    {
        if(*pointer)
        {
            *pointer = SU::ToUpper(*pointer);
            pointer++;
        }

        while(*pointer)
        {
            *pointer = SU::ToLower(*pointer);
            pointer++;
        }
    }
}


int String::DrawSubString(int x, int y, pString t)
{
    const char *text = t;
    int numSymbols = 0;
    while (((*text) != ' ') && ((*text) != '\0'))
    {
        x = Char(*text).Draw(x, y) + 1;
        numSymbols++;
        text++;
    }
    return numSymbols;
}


int String::DrawSpaces(int x, int y, pString t, int *numSymbols)
{
    const char *text = t;
    *numSymbols = 0;
    while (*text == ' ')
    {
        x = Char(*text).Draw(x, y);
        text++;
        (*numSymbols)++;
    }
    return x;
}


bool String::GetHeightTextWithTransfers(int left, int top, int right, int *height)
{
    char buf[20];
    int numSymbols = static_cast<int>(std::strlen(buffer));

    int y = top - 1;
    int x = left;

    int curSymbol = 0;

    while (y < 231 && curSymbol < numSymbols)
    {
        while (x < right - 1 && curSymbol < numSymbols)
        {
            int length = 0;
            char *word = Text::GetWord(buffer + curSymbol, &length, buf);

            if (length <= 1)                            // Нет буквенных символов или один, т.е. слово не найдено
            {
                char symbol = buffer[curSymbol++];
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
    if (std::strlen(buffer) == numSymbols)
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols);
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0';
    }
}


uint String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return std::strlen(buffer);
}


char &String::operator[](uint i)
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}
