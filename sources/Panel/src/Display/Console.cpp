#include "Console.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include <string.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char Console::buffer[MAX_STRINGS][MAX_SYMBOLS_IN_STRING] = {};
int  Console::numStrings = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::AddString(char *string)
{
    // Находим номер первой пустой строки
    int numEmptyString = 0;

    for (; numEmptyString < MAX_STRINGS; numEmptyString++)
    {
        if (!buffer[numEmptyString][0])
        {
            break;
        }
    }


    // Если пустой строки нет
        if (numEmptyString == MAX_STRINGS)
    {
        // То сдвигаем все строки на одну
        for (int i = 1; i < MAX_STRINGS; i++)
        {
            strcpy(&buffer[i - 1][0], &buffer[i][0]);
        }
        numEmptyString = MAX_STRINGS - 1;
    }

    char tempBuffer[1000] = "";

    static int numLines = 0;

    sprintf(tempBuffer, "%d %s", numLines++, string);

    if (strlen(tempBuffer) > MAX_SYMBOLS_IN_STRING - 1)
    {
        tempBuffer[MAX_SYMBOLS_IN_STRING - 1] = 0;
    }

    strcpy(&buffer[numEmptyString][0], tempBuffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Console::Draw(void)
{
    if (!CONSOLE_ENABLED)
    {
        return;
    }

    int y = 1;
    for (int i = 0; i < numStrings; i++)
    {
        int length = Font::GetLengthText(&buffer[i][0]);
        Painter::FillRegion(2, y, length, 8, Color::BACK);
        Text::DrawText(2, y, &buffer[i][0], Color::FILL);
        y += 8;
    }
}
