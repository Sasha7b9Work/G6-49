#include "AD5697.h"
#include "FPGA.h"
#include "Generator_d.h"
#include "GeneratorSettingsTypes.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "FreqMeter/FreqMeter_d.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "CommonTypes.h"
#include <stm32f4xx_hal.h>


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
        static const HPort::E gpio0[Chan::Count] = { HPort::_E, HPort::_F };
        static const HPort::E gpio1[Chan::Count] = { HPort::_B, HPort::_C };

        static const uint16 pin0[Chan::Count] = { HPin::_15, HPin::_5 };
        static const uint16 pin1[Chan::Count] = { HPin::_5,  HPin::_13 };

        static const HState::E state0[Attenuation::Count] =
        {
            HState::Reset,
            HState::Set,
            HState::Reset,
            HState::Set
        };

        static const HState::E state1[Attenuation::Count] =
        {
            HState::Reset,
            HState::Reset,
            HState::Set,
            HState::Set
        };

        HAL_PIO::Write(gpio0[ch], pin0[ch], state0[attenuation]);
        HAL_PIO::Write(gpio1[ch], pin1[ch], state1[attenuation]);
    }

    static void Init()
    {
        HAL_PIO::Init(HPort::_E, HPin::_15, HMode::Output_PP, HPull::No); //-V525

        HAL_PIO::Init(HPort::_B, HPin::_10, HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_F, HPin::_0,  HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_F, HPin::_5,  HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_C, HPin::_13, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_C, HPin::_14, HMode::Output_PP, HPull::No);
    }
};


struct Amplifier
{
    static void Init()
    {
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
        static const HPort::E gpio[Chan::Count] = { HPort::_F, HPort::_C };
        static const uint16   pin[Chan::Count]  = { HPin::_0,  HPin::_14 };

        HAL_PIO::Write(gpio[ch], pin[ch], state);
    }

};


struct Filtr
{
    static void Init()
    {
        HAL_PIO::Init(HPort::_E, HPin::_12, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_E, HPin::_13, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_E, HPin::_14, HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_F, HPin::_4, HMode::Output_PP, HPull::No);
    }

    static void Tune(Chan::E ch, Form::E form)
    {
        if (form == Form::Sine)
        {
            SetType(ch, Type::Chebyshev);
        }
        else if(form == Form::Impulse || form == Form::PacketImpuls || form == Form::Meander)
        {
            SetType(ch, Type::None);
        }
        else
        {
            SetType(ch, Type::Bessel);
        }
    }

private:

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

    static void SetType(Chan::E ch, Type::E type)
    {
        /*  +-----------+---------+---------+
            |           | Канал А | Канал B |
            +-----------+----+----+----+----+
            |           |PE12|PE14|PE13|PF4 |
            |  Сигнал   | p1 | p3 | p2 | p8 |
            +-----------+----+----+----+----+
            | Синус     | X    1  | X    1  |
            +-----------+---------+---------+
            | Импульс   |         |         |
            | Пакет     | 0    0  | 0    0  |
            | Меандр    |         |         |
            +-----------+---------+---------+
            | остальные | 1    0  | 1    0  |
            +-----------+---------+---------+
        */

        static const HPort::E gpio0[Chan::Count] = { HPort::_E, HPort::_E };
        static const HPort::E gpio1[Chan::Count] = { HPort::_E, HPort::_F };

        static const uint16 pin0[Chan::Count] = { HPin::_12, HPin::_13 };
        static const uint16 pin1[Chan::Count] = { HPin::_14, HPin::_4 };
        
        //                                                   нет         Чебышев      Бессель
        static const HState::E state0[Type::Count] = { HState::Reset, HState::Set, HState::Set   };
        static const HState::E state1[Type::Count] = { HState::Reset, HState::Set, HState::Reset };

        HAL_PIO::Write(gpio0[ch], pin0[ch], state0[type]);
        HAL_PIO::Write(gpio1[ch], pin1[ch], state1[type]);
    }
};


static bool waveIsSine = true;          // Нужно для того, чтобы писать частоту в правильное место - ПЛИС или AD9952


void DGenerator::Init()
{
    EnableChannel(Chan::A, false);
    EnableChannel(Chan::B, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
    DFreqMeter::Init();
    Amplifier::Init();
    Filtr::Init();
}


void DGenerator::EnableChannel(Chan::E ch, bool enable)
{
    if(!FPGA::Start())
    {
        StructPIN pins[Chan::Count] = { {WR_OUT_A}, {WR_OUT_B} };

        HAL_PIO::Write(pins[ch].port, pins[ch].pin, enable);
    }
}



void DGenerator::SetFormWave(Chan::E ch, Form::E form)
{
    Filtr::Tune(ch, form);

    if(ch < Chan::Count && form < Form::Count)
    {
        waveIsSine = (form == Form::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}


void DGenerator::SetFrequency(Chan::E ch, ParamValue frequency)
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


void DGenerator::SetManipulationPeriod(Chan::E ch, ParamValue period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetManipulationDuration(Chan::E ch, ParamValue duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}


void DGenerator::SetPeriod(Chan::E ch, ParamValue period)
{
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetAmplitude(Chan::E ch, ParamValue amplitude)
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


void DGenerator::SetOffset(Chan::E ch, ParamValue offset)
{
    set.offset[ch] = offset;

    Amplifier::Tune(ch);

    AD5697::SetOffset(ch, offset);
}


void DGenerator::SetPhase(Chan::E ch, ParamValue phase)
{
    AD9952::SetPhase(ch, phase);
}


void DGenerator::SetPacketNumber(Chan::E, ParamValue number)
{
    FPGA::PacketImpulse::SetNumberImpules(static_cast<uint>(number.ToFloat() + 0.5F));
}


void DGenerator::SetPacketPeriod(Chan::E, ParamValue period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}


void DGenerator::SetDutyRatio(Chan::E, ParamValue)
{

}


void DGenerator::SetDuration(Chan::E ch, ParamValue value)
{
    FPGA::SetDurationImpulse(ch, value);
}


void DGenerator::SetDelay(Chan::E, ParamValue)
{
}
