// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface_p.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/SCPI.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Tests/Tests.h"
#include "Utils/StringUtils.h"


int main()
{
    CPU::Init();
    Timer::Init();
    Display::Init();
    
    Locale::FindSeparator();

    Display::DrawScreenSplash();

    HAL_TIM::Delay(5000);       // Задержка нельзя удалять не только ради стартового экрана, но и потому,
                                // что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    TestsHardware::Run();

    setCal.Load();
    set.LoadDefault();
    set.LoadToDevice();

    Menu::Init();

    PGenerator::LoadPictureDDSfromStorage(ChA);
    PGenerator::LoadPictureDDSfromStorage(ChB);

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        PInterface::Update();
        SCPI::Update();
    }
}
