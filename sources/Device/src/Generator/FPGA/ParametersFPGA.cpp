// 2022/09/09 09:22:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/ParametersFPGA.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/RegistersFPGA.h"


void FPGA::Param::Packet::Period::Set(const Value &period)
{
    Clock::Impulse::SetPeriod(ChA, period);

    uint64 value = period.ToUINT64() / Clock::Impulse::GetDivider();

    Register::Write(Register::_5_PerImp_Freq_A_PerPack, value);
}
