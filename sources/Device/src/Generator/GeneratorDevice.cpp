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


struct Filtr
{
    struct Type
    {
        enum E
        {
            None,
            Chebyshev,
            Bessel,
            Count
        };
    };

    static void Init()
    {
        GPIO_InitTypeDef isGPIO;
        isGPIO.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOE, &isGPIO);

        isGPIO.Pin = GPIO_PIN_4;
        HAL_GPIO_Init(GPIOF, &isGPIO);
    }

    static void Tune(Chan::E ch, Form::E form)
    {
        if (form == Form::Sine)
        {
            SetType(ch, Type::Chebyshev);
        }
        else if (form == Form::Meander)
        {
            SetType(ch, Type::Bessel);
        }
        else
        {
            SetType(ch, Type::None);
        }
    }

private:

    static void SetType(Chan::E ch, Type::E type)
    {
        static GPIO_TypeDef *const gpio1[Chan::Count] = { GPIOE, GPIOF };

        static const uint16 pin0[Chan::Count] = { GPIO_PIN_12, GPIO_PIN_13 };
        static const uint16 pin1[Chan::Count] = { GPIO_PIN_14, GPIO_PIN_4 };

        static const GPIO_PinState state0[Type::Count] =
        {
            GPIO_PIN_SET,
            GPIO_PIN_SET,
            GPIO_PIN_RESET,
        };

        static const GPIO_PinState state1[Type::Count] =
        {
            GPIO_PIN_SET,
            GPIO_PIN_RESET,
            GPIO_PIN_RESET
        };

        HAL_GPIO_WritePin(GPIOE, pin0[ch], state0[type]);
        HAL_GPIO_WritePin(gpio1[ch], pin1[ch], state1[type]);
    }
};


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
    Filtr::Init();
}


void Generator::EnableChannel(Chan::E ch, bool enable)
{
    if(!FPGA::Start())
    {
        static const GeneratorWritePin pin[Chan::Count] = { GeneratorWritePin::Pin_OutA, GeneratorWritePin::Pin_OutB };

        CPU::WritePin(pin[ch], enable);
    }
}



void Generator::SetFormWave(Chan::E ch, Form::E form)
{
    Filtr::Tune(ch, form);

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
