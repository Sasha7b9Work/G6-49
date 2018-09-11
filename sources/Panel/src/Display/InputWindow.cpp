#include "globals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (m_iws.numLockMode)

Chan                    InputWindow::chan = Chan::A;
Wave::Form              InputWindow::form = Wave::Form::Sine;
Wave::Form::Parameter   InputWindow::m_param = Wave::Form::Parameter::Amplitude;
InputWindow::Struct     InputWindow::m_iws = InputWindow::Struct(' ');


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init()
{
    chan = CURRENT_CHANNEL;
    form = WAVE_CURRENT.GetCurrentForm();
    m_param = CURRENT_PARAMETER(form);
    m_iws = INPUT_WINDOW_STRUCT(chan, form, m_param);

	(&m_iws)->Fill(chan, form, m_param);
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
    
    static const pFuncVII func[Wave::Form::Parameter::Number] =
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

    func[m_param](x + 10, y + 10);

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

    Text::DrawBigText(x, y, 3, m_param.Name());

    char buf[2] = "0";

    y += 40;

    if(m_iws.sign != ' ')
    {
        Text::DrawBigChar(x - 24, y - 1, SIZE_TEXT, m_iws.sign);
    }

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        Painter::FillRegion(x, y, WIDTH_DIGIT, HEIGHT_DIGIT, Color::BACK);

        Painter::SetColor(Color::FILL);

        if (i == m_iws.hightLightDigit)
        {
            Font::Set(Font::Type::_UGO2);
            Text::Draw4SymbolsInRect(x + 2, y - 10, '\xa2');
            Text::Draw4SymbolsInRect(x + 2, y + 35, '\xa0');
            Font::Set(Font::Type::_8);
        }

        if (m_iws.buffer[i])
        {
            buf[0] = m_iws.buffer[i];
            Text::DrawBigText(x, y - 1, SIZE_TEXT, buf);
        }

        x += 24;

        if (m_iws.posComma == i)
        {
            Painter::FillRegion(x - 3, y + HEIGHT_DIGIT - 2, SIZE_TEXT, SIZE_TEXT + 1);
            x += 4;
        }
    }

    char buffer[10] = {0};
    Text::DrawBigText(x, y, SIZE_TEXT, NameUnit(buffer, (Order::E)m_iws.order, m_iws.param));
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::FillAllowParameters(Chan ch_, Wave::Form form_, AllowableParameters *allowParameters)
{
    for (int i = 0; i < Wave::Form::Parameter::Number; i++)
    {
        allowParameters->allow[i] = INPUT_WINDOW_STRUCT(ch_, form_, i).allow;
    }
}
