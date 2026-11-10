// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SU
{
    char *Float2String(float value);

    char *Buffer2FloatString(const uint8 *buffer);

    float Buffer2Float(const uint8 *buffer);

    // ѕереводит числов в двоичный вид с заданный количеством разр€дов
    char *Bin2StringN(uint value, char buffer[33], int n);

    // ќставить number последних символов от in. ≈сли строка меньше, то в начале добавить пробелы
    pchar LeaveTheLastOnes(pchar in, int number);
};
