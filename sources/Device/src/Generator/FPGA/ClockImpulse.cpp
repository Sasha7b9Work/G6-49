// 2022/08/30 11:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/ClockImpulse.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    static ClockImpulse::E clock = ClockImpulse::_100MHz;
}


bool FPGA::ClockImpulse::Is1MHz()
{
    return clock == _1MHz;
}


bool FPGA::ClockImpulse::Is100MHz()
{
    return clock == _100MHz;
}


int FPGA::ClockImpulse::GetDivider()
{
    return (clock == _100MHz) ? 10 : 1000;
}


void FPGA::ClockImpulse::Set(ClockImpulse::E _clock)
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


void FPGA::RecalculateImpulseRegistersTo(ClockImpulse::E _clock)
{
    static const Register::E registers[4] =
    {
        Register::_5_PeriodImpulseA,
        Register::_6_DurationImpulseA,
        Register::_7_PeriodImpulseB,
        Register::_8_DurationImpulseB
    };

    if (_clock == ClockImpulse::_1MHz)       // Ѕыло 100 ћ√ц, нужно уменьшить все значени€ в 100 раз
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value /= 100;
            Register::Write(registers[i], value);
        }
    }
    else                                    // Ѕыл 1 ћ√ц, нужно увеличить все значени€ в 100 раз
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value *= 100;
            Register::Write(registers[i], value);
        }
    }
}


void FPGA::RecalculateImpulseRegistersIfNeed(const Value duration[Chan::Count])
{
    if ((duration[ChA] > Value(40) || duration[ChB] > Value(40)) &&
        ClockImpulse::Is100MHz())
    {
        ClockImpulse::Set(ClockImpulse::_1MHz);

        RecalculateImpulseRegistersTo(ClockImpulse::_1MHz);
    }
    else if ((duration[ChA] <= Value(40) && duration[ChB] <= Value(40)) &&
        ClockImpulse::Is1MHz())
    {
        ClockImpulse::Set(ClockImpulse::_100MHz);

        RecalculateImpulseRegistersTo(ClockImpulse::_100MHz);
    }
}
