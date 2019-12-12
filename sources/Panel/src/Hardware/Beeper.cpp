#include "defines.h"
#include "Beeper.h"
#include "Timer.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <cmath>



static bool soundWarnIsBeep = false;
static bool buttonIsPressed = false;    ///< \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                        ///< отпускания
static volatile bool isBeep = false;

static bool bellIsEnabled = false;


static void Stop();


void Beeper::Init()
{
    HAL_DAC2::Init();
}


static void Stop()
{
    HAL_DAC2::StopDMA();
    isBeep = false;
    soundWarnIsBeep = false;
}


static void Beep()
{
    HAL_DAC2::StartDMA();
    Timer::SetAndStartOnce(Timer::Type::StopSound, Stop, 50);
}


void Beeper::WaitForCompletion()
{
    while (isBeep)
    {
    };
}


void Beeper::ButtonPress()
{
    ::Beep();
    buttonIsPressed = true;
}


void Beeper::ButtonRelease()
{
    if (buttonIsPressed)
    {
        ::Beep();
        buttonIsPressed = false;
    }
}



void Beeper::GovernorChangedValue()
{
    ::Beep();
    buttonIsPressed = false;
}



void Beeper::RegulatorShiftRotate()
{
    ::Beep();
    buttonIsPressed = false;
}



void Beeper::RegulatorSwitchRotate()
{
    ::Beep();
    buttonIsPressed = false;
}



void Beeper::WarnBeepBad()
{
    ::Beep();
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}
 

void Beeper::WarnBeepGood()
{
    ::Beep();
    buttonIsPressed = false;
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
    RegulatorShiftRotate();
    RegulatorSwitchRotate();
    WarnBeepBad();
    WarnBeepGood();
}


uint8 Beeper::Volume()
{
    return 100;
}
