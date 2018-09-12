#include "defines.h"
#include "globals.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Display/Console.h"


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
       
    Generator::EmptyCommand();

    for(int i = 0; i < 10; i++)
    {
        Console::AddString("test");
    }

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        Generator::Update();
    }
}


/** @} */
