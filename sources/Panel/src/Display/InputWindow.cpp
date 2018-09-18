#include "Generator/Signals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_NUM_LOCK_MODE    (param->InNumLockMode())

StructValue InputWindow::m_iws;
Parameter  *InputWindow::param = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init()
{
    param = PARAM_CURRENT;
    m_iws.Set(param);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::Draw()
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

    struct StructFunc
    {
        pFuncVII func;
        StructFunc(pFuncVII f) : func(f) {};
    }
    func[Parameter::Number] =
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
        DrawDutyFactor,
        DrawModulationRampSine,
        DrawRampSineDuration,
        DrawRampSineAmplitude,
        DrawPacketPeriod,
        DrawPacketNumber,
        DrawExit
    };

    x += 40;
    y += 10;

    DrawDigits(x, y);

    func[param->value].func(x + 10, y + 10);

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
    Text::DrawBigText(x, y, SIZE_TEXT, param->NameUnit(buffer));
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
void InputWindow::DrawPacketNumber(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPacketPeriod(int, int)
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
void InputWindow::DrawModulationRampSine(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawRampSineDuration(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawRampSineAmplitude(int, int)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawExit(int, int)
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
void InputWindow::ProcessContorl(Control key)
{
    if ((key >= Control::_0 && key <= Control::_9) || key.Is(Control::Dot) || key.Is(Control::Minus))
    {
        if (key.action.Is(Control::Action::Down))
        {
            m_iws.PressKey(key);
        }
    }
    else if (key.Is(Control::Reg::Left))
    {
        if (!IN_NUM_LOCK_MODE)
        {
            m_iws.RegLeft();
        }
    }
    else if (key.Is(Control::Reg::Right))
    {
        if (!IN_NUM_LOCK_MODE)
        {
            m_iws.RegRight();
        }
    }
    else if ((key >= Control::F1 && key <= Control::F4) && key.action.Is(Control::Action::Up))
    {
        if(CURRENT_PAGE)
        {
            CURRENT_PAGE->GetItem(key)->Press(key);
        }
    }
}
