// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface_p.h"
#include "common/Messages_l.h"
#include "Updater_pl.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"
#include "Utils/Debug.h"
#include "common/Handlers_p.h"
#include <stm32f429xx.h>


static void StartToMainApplication();
void MainCycle();
void TestCycle();


int main()
{
    CPU::Init();
    Timer::Init();
    HAL_LTDC::Init();
    Keyboard::Init();

    while(true)
    {
        MainCycle();
    }
}


void MainCycle()
{
    static TimeMeterMS meter;

    Updater::Update();

    PInterface::Update();

    if (meter.ElapsedTime() > 3000)
    {
        StartToMainApplication();
    }
}


void TestCycle()
{
    PInterface::Test();
}


void StartToMainApplication()
{
    typedef void(*pFunction)();

    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(Updater::MAIN_PROGRAM_START_ADDRESS + 4));

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();
}
