#include "PainterC.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PainterC::FillRegion(int x, int y, int width, int height, Color color)
{
    Painter::SetColor(color);
    Painter::FillRegion(x, y, width, height);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void PainterC::DrawRectangle(int x, int y, int width, int height, Color color)
{
    Painter::SetColor(color);
    Painter::DrawRectangle(x, y, width, height);
}
