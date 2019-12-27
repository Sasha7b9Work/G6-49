#include "defines.h"
#include <stm32f4xx.h>
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal.h>
#include <cstdlib>
#include <cstring>


/// ¬рем€ последней установки состо€ни€ "зан€то"
static uint timeBusy = 0;


void CPU::Init()
{
    HAL::Init();

    InitGPIOS();

    HAL_PIO::Set(WR_OUT_A);
    HAL_PIO::Set(WR_OUT_B);
}


void CPU::InitGPIOS()
{
    SetBusy();

    GPIO_InitTypeDef isGPIO;

    isGPIO.Pin = GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF12_OTG_HS_FS;
    HAL_GPIO_Init(GPIOB, &isGPIO);

    isGPIO.Pin = GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    isGPIO.Mode = GPIO_MODE_AF_PP;
    isGPIO.Pull = GPIO_NOPULL;
    isGPIO.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    isGPIO.Alternate = GPIO_AF10_OTG_FS;
    HAL_GPIO_Init(GPIOA, &isGPIO);
}


void CPU::SetBusy()
{
    HAL_PIO::Reset(WR_CPU_BUSY);

    timeBusy = TIME_MS;
}


void CPU::SetReady()
{
    while (TIME_MS - timeBusy < 2)
    {
    };

    HAL_PIO::Set(WR_CPU_BUSY);
}
