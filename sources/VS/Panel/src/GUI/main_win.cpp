#include "defines.h"
#include "Display/Display.h"
#include "Interface/Interface_p.h"
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
    CPU::Update();

    Menu::Update();

    Display::Update();

    PInterface::Update();

    SCPI::Update();
}

