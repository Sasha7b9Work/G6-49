#include "defines.h"
#include "Beeper.h"
#include "Timer.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <cmath>



#define POINTS_IN_PERIOD_SOUND 10
static uint8 points[POINTS_IN_PERIOD_SOUND] = {0};
static float frequency = 0.0F;
static float amplitude = 0.0F;
static TypeWave::E typeWave = TypeWave::Sine;
static bool soundWarnIsBeep = false;
static bool buttonIsPressed = false;    ///< \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                        ///< отпускания
static volatile bool isBeep = false;

static bool bellIsEnabled = false;



static void Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration);

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


static void Beep(const TypeWave::E, const float, const float, const int)
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
    ::Beep(TypeWave::Sine, 2000.0F, 0.75F, 50);
    buttonIsPressed = true;
}


void Beeper::ButtonRelease()
{
    if (buttonIsPressed)
    {
        ::Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
        buttonIsPressed = false;
    }
}



void Beeper::GovernorChangedValue()
{
    ::Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
    buttonIsPressed = false;
}



void Beeper::RegulatorShiftRotate()
{
    ::Beep(TypeWave::Sine, 1.0F, 0.2F, 3);
    buttonIsPressed = false;
}



void Beeper::RegulatorSwitchRotate()
{
    ::Beep(TypeWave::Sine, 500.0F, 0.5F, 75);
    buttonIsPressed = false;
}



void Beeper::WarnBeepBad()
{
    ::Beep(TypeWave::Meandr, 500.0F, 1.0F, 500);
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}
 

void Beeper::WarnBeepGood()
{
    ::Beep(TypeWave::Triangle, 1000.0F, 1.0F, 500);
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
