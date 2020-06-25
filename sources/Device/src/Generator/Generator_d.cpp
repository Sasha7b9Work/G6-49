#include "defines.h"
#include "Generator/Generator_d.h"
#include "FreqMeter/FreqMeter_d.h"
#include <cmath>


bool SettingsGenerator::waveIsSine[Chan::Count] = { true, true };
Value SettingsGenerator::amplitude[Chan::Count] = { Value("10"), Value("10") };
Value SettingsGenerator::frequency[Chan::Count] = { Value("1000"), Value("1000") };
Value SettingsGenerator::offset[Chan::Count] = { Value("0"), Value("0") };
Attenuation::E Amplifier::attenuation[Chan::Count] = { Attenuation::_0Db, Attenuation::_0Db };
bool Amplifier::isBlocked = false;
bool Amplifier::isEnabled[Chan::Count] = { false, false };


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
        else if(form == TypeForm::Impulse || form == TypeForm::PacketImpuls || form == TypeForm::Meander)
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



void DGenerator::SetFormWave(Chan::E ch, TypeForm::E form)
{
    Filtr::Tune(ch, form);

    if(ch < Chan::Count && form < TypeForm::Count)
    {
        SettingsGenerator::waveIsSine[ch] = (form == TypeForm::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}


void DGenerator::SetFrequency(Chan::E ch, Value freq)
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


void DGenerator::SetManipulationPeriod(Chan::E ch, Value period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetManipulationDuration(Chan::E ch, Value duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}


void DGenerator::SetPeriod(Chan::E ch, Value period)
{
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetAmplitude(Chan::E ch, Value ampl)
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
}


void DGenerator::SetOffset(Chan::E ch, Value off)
{
    SettingsGenerator::offset[ch] = off;

    Amplifier::Tune(ch);

    AD5697::SetOffset(ch);
}


void DGenerator::SetPhase(Chan::E ch, Value phase)
{
    AD9952::SetPhase(ch, phase);
}


void DGenerator::SetPacketNumber(Chan::E, Value number)
{
    FPGA::PacketImpulse::SetNumberImpules(static_cast<uint>(number.ToDouble() + 0.5));
}


void DGenerator::SetPacketPeriod(Chan::E, Value period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}


void DGenerator::SetDutyRatio(Chan::E, Value)
{

}


void DGenerator::SetDuration(Chan::E ch, Value value)
{
    FPGA::SetDurationImpulse(ch, value);
}


void DGenerator::SetDelay(Chan::E, Value)
{
}


void Amplifier::SetAttenuation(Chan::E ch, Attenuation::E att)
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


void Amplifier::Tune(Chan::E ch)
{
    if(isBlocked)
    {
        return;
    }

    if (SettingsGenerator::AmplitudeValue(ch).Abs() == 0)
    {
        bool moreTwoAndHalfVolts = SettingsGenerator::Offset(ch) > 2.5F;

        SetAttenuation(ch, moreTwoAndHalfVolts ? Attenuation::_0Db : Attenuation::_10Db);

        Enable(ch, moreTwoAndHalfVolts);
    }
    else
    {
        double amplitude = SettingsGenerator::Amplitude(ch);

        if (amplitude > Attenuation(Attenuation::_10Db).Units())                     // 1 диапазон   3.16
        {
            SetAttenuation(ch, Attenuation::_0Db);
        }
        else if (amplitude > 1.0F)                                                   // 2 диапазон  1
        {
            SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 10.0F)        // 4 диапазоны 0.316
        {
            SetAttenuation(ch, Attenuation::_0Db);
        }
        else if (amplitude > 0.100F)                                                 // 5 диапазон 0.100
        {
            SetAttenuation(ch, Attenuation::_10Db);
        }
        else if (amplitude > Attenuation(Attenuation::_10Db).Units() / 100.0F)       // 6 диапазон
        {
            SetAttenuation(ch, Attenuation::_20Db);
        }
        else
        {
            SetAttenuation(ch, Attenuation::_30Db);
        }

        Enable(ch, amplitude > 1.0F);
    }
}


void Amplifier::Enable(Chan::E ch, bool enable)
{
    static const HPort::E gpio[Chan::Count] = { HPort::_F, HPort::_C };
    static const uint16   pin[Chan::Count] =  { HPin::_0,  HPin::_14 };

    HAL_PIO::Write(gpio[ch], pin[ch], enable);

    isEnabled[ch] = enable;
}


double Amplifier::GetAmplification(Chan::E ch)
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
