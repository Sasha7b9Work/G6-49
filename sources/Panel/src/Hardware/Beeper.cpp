#include "defines.h"
#include "Beeper.h"
#include "Timer.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <cmath>


static void Stop();


void Beeper::Init()
{
    HAL_DAC2::Init();
}


static void Stop()
{
    HAL_DAC2::StopDMA();
}


static void Beep(uint period)
{
    HAL_DAC2::StartDMA(period);
    Timer::SetAndStartOnce(Timer::Type::StopSound, Stop, 5000);
}


void Beeper::ButtonPress()
{
    ::Beep(1);
}


void Beeper::ButtonRelease()
{
    ::Beep(0xf);
}


void Beeper::GovernorChangedValue()
{
    ::Beep(0x5);
}


void Beeper::WarnBeepBad()
{
    ::Beep(0xff);
}
 

void Beeper::WarnBeepGood()
{
    ::Beep(0xff);
}


void Beeper::Beep(Action::E action)
{
    static const pFuncVV func[Action::Count] =
    {
        Beeper::ButtonPress,
        Beeper::ButtonRelease,
        Beeper::ButtonRelease
    };

    if (action < Action::Count)
    {
        func[action]();
    }
}


void Beeper::Test()
{
    ButtonPress();
    ButtonRelease();
    GovernorChangedValue();
    WarnBeepBad();
    WarnBeepGood();
}


uint8 Beeper::Volume()
{
    return 100;
}
