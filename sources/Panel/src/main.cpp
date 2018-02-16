#include "defines.h"
#include "Log.h"
#include "Display/Display.h"
#include "Hardware/Hardware.h"
#include "Generator/Generator.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


/** @defgroup Panel
 *  @{
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    Hardware_Init();

    Keyboard::Init();

    uint time = HAL_GetTick();

    while (HAL_GetTick() - time < 200)
    {
    };    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо
                                                // быстрее панели

    Generator_Reset();

    time = HAL_GetTick();

    while (HAL_GetTick() - time < 200) { };

    Settings_Load();

    gMenu.Init();

    while (1)
    {
        gMenu.Update();
        Display::Update();
    }
}


/** @} */
