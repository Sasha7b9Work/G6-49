#pragma once
#include "Display/Colors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LTDC_
{
friend class Display;

private:

    static void Init(uint frontBuffer, uint backBuffer);

public:

    static void SetColors(uint clut[], uint8 numColors);

    static void ToggleBuffers();

    static void FillRegion(int x, int y, int width, int height, Color color);

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);

    static DMA2D_HandleTypeDef hDMA2D;
};
