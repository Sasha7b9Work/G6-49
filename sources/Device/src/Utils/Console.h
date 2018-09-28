#pragma once
#include "Command.h"


class Console
{
public:
    static void AddString(char *buffer);
    static bool ExistString();
    static void GetString(char buffer[LENGTH_SPI_BUFFER - 1]);
};
