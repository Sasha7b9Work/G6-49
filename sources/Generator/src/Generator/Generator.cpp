#include "AD5697.h"
#include "FPGA.h"
#include "Generator.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::Init()
{
    EnableChannel(A, false);
    EnableChannel(B, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Channel ch, bool enable)
{
    static const GeneratorPin pin[NumChannels] = { Pin_P3_OutA, Pin_P4_OutB };

    CPU::WritePin(pin[ch], enable);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Channel ch, Type_WaveForm form)
{
    static const GeneratorPin pin[NumChannels] = {Pin_P1_AmplifierA, Pin_P2_AmplifierB};

    waveIsSine = (form == Sine);

    // Если нужен синус, то пишем ноль, чтобы обеспечить прохождение сигнала DDS. Иначе 1, чтобы обеспечить прохождение сигнала ПЛИС
    CPU::WritePin(pin[ch], !waveIsSine);

    if (!waveIsSine)
    {
        FPGA::SetWaveForm(form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Channel ch, CommandWrite command, float value)
{
    typedef void (*pFuncChF)(Channel, float);

    static const pFuncChF func[NUM_COMMAND_WRITE] =
    {
        0,
        0,
        0,
        &Generator::SetFrequency,
        &Generator::SetAmplitude,
        &Generator::SetOffset,
        &Generator::SetDuration,
        &Generator::SetDutyRatio,
        &Generator::SetPhase,
        0,
        0,
        &Generator::SetDelay
    };

    pFuncChF f = func[command];

    if (f)
    {
        (f)(ch, value);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFrequency(Channel ch, float frequency)
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
void Generator::SetAmplitude(Channel ch, float amplitude)
{
    if (waveIsSine)
    {
        AD9952::SetAmplitude(ch, amplitude);
    }
    else
    {
        AD5697::SetAmplitude(ch, amplitude);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Channel ch, float offset)
{
    AD5697::SetOffset(ch, offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPhase(Channel, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Channel, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Channel ch, float duration)
{
    FPGA::SetDuration(ch, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Channel, float delay)
{
    FPGA::SetDelay(delay);
}
