#include "defines.h"
#include "Display/Display.h"
#include "Interface/InterfacePanel.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


void init()
{
    CPU::Init();

    Timer::Init();

    Display::Init();

    Timer::PauseOnTime(1000);

    setCal.Load();

    set.Load();

    Menu::Init();
}



void update()
{
    CPU::Update();

    Menu::Update();

    Display::Update();

    Interface::Update();
}

