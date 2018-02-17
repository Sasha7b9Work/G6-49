#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wold-style-cast"

#include "stm32f7xx_hal.h"

#pragma clang diagnostic warning "-Wpadded"
#pragma clang diagnostic warning "-Wold-style-cast"


void HAL_MspInit()
{
#pragma clang diagnostic ignored "-Wold-style-cast"
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
#pragma clang diagnostic warning "-Wold-style-cast"
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
