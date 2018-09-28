#include "Console.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_STRINGS 100
#define SIZE_STRING (LENGTH_SPI_BUFFER - 1)


static char buffer[MAX_STRINGS][SIZE_STRING] = {};
static int numStrings = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::AddString(char *string)
{
    if(numStrings < MAX_STRINGS - 1)
    {
        for(int i = 0; i < SIZE_STRING; i++)
        {
            buffer[numStrings][i] = string[i];
        }
        numStrings++;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Console::ExistString()
{
    return numStrings != 0;
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

