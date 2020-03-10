#include "common/CommonTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Generator/Signals.h"
#include "Generator/Viewer.h"
#include "Menu/Pages/PageSignals/PageSignals.h"
#include "Settings/Settings.h"


Viewer::Viewer(Parameter *_param) : param(_param)
{

}


void Viewer::Draw(int x, int y)
{
    Chan::E ch = param->GetForm()->GetWave()->GetChannel();

    if((ch == CURRENT_CHANNEL) && std::strcmp(CURRENT_PARAM->Name(LANG_IS_EN), param->Name(LANG_IS_EN)) == 0 && CURRENT_PAGE == PageSignals::self)
    {
        Painter::FillRegion(x, y, 139, 9, Color::GRAY_25);
    }

    String(param->Name(LANG_IS_EN)).Draw(x + 1, y, Color::Chan(ch));

    DrawParameterValue(x + 80, y);
}


void Viewer::DrawParameterValue(int x, int y)
{
    if(param->IsChoice() && static_cast<ParameterChoice *>(param)->DrawChoice(x + 6, y))
    {
        return;
    }

    Text::SetUpperCase(false);
    if(param->IsValue() && static_cast<ParameterValue *>(param)->IsSigned())
    {
        x -= 4;
    }
    String(param->GetStringValue()).Draw(x, y);
    Text::SetUpperCase(true);
}
