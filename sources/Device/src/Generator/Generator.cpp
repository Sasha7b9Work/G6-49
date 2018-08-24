#include "AD5697.h"
#include "FPGA.h"
#include "Generator.h"
#include "Multiplexor.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"


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
    Multiplexor::Init();
    FreqMeter::Init();

    for(int i = 0; i < 2; i++)
    {
        Chan ch = (Chan::E)i;
        Generator::EnableChannel(ch, true);
        Generator::SetFormWave(ch, WaveForm::Sine);
        Generator::SetFrequency(ch, 2000.0f);
        Generator::SetAmplitude(ch, 4096.0f);
        Generator::SetOffset(ch, 2048.0f);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Chan ch, bool enable)
{
    static const GeneratorWritePin pin[Chan::Number] = { GeneratorWritePin::Pin_P3_OutA, GeneratorWritePin::Pin_P4_OutB };

    CPU::WritePin(pin[ch], !enable);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, WaveForm form)
{
    if(ch < Chan::Number && form < WaveForm::Number)
    {
        static const GeneratorWritePin pin[Chan::Number] = { GeneratorWritePin::Pin_P1_AmplifierA, GeneratorWritePin::Pin_P2_AmplifierB };

        waveIsSine = form == WaveForm::Sine;

        // Если нужен синус, то пишем ноль, чтобы обеспечить прохождение сигнала DDS. Иначе 1, чтобы обеспечить прохождение сигнала ПЛИС
        CPU::WritePin(pin[ch], !waveIsSine);

        if (!waveIsSine)
        {
            FPGA::SetWaveForm(form);
        }

        Multiplexor::SetMode(ch, form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Chan ch, CommandPanel command, float value)
{
    typedef void (*pFuncChF)(Chan, float);

    static const pFuncChF func[CommandPanel::Number] =
    {
        0,
        0,
        0,
        SetFrequency,
        SetAmplitude,
        SetOffset,
        SetDuration,
        SetDutyRatio,
        SetPhase,
        0,
        0,
        SetDelay,
        0
    };

    pFuncChF f = func[command];

    if (f)
    {
        (f)(ch, value);
    }
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
void Generator::SetAmplitude(Chan ch, float amplitude)
{
    if (waveIsSine)
    {
        AD9952::SetAmplitude(ch, amplitude);
    }
    else
    {
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Chan ch, float offset)
{
    AD5697::SetOffset(ch, offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetPhase(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Chan ch, float duration)
{
    FPGA::SetDuration(ch, duration);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Chan, float delay)
{
    FPGA::SetDelay(delay);
}
