// 2022/08/30 11:42:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Common/Common.h"
#include "Common/CommonTypes.h"


namespace FPGA
{
    namespace Clock
    {
        // Опорная частота для формирования импульов и пакетов импульсов. Для длительности импульса [0 сек ... 40 сек]
        // опорная частота 100 МГц, для длительности более - 1 МГц
        namespace Impulse
        {
            bool Is100MHz();

            // Делитель для расчёта счётчиков длительности и периода
            int GetDivider();

            // При установке длительности импульса нужно вызывать эту функцию
            void SetDuration(const Chan &, const Value &);

            // При установке периода импульса нужно вызывать эту функцию
            void SetPeriod(const Chan &, const Value &);
        }
    }

    // Тактовая частота формирователя произвольных сигналов
    namespace ClockAD992
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        void Set(E);

        E Get();
    };
}
