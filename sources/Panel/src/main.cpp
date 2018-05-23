#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


/** @defgroup Panel
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    CPU::Init();
    
    Timer::Init();
    
    Display::Init();

    Timer::PauseOnTime(200);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    Generator::Reset();

    Settings::Load();
       
    Menu::Init();

    LOG_WRITE("%d %d", sizeof(InputWindowStruct), sizeof(Settings));

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
    }
}


/** @} */
