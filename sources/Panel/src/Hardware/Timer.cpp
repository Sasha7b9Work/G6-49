#include "Timer.h"
#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    pFuncVV func;        // Функция таймера
    uint    dTms;        // Период срабатывания, мс
    uint    timeFirstMS; // Время первого срабатывания
    uint    timeNextMS;  // Время следующего срабатывания. Если == 0xffffffff, то таймер неактивен
    bool    repeat;      // Если true, будет срабатывать, пока не будет вызвана функция Timer_Disable()
    uint8   notUsed[3];
} TimerStruct;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TimerStruct timers[NumTimers];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static uint NearestTime();          // Возвращает время срабатывания ближайщего таймера, либо 0, если таймеров нет
static void TuneTIM(TypeTimer2 type);   // Настроить систему на таймер


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Timer::IsRun(TypeTimer2 type)
{
    return TIME_NEXT(type) != MAX_UINT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Init(void)
{
    for(uint i = 0; i < NumTimers; i++)
    {
        timers[i].timeNextMS = MAX_UINT;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Set(TypeTimer2 type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndStartOnce(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    StartOnce(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndEnable(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartOnce(TypeTimer2 type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Enable(TypeTimer2 type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static void TuneTIM(TypeTimer2 type)
{
    TimerStruct *timer = &timers[type];
    timer->timeFirstMS = TIME_MS;

    uint timeNearest = NearestTime();

    uint timeNext = timer->timeFirstMS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)          // Если таймер должен сработать раньше текущего
    {
        CPU::TIM3_::Start(timeNext);   // то заводим таймер на наше время
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Disable(TypeTimer2 type)
{
    timers[type].timeNextMS = MAX_UINT;
    timers[type].repeat = false;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint NearestTime(void)
{
    uint time = MAX_UINT;

    for(uint type = 0; type < NumTimers; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTime(uint timeMS)
{
    uint time = TIME_MS;
    while (TIME_MS - time < timeMS)
    {
    };
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTicks(uint numTicks)
{
    uint startTicks = TIME_TICKS;
    while (TIME_TICKS - startTicks < numTicks)
    {
    };
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartLogging(void)
{
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointUS(char *)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointMS(char *)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
