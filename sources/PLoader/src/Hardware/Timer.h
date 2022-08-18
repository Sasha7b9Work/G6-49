// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/HAL/HAL.h"


#define _TIME_TICKS HAL_TIM2::GetTicks()
#define _TIME_US    HAL_TIM2::TimeUS()
#define _TIME_MS    HAL_TIM::TimeMS()


namespace Timer
{
    struct Type
    {
        enum E
        {
            StopSound,      // Выключить звук
            Temp,
            Count
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    void Init();

    void DeInit();

    // Назначает таймеру timer функцию и время срабатывания
    void Set(Type, pFuncVV, uint dTms);

    void SetAndStartOnce(Type, pFuncVV, uint dTms);

    void SetAndEnable(Type, pFuncVV, uint dTms);

    void StartOnce(Type);

    void Enable(Type);

    void Disable(Type);

    bool IsRun(Type);

    void PauseOnTicks(uint numTicks);

    // Вызывается при срабатывании таймера
    void ElapsedCallback();
};


struct TimeMeterMS
{
    TimeMeterMS() { Reset(); }
    void Reset() { time_reset = _TIME_MS; }
    uint ElapsedTime() { return _TIME_MS - time_reset; }
private:
    uint time_reset;
};
