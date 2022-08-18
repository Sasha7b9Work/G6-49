// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Common.h"
#include "common/CommonTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"


namespace AD9952
{
    void Init();

    // При установке формы сигнала "меандр" нужно вызывать эту функцию, чтобы при нулевой амплитуде в синусе меандр поступал-таки на выход
    void SetAmplitudeForMeander(const Chan &);

    void SetAmplitude(const Chan &);

    void SetFrequency(const Chan &);

    void SetPhase(const Chan &, Value phase);

    // Манипуляция
    namespace Manipulation
    {
        // True означает, что манипуляция включена
        extern bool enabled[Chan::Count];

        // Установить/отменить модулирование синусоиды сигналом "пилы"
        void SetEnabled(const Chan &, bool enable);

        inline bool IsEnabled(const Chan &ch) { return enabled[ch]; };
    };
};
