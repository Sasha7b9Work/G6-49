#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LTDC_
{
friend class Display;

private:

    static void Init(uint frontBuffer, uint backBuffer);

public:

    static void SetColors(uint clut[], uint8 numColors);

    static void ToggleBuffers();

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);

    static DMA2D_HandleTypeDef hDMA2D;
};
