#pragma clang diagnostic ignored "-Wpadded"
#include <stm32f4xx.h>
#pragma clang diagnostic warning "-Wpadded"
#include "CPU.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Эта функция вызывается при срабатывании любого аппаратного таймера
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    CPU::Keyboard::TIM4_::ElapsedCallback(htim);
}
