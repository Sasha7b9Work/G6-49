#include "defines.h"
#include "common/Interface_p.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "SCPI/SCPI.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


void init()
{
    CPU::Init();

    Timer::Init();

    Display::Init();

    HAL_TIM::Delay(1);

    setCal.Load();

    set.LoadFromMemory();
    set.LoadToDevice();

    Menu::Init();
}



void update()
{
    static bool first = true;

    static uint timeStart = 0;

    if (first)
    {
        first = false;

        timeStart = TIME_MS;
    }

    if (TIME_MS - timeStart < 1000)
    {
        Display::DrawScreenSplash();
    }
    else
    {
        CPU::Update();

        Menu::Update();

        Display::Update();

        PInterface::Update();

        SCPI::Update();
    }
}

