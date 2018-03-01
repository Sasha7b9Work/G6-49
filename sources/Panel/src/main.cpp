#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


/** @defgroup Panel
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    CPU::Init();
   
    Display::Init();

    uint time = TIME_MS;

    while (TIME_MS - time < 200)
    {
    };    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо
                                                // быстрее панели

    Generator::Reset();

    time = TIME_MS;

    while (TIME_MS - time < 200) { };

    Settings_Load();

    Menu::Init();

    while (1)
    {
        Menu::Update();
        Display::Update();
    }
}


/** @} */
