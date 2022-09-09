// 2022/09/09 09:22:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/ParametersFPGA.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/RegistersFPGA.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    namespace Packet
    {
        namespace Period
        {
            static Value current(1.0);
        }
    }

    namespace Impulse
    {
        namespace Duration
        {
            static Value current[Chan::Count] = { Value(1e-3), Value(1e-3) };

            const Value &Gurrent(const Chan &ch) { return current[ch]; }
        }
    }
}


void FPGA::Packet::Period::Set(const Value &period)
{
    current = period;

    Clock::Impulse::SetPeriod(ChA, period);

    uint64 value = period.ToUINT64() / Clock::Impulse::GetDivider();

    Register::Write(Register::_5_PerImp_Freq_A_PerPack, value);
}


void FPGA::Impulse::Duration::Set(const Chan &ch, const Value &duration)
{
    current[ch] = duration;

    Register::E reg = ch.IsA() ? Register::_6_DurImp_A_NumbImp : Register::_8_DurImp_B;

    if (ch.IsA() && (ModeWork::Current(ChA) == ModeWork::PackedImpulse))
    {
        reg = Register::_8_DurImp_B;
    }

    Clock::Impulse::SetDuration(ch, duration);

    uint64 value = duration.ToUINT64() / Clock::Impulse::GetDivider();

    Register::Write(reg, value);
}
