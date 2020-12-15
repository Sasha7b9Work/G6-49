#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


static TIM_HandleTypeDef handler;


void HAL_TIM2::Init()
{
    __HAL_RCC_TIM2_CLK_ENABLE(); //-V2571

    handler.Instance = TIM2; //-V2571
    handler.Init.Prescaler = 0;
    handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    handler.Init.Period = static_cast<uint>(-1);
    handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handler);

    HAL_TIM_Base_Start(&handler);
}


uint HAL_TIM2::GetTicks()
{
    return TIM2->CNT; //-V2571
}


void HAL_TIM2::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN; //-V2571
    TIM2->CNT = 0; //-V2571
    TIM2->CR1 |= TIM_CR1_CEN; //-V2571
}


uint HAL_TIM2::TimeUS()
{
    return (TIM2->CNT / 90); //-V2571
}
