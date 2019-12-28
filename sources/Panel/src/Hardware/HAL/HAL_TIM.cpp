#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


uint HAL_TIM::TimeMS()
{
    return HAL_GetTick();
}
