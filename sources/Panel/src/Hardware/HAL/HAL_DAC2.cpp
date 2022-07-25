// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include <stm32f4xx_hal.h>


static DAC_HandleTypeDef handleDAC = { DAC };

static TIM_HandleTypeDef hTIM;

static bool isRunning = false;


void HAL_DAC2::Init()
{
    __TIM7_CLK_ENABLE();

    GPIO_InitTypeDef structGPIO =
    {
        GPIO_PIN_5,
        GPIO_MODE_OUTPUT_PP,
        GPIO_NOPULL,
        0, 0
    };

    HAL_GPIO_Init(GPIOA, &structGPIO);

    HAL_NVIC_SetPriority(TIM7_IRQn, 5, 5);

    HAL_NVIC_EnableIRQ(TIM7_IRQn);

    hTIM.Instance = TIM7;
    /*
    * 100 - 5 êÃö
    * 125 - 4 êÃö
    * 200 - 2.5 êÃö
    * 400 - 1.25 êÃö
    */
    switch (setCal.soundVolume)
    {
        case 1: hTIM.Init.Period = 165;
                hTIM.Init.Prescaler = 90;   break;

        case 2: hTIM.Init.Period = 140;
                hTIM.Init.Prescaler = 90;   break;

        default: hTIM.Init.Period = 100;
                 hTIM.Init.Prescaler = 90;  break;
    }

    hTIM.Init.ClockDivision = 0;
    hTIM.Init.CounterMode = TIM_COUNTERMODE_UP;
}


void HAL_DAC2::Start()
{
    if(setCal.soundVolume != 0)
    {
        HAL_TIM_Base_Init(&hTIM);
        HAL_TIM_Base_Start_IT(&hTIM);
    }

    isRunning = true;
}


void HAL_DAC2::Stop()
{
    HAL_TIM_Base_Stop_IT(&hTIM);
    HAL_TIM_Base_DeInit(&hTIM);

    isRunning = false;
}


bool HAL_DAC2::IsRun()
{
    return isRunning;
}


#ifdef __cplusplus
extern "C" {
#endif


void TIM7_IRQHandler()
{
    if ((TIM7->SR & TIM_SR_UIF) == TIM_SR_UIF)
    {
        if ((TIM7->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
        {
            TIM7->SR = ~TIM_DIER_UIE;

            if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) == GPIO_PIN_SET)
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            }
            else
            {
                HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            }
        }
    }
}

#ifdef __cplusplus
}
#endif
