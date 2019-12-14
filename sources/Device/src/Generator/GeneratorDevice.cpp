#include "stdafx.h"
#ifndef WIN32
#include "AD5697.h"
#include "FPGA.h"
#include "GeneratorDevice.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "CommonTypes.h"
#endif


struct Attenuation
{
    enum E
    {
        _0Db,
        _10Db,
        _20Db,
        _30Db,
        Count
    };
};


struct Attenuator
{
    static void SetAttenuation(Chan::E ch, Attenuation::E attenuation)
    {
        static GPIO_TypeDef *const gpio0[Chan::Count] = { GPIOE, GPIOF };
        static GPIO_TypeDef *const gpio1[Chan::Count] = { GPIOB, GPIOC };

        static const uint16 pin0[Chan::Count] = { GPIO_PIN_15, GPIO_PIN_5 };
        static const uint16 pin1[Chan::Count] = { GPIO_PIN_5, GPIO_PIN_13 };

        static const GPIO_PinState state0[Attenuation::Count] =
        {
            GPIO_PIN_RESET,
            GPIO_PIN_SET,
            GPIO_PIN_RESET,
            GPIO_PIN_SET
        };

        static const GPIO_PinState state1[Attenuation::Count] =
        {
            GPIO_PIN_RESET,
            GPIO_PIN_RESET,
            GPIO_PIN_SET,
            GPIO_PIN_SET
        };

        HAL_GPIO_WritePin(gpio0[ch], pin0[ch], state0[attenuation]);
        HAL_GPIO_WritePin(gpio1[ch], pin1[ch], state1[attenuation]);
    }

    static void Init()
    {
        GPIO_InitTypeDef isGPIO;
        isGPIO.Pin = GPIO_PIN_15;
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOE, &isGPIO);

        isGPIO.Pin = GPIO_PIN_10;
        HAL_GPIO_Init(GPIOB, &isGPIO);

        isGPIO.Pin = GPIO_PIN_5;
        HAL_GPIO_Init(GPIOF, &isGPIO);

        isGPIO.Pin = GPIO_PIN_13;
        HAL_GPIO_Init(GPIOC, &isGPIO);
    }
};


struct Amplifier
{
    static void Init()
    {
        GPIO_InitTypeDef isGPIO;
        isGPIO.Pin = GPIO_PIN_0;
        isGPIO.Mode = GPIO_MODE_OUTPUT_PP;
        isGPIO.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOF, &isGPIO);

        isGPIO.Pin = GPIO_PIN_14;
        HAL_GPIO_Init(GPIOC, &isGPIO);

        Attenuator::Init();
    }


    static void Tune(Chan::E ch)
    {
        float amplitude = set.amplitude[ch].ToFloat();

        if (amplitude > 3.16F)              // 1 диапазон
        {
            SetState(ch, true);
            Attenuator::SetAttenuation(ch, Attenuation::_0Db);
        }
        else if (amplitude > 1.0F)          // 2 диапазон
        {
            SetState(ch, true);
            Attenuator::SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > 0.316F)        // 3,4 диапазоны
        {
            if (set.offset[ch].ToFloat() > 2.5F)
            {
                SetState(ch, true);
                Attenuator::SetAttenuation(ch, Attenuation::_20Db);
            }
            else
            {
                SetState(ch, false);
                Attenuator::SetAttenuation(ch, Attenuation::_0Db);
            }
        }
        else if (amplitude > 0.100F)        // 5 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > 0.0316F)        // 6 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_20Db);
        }
        else                                // 7 диапазон
        {
            SetState(ch, false);
            Attenuator::SetAttenuation(ch, Attenuation::_30Db);
        }
    }


    static void SetState(Chan::E ch, bool state)
    {
        static GPIO_TypeDef *const gpio[Chan::Count] = { GPIOF, GPIOC };
        static const uint16         pin[Chan::Count] = { GPIO_PIN_0, GPIO_PIN_14 };

        HAL_GPIO_WritePin(gpio[ch], pin[ch], state ? GPIO_PIN_SET : GPIO_PIN_RESET);
    }

};


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
            GPIO_PIN_RESET,
            GPIO_PIN_SET,
            GPIO_PIN_SET
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
