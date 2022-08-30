// 2022/08/30 11:42:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Common/Common.h"
#include "Common/CommonTypes.h"


namespace FPGA
{
    // Опорная частота для формирования импульов и пакетов импульсов. Для длительности импульса [0 сек ... 40 сек]
    // опорная частота 100 МГц, для длительности более - 1 МГц
    namespace ClockImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        void Set(E v);
        bool Is1MHz();

        // Делитель для расчёта счётчиков длительности и периода
        int GetDivider();

        void RecalculateImpulseRegistersIfNeed(const Value duration[Chan::Count]);
    };
}
