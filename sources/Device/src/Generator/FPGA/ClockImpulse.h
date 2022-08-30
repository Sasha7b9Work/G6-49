// 2022/08/30 11:42:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace FPGA
{
    // Опорная частота для формирования импульов и пакетов импульсов. Для длительности импульса [0 сек ... 40 сек]
    // опорная частота 100 МГц, для длительности более - 1 МГц
    struct ClockImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        static void Set(E v);
        static bool Is1MHz() { return value == _1MHz; }
        static bool Is100MHz() { return value == _100MHz; }
        static E Get() { return value; };
    private:
        static E value;
    };

    // Если при установке длительности импульса нужно изменять опорную частоту - пересчитать все остальные значения:
    // период импульса, период пакета, задержка между каналами.
    // Пересчёт производится в пересчёте на то, что опорная частота раньше была не clock, а теперь стала clock
    void RecalculateImpulseRegistersTo(ClockImpulse::E clock);
}
