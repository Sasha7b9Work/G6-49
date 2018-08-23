#include "defines.h"
#include "globals.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Generator/Generator.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


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

    Timer::PauseOnTime(200);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    Generator::Reset();

    Settings::Load();
       
    Menu::Init();

    Generator::LoadRegister(Register::OffsetA, 2048);
    
    Generator::LoadRegister(Register::OffsetB, 2048);

    while (1)
    {
        if(WAVE_FORM_CH(Chan::A).Is(WaveForm::Sine) && CURRENT_CHANNEL.IsA())
        {
        }

        CPU::Update();
        Menu::Update();
        Display::Update();
        Generator::Update();
        //SendToGenerator();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SendToGenerator()
{
    Generator::LoadRegister(Register::Multiplexor1, 100);
}


/** @} */
