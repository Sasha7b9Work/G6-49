// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


struct TimerStruct
{
    pFuncVV func;       // ������� �������
    uint    dTms;          // ������ ������������, ��
    uint    timeNextMS;    // ����� ���������� ������������. ���� == 0xffffffff, �� ������ ���������
    bool    repeat;        // ���� true, ����� �����������, ���� �� ����� ������� ������� Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};



static TimerStruct timers[Timer::Type::Count];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;



#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)



// ������� �����, ������� ����������� � timeStop ��
static void StartTIM(uint timeStop);

static void StopTIM();
// ���������� ����� ������������ ���������� �������, ���� 0, ���� �������� ���
static uint NearestTime();
// ��������� ������� �� ������
static void TuneTIM(Timer::Type type);


bool Timer::IsRun(Type type)
{
    return TIME_NEXT(type) != UINT_MAX;
}


void Timer::Init()
{
    for(uint i = 0; i < Timer::Type::Count; i++)
    {
        timers[i].timeNextMS = UINT_MAX;
    }

    HAL_TIM2::Init();
    HAL_TIM3::Init();
}


void Timer::ElapsedCallback()
{
    uint time = TIME_MS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < Timer::Type::Count; type++)
    {
        if (TIME_NEXT(type) <= time)            // ���� ������ ����� ������������
        {
            TimerStruct *timer = &timers[type];
            timer->func();
            if (timer->repeat)
            {
                do      // ���� ����� ������, ��� ��������� ������ SysTick, ������� ����������� ������������, ����� ��������� ���������,
                {       // � ���� ������� ����������� ������, ��� timer->dTm ��, �� ��� ��� ��������
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < TIME_MS);

            }
            else
            {
                timer->timeNextMS = UINT_MAX;
            }
        }
    }

    StartTIM(NearestTime());
}


void Timer::Set(Type type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}


void Timer::SetAndStartOnce(Type type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}


void Timer::SetAndEnable(Type type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}


void Timer::StartOnce(Type type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}


void Timer::Enable(Type type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}


static void TuneTIM(Timer::Type type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = TIME_MS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // ���� ������ ������ ��������� ������ ��������
    {
        StartTIM(timeNext);         // �� ������� ������ �� ���� �����
    }
}


void Timer::Disable(Type type)
{
    timers[type].timeNextMS = UINT_MAX;
    timers[type].repeat = false;
}


static uint NearestTime()
{
    uint time = UINT_MAX;

    for(uint type = 0; type < Timer::Type::Count; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}


static void StartTIM(uint timeStopMS)
{
    StopTIM();

    if(timeStopMS == UINT_MAX)
    {
        return;
    }

    uint dT = timeStopMS - TIME_MS;

    // 10 ������������� 0.1��. �.�. ���� ��� ����� 1��, ����� �������� (100 - 1)
    HAL_TIM3::StartIT((dT * 2) - 1);
}


static void StopTIM()
{
    HAL_TIM3::StopIT();
}


void Timer::PauseOnTicks(uint numTicks)
{
    volatile uint startTicks = TIME_TICKS;
    while (TIME_TICKS - startTicks < numTicks)
    {
    };
}


void Timer::StartLogging()
{
    timeStartLogging = TIME_TICKS;
    timePrevPoint = timeStartLogging;
}


uint Timer::LogPointUS(char * /*name*/)
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}


uint Timer::LogPointMS(char * /*name*/) //-V524
{
    uint interval = TIME_TICKS - timePrevPoint;
    timePrevPoint = TIME_TICKS;
    return interval;
}


void TimeMeterMS::Reset()
{
    time_reset = TIME_MS;
}


uint TimeMeterMS::ElapsedTime()
{
    return TIME_MS - time_reset;
}

#undef TIME_NEXT
