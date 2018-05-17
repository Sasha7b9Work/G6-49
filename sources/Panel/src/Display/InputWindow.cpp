#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (iws.numLockMode)

static Channel           chan;
static WaveForm          form;
static WaveParameter     param;
static InputWindowStruct iws;


static void DrawAmplitude(int x, int y);
static void DrawFrequency(int x, int y);
static void DrawOffset(int x, int y);
static void DrawPhase(int x, int y);
static void DrawDelay(int x, int y);
static void DrawPeriod(int x, int y);
static void DrawDutyRatio(int x, int y);
static void DrawDuration(int x, int y);
static void DrawDigits(int x, int y);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init(void)
{
    chan = CURRENT_CHANNEL;
    form = WAVE_FORM_CH(chan);
    param = CURRENT_PARAMETER(form);
    iws = INPUT_WINDOW_STRUCT(chan, form, param);

	(&iws)->Fill(chan, form, param);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::Draw(void)
{
    int x = 0;
    int y = MP_TITLE_HEIGHT;
    int width = SIGNAL_WIDTH;
    int height = SIGNAL_HEIGHT;

    if (CURRENT_CHANNEL_IS_A)
    {
        y += SIGNAL_HEIGHT;
    }

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    
    static const pFuncVII func[NumParameters] =
    {
        DrawFrequency,
        DrawPeriod,
        DrawAmplitude,
        DrawOffset,
        DrawDuration,
        DrawDutyRatio,
        DrawPhase,
        DrawDelay
    };

    x += 40;
    y += 10;

    DrawDigits(x, y);

    func[param](x + 10, y + 10);

    if (IN_NUM_LOCK_MODE)
    {
        InputWindowStruct::DrawInputField(10, y + 27);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    Painter::DrawBigText(x, y, 3, Parameter_Name(param));

    char buf[2] = "0";

    y += 40;

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        Painter::FillRegion(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, Color::BACK);

        Painter::SetColor(Color::FILL);

        if (i == iws.hightLightDigit)
        {
            Painter::SetFont(TypeFont_UGO2);
            Painter::Draw4SymbolsInRect(x + 2, y - 10, '\xa2');
            Painter::Draw4SymbolsInRect(x + 2, y + 35, '\xa0');
            Painter::SetFont(TypeFont_8);
        }

        if (iws.inputBuffer[i])
        {
            buf[0] = iws.inputBuffer[i];
            Painter::DrawBigText(x, y - 1, SIZE_TEXT, buf);
        }

        x += 24;

        if (iws.posComma == i)
        {
            Painter::FillRegion(x - 3, y + HEIGHT_DIGIT - 2, SIZE_TEXT, SIZE_TEXT + 1);
            x += 4;
        }
    }

    char buffer[10] = {0};
    Painter::DrawBigText(x, y, SIZE_TEXT, NameUnit(buffer, iws.order, iws.param));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawAmplitude(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFrequency(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawOffset(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPhase(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDelay(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPeriod(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDutyRatio(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDuration(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyLeft()
{
    if (!IN_NUM_LOCK_MODE)
    {
        InputWindowStruct::KeyLeft();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyRight()
{
    if (!IN_NUM_LOCK_MODE)
    {
        InputWindowStruct::KeyRight();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyEnter()
{
    InputWindowStruct::SaveValue();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::ProcessContorl(StructControl strContr)
{
    PanelControl key = strContr.key;
    TypePress pressed = strContr.typePress;

    if (key == B_0 || key == B_1 || key == B_2 || key == B_3 || key == B_4 || key == B_5 || key == B_6 || key == B_7 || key == B_8 || key == B_9 ||
        key == B_Dot)
    {
        if (pressed == TypePress_Release)
        {
            InputWindowStruct::PressKey(key);
        }
    }
    else if (key == REG_LEFT)
    {
        if (!IN_NUM_LOCK_MODE)
        {
            InputWindowStruct::RegLeft();
        }
    }
    else if (key == REG_RIGHT)
    {
        if (!IN_NUM_LOCK_MODE)
        {
            InputWindowStruct::RegRight();
        }
    }
    else if ((key >= B_F1 && key <= B_F4) && pressed == TypePress_Release)
    {
        Menu::CurrentPage()->Item(key - B_F1)->Press(strContr);
    }
}
