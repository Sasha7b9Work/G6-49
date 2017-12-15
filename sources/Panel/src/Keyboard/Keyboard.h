#pragma once
#include "Controls.h"
#include <stm32f4xx_hal.h>


#define TIME_LONG_PRESSURE 1000


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Keyboard_Init();
/// Возвращает true, если буфер пуст
bool Keyboard_BufferIsEmpty();
/// Возвращает следующий орган управления, если таковой имеется
StructControl Keyboard_GetNextControl();
