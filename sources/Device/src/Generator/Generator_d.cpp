#include "AD5697.h"
#include "FPGA.h"
#include "Generator_d.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "FreqMeter/FreqMeter_d.h"
#include "Settings/CalibrationSettings.h"
#include "common/CommonTypes.h"


bool DGenerator::waveIsSine[Chan::Count] = { true, true };
float DGenerator::amplitude[Chan::Count] = { 10.0F, 10.0F };
float DGenerator::offset[Chan::Count] = { 0.0F, 0.0F };


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
        waveIsSine[ch] = (form == TypeForm::Sine);

        FPGA::SetWaveForm(ch, form);
    }
}


void DGenerator::SetFrequency(Chan::E ch, FloatValue frequency)
{
    if (waveIsSine[ch])
    {
        AD9952::SetFrequency(ch, frequency);
    }
    else
    {
        FPGA::SetFrequency(ch, frequency);
    }
}


void DGenerator::SetManipulationPeriod(Chan::E ch, FloatValue period)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetManipulationDuration(Chan::E ch, FloatValue duration)
{
    FPGA::SetPolarity(ch, 0);
    FPGA::SetDurationImpulse(ch, duration);
}


void DGenerator::SetPeriod(Chan::E ch, FloatValue period)
{
    FPGA::SetPeriodImpulse(ch, period);
}


void DGenerator::SetAmplitude(Chan::E ch, FloatValue ampl)
{
    amplitude[ch] = ampl.ToFloat();

    Amplifier::Tune(ch);

    if (waveIsSine[ch])
    {
        AD9952::SetAmplitude(ch, ampl);
    }
    else
    {
        FPGA::SetAmplitude(ch, ampl);
    }
}


void DGenerator::SetOffset(Chan::E ch, FloatValue off)
{
    offset[ch] = off.ToFloat();

    Amplifier::Tune(ch);

    AD5697::SetOffset(ch, off);
}


void DGenerator::SetPhase(Chan::E ch, FloatValue phase)
{
    AD9952::SetPhase(ch, phase);
}


void DGenerator::SetPacketNumber(Chan::E, FloatValue number)
{
    FPGA::PacketImpulse::SetNumberImpules(static_cast<uint>(number.ToFloat() + 0.5F));
}


void DGenerator::SetPacketPeriod(Chan::E, FloatValue period)
{
    FPGA::PacketImpulse::SetPeriodPacket(period);
}


void DGenerator::SetDutyRatio(Chan::E, FloatValue)
{

}


void DGenerator::SetDuration(Chan::E ch, FloatValue value)
{
    FPGA::SetDurationImpulse(ch, value);
}


void DGenerator::SetDelay(Chan::E, FloatValue)
{
}
