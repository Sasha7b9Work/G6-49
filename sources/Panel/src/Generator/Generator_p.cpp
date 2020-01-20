#include "defines.h"
#include "Generator_p.h"
#include "Log.h"
#include "common/Message_p.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Settings/Settings.h"
#include "Display/Console.h"
#include "Signals.h"
#include "common/Command.h"
#include "structs.h"
#include "Signals.h"
#include "Display/Console.h"
#include "Utils/Math.h"
#include "Utils/Debug.h"
#include <cmath>
#include <string.h>
#include <cstdlib>


void PGenerator::EnableChannel(Chan::E ch, bool enable)
{
    Message::EnableChannel(static_cast<uint8>(ch), static_cast<uint8>(enable ? 1U : 0U)).Transmit();
}


void PGenerator::LoadStartMode(Chan::E ch, int mode)
{
    Message::StartMode(static_cast<uint8>(ch), static_cast<uint8>(mode)).Transmit();
}


void PGenerator::LoadRegister(Register::E reg, uint64 value)
{
    Message::WriteRegister(static_cast<uint8>(reg), value).Transmit();
}


void PGenerator::SetDebugMode(bool enable)
{
    Message::DebugMode(static_cast<uint8>(enable ? 1 : 0)).Transmit();
}


void PGenerator::Reset()
{
    Message::Reset().Transmit();
}


void PGenerator::SetFormWave(const Wave *w)
{
    Chan::E ch = w->GetChannel();
    uint8 form = static_cast<uint8>(FORM(ch)->value);

    if(FORM(ch)->IsDDS())
    {
        LoadFormDDS(FORM(ch));
    }

    Message::Set::FormWave(static_cast<uint8>(ch), form).Transmit();
}


void PGenerator::SetFormWave(Chan::E ch, TypeForm::E form)
{
    /// \todo Здесь, наверное, неправильная установка формы сигнала - что будет при установке произвольной формы?

    Message::Set::FormWave(static_cast<uint8>(ch), static_cast<uint8>(form)).Transmit();
}


void PGenerator::LoadFormDDS(Form *form)
{
    float data[DDS_NUM_POINTS];

    switch (form->value)
    {
        case TypeForm::RampPlus:
            {
                float step = 2.0F / DDS_NUM_POINTS;

                for(int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = -1.0F + step * i;
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::RampMinus:
            {
                float step = 2.0F / DDS_NUM_POINTS;

                for (int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = 1.0F - step * i;
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::Triangle:
            {
                float step = 2.0F / (DDS_NUM_POINTS / 2);

                for (int i = 0; i < DDS_NUM_POINTS / 2; i++)
                {
                    data[i] = - 1.0F + step * i;
                }

                for(int i = DDS_NUM_POINTS / 2; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = 1.0F - step * (i - DDS_NUM_POINTS / 2);
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::Meander:
        case TypeForm::Sine:
        case TypeForm::Impulse:
        case TypeForm::PacketImpuls:
        case TypeForm::Free:
        case TypeForm::Count:
            break;
    }
}


void PGenerator::TransformDataToCodeAndTransmit(const float d[DDS_NUM_POINTS], Form *form)
{
    uint16 buffer[DDS_NUM_POINTS];

    uint8 *code = reinterpret_cast<uint8 *>(buffer);

    int max = 0x1fff;

    for (int i = 0; i < DDS_NUM_POINTS; i++)
    {
        uint16 c = static_cast<uint16>(std::fabs(d[i]) * max);

        if (Math::Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i] = static_cast<uint8>(c);
        code[i + DDS_NUM_POINTS] = static_cast<uint8>(c >> 8);
    }

    Message::LoadFormDDS(static_cast<uint8>(form->GetWave()->GetChannel()), buffer).Transmit();
}


void PGenerator::SetParameter(ParameterChoice *param)
{
    static const Command::E commands[ParameterChoice::Count] =
    {
        Command::SetPolarity,
        Command::SetStartMode,
        Command::deleted_SetManipulationMode,
        Command::SetManipulation
    };

    Message::Set::Parameter(commands[param->Type()],
        static_cast<uint8>(param->GetForm()->GetWave()->GetChannel()),
        static_cast<uint8>(param->GetChoice())).Transmit();
}


void PGenerator::SetOffset(Chan::E ch, float offset)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Offset(static_cast<uint8>(ch), FloatValue(offset).ToUINT64()).Transmit();
}


void PGenerator::SetAmplitude(Chan::E ch, float amplitude)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Amplitude(static_cast<uint8>(ch), FloatValue(amplitude).ToUINT64()).Transmit();
}


void PGenerator::SetParameter(ParameterValue *param)
{
    static const Command::E commands[ParameterValue::Count] =
    {
        Command::SetFrequency,
        Command::SetPeriod,                     // 
        Command::SetAmplitude,
        Command::SetOffset,
        Command::SetDuration,                   // 
        Command::SetDutyRatio,
        Command::SetPhase,
        Command::SetDelay,
        Command::SetDurationRise,
        Command::SetDurationFall,
        Command::SetDurationStady,
        Command::SetDutyFactor,
        Command::SetManipulationDuration,
        Command::SetManipulationPeriod,
        Command::SetPacketPeriod,
        Command::SetPacketNumber,
        Command::RequestData
    };

    FloatValue value = param->GetValueNano();

    if (param->Is(ParameterValue::Offset))
    {
        value.Add(-5.0F);
    }

    Chan ch(param->GetForm()->GetWave()->GetChannel());

    Command com(commands[param->value]);

    Message::Set::Parameter(com, ch, value.ToUINT64()).Transmit();
}


void PGenerator::TuneChannel(Chan::E ch)
{
    EnableChannel(ch, CHANNEL_ENABLED(ch));

    FORM(ch)->TuneGenerator(ch);
}


void PGenerator::LoadSettings()
{
    PGenerator::TuneChannel(Chan::A);
    PGenerator::TuneChannel(Chan::B);
}
