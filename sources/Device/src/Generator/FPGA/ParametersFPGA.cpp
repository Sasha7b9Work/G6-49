// 2022/09/09 09:22:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/FPGA/ParametersFPGA.h"
#include "Generator/FPGA/Clock.h"
#include "Generator/FPGA/RegistersFPGA.h"
#include "Generator/FPGA/FPGA.h"


namespace FPGA
{
    // –ежим запуска дл€ произвольного сигнала (0) и дл€ импульсного сигнала (1)
    static StartMode::E startMode[Chan::Count][2] = { { StartMode::Auto, StartMode::Auto }, { StartMode::Auto, StartMode::Auto } };

    StartMode::E Current(const Chan &ch, int type_signal)
    {
        return startMode[ch][type_signal];
    }

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

        namespace Period
        {
            static Value current[Chan::Count] = { Value(1e-2), Value(1e-2) };

            const Value &Current(const Chan &ch) { return current[ch]; }
        }
    }

    namespace StartStop
    {
        namespace Mode
        {
            static E current = Disable;

            E Current() { return current; }
        }

        namespace Delay
        {
            static Value current(1e-3);
        }
    }
}


void FPGA::Packet::Period::Set(const Value &period)
{
    current = period;

    Clock::Impulse::RecalculateRegistersIfNeed();

    Register::Write(Register::_5_PerImp_Freq_A_PerPack,
        current.GetRawValue() / Clock::Impulse::GetDivider100MHz(),
        current.GetRawValue() / Clock::Impulse::GetDivider1MHz()
    );
}


Value &FPGA::Packet::Period::Current()
{
    return current;
}


void FPGA::Impulse::Duration::Set(const Chan &ch, const Value &duration)
{
    current[ch] = duration;

    Register::E reg = ch.IsA() ? Register::_6_DurImp_A_NumbImp : Register::_8_DurImp_B;

    if (ch.IsA() && (ModeWork::Current(ChA) == ModeWork::PackedImpulse))
    {
        reg = Register::_8_DurImp_B;
    }

    // ѕри установке длительности импульса пересчитывать опорную частоту не нужно, потому что длительность всегда меньше периода

    Register::Write(reg,
        current[ch].GetRawValue() / Clock::Impulse::GetDivider100MHz(),
        current[ch].GetRawValue() / Clock::Impulse::GetDivider1MHz()
    );
}


const Value &FPGA::Impulse::Duration::Current(const Chan &ch)
{
    return current[ch];
}


void FPGA::Packet::Number::Set(const uint value)
{
    uint64 n = (uint64)(((value - 1) * Impulse::Period::Current(ChA).ToDouble() + Impulse::Duration::Gurrent(ChA).ToDouble()) / 10E-9);

    Register::Write(Register::_6_DurImp_A_NumbImp, n, n);
}


void FPGA::Impulse::Period::Set(const Chan &ch, const Value &period)
{
    current[ch] = period;

    Register::E reg = ch.IsA() ? Register::_5_PerImp_Freq_A_PerPack : Register::_7_PerImp_Freq_B_DelayStartStop;

    if (ch.IsA() && (ModeWork::Current(ChA) == ModeWork::PackedImpulse))
    {
        reg = Register::_7_PerImp_Freq_B_DelayStartStop;
    }

    Clock::Impulse::RecalculateRegistersIfNeed();

    Register::Write(reg,
        current[ch].GetRawValue() / Clock::Impulse::GetDivider100MHz() - 2,
        current[ch].GetRawValue() / Clock::Impulse::GetDivider1MHz() - 2
    );
}


void FPGA::StartStop::Delay::Set(const Value &delay)
{
    current = delay;

    // ѕри изменении задержки пересчитывать опорную частоту не нужно, потому что задержка не может быть больше периода

    Register::Write(Register::_7_PerImp_Freq_B_DelayStartStop,
        current.GetRawValue() / Clock::Impulse::GetDivider100MHz() - 2,
        current.GetRawValue() / Clock::Impulse::GetDivider1MHz() - 2
    );
}


Value &FPGA::StartStop::Delay::Current()
{
    return current;
}


void FPGA::StartStop::Mode::Set(E mode)
{
    current = mode;
    WriteControlRegister();
}


void FPGA::SetStartMode(const Chan &ch, uint8 signal, StartMode::E mode)
{
    startMode[ch][signal] = mode;
    WriteControlRegister();
}
