// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SU
{
    static char *Float2String(float value);

    static char *Buffer2FloatString(const uint8 *buffer);

    static float Buffer2Float(const uint8 *buffer);
};
