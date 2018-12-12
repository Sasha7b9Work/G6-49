#include "stdafx.h"
#ifndef WIN32
#include "Generator/Signals.h"
#include "InputWindow.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Symbols.h"
#include <cstdio>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
StructValue InputWindow::iws;
ParameterValue  *InputWindow::param = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void InputWindow::Init()
{
    param = (ParameterValue *)PARAM_CURRENT;
    iws.Set(param);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::Draw()
{
    Font::SetType(Font::Type::_7);

    int x = 0;
    int y = Page::Title::HEIGHT;
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
    func[ParameterValue::Number] =
    {
        DrawFrequency,
        DrawPeriod,
        DrawAmplitude,
        DrawOffset,
        DrawDuration,
        DrawDutyRatio,
        DrawPhase,
        DrawDelay,
        DrawDurationRise,
        DrawDurationFall,
        DrawDurationStady,
        DrawDutyFactor,
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

    Font::SetType(Font::Type::_8);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDigits(int x, int y)
{
#define SIZE_TEXT       5
#define WIDTH_DIGIT     (4 * SIZE_TEXT - 1)
#define HEIGHT_DIGIT    (7 * SIZE_TEXT - 1)

    if (!FREQ_ENABLED)
    {
        Text::DrawBigText(x, y, 3, param->Name());
    }

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
            Font::SetType(Font::Type::_8);
            Text::Draw4SymbolsInRect(x + 2, y - 10, Ideograph::_8::FillDown);
            Text::Draw4SymbolsInRect(x + 2, y + 35, Ideograph::_8::FillUp);
            Font::SetType(Font::Type::_7);
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
    Text::DrawBigText(x, y - 1, SIZE_TEXT, param->NameUnit(buffer));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationRise(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationFall(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDutyFactor(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDurationStady(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawAmplitude(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawFrequency(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawOffset(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPhase(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDelay(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPeriod(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPacketNumber(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawPacketPeriod(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDutyRatio(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawDuration(int, int)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawRampSineDuration(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawRampSineAmplitude(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::DrawExit(int, int)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyLeft()
{
    iws.KeyLeft();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyRight()
{
    iws.KeyRight();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::OrderUp()
{
    if(param->order < Order::Number - 1)
    {
        param->order++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::OrderDown()
{
    if(param->order > 0)
    {
        param->order--;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::KeyEnter()
{
    iws.SaveValue();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void InputWindow::ProcessContorl(Control key)
{
    if (key.IsDigit() || key.Is(Control::Dot) || key.Is(Control::Minus))
    {
        if (key.action.Is(Control::Action::Down))
        {
            iws.PressKey(key);
        }
    }
    else if (key.Is(Control::RegLeft))
    {
        iws.RegLeft();
    }
    else if (key.Is(Control::RegRight))
    {
        iws.RegRight();
    }
    else if (key.IsFunctional() && key.action.Is(Control::Action::Up))
    {
        if(CURRENT_PAGE)
        {
            CURRENT_PAGE->GetItem(key)->Press(key);
        }
    }
    else if(key.Is(Control::Left, Control::Action::Up))
    {
        KeyLeft();
    }
    else if(key.Is(Control::Right, Control::Action::Up))
    {
        KeyRight();
    }
    else if(key.Is(Control::Esc, Control::Action::Up))
    {
        Menu::ResetAdditionPage();
    }
}
