// 2022/08/30 11:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/FPGA.h"
#include "Generator/FPGA/ParametersFPGA.h"
#include <cstring>


namespace FPGA
{
    namespace Clock
    {
        namespace Impulse
        {
            static E clock = _100MHz;

            bool Is1MHz() { return clock == _1MHz; }

            static void Set(E);

            // ¬озвращает true, если хот€ бы одно значение периода либо длительности меньше value
            static bool AtLeastOneValueGreater(const Value &value);

            // ¬озращает true, если все значени€ меньше либо равны value
            static bool AllValuesLessOrEqual(const Value &value);

            // ѕодготовить массив провер€емых значений (дл€ данного режима)
            static Value **PrepareTestValues();
        }

        namespace AD992
        {
            static E clock = _100MHz;
        }

    }
}


bool FPGA::Clock::Impulse::AtLeastOneValueGreater(const Value &min)
{
    Value **values = PrepareTestValues();

    while (values++)
    {
        Value *value = *values;

        if (*value > min)
        {
            return true;
        }
    }

    return false;
}


bool FPGA::Clock::Impulse::AllValuesLessOrEqual(const Value &max)
{
    Value **values = PrepareTestValues();

    while (values++)
    {
        Value *value = *values;

        if (*value > max)
        {
            return false;
        }
    }

    return true;
}


void FPGA::Clock::Impulse::RecalculateRegistersIfNeed()
{
    if (AtLeastOneValueGreater(Value(40)) && Is100MHz())
    {
        Set(_1MHz);

        Register::RewriteImpulseRegisters();
    }
    else if (AllValuesLessOrEqual(Value(40)) && Is1MHz())
    {
        Set(_100MHz);

        Register::RewriteImpulseRegisters();
    }
}


Value **FPGA::Clock::Impulse::PrepareTestValues()
{
    static const int SIZE_BUFFER = 10;

    static Value *values[SIZE_BUFFER];

    std::memset(values, 0, SIZE_BUFFER * sizeof(values[0]));

    if (ModeWork::Current(ChA) == ModeWork::Impulse)
    {
    }

    if (ModeWork::Current(ChB) == ModeWork::Impulse)
    {
    }

    return values;
}


bool FPGA::Clock::Impulse::Is100MHz()
{
    return clock == _100MHz;
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


void FPGA::Clock::AD992::Set(E _clock)
{
    clock = _clock;
    WriteControlRegister();
}


bool FPGA::Clock::AD992::Is100MHz()
{
    return (clock == _100MHz);
}
