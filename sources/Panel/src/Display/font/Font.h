#pragma once
#include "Display/DisplayTypes.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};


struct TypeFont
{
    enum E
    {
        _5,
        _7,
        _8,
        _GOSTB20,
        _GOST28,
        Count
    };
};


struct Font
{
    static int GetHeight();
    static int GetHeight(char symbol);
    static int GetWidth(char symbol);
    static void StoreAndSet(TypeFont::E typeFont);
    static void Set(TypeFont::E typeFont);
    static void ToggleCharacterSize();
    static int8 Width(char symbol);
    static int8 Width(uint8 symbol);
    static int8 Height();
    static bool RowNotEmpty(int s, int row);
    static bool BitIsExist(int s, int row, int bit);
    static bool IsAdvanced();
    // Выводить все символы в верхнем регистре
    static void ForceUpperCase(bool upper);
    static bool InUpperCase();

    // Восстановить шрифт, который был сохранён функцией StoreAndSet()
    static void Restore();
    
    int height;
    Symbol symbol[256];

private:

    static TypeFont::E current;     // Этим ширфтом производится вывод
    static bool upperCase;          // Если true, текст будет выводиться большими символы, независимо от того, какие символы в оригинале

    // Сохранить текущий шрифт
    static void Store();
};
