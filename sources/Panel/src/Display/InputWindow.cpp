#include "globals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (iws.numLockMode)

Channel            InputWindow::chan;
WaveForm           InputWindow::form;
Type_WaveParameter InputWindow::param;
InputWindowStruct  InputWindow::iws;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init()
{
    chan = CURRENT_CHANNEL;
    form = WAVE_FORM_CH(chan);
    param = CURRENT_PARAMETER(form);
    iws = INPUT_WINDOW_STRUCT(chan, form, param);

	(&iws)->Fill(chan, form, param);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::Draw()
{
    if(ADDITION_PAGE_IS_NONE)
    {
        return;
    }

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
        DrawDelay,
        DrawDepthModulation,
        DrawPolarity,
        DrawDurationRise,
        DrawDurationFall,
        DrawDurationStady,
        DrawDutyFactor
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
void InputWindow::DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    Text::DrawBigText(x, y, 3, Parameter_Name(param));

    char buf[2] = "0";

    y += 40;

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        Painter::FillRegion(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, Color::BACK);

        Painter::SetColor(Color::FILL);

        if (i == iws.hightLightDigit)
        {
            Text::SetFont(TypeFont_UGO2);
            Text::Draw4SymbolsInRect(x + 2, y - 10, '\xa2');
            Text::Draw4SymbolsInRect(x + 2, y + 35, '\xa0');
            Text::SetFont(TypeFont_8);
        }

        if (iws.inputBuffer[i])
        {
            buf[0] = iws.inputBuffer[i];
            Text::DrawBigText(x, y - 1, SIZE_TEXT, buf);
        }

        x += 24;

        if (iws.posComma == i)
        {
            Painter::FillRegion(x - 3, y + HEIGHT_DIGIT - 2, SIZE_TEXT, SIZE_TEXT + 1);
            x += 4;
        }
    }

    char buffer[10] = {0};
    Text::DrawBigText(x, y, SIZE_TEXT, NameUnit(buffer, iws.order, iws.param));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDepthModulation(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPolarity(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationRise(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationFall(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDutyFactor(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationStady(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawAmplitude(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawFrequency(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawOffset(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPhase(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDelay(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPeriod(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDutyRatio(int, int)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDuration(int, int)
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
    Control key = strContr.key;
    TypePress pressed = strContr.typePress;

    if ((key >= B_0 && key <= B_9) || key == B_Dot)
    {
        if (pressed == Up)
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
    else if ((key >= B_F1 && key <= B_F4) && pressed == Up)
    {
        Menu::CurrentPage()->GetItem(key - B_F1)->Press(strContr);
    }
}
