#pragma once
#include "defines.h"


#define TIME_TICKS (TIM2->CNT)
#define TIME_US    (TIM2->CNT / 90)
#define TIME_MS    HAL_TIM::TimeMS()


struct Timer
{
    struct Type
    {
        enum E
        {
            StopSound,      ///< Выключить звук
            Temp,
            Count
        } value;
        Type(E v) : value(v) {};
        operator uint8() const { return static_cast<uint8>(value); };
    };

    static void Init();

    void DeInit();
    /// Назначает таймеру timer функцию и время срабатывания
    static void Set(Type type, pFuncVV func, uint dTms);

    static void SetAndStartOnce(Type type, pFuncVV func, uint dTms);

    void SetAndEnable(Type type, pFuncVV func, uint dTms);

    static void StartOnce(Type type);

    void Enable(Type type);

    void Disable(Type type);

    bool IsRun(Type type);

    static void PauseOnTime(uint timeMS);

    void PauseOnTicks(uint numTicks);
    /// Запускает счётчик для измерения малых отрезков времени
    void StartMultiMeasurement();
};
