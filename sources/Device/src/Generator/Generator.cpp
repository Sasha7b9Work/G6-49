#include "AD5697.h"
#include "FPGA.h"
#include "Generator.h"
#include "Multiplexor.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952
Generator::SourceManipulation Generator::sourceManipulation[Chan::Number] = {Generator::SourceManipulation::None, Generator::SourceManipulation::None};


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
    
    /*
    for(int i = 0; i < 2; i++)
    {
        Chan ch = (Chan::E)i;
        EnableChannel(ch, true);
        SetFormWave(ch, Wave::Form::Sine);
        SetFrequency(ch, 1e6f);
        SetAmplitude(ch, 5.0f);
        if(ch == Chan::A)
        {
            AD9952::Shape::SetEnabled(ch, true);
        }
        //AD9952::Shape::SetDuration(ch, 100.0f);
        //AD9952::Shape::SetAmplitude(ch, 100.0f);
    }
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EnableChannel(Chan ch, bool enable)
{
    static const GeneratorWritePin pin[Chan::Number] = { GeneratorWritePin::Pin_P3_OutA, GeneratorWritePin::Pin_P4_OutB };

    CPU::WritePin(pin[ch], !enable);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, Wave::Form form)
{
    if(ch < Chan::Number && form < Wave::Form::Number)
    {
        waveIsSine = form == Wave::Form::Sine;

        if (!waveIsSine)
        {
            FPGA::SetWaveForm(ch, form);
        }

        Multiplexor::SetMode(ch, form);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Chan ch, CommandPanel command, float value)
{
    static const struct StructFunc
    {
        typedef void (*pFuncChF)(Chan, float);
        pFuncChF func;
        StructFunc(pFuncChF f) : func(f) {};
    }
    func[CommandPanel::Number] =
    {
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        SetFrequency,
        SetAmplitude,
        SetOffset,
        SetDuration,
        SetDutyRatio,
        SetPhase,
        EmptyFunc,
        EmptyFunc,
        SetDelay,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc,
        EmptyFunc
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
void Generator::SetPhase(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDutyRatio(Chan, float)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDuration(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDelay(Chan, float)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EmptyFunc(Chan, float)
{
}
