#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Console.h"
#include "Utils/Debug.h"


/** @defgroup Panel
 *  @{
 */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    
    Timer::Init();
    
    Display::Init();

    Timer::PauseOnTime(1000);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    Settings::Load();

    Menu::Init();
       
    Generator::EmptyCommand();

    for(int i = 0; i < 100; i++)
    {
        Console::AddString("test asdf");
    }

    while (1)
    {
        DEBUG_POINT;
        CPU::Update();
        DEBUG_POINT;
        Menu::Update();
        DEBUG_POINT;
        Display::Update();
        DEBUG_POINT;
        Generator::Update();
        DEBUG_POINT;
    }
}


/** @} */
