#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LTDC_
{
friend class Display;

private:

    static void Init();

public:

    static void SetColors(uint clut[], uint numColors);

    static void SetBuffers(uint frontBuffer, uint backBuffer);

    static void ToggleBuffers();
};
