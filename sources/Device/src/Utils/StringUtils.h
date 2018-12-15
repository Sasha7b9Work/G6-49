#pragma once


char *Float2String(float value);

char *Buffer2FloatString(const uint8 *buffer);

float Buffer2Float(const uint8 *buffer);


class SU
{
public:
    static char *ToUpper(void *str, uint size);

    static bool EqualsStrings(void *str1, void *str2, uint size);

    static bool EqualsStrings(uint8 *str1, char *str2, uint size);
};

