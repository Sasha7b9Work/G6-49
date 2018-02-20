#include "Timer.h"
#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct
{
    pFuncVV func;       // Функция таймера
    uint dTms;          // Период срабатывания, мс
    uint timeFirstMS;   // Время первого срабатывания
    uint timeNextMS;    // Время следующего срабатывания. Если == 0xffffffff, то таймер неактивен
    bool repeat;        // Если true, будет срабатывать, пока не будет вызвана функция Timer_Disable()
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
bool Timer_IsRun(TypeTimer2 type)
{
    return TIME_NEXT(type) != MAX_UINT;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_Init(void)
{
    for(uint i = 0; i < NumTimers; i++)
    {
        timers[i].timeNextMS = MAX_UINT;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_Set(TypeTimer2 type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_SetAndStartOnce(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Timer_Set(type, func, dTms);
    Timer_StartOnce(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_SetAndEnable(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Timer_Set(type, func, dTms);
    Timer_Enable(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_StartOnce(TypeTimer2 type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_Enable(TypeTimer2 type)
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
        CPU::_TIM3_::Start(timeNext);   // то заводим таймер на наше время
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_Disable(TypeTimer2 type)
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
void Timer_PauseOnTime(uint timeMS)
{
    uint time = TIME_MS;
    while (TIME_MS - time < timeMS)
    {
    };
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_PauseOnTicks(uint numTicks)
{
    uint startTicks = TIME_TICKS;
    while (TIME_TICKS - startTicks < numTicks)
    {
    };
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer_StartLogging(void)
{
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer_LogPointUS(char *)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer_LogPointMS(char *)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
