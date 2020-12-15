#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


TIM_HandleTypeDef handler;


void HAL_TIM3::Init()
{
    __HAL_RCC_TIM3_CLK_ENABLE(); //-V2571

    handler.Instance = TIM3; //-V2571
    handler.Init.Prescaler = 54000 - 1;
    handler.Init.CounterMode = TIM_COUNTERMODE_UP;
    handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handler);

    HAL_NVIC_EnableIRQ(TIM3_IRQn);

    HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);
}


void HAL_TIM3::StartIT(uint period)
{
    handler.Init.Period = period;
    HAL_TIM_Base_Init(&handler);
    HAL_TIM_Base_Start_IT(&handler);
}


void HAL_TIM3::StopIT()
{
    HAL_TIM_Base_Stop_IT(&handler);
}


void HAL_TIM3::DeInit()
{
    HAL_NVIC_DisableIRQ(TIM3_IRQn);

    HAL_TIM_Base_Stop_IT(&handler);

    __HAL_RCC_TIM3_CLK_DISABLE(); //-V2571
}
