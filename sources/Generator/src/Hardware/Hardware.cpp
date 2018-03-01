#include "Hardware.h"
#include "Generator/Generator.h"
#include "Interface/Interface.h"
#include "Hardware/Timer.h"
#include "Hardware/CPU.h"
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Hardware hardware;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Init(void)
{
    CPU::Init();
    InitPins();
    GPIOS_Init();
    WritePin(Pin_P3_OutA, false);
    WritePin(Pin_P4_OutB, false);
    timer.PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    generator.Init();
    interface.Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::GPIOS_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO;

    // На этом пине будем выставлять занятость генератора
    isGPIO.Pin = GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
    isGPIO.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &isGPIO);

    hardware.SetBusy();

    isGPIO.Pin = GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    isGPIO.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &isGPIO);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetBusy(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetReady(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

#ifdef USE_FULL_ASSERT

//----------------------------------------------------------------------------------------------------------------------------------------------------
void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

