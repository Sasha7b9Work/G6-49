#include "stdafx.h"
#ifndef WIN32
#include "AD5697.h"
#include "FPGA.h"
#include "GeneratorDevice.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU/CPU.h"
#include "FreqMeter/FreqMeter.h"
#include "Utils/Console.h"
#include "CommonTypes.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::Init()
{
    EnableChannel(Chan::A, false);
    EnableChannel(Chan::B, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
    FreqMeter::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Chan ch, bool enable)
{
    if(!FPGA::Start())
    {
        static const GeneratorWritePin pin[Chan::Number] = { GeneratorWritePin::Pin_P3_OutA, GeneratorWritePin::Pin_P4_OutB };

        CPU::WritePin(pin[ch], !enable);
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, Form form)
{
    if(ch.value < Chan::Number && form.value < Form::Number)
    {
        waveIsSine = form.Is(Form::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Chan ch, Command command, float value)
{
    static const struct StructFunc
    {
        typedef void (*pFuncChF)(Chan, float);
        pFuncChF func;
        StructFunc(pFuncChF f) : func(f) {};
    }
    func[Command::Number] =
    {
/* RequestData             */ EmptyFunc,
/* EnableChannel           */ EmptyFunc,
/* SetFormWave             */ EmptyFunc,
/* SetFrequency            */ SetFrequency,
/* SetAmplitude            */ SetAmplitude,
/* SetOffset               */ SetOffset,
/* SetDuration             */ SetDuration,
/* SetDutyRatio            */ SetDutyRatio,
/* SetPhase                */ SetPhase,
/* RunReset                */ EmptyFunc,
/* ModeDebug               */ EmptyFunc,
/* SetDelay                */ SetDelay,
/* WriteRegister           */ EmptyFunc,
/* SetDurationRise         */ EmptyFunc,
/* SetDurationFall         */ EmptyFunc,
/* SetDurationStady        */ EmptyFunc,
/* SetDutyFactor           */ EmptyFunc,
/* SetManipulation         */ EmptyFunc,
/* SetManipulationDuration */ SetManipulationDuration,
/* SetManipulationPeriod   */ SetManipulationPeriod,
/* SetPacketPeriod         */ SetPacketPeriod,
/* SetPacketNumber         */ SetPacketNumber,
/* SetStartMode            */ EmptyFunc,
/* SetPeriod               */ SetPeriod,
/* SetPolarity             */ EmptyFunc,
/* SetManipulationMode     */ EmptyFunc,
/* LoadFromDDS             */ EmptyFunc,
/* FreqMeasure             */ EmptyFunc,
/* Log                     */ EmptyFunc,
/* FDrive_NumDirsAndFiles  */ EmptyFunc,
/* FDrive_Mount            */ EmptyFunc,
/* FDrive_RequestDir       */ EmptyFunc,
/* FDrive_RequestFile      */ EmptyFunc
    };

    func[command].func(ch, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFrequency(Chan ch, float frequency)
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetManipulationPeriod(Chan ch, float period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetManipulationDuration(Chan ch, float duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPeriod(Chan ch, float period)
{
    FPGA::SetPeriodImpulse(ch, period);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetAmplitude(Chan ch, float amplitude)
{
    if (waveIsSine)
    {
        AD9952::SetAmplitude(ch, amplitude);
    }
    else
    {
        FPGA::SetAmplitude(ch, amplitude);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Chan ch, float offset)
{
    if(waveIsSine)
    {
        AD5697::SetOffset(ch, offset);
    }
    else
    {
        FPGA::SetOffset(ch, offset);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPhase(Chan ch, float phase)
{
    AD9952::SetPhase(ch, phase);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPacketNumber(Chan, float number)
{
    FPGA::PacketImpulse::SetNumberImpules((uint)(number + 0.5f));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPacketPeriod(Chan, float period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Chan ch, float value)
{
    FPGA::SetDurationImpulse(ch, value);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EmptyFunc(Chan, float)
{
}
