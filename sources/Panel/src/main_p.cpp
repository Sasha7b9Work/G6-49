#include "defines.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Interface/Interface_p.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageService.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


int main()
{
    CPU::Init();
    Timer::Init();
    Display::Init(); 
    HAL_TIM::Delay(1000);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели
    PageService::OnPress_Reset();
    Menu::Init();

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        PInterface::Update();
    }
}
