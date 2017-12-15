#pragma once
#include "defines.h"
#include "Keyboard/Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Инициализация дисплея.
void Display_Init();

void Display_Update();

void Display_ToggleBuffers();

uint8 *Display_GetBuffer();

void Display_SetButton(Control control);

void Display_AddStringToConsole(char *string);

void Display_SetColorBackground();
