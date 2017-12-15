#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "DisplayTypes.h"
#include "PainterC.h"
#include "Menu/MenuItemsLogic.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (iws.numLockMode)

static Channel chan;
static WaveForm form;
static WaveParameter param;
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
void InputWindow_Init(void)
{
    chan = CHANNEL;
    form = WAVE_FORM_CH(chan);
    param = set.sig_parameter[form];
    iws = INPUT_WINDOW_STRUCT(chan, form, param);

	IWS_Fill(&iws, chan, form, param);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow_Draw(void)
{
    int x = 0;
    int y = HEIGHT_TITLE;
    int width = WIDTH_SIGNAL;
    int height = HEIGHT_SIGNAL;

    if (CHANNEL_IS_A)
    {
        y += HEIGHT_SIGNAL;
    }

    Painter_FillRegionC(x, y, width, height, COLOR_BACK);
    Painter_DrawRectangleC(x, y, width, height, COLOR_FILL);

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
        IWS_DrawInputField(10, y + 27);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    Painter_DrawBigText(x, y, 3, NameParameter(param));

    char buf[2] = "0";

    y += 40;

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        Painter_FillRegionC(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, COLOR_EMPTY_DIGIT);

        Painter_SetColor(COLOR_FILL);

        if (i == iws.hightLightDigit)
        {
            Painter_SetFont(TypeFont_UGO2);
            Painter_Draw4SymbolsInRect(x + 2, y - 10, '\xa2');
            Painter_Draw4SymbolsInRect(x + 2, y + 35, '\xa0');
            Painter_SetFont(TypeFont_8);
        }

        if (iws.inputBuffer[i])
        {
            buf[0] = iws.inputBuffer[i];
            Painter_DrawBigText(x, y - 1, SIZE_TEXT, buf);
        }

        x += 24;

        if (iws.posComma == i)
        {
            Painter_FillRegion(x - 3, y + HEIGHT_DIGIT - 2, SIZE_TEXT, SIZE_TEXT + 1);
            x += 4;
        }
    }

    char buffer[10] = {0};
    Painter_DrawBigText(x, y, SIZE_TEXT, NameUnit(buffer, iws.order, iws.param));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawAmplitude(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawFrequency(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawOffset(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPhase(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDelay(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPeriod(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDutyRatio(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawDuration(int x, int y)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow_KeyLeft(void)
{
    if (!IN_NUM_LOCK_MODE)
    {
        IWS_KeyLeft();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow_KeyRight(void)
{
    if (!IN_NUM_LOCK_MODE)
    {
        IWS_KeyRight();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow_KeyEnter(void)
{
    IWS_SaveValue();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow_ProcessContorl(StructControl control)
{
    Control key = control.control;
    TypePress pressed = control.typePress;

    if (key == B_0 || key == B_1 || key == B_2 || key == B_3 || key == B_4 || key == B_5 || key == B_6 || key == B_7 || key == B_8 || key == B_9 ||
        key == B_Dot)
    {
        if (pressed == TypePress_Release)
        {
            IWS_PressKey(key);
        }
    }
    else if (key == REG_A_LEFT)
    {
        if (!IN_NUM_LOCK_MODE)
        {
            IWS_RegLeft();
        }
    }
    else if (key == REG_B_RIGHT)
    {
        if (!IN_NUM_LOCK_MODE)
        {
            IWS_RegRight();
        }
    }
    else if ((key >= B_F1 & key <= B_F4) && pressed == TypePress_Release)
    {
        void *address = ItemFromPage(key - B_F1);
        if (ItemIsChoice(address))
        {
            Choice_Press((Choice *)address, pressed);
        }
        else if (ItemIsButton(address))
        {
            Button_Press((Button *)address);
        }
        else if (ItemIsSButton(address))
        {
            SButton_Press((SButton *)address);
        }
        else if (ItemIsChoiceParameter(address))
        {
            ChoiceWaveParameter_Press((ChoiceParameter *)address, pressed);
        }
    }
}
