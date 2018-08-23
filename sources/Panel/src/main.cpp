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

    Timer::PauseOnTime(200);    // Задержка введена, потому что без неё не запускается генератор. Видимо, он инициализируется гораздо быстрее панели

    Generator::Reset();

    Settings::Load();
       
    Menu::Init();

    Generator::LoadRegister(Register::OffsetA, 2048);
    
    Generator::LoadRegister(Register::OffsetB, 2048);

    while (1)
    {
        if(Wave::Signal::Form(Chan::A) == WaveForm::Sine && CURRENT_CHANNEL == Chan::A)
        {
        }

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
    for(int i = 0 ; i < 100; i++)
    {
        //Generator::EnableChannel(Chan::A, !Generator::ChannelEnabled(Chan::A));
        //Generator::EnableChannel(Chan::B, !Generator::ChannelEnabled(Chan::B));
    }
}


/** @} */
