#pragma once
#include "Command.h"


class Console
{
public:
    static void AddString(char *buffer);
    static void AddString(pString buffer);
    static void AddFloat(float value);
    static bool ExistString();
    static void GetString(char buffer[LENGTH_SPI_BUFFER - 1]);
};
