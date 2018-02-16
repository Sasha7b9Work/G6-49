#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static void Init();

    static void Update();

    static void ToggleBuffers();

    static uint8 *GetBuffer();

    static void SetButton(Control control);

    static void AddStringToConsole(char *string);

    static void SetColorBackground();
};
