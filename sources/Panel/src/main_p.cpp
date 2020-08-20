#include "defines.h"
#include "common/Interface_p.h"
#include "Display/Display.h"
#include "Hardware/Beeper.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "SCPI/SCPI.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


int main()
{
    CPU::Init();
    Timer::Init();
    Display::Init();

    Display::DrawScreenSplash();

    HAL_TIM::Delay(1000);       // Задержка нельзя удалять не только ради стартового экрана, но и потому, что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    setCal.Load();
    set.LoadDefault();
    set.LoadToDevice();

    Menu::Init();

    PGenerator::LoadPictureDDSfromStorage(Chan::A);
    PGenerator::LoadPictureDDSfromStorage(Chan::B);

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        PInterface::Update();
        SCPI::Update();

        //Beeper::Beep(Action::Down);
    }
}
