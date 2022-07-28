// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Log.h"
#include "Generator/Generator_d.h"
#include "FreqMeter/FreqMeter_d.h"
#include <cmath>


bool SettingsGenerator::waveIsSine[Chan::Count] = { true, true };
Value SettingsGenerator::amplitude[Chan::Count] = { Value("10", Order::One), Value("10", Order::One) };
Value SettingsGenerator::frequency[Chan::Count] = { Value("1000", Order::One), Value("1000", Order::One) };
Value SettingsGenerator::offset[Chan::Count] = { Value("0", Order::One), Value("0", Order::One) };
Attenuation::E Amplifier::attenuation[Chan::Count] = { Attenuation::_0Db, Attenuation::_0Db };
bool Amplifier::isEnabled[Chan::Count] = { false, false };
bool Amplifier::locked = false;


struct Filtr
{
    static void Init()
    {
        HAL_PIO::Init(HPort::_E, HPin::_12, HMode::Output_PP, HPull::No); //-V525
        HAL_PIO::Init(HPort::_E, HPin::_13, HMode::Output_PP, HPull::No);
        HAL_PIO::Init(HPort::_E, HPin::_14, HMode::Output_PP, HPull::No);

        HAL_PIO::Init(HPort::_F, HPin::_4, HMode::Output_PP, HPull::No);
    }

