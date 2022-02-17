#include "Console.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstring>
#include <cstdio>


using namespace Primitives;


static const int MAX_STRINGS = 20;
static const int MAX_SYMBOLS_IN_STRING = 64;
// ����� �������� ������ �������
static char buffer[MAX_STRINGS][MAX_SYMBOLS_IN_STRING] = {};
// ���������� ����� � �������
static int  numStrings = 0;



void Console::AddString(const char *string)
{
    static int fullNumber = 0;

    if(numStrings == MAX_STRINGS)
    {
        for(int i = 1; i < MAX_STRINGS; i++)
        {
            std::strcpy(&buffer[i - 1][0], &buffer[i][0]);
        }
        numStrings--;
    }
    std::sprintf(&buffer[numStrings][0], "%d ", fullNumber);
    std::strcat(&buffer[numStrings][0], string);
    numStrings++;
    fullNumber++;
}


void Console::Draw()
{
    if (!set.dbg.showConsole)
    {
        return;
    }

    Font::ForceUpperCase(false);

    int y = 0;
    for (int i = 0; i < numStrings; i++)
    {
        String string(&buffer[i][0]);
        Rectangle(string.Width(), 9).Fill(2, y, Color::GREEN_25);
        string.Draw(2, y, Color::FILL);
        y += 9;
    }

    Font::ForceUpperCase(true);
}
