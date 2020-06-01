#include "common/CommonTypes.h"
#include "Display/Painter.h"
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

    Font::SetUpperCase(true);

    String(param->Name()).Draw(x + 1, y, Color::Chan(ch));

    DrawParameterValue(x + 80, y);
}


void Viewer::DrawParameterValue(int x, int y)
{
    if(param->IsChoice() && static_cast<ParameterChoice *>(param)->DrawChoice(x + 6, y))
    {
        return;
    }

    Font::SetUpperCase(false);
    if(param->IsDouble() && static_cast<ParameterDouble *>(param)->IsSigned())
    {
        x -= 4;
    }

    String(param->ToString()).Draw(x, y);

    Font::SetUpperCase(true);
}