    static void Tune(Chan::E ch, TypeForm::E form)
    {
        if (form == TypeForm::Sine)
        {
            SetType(ch, Type::Chebyshev);
        }
        else if(form == TypeForm::Impulse || form == TypeForm::Packet || form == TypeForm::Meander)
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


void DGenerator::Init()
{
    EnableChannel(ChA, false);
    EnableChannel(ChB, false);
    AD9952::Init();
    AD5697::Init();
    FPGA::Init();
    DFreqMeter::Init();
    Amplifier::Init();
    Filtr::Init();
}


void DGenerator::EnableChannel(const Chan &ch, bool enable)
{
    static const StructPIN pins[Chan::Count] = { {WR_OUT_A}, {WR_OUT_B} };

    HAL_PIO::Write(pins[ch].port, pins[ch].pin, enable);
}


void DGenerator::SingleStart()
{
    FPGA::SingleStart();
}



void DGenerator::SetFormWave(const Chan &ch, TypeForm::E form)
{
    Filtr::Tune(ch, form);

    if(ch < Chan::Count && form < TypeForm::Count)
    {
        SettingsGenerator::waveIsSine[ch] = (form == TypeForm::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}


void DGenerator::SetFrequency(const Chan &ch, Value freq)
{
    SettingsGenerator::frequency[ch] = freq;

    if (SettingsGenerator::waveIsSine[ch])
    {
        AD9952::SetFrequency(ch);
    }
    else
    {
        FPGA::SetFrequency(ch);
    }
}


void DGenerator::SetManipulationPeriod(const Chan &ch, Value period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetManipulationDuration(const Chan &ch, Value duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}


void DGenerator::SetPeriod(const Chan &ch, Value period)
{
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetAmplitude(const Chan &ch, Value ampl)
{
    SettingsGenerator::amplitude[ch] = ampl;

    Amplifier::Tune(ch);

    if (SettingsGenerator::waveIsSine[ch])
    {
        AD9952::SetAmplitude(ch);
    }
    else
    {
        FPGA::SetAmplitude();
    }

    SetOffset(ch, SettingsGenerator::OffsetValue(ch));
}


void DGenerator::SetOffset(const Chan &ch, Value off)
{
    SettingsGenerator::offset[ch] = off;

    Amplifier::Tune(ch);

    AD5697::SetOffset(ch);
}


void DGenerator::SetPhase(const Chan &ch, Value phase)
{
    AD9952::SetPhase(ch, phase);
}


void DGenerator::SetPacketNumber(const Chan &, Value number)
{
    FPGA::PacketImpulse::SetNumberImpules(static_cast<uint>(number.ToDouble() + 0.5));
}


void DGenerator::SetPacketPeriod(const Chan &, Value period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}


void DGenerator::SetDutyRatio(const Chan &, Value)
{

}


void DGenerator::SetDuration(const Chan &ch, Value value)
{
    FPGA::SetDurationImpulse(ch, value);
}


void DGenerator::SetDelay(const Chan &, Value)
{
}


void Amplifier::SetAttenuation(const Chan &ch, Attenuation::E att)
{
    static const HPort::E gpio0[Chan::Count] = { HPort::_E, HPort::_F };
    static const HPort::E gpio1[Chan::Count] = { HPort::_B, HPort::_C };

    static const uint16 pin0[Chan::Count]    = { HPin::_15, HPin::_5 };
    static const uint16 pin1[Chan::Count]    = { HPin::_10,  HPin::_13 };

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

    HAL_PIO::Write(gpio0[ch], pin0[ch], state0[att]);
    HAL_PIO::Write(gpio1[ch], pin1[ch], state1[att]);

    attenuation[ch] = att;
}


void Amplifier::Init()
{
    HAL_PIO::Init(HPort::_E, HPin::_15, HMode::Output_PP, HPull::No); //-V525

    HAL_PIO::Init(HPort::_B, HPin::_10, HMode::Output_PP, HPull::No);

    HAL_PIO::Init(HPort::_F, HPin::_0, HMode::Output_PP, HPull::No);
    HAL_PIO::Init(HPort::_F, HPin::_5, HMode::Output_PP, HPull::No);

    HAL_PIO::Init(HPort::_C, HPin::_13, HMode::Output_PP, HPull::No);
    HAL_PIO::Init(HPort::_C, HPin::_14, HMode::Output_PP, HPull::No);
}


void Amplifier::TuneAndLock(const Chan &ch, bool enabled, Attenuation::E att)
{
    Enable(ch, enabled);
    SetAttenuation(ch, att);
    locked = true;
}


void Amplifier::Unlock()
{
    locked = false;
}


void Amplifier::Lock()
{
    locked = true;
}


void Amplifier::Tune(const Chan &ch, int range)
{
    static float amplitudes[6] =
    {
        0.05f,
        0.3f,
        0.9f,
        3.0f,
        9.0f
    };

    float amplitude = amplitudes[range];

    if (amplitude > Attenuation(Attenuation::_10Db).Units())                     // (3,16 ... 10.0] В
    {
        SetAttenuation(ch, Attenuation::_0Db);
    }
    else if (amplitude > 1.0F)                                                   // (1 ... 3.16] В 
    {
        SetAttenuation(ch, Attenuation::_10Db);
    }
    else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 10.0F)        // (0.316 ... 1] В
    {
        SetAttenuation(ch, Attenuation::_0Db);
    }
    else if (amplitude > 0.100F)                                                 // (0.100 ... 0.316] В
    {
        SetAttenuation(ch, Attenuation::_10Db);
    }
    else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 100.0F)       // (31.6 ... 0.100] В
    {
        SetAttenuation(ch, Attenuation::_20Db);
    }
    else
    {
        SetAttenuation(ch, Attenuation::_30Db);                                  // [0 ... 31.6] В
    }

    Enable(ch, amplitude > 1.0F);
}


void Amplifier::Tune(const Chan &ch)
{
    if (locked)
    {
        return;
    }

    if (SettingsGenerator::AmplitudeValue(ch).Abs() == 0)
    {
        if (std::fabs(SettingsGenerator::Offset(ch)) <= 2.5)
        {
            SetAttenuation(ch, Attenuation::_20Db);
            Enable(ch, false);
        }
        else
        {
            SetAttenuation(ch, Attenuation::_10Db);
            Enable(ch, true);
        }
    }
    else
    {
        double amplitude = SettingsGenerator::Amplitude(ch);

        if (amplitude > Attenuation(Attenuation::_10Db).Units())                     // (3,16 ... 10.0] В
        {
            SetAttenuation(ch, Attenuation::_0Db);
        }
        else if (amplitude > 1.0F)                                                   // (1 ... 3.16] В 
        {
            SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 10.0F)        // (0.316 ... 1] В
        {
            SetAttenuation(ch, Attenuation::_0Db);
        }
        else if (amplitude > 0.100F)                                                 // (0.100 ... 0.316] В
        {
            SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 100.0F)       // (31.6 ... 0.100] В
        {
            SetAttenuation(ch, Attenuation::_20Db);
        }
        else
        {
            SetAttenuation(ch, Attenuation::_30Db);                                  // [0 ... 31.6] В
        }

        Enable(ch, amplitude > 1.0F);
    }
}


void Amplifier::Enable(const Chan &ch, bool enable)
{
    static const HPort::E gpio[Chan::Count] = { HPort::_F, HPort::_C };
    static const uint16   pin[Chan::Count] =  { HPin::_0,  HPin::_14 };

    HAL_PIO::Write(gpio[ch], pin[ch], enable);

    isEnabled[ch] = enable;
}


double Amplifier::GetAmplification(const Chan &ch)
{
    static const Attenuation att[Attenuation::Count] =
    {
        Attenuation(Attenuation::_0Db),
        Attenuation(Attenuation::_10Db),
        Attenuation(Attenuation::_20Db),
        Attenuation(Attenuation::_30Db)
    };

    return (isEnabled[ch] ? 10.0 : 1.0) / att[attenuation[ch]].Units();
}


float Attenuation::Units() const
{
    static const float att[Count] =
    {
        std::powf(10.0F, 0.05F * 0.0F),
        std::powf(10.0F, 0.05F * 10.0F),
        std::powf(10.0F, 0.05F * 20.0F),
        std::powf(10.0F, 0.05F * 30.0F)
    };

    return att[value];
}


pString Attenuation::Name() const
{
    static const pString name[Count] =
    {
        "0",
        "10",
        "20",
        "30"
    };

    return name[value];
}


//void Amplifier::LogState()
//{
//    static const pString att[] =
//    {
//        "0Db",
//        "10Db",
//        "20Db",
//        "30Db"
//    };
//
//    LOG_WRITE("ampl en = %d/%d, at = %s/%s", isEnabled[Chan::A], isEnabled[Chan::B], att[attenuation[Chan::A]], att[attenuation[Chan::B]]);
//}
