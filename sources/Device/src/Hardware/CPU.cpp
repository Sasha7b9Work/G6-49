#include "defines.h"
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
