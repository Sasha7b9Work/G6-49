#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handler;


void HAL_TIM2::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    handler.Instance = TIM2;
    handler.Init.Prescaler = 0;
    handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    handler.Init.Period = static_cast<uint>(-1);
    handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handler);

    HAL_TIM_Base_Start(&handler);
}


uint HAL_TIM2::TimeUS()
{
    return (TIM2->CNT / 90);
}


void HAL_TIM2::DeInit()
{
    HAL_TIM_Base_Stop(&handler);

    __HAL_RCC_TIM2_CLK_DISABLE();
}
