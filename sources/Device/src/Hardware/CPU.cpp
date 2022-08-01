// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cstdlib>
#include <cstring>


namespace CPU
{
    static void InitGPIOS();

    // ¬рем€ последней установки состо€ни€ "зан€то"
    static uint timeBusy = 0;
}


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

    timeBusy = HAL_TIM::TimeMS();
}


void CPU::SetReady()
{
    while (HAL_TIM::TimeMS() - timeBusy < 2)
    {
    };

    HAL_PIO::Set(WR_CPU_BUSY);
}
