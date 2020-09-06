#include "defines.h"
#include "Editor/Painter/Canvas.h"
#include "Editor/Painter/Zoomer.h"
#include <vector>


int Zoomer::scale = 100;


static const int SCALE_MIN = 100;
static const int SCALE_MAX = 13000;

static const std::vector<int> scales { SCALE_MIN, 150, 225, 350, 500, 800, 1100, 1700, 2600, 3800, 5800, 8600, SCALE_MAX };


void Zoomer::Increase(int /*mouseX*/)
{
    for (uint i = 0; i < scales.size() - 1; i++)
    {
        if (scales[i] == scale)
        {
            scale = scales[i + 1];
            break;
        }
    }

    TheCanvas->Redraw();
}


void Zoomer::Decrease(int /*mouseX*/)
{
    for (uint i = scales.size() - 1; i > 0; i--)
    {
        if (scales[i] == scale)
        {
            scale = scales[i - 1];
            break;
        }
    }

    TheCanvas->Redraw();
}

void Zoomer::SetOnRegion()
{

}


bool Zoomer::NoScaling()
{
    return (scale == 100);
}


int Zoomer::Scale()
{
    return scale;
}
