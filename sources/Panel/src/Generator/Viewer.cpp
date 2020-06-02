#include "common/CommonTypes.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Generator/Signals.h"
#include "Generator/Viewer.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"


using namespace Primitives;


Viewer::Viewer(Parameter *_param) : param(_param)
{

}


void Viewer::Draw(int x, int y)
{
    Chan::E ch = param->GetForm()->GetWave()->GetChannel();

    if((ch == CURRENT_CHANNEL) && 
       (std::strcmp(CURRENT_PARAM->Name(), param->Name()) == 0) &&
       (CURRENT_PAGE == PageSignals::self))
    {
        Rectangle(139, 9).Fill(x, y, Color::GRAY_25);
    }

    Font::ForceUpperCase(true);

    String(param->Name()).Draw(x + 1, y, Color::Chan(ch));

    typedef void (Viewer::*funcDraw)(int, int);

    static const funcDraw funcs[ParameterKind::Count] =
    {
        &Viewer::DrawDouble,
        &Viewer::DrawEmpty,
        &Viewer::DrawChoice,
        &Viewer::DrawEmpty
    };

    (this->*funcs[param->GetKind()])(x + 80, y);
}


void Viewer::DrawDouble(int x, int y)
{
    Font::ForceUpperCase(false);

    if(static_cast<ParameterDouble *>(param)->IsSigned())
    {
        x -= 4;
    }

    String(param->ToString()).Draw(x, y);
}


void Viewer::DrawChoice(int x, int y)
{
    String string = param->ToString();

    int spacing = ((string[0] == Ideograph::_8::PolarityPos) || (string[0] == Ideograph::_8::PolarityNeg)) ? 0 : 1;

    Font::Spacing::SetAndStore(spacing);

    string.Draw(x, y);

    Font::Spacing::Restore();
}


void Viewer::DrawEmpty(int, int)
{

}
