// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Font.h"


namespace AdvancedFont
{
    void Set(TypeFont::E type);

    // Возвращает true, если в строке row есть пиксели
    bool RowNotEmpty(uint8 symbol, int row);

    // Возвращает ширину символа
    uint8 GetWidth(uint8 symbol);

    // Возвращает высоту символа
    uint8 GetHeight(uint8 symbol);

    // Возвращает высоту самого высокого символа
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);
};
