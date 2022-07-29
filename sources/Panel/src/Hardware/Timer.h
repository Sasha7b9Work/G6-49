// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define _TIME_TICKS HAL_TIM2::GetTicks()
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
        operator uint8() const { return static_cast<uint8>(value); };
    };

    void Init();

    void DeInit();

    // Назначает таймеру timer функцию и время срабатывания
    void Set(Type type, pFuncVV func, uint dTms);

    void SetAndStartOnce(Type type, pFuncVV func, uint dTms);

    void SetAndEnable(Type type, pFuncVV func, uint dTms);

    void StartOnce(Type type);

    void Enable(Type type);

    void Disable(Type type);

    bool IsRun(Type type);

    void PauseOnTicks(uint numTicks);

    // Вызывается при срабатывании таймера
    void ElapsedCallback();
};
