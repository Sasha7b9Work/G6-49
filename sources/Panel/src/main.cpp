#include "defines.h"
#include "globals.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Generator/Wave.h"


/** @defgroup Panel
 *  @{
 */

static void SendToGenerator();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    
    Timer::Init();
    
    Display::Init();

    Timer::PauseOnTime(1000);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    Settings::Load();
       
    Menu::Init();

    Generator::LoadRegister(Register::OffsetA, 2048);
    
    Generator::LoadRegister(Register::OffsetB, 2048);

    while (1)
    {
        CPU::Update();
        Menu::Update();
        Display::Update();
        Generator::Update();
        SendToGenerator();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SendToGenerator()
{
    //Generator::LoadRegister(Register::Multiplexor1, 1);
    //Generator::LoadRegister(Register::Multiplexor1, 0);
}


/** @} */
