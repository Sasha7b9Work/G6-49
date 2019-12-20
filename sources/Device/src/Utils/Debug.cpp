#include "Debug.h"
#include "Hardware/Timer.h"


volatile static uint timeStart = 0;
static uint timeCounter = 0;
static uint timeStartCounter = 0;
char *Debug::file = 0;
int   Debug::line = 0;


void Debug::StartProfiling()
{
    timeStart = TIME_US;
}


void Debug::PointProfiling(char *)
{
    //LOG_WRITE("%s %d", name, TIME_US - timeStart);
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
