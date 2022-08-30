// 2022/08/30 11:42:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/ClockImpulse.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    ClockImpulse::E ClockImpulse::value = ClockImpulse::_100MHz;
}


void FPGA::RecalculateImpulseRegistersTo(ClockImpulse::E clock)
{
    static const Register::E registers[4] =
    {
        Register::_5_PeriodImpulseA,
        Register::_6_DurationImpulseA,
        Register::_7_PeriodImpulseB,
        Register::_8_DurationImpulseB
    };

    if (clock == ClockImpulse::_1MHz)       // ���� 100 ���, ����� ��������� ��� �������� � 100 ���
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value /= 100;
            Register::Write(registers[i], value);
        }
    }
    else                                    // ��� 1 ���, ����� ��������� ��� �������� � 100 ���
    {
        for (int i = 0; i < 4; i++)
        {
            uint64 value = Register::Read(registers[i]);
            value *= 100;
            Register::Write(registers[i], value);
        }
    }
}
