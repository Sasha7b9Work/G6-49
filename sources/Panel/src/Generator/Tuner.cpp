#include "defines.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/Parameters.h"
#include "Generator/Tuner.h"
#include "Generator/Wave.h"
#include "Settings/Settings.h"


using namespace Primitives;


Tuner::Tuner(Parameter *_param) : param(_param), indicator(param)
{

}


void Tuner::Draw()
{
    indicator.Draw();
}


bool Tuner::OnKeyControl(const Control)
{
    return false;
}


void Indicator::Draw()
{
    Chan ch = param->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Rectangle(WaveGraphics::Width(), WaveGraphics::Height()).Fill(x, y, Color::BLUE_10);

    DrawTitle(x, y, WaveGraphics::Width());

    DrawValue();
}


void Indicator::DrawTitle(int x, int y, int)
{
    Font::Store();

    Font::Set(TypeFont::_GOSTB20);

    Text::Draw(x, y, param->Name(LANGUAGE), Color::WHITE);

    Font::Restore();
}


void Indicator::DrawValue()
{

}
