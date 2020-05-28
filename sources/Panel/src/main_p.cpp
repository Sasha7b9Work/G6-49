#include "defines.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface_p.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


int main()
{
    CPU::Init();
    Timer::Init();
    Display::Init(); 
    HAL_TIM::Delay(500);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели
    
    setCal.Load();
    set.LoadDefault();
    set.LoadToDevice();

    Menu::Init();

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        PInterface::Update();
    }
}
