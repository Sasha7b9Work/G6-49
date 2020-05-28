#pragma once
#include "Display/Colors.h"


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *c_str() const;

    int Draw(int x, int y, Color color = Color::NUMBER) const;
    int DrawInCenterRect(int x, int y, int width, int height, Color color = Color::NUMBER);
    void DrawRelativelyRight(int xRight, int y, Color color = Color::NUMBER);
    void DrawInColumn(int x, int y, int width);
    // Возвращает нижнюю координату прямоугольника
    int DrawInBoundedRectWithTransfers(int x, int y, int width, Color colorBack, Color colorRect, Color colorText);
    // Пишет текст с переносами
    int DrawInColumnWithTransfers(const int left, const int top, const int width, const Color color = Color::NUMBER);

    int Length() const;

    void Free();

    void Append(const char *str);

    void Append(const char *str, uint numSymbols);

    void Append(char symbol);
    // Удаляет numSymbols из начала строки
    void RemoveFromBegin(uint numSymbols);

    void RemoveFromEnd();

    uint Size() const;

    char &operator[](uint i);

private:

    char *buffer;

    bool Allocate(uint size);
    
    void Conversion(TypeConversionString::E conv);

    int DrawSubString(int x, int y, pString t);
    int DrawSpaces(int x, int y, pString t, int *numSymbols);
    // Возвращает высоту экрана, которую займёт текст text, при выводе от left до right в переменной height. Если bool == false, то текст не влезет на экран 
    bool GetHeightTextWithTransfers(int left, int top, int right, int *height);
};
