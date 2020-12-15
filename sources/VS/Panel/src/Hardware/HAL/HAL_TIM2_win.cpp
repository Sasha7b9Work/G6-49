#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handler;


void HAL_TIM2::Init()
{

}


void HAL_TIM2::DeInit()
{
}


uint HAL_TIM2::TimeUS()
{
    return TIM2->CNT / 90; //-V2571
}

uint HAL_TIM2::GetTicks()
{
    return TIM2->CNT; //-V2571
}
