#include "globals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (param->InNumLockMode())

InputWindow::Struct InputWindow::m_iws;
Parameter           *InputWindow::param = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init()
{
    param = PARAM_CURRENT;
    m_iws.Set(FORM_CURRENT, param);
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
    
    static const pFuncVII func[Parameter::Number] =
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

    func[param->value](x + 10, y + 10);

    if (IN_NUM_LOCK_MODE)
    {
        m_iws.DrawInputField(10, y + 27);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    Text::DrawBigText(x, y, 3, param->Name());

    char buf[2] = "0";

    y += 40;

    if(param->sign != ' ')
    {
        Text::DrawBigChar(x - 24, y - 1, SIZE_TEXT, param->sign);
    }

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        Painter::FillRegion(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, Color::BACK);

        Painter::SetColor(Color::FILL);

        if (i == param->hightLightDigit)
        {
            Font::Set(Font::Type::_UGO2);
            Text::Draw4SymbolsInRect(x + 2, y - 10, '\xa2');
            Text::Draw4SymbolsInRect(x + 2, y + 35, '\xa0');
            Font::Set(Font::Type::_8);
        }

        if (param->buffer[i])
        {
            buf[0] = param->buffer[i];
            Text::DrawBigText(x, y - 1, SIZE_TEXT, buf);
        }

        x += 24;

        if (param->posComma == i)
        {
            Painter::FillRegion(x - 3, y + HEIGHT_DIGIT - 2, SIZE_TEXT, SIZE_TEXT + 1);
            x += 4;
        }
    }

    char buffer[10] = {0};
    Text::DrawBigText(x, y, SIZE_TEXT, NameUnit(buffer, param->order, param->value));
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
        m_iws.KeyLeft();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyRight()
{
    if (!IN_NUM_LOCK_MODE)
    {
        m_iws.KeyRight();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyEnter()
{
    m_iws.SaveValue();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::ProcessContorl(Control control)
{
    if ((control >= Control::B_0 && control <= Control::B_9) || control.Is(Control::B_Dot) || control.Is(Control::B_Minus))
    {
        if (control.action.Is(Control::Action::Up))
        {
            m_iws.PressKey(control);
        }
    }
    else if (control.Is(Control::REG_LEFT))
    {
        if (!IN_NUM_LOCK_MODE)
        {
            m_iws.RegLeft();
        }
    }
    else if (control.Is(Control::REG_RIGHT))
    {
        if (!IN_NUM_LOCK_MODE)
        {
            m_iws.RegRight();
        }
    }
    else if ((control >= Control::B_F1 && control <= Control::B_F4) && control.action.Is(Control::Action::Up))
    {
        Menu::CurrentPage()->GetItem(control - Control::B_F1)->Press(control);
    }
}
