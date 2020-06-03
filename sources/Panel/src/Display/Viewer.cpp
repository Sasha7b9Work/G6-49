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


void Viewer::Draw(int y, int xName, int xValue, int xUnits)
{
    Chan::E ch = param->GetForm()->GetWave()->GetChannel();

    if((ch == CURRENT_CHANNEL) && 
       (std::strcmp(CURRENT_PARAM->Name(), param->Name()) == 0) &&
       (CURRENT_PAGE == PageSignals::self))
    {
        Rectangle(141, 9).Fill(xName, y, Color::GRAY_25);
    }

    Font::ForceUpperCase(true);

    String(param->Name()).Draw(xName + 1, y, Color::Chan(ch));

    typedef void (Viewer::*funcDrawValue)(int, int, int);

    static const funcDrawValue funcs[ParameterKind::Count] =
    {
        &Viewer::DrawDoubleValue,
        &Viewer::DrawChoiceValue,
        &Viewer::DrawCompositeValue,
        &Viewer::DrawButton,
        &Viewer::DrawIntegerValue
    };

    (this->*funcs[param->GetKind()])(y, xValue, xUnits);
}


void Viewer::DrawDoubleValue(int y, int xValue, int xUnits)
{
    Font::ForceUpperCase(false);

    String units;

    param->ToString(units).Draw(xValue, y);

    units.Draw(xUnits, y);
}


void Viewer::DrawIntegerValue(int, int, int)
{

}


void Viewer::DrawChoiceValue(int y, int xValue, int)
{
    String units;
    String value = param->ToString(units);

    int spacing = ((value[0] == Ideograph::_8::PolarityPos) || (value[0] == Ideograph::_8::PolarityNeg)) ? 0 : 1;

    Font::Spacing::SetAndStore(spacing);

    value.Draw(xValue, y);

    Font::Spacing::Restore();
}


void Viewer::DrawCompositeValue(int y, int xValue, int)
{
    String units;

    param->ToString(units).Draw(xValue, y);
}


void Viewer::DrawButton(int, int, int)
{
    
}
