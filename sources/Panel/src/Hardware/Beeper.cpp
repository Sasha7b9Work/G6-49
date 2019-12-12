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
    Timer::SetAndStartOnce(Timer::Type::StopSound, Stop, 50);
}


void Beeper::ButtonPress()
{
    ::Beep(0x2);
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


void Beeper::Beep(KeyEvent::Action::E type)
{
    static const pFuncVV func[KeyEvent::Action::Count] =
    {
        Beeper::ButtonPress,
        Beeper::ButtonRelease,
        Beeper::ButtonRelease
    };

    if (type < KeyEvent::Action::Count)
    {
        func[type]();
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
