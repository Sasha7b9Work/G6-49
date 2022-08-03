// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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
#include "Tests/Tests.h"
#include "Utils/StringUtils.h"


void init()
{
    FreeConsole();

    CPU::Init();

    Timer::Init();

    Display::Init();

    HAL_TIM::Delay(1);

    setCal.Load();

    set.LoadToDevice();

    Menu::Init();

    Locale::FindSeparator();

    PGenerator::EnableChannel(ChA, true);

    PGenerator::EnableChannel(ChB, true);
}



void update()
{
    static bool first = true;

    static uint timeStart = 0;

    if (first)
    {
        first = false;

        timeStart = _TIME_MS;
    }

    if (_TIME_MS - timeStart < 1)
    {
        Display::DrawScreenSplash();
    }
    else if (_TIME_MS - timeStart < 3)
    {
        TestsHardware::Run();
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

