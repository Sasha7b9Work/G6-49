// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SU
{
    char *Float2String(float value);

    char *Buffer2FloatString(const uint8 *buffer);

    float Buffer2Float(const uint8 *buffer);

    // Переводит числов в двоичный вид с заданный количеством разрядов
    char *Bin2StringN(uint value, char buffer[33], int n);
};
