#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Generator/Tuner.h"
#include "Generator/Wave.h"
#include "Settings/Settings.h"


using namespace Primitives;


Indicator::Indicator() : indexHighlight(0)
{
    digits[MAX_NUM_DIGITS - 1] = '\0';
}


void Indicator::Draw(int x, int y)
{
    static const int dx = 12;

    Font::Store();

    Font::Set(TypeFont::_GOSTB20);

    int pos = 0;

    while (digits[pos].value != '\0')
    {
        Char(digits[pos].value).Draw(x, y);

        if (pos == indexHighlight)
        {
            HighlightSymbol(x, y);
        }

        x += dx;
        pos++;
    }

    Font::Restore();
}


void Indicator::HighlightSymbol(int x, int y)
{
    Font::Store();

    Font::Set(TypeFont::_7);

    x += 2;

    Char(Ideograph::_7::FillDown).Draw(x, y - 7);

    Char(Ideograph::_7::FillUp).Draw(x, y + 19);

    Font::Restore();
}


void TunerDisplay::Draw()
{
    Chan ch = param->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Rectangle(WaveGraphics::Width(), WaveGraphics::Height()).DrawFilled(x, y, Color::BLUE_10, Color::WHITE);

    DrawTitle(x, y, WaveGraphics::Width());

    DrawValue(x, y + 50);
}


void TunerDisplay::DrawTitle(int x, int y, int width)
{
    Font::Store();

    Font::Set(TypeFont::_GOSTB20);

    Text(param->Name(LANGUAGE)).DrawInCenterRect(x, y, width, 30, Color::WHITE);

    Font::Restore();
}


void TunerDisplay::DrawValue(int x, int y)
{
//    Order::E order = Order::One;
//
//    pString str = MathFloatValue::GetStringValue(param->value, param->IsSigned(), 10, &order);

    indicator.Draw(x + 20, y);
}


Tuner::Tuner(ParameterValue *_param) : param(_param), display(param)
{

}


void Tuner::Draw()
{
    display.Draw();
}


bool Tuner::OnKeyControl(const Control)
{
    return false;
}
