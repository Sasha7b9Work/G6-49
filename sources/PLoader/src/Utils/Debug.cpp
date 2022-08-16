// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "debug.h"
#include "Hardware/Timer.h"



static uint timeStart;
static uint timePrev;
static uint timeCounter;
static uint timeStartCounter;

// Время старта профилировщика
volatile static uint timeStartMS = 0;
// Время предыдущей точки профилировщика
volatile static uint timePrevMS = 0;

const char *Debug::file[10] = { 0 };
int   Debug::line[10] = { 0 };


void Debug::StartProfilingMS()
{
    timeStartMS = _TIME_MS;
    timePrevMS = _TIME_MS; //-V656
}


uint Debug::PointProfilingMS(const char * /*_file*/, int /*_line*/)
{
    uint d = _TIME_MS - timePrevMS;
    timePrevMS = _TIME_MS;

    return d;
}


void Debug::StartProfilingUS()
{
    timeStart = _TIME_US;
    timePrev = timeStart;
}


uint Debug::PointProfilingUS(const char * /*_file*/, int /*_line*/)
{
    uint d = _TIME_US - timePrev;
    timePrev = _TIME_US;
    return d;
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = _TIME_US;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (_TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
