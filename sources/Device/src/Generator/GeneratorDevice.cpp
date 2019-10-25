#include "stdafx.h"
#ifndef WIN32
#include "AD5697.h"
#include "FPGA.h"
#include "GeneratorDevice.h"
#include "GeneratorSettingsTypes.h"
#include "Generator/Amplifier.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "CommonTypes.h"
#endif



static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952



void Generator::Init()
{
    EnableChannel(Chan::A, false);
    EnableChannel(Chan::B, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
    FreqMeter::Init();
    Amplifier::Init();
}


void Generator::EnableChannel(Chan::E ch, bool enable)
{
    if(!FPGA::Start())
    {
        static const GeneratorWritePin pin[Chan::Count] = { GeneratorWritePin::Pin_P3_OutA, GeneratorWritePin::Pin_P4_OutB };

        CPU::WritePin(pin[ch], !enable);
    }
}



void Generator::SetFormWave(Chan::E ch, Form::E form)
{
    if(ch < Chan::Count && form < Form::Count)
    {
        waveIsSine = (form == Form::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}


void Generator::SetFrequency(Chan::E ch, ParamValue frequency)
{
    if (waveIsSine)
    {
        AD9952::SetFrequency(ch, frequency);
    }
    else
    {
        FPGA::SetFrequency(ch, frequency);
    }
}


void Generator::SetManipulationPeriod(Chan::E ch, ParamValue period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}


void Generator::SetManipulationDuration(Chan::E ch, ParamValue duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}


void Generator::SetPeriod(Chan::E ch, ParamValue period)
{
    FPGA::SetPeriodImpulse(ch, period);
}


void Generator::SetAmplitude(Chan::E ch, ParamValue amplitude)
{
    Amplifier::Tune(ch);

    if (waveIsSine)
    {
        AD9952::SetAmplitude(ch, amplitude);
    }
    else
    {
        FPGA::SetAmplitude(ch, amplitude);
    }
}


void Generator::SetOffset(Chan::E ch, ParamValue offset)
{
    set.offset[ch] = offset;

    Amplifier::Tune(ch);

    if(waveIsSine)
    {
        AD5697::SetOffset(ch, offset);
    }
    else
    {
        AD5697::SetOffset(ch, ParamValue(0.0F));
        FPGA::SetOffset(ch, offset);
    }
}


void Generator::SetPhase(Chan::E ch, ParamValue phase)
{
    AD9952::SetPhase(ch, phase);
}


void Generator::SetPacketNumber(Chan::E, ParamValue number)
{
    FPGA::PacketImpulse::SetNumberImpules(static_cast<uint>(number.ToFloat() + 0.5F));
}


void Generator::SetPacketPeriod(Chan::E, ParamValue period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}


void Generator::SetDutyRatio(Chan::E, ParamValue)
{

}


void Generator::SetDuration(Chan::E ch, ParamValue value)
{
    FPGA::SetDurationImpulse(ch, value);
}


void Generator::SetDelay(Chan::E, ParamValue)
{
}
