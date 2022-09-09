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

            // ѕереписать значени€ регистров, если необходимо
            static void RewriteRegistersIfNeed(const Chan &);

            // ≈сли при установке длительности импульса нужно измен€ть опорную частоту - переписать все значени€
            static void RewriteRegisters(const Chan &);

            static bool Is1MHz() { return clock == _1MHz; }

            static void Set(E);

            // ¬озвращает true, если хот€ бы одно значение периода либо длительности меньше value
            static bool AtLeastOneValueGreater(const Value &value);

            // ¬озращает true, если все значени€ меньше либо равны value
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

    RewriteRegistersIfNeed(ch);
}


void FPGA::Clock::Impulse::SetPeriod(const Chan &ch, const Value &_period)
{
    period[ch] = _period;

    RewriteRegistersIfNeed(ch);
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


void FPGA::Clock::Impulse::RewriteRegistersIfNeed(const Chan &ch)
{
    if (AtLeastOneValueGreater(Value(40)) && Is100MHz())
    {
        Set(_1MHz);

        RewriteRegisters(ch);
    }
    else if (AllValuesLessOrEqual(Value(40)) && Is1MHz())
    {
        Set(_100MHz);

        RewriteRegisters(ch);
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


void FPGA::Clock::Impulse::RewriteRegisters(const Chan &ch)
{
    ModeWork::E mode = ModeWork::Current(ch);

    switch (mode)
    {
    case ModeWork::Impulse:
        break;

    case ModeWork::PackedImpulse:
        break;
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
