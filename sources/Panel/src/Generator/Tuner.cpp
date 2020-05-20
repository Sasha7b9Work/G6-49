#include "defines.h"
#include "Display/Painter.h"
#include "Display/WaveGraphics.h"
#include "Generator/Parameters.h"
#include "Generator/Tuner.h"
#include "Generator/Wave.h"


Tuner::Tuner(Parameter *_param) : param(_param)
{

}


void Tuner::Draw()
{
    Chan ch = param->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Region(WaveGraphics::Width(), WaveGraphics::Height()).Fill(x, y, Color::BLUE_10);
}
