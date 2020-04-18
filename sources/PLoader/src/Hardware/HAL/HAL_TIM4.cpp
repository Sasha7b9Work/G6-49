#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


#define TIME_UPDATE 2   ///< Время между опросами клавиатуры


// Таймер для опроса клавиатуры
static TIM_HandleTypeDef handleTIM4;


static void(*callbackKeyboard)() = 0;


void HAL_TIM4::Init(void (*func)())
{
    callbackKeyboard = func;

    // Инициализируем таймер, по прерываниям которого будем опрашивать клавиатуру
    HAL_NVIC_SetPriority(TIM4_IRQn, 0, 1);

    HAL_NVIC_EnableIRQ(TIM4_IRQn);

    handleTIM4.Instance = TIM4;
    handleTIM4.Init.Period = TIME_UPDATE * 10 - 1;
    handleTIM4.Init.Prescaler = static_cast<uint>((SystemCoreClock / 2) / 10000) - 1;
    handleTIM4.Init.ClockDivision = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;

    if(HAL_TIM_Base_Init(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }

    if(HAL_TIM_Base_Start_IT(&handleTIM4) != HAL_OK)
    {
        ERROR_HANDLER();
    }
}


void HAL_TIM4::Start(uint timeStopMS)
{
    Stop();

    if(timeStopMS == MAX_UINT)
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    handleTIM4.Init.Period = (dT * 2) - 1;  // 10 соответствует 0.1мс. Т.е. если нам нужна 1мс, нужно засылать (100 - 1)

    HAL_TIM_Base_Init(&handleTIM4);
    HAL_TIM_Base_Start_IT(&handleTIM4);
}


void HAL_TIM4::Stop()
{
    HAL_TIM_Base_Stop_IT(&handleTIM4);
}

void HAL_TIM4::ElapsedCallback()
{
    if(callbackKeyboard)
    {
        callbackKeyboard();
    }
}
