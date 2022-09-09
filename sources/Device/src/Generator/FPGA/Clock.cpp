// 2022/08/30 11:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    namespace Clock
    {
        namespace Impulse
        {
            enum E
            {
                _100MHz,
                _1MHz
            };

            static E clock = _100MHz;

            static Value duration[Chan::Count] = { Value(0), Value(0) };
            static Value period[Chan::Count] = { Value(0), Value(0) };

            // Пересчитать значения импульсных регистров (длительность, период), если нужно
            static void RecalculateImpulseRegistersIfNeed();

            // Если при установке длительности импульса нужно изменять опорную частоту - пересчитать все остальные значения:
            // период импульса, период пакета, задержка между каналами.
            // Пересчёт производится в пересчёте на то, что опорная частота раньше была не clock, а теперь стала clock
            static void RecalculateImpulseRegistersTo(E clock);

            static bool Is1MHz() { return clock == _1MHz; }

            static void Set(E);

            // Возвращает true, если хотя бы одно значение периода либо длительности меньше value
            static bool AtLeastOneValueGreater(const Value &value);

            // Возращает true, если все значения меньше либо равны value
            static bool AllValuesLessOrEqual(const Value &value);
        }

        namespace AD992
        {
            static E clock = _100MHz;
        }

    }
}


void FPGA::Clock::Impulse::SetDuration(const Chan &ch, const Value &_duration)
{
    duration[ch] = _duration;

    RecalculateImpulseRegistersIfNeed();
}


void FPGA::Clock::Impulse::SetPeriod(const Chan &ch, const Value &_period)
{
    period[ch] = _period;

    RecalculateImpulseRegistersIfNeed();
}


bool FPGA::Clock::Impulse::AtLeastOneValueGreater(const Value &value)
{
    static const Value *values[4] = { &duration[ChA], &duration[ChB], &period[ChA], &period[ChB] };

    for (int i = 0; i < 4; i++)
    {
        if (*values[i] > value)
        {
            return true;
        }
    }

    return false;
}


bool FPGA::Clock::Impulse::AllValuesLessOrEqual(const Value &value)
{
    static const Value *values[4] = { &duration[ChA], &duration[ChB], &period[ChA], &period[ChB] };

    for (int i = 0; i < 4; i++)
    {
        if (*values[i] > value)
        {
            return false;
        }
    }

    return true;
}


void FPGA::Clock::Impulse::RecalculateImpulseRegistersIfNeed()
{
    if (AtLeastOneValueGreater(Value(40)) && Is100MHz())
    {
        Set(_1MHz);

        RecalculateImpulseRegistersTo(_1MHz);
    }
    else if (AllValuesLessOrEqual(Value(40)) && Is1MHz())
    {
        Set(_100MHz);

        RecalculateImpulseRegistersTo(_100MHz);
    }
}


bool FPGA::Clock::Impulse::Is100MHz()
{
    return clock == _100MHz;
}


int FPGA::Clock::Impulse::GetDivider()
{
    return (clock == _100MHz) ? 10 : 1000;
}


void FPGA::Clock::Impulse::Set(E _clock)
{
    clock = _clock;

    uint64 reg = Register::Read(Register::_0_Control);

    if (clock == _100MHz)
    {
        _CLEAR_BIT(reg, RG0::_4_ClockImpulse);
    }
    else
    {
        _SET_BIT(reg, RG0::_4_ClockImpulse);
    }

    Register::Write(Register::_0_Control, reg);
}


void FPGA::Clock::Impulse::RecalculateImpulseRegistersTo(E _clock)
{
    static const Register::E registers[4] =
    {
        Register::_5_PerImp_Freq_A,
        Register::_6_DurationImpulseA,
        Register::_7_PeriodImpulseB,
        Register::_8_DurationImpulseB
    };

    if (_clock == _1MHz)       // Было 100 МГц, нужно уменьшить все значения в 100 раз
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value /= 100;
            Register::Write(registers[i], value);
        }
    }
    else                                    // Был 1 МГц, нужно увеличить все значения в 100 раз
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value *= 100;
            Register::Write(registers[i], value);
        }
    }
}


void FPGA::Clock::AD992::Set(E _clock)
{
    clock = _clock;
    WriteControlRegister();
}


bool FPGA::Clock::AD992::Is100MHz()
{
    return (clock == _100MHz);
}
