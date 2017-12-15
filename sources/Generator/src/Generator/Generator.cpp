#include "AD5697.h"
#include "FPGA.h"
#include "Common.h"
#include "Generator.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Generator generator;
static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::Init()
{
    EnableChannel(A, false);
    EnableChannel(B, false);
    ad9952.Init();
    ad5697.Init();
    fpga.Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Channel ch, bool enable)
{
    static const GeneratorPin pin[NumChannels] = { Pin_P3_OutA, Pin_P4_OutB };

    WritePin(pin[ch], enable);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Channel ch, WaveForm form)
{
    static const GeneratorPin pin[NumChannels] = {Pin_P1_AmplifierA, Pin_P2_AmplifierB};

    waveIsSine = (form == Form_Sine);

    // Если нужен синус, то пишем ноль, чтобы обеспечить прохождение сигнала DDS. Иначе 1, чтобы обеспечить прохождение сигнала ПЛИС
    WritePin(pin[ch], !waveIsSine);

    if (!waveIsSine)
    {
        fpga.SetWaveForm(form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Channel ch, CommandWrite command, float value)
{
    typedef void(Generator::*pFuncChF)(Channel, float);

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
        (this->*f)(ch, value);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFrequency(Channel ch, float frequency)
{
    if (waveIsSine)
    {
        ad9952.SetFrequency(ch, frequency);
    }
    else
    {
        fpga.SetFrequency(ch, frequency);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetAmplitude(Channel ch, float amplitude)
{
    if (waveIsSine)
    {
        ad9952.SetAmplitude(ch, amplitude);
    }
    else
    {
        ad5697.SetAmplitude(ch, amplitude);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Channel ch, float offset)
{
    ad5697.SetOffset(ch, offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPhase(Channel ch, float phase)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Channel ch, float dutyRatio)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Channel ch, float duration)
{
    fpga.SetDuration(ch, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Channel ch, float delay)
{
    fpga.SetDelay(delay);
}
