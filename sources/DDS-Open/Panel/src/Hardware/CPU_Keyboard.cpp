#include "CPU.h"
#include "Display/Painter.h"
#include "Display/Colors.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPU::Keyboard::Init()
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool CPU::Keyboard::BufferIsEmpty()
{
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
StructControl CPU::Keyboard::GetNextControl()
{
    StructControl retValue;
    retValue.typePress = TypePress_Press;
    return retValue;
}

#define WIDTH_BUTTON 150
#define HEIGHT_BUTTON 80
#define DELTA 10

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::Draw()
{
    int x0 = 350;
    int y0 = 25;

    static char *titles[6][4] =
    {
        {"F1", "1", "2", "3"},
        {"F2", "4", "5", "6"},
        {"F3", "7", "8", "9"},
        {"F4", ".", "0", "-"},
        {"", "LEFT", "BTN", "RIGHT"},
        {"ON1", "ON2", "", ""}
    };

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            char *title = titles[i][j];
            if (title[0])
            {
                DrawButton(x0 + j * (WIDTH_BUTTON + DELTA), y0 + i * (HEIGHT_BUTTON + DELTA), title);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void CPU::Keyboard::DrawButton(int x, int y, char *title)
{
    Painter::DrawRectangle(x, y, WIDTH_BUTTON, HEIGHT_BUTTON, Color::FILL);
    Painter::DrawStringInCenterRect(x, y, WIDTH_BUTTON, HEIGHT_BUTTON, title);
}

