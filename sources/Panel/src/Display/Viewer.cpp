#include "common/CommonTypes.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/Viewer.h"
#include "Generator/Signals.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


using namespace Primitives;


Viewer::Viewer(Parameter *_param) : param(_param)
{

}


void Viewer::Draw(int y, int xName, int xValue, int xUnits, Chan::E ch)
{
    Color color = Color::Chan(ch);

    if((ch == CURRENT_CHANNEL) &&
       (std::strcmp(CURRENT_PARAM->Name(), param->Name()) == 0) && //-V2513
       PageSignals::OnSubPageTuneChannels())
    {
        Rectangle(141, 9).Fill(xName, y, Color::GRAY_50);
        color = Color::BACK;
    }

    Font::ForceUpperCase(true);

    String(param->Name()).Draw(xName + 1, y, color);

    typedef void (Viewer::*funcDrawValue)(int, int, int);

    static const funcDrawValue funcs[ParameterKind::Count] =
    {
        &Viewer::DrawValue,
        &Viewer::DrawChoice,
        &Viewer::DrawComposite,
        &Viewer::DrawButton,
        &Viewer::DrawValue
    };

    (this->*funcs[param->GetKind()])(y, xValue, xUnits);
}


void Viewer::DrawName(int x, int y, int width, Chan::E ch)
{
    Color color = Color::FILL;

    if ((ch == CURRENT_CHANNEL) &&
        (std::strcmp(CURRENT_PARAM->Name(), param->Name()) == 0) && //-V2513
        PageSignals::OnSubPageTuneChannels())
    {
        Rectangle(width - 2, 8).Fill(x + 1, y + 1, Color::GREEN_50);
        HLine::Draw(y, x + 1, x - 1 + Item::WIDTH - 1, Color::GREEN_75);
        VLine::Draw(x, y, y - 1 + 10);
        HLine::Draw(y + 9, x, x + Item::WIDTH - 2, Color::GREEN_25);
        VLine::Draw(x - 2 + Item::WIDTH, y, y + 9);
        color = Color::BACK;
    }

    Font::ForceUpperCase(true);

    String(param->Name()).Draw(x + 1, y, color);
}


void Viewer::DrawValue(int y, int xValue, int xUnits)
{
    Font::ForceUpperCase(false);

    String units;

    param->ToString(units).Draw(xValue, y);

    units.Draw(xUnits, y);
}


void Viewer::DrawChoice(int y, int xValue, int)
{
    String units;
    String value = param->ToString(units);

    int spacing = ((value[0] == Ideograph::_8::PolarityPos) || (value[0] == Ideograph::_8::PolarityNeg)) ? 0 : 1;

    Font::Spacing::SetAndStore(spacing);

    value.Draw(xValue, y);

    Font::Spacing::Restore();
}


void Viewer::DrawComposite(int y, int xValue, int)
{
    String units;

    param->ToString(units).Draw(xValue, y);
}


void Viewer::DrawButton(int, int, int)
{
    
}
