#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Interface/InterfacePanel.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstdlib>
#endif


/** @defgroup Panel
 *  @{
 */

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    
    Timer::Init();
    
    Display::Init();

    Timer::PauseOnTime(1000);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    set.Load();

    Menu::Init();

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        Interface::Update();
    }
}


/** @} */
