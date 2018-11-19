#include "stdafx.h"
#ifndef WIN32
#include "Console.h"
#include <stdarg.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_STRINGS 100
#define SIZE_STRING (LENGTH_SPI_BUFFER - 1)


static char buffer[MAX_STRINGS][SIZE_STRING] = {};
static int numStrings = 0;
/// Если true, то как раз в это время происходит запись в консоль. Поэтому нельзя работать с консолью.
static bool isBusy = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::AddString(char *string)
{
    isBusy = true;

    if(numStrings < MAX_STRINGS - 1)
    {
        for(int i = 0; i < SIZE_STRING; i++)
        {
            buffer[numStrings][i] = string[i];
        }
        numStrings++;
    }

    isBusy = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Console::AddFormatString(char *format, ...)
{
    char buf[100];
    char *pointer = buf;

    va_list args;
    va_start(args, format);
    vsprintf(pointer, format, args);
    va_end(args);

    Console::AddString(buf);
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Console::AddFloat(float value)
{
    char string[50];
    sprintf(string, "%f", value);
    AddString(string);
}
*/

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Console::AddInt(int value)
{
    char string[50];
    sprintf(string, "%d", value);
    AddString(string);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Console::AddString(pString string)
{
    AddString((char *)string);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Console::ExistString()
{
    return (!isBusy) && (numStrings != 0);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Console::GetString(char dest[SIZE_STRING])
{
    for(int i = 0; i < SIZE_STRING; i++)
    {
        dest[i] = buffer[0][i];
    }

    for(int i = 1; i < numStrings; i++)
    {
        for(int pos = 0; pos < SIZE_STRING; pos++)
        {
            buffer[i - 1][pos] = buffer[i][pos];
        }
    }

    numStrings--;
}

