#include "defines.h"
#include "FDrive.h"
#include "Display/Painter.h"
#include "Wave.h"
#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FDrive::Draw()
{
    int x = Wave::Graphics::X();
    int y = Wave::Graphics::Y(Chan::A) + 1;
    int width = Wave::Graphics::Width() - 2;
    int height = Wave::Graphics::Height() * 2;
    Painter::FillRegion(x, y, width, height, Color::BACK);
}
