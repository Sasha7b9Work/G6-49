#include "defines.h"
#include "Generator_p.h"
#include "Log.h"
#include "Message.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Settings/Settings.h"
#include "Display/Console.h"
#include "Signals.h"
#include "Command.h"
#include "structs.h"
#include "Signals.h"
#include "Display/Console.h"
#include "Utils/Math.h"
#include "Utils/Debug.h"
#include <cmath>
#include <string.h>
#include <cstdlib>


namespace Generator
{
    /// Загружает форму произвольного сигнала
    static void LoadFormDDS(Form *form);
    /// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
    static void TransformDataToCodeAndTransmit(const float data[DDS_NUM_POINTS], Form *form);
}



void Generator::EnableChannel(Chan::E ch, bool enable)
{
    Message::EnableChannel(static_cast<uint8>(ch), static_cast<uint8>(enable ? 1U : 0U)).Transmit();
}


void Generator::LoadStartMode(Chan::E ch, int mode)
{
    Message::StartMode(static_cast<uint8>(ch), static_cast<uint8>(mode)).Transmit();
}


void Generator::LoadRegister(Register::E reg, uint64 value)
{
    Message::WriteRegister(static_cast<uint8>(reg), value).Transmit();
}


void Generator::SetDebugMode(bool enable)
{
    Message::DebugMode(static_cast<uint8>(enable ? 1 : 0)).Transmit();
}


void Generator::Reset()
{
    Message::Reset().Transmit();
}


void Generator::SetFormWave(const Wave *w)
{
    Chan::E ch = w->GetChannel();
    uint8 form = static_cast<uint8>(FORM(ch)->value);

    if(FORM(ch)->IsDDS())
    {
        LoadFormDDS(FORM(ch));
    }

    Message::Set::FormWave(static_cast<uint8>(ch), form).Transmit();
}


void Generator::SetFormWave(Chan::E ch, Form::E form)
{
    /// \todo Здесь, наверное, неправильная установка формы сигнала - что будет при установке произвольной формы?

    Message::Set::FormWave(static_cast<uint8>(ch), static_cast<uint8>(form)).Transmit();
}


void Generator::LoadFormDDS(Form *form)
{
    float data[DDS_NUM_POINTS];

    switch (form->value)
    {
        case Form::RampPlus:
            {
                float step = 2.0F / DDS_NUM_POINTS;

                for(int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = -1.0F + step * i;
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case Form::RampMinus:
            {
                float step = 2.0F / DDS_NUM_POINTS;

                for (int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = 1.0F - step * i;
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case Form::Triangle:
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
        case Form::Meander:
        case Form::Sine:
        case Form::Impulse:
        case Form::PacketImpuls:
        case Form::DDS:
        case Form::Count:
            break;
    }
}


void Generator::TransformDataToCodeAndTransmit(const float d[DDS_NUM_POINTS], Form *form)
{
    uint16 buffer[DDS_NUM_POINTS];

    uint8 *code = reinterpret_cast<uint8 *>(buffer);

    int max = 0x1fff;

    for (int i = 0; i < DDS_NUM_POINTS; i++)
    {
        uint16 c = static_cast<uint16>(std::fabs(d[i]) * max);

        if (Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i] = static_cast<uint8>(c);
        code[i + DDS_NUM_POINTS] = static_cast<uint8>(c >> 8);
    }

    Message::LoadFormDDS(static_cast<uint8>(form->GetWave()->GetChannel()), buffer).Transmit();
}


void Generator::SetParameter(ParameterChoice *param)
{
    static const Command::E commands[ParameterChoice::Count] =
    {
        Command::SetPolarity,
        Command::SetStartMode,
        Command::deleted_SetManipulationMode,
        Command::SetManipulation
    };

    Message::Set::Parameter(commands[param->value],
        static_cast<uint8>(param->GetForm()->GetWave()->GetChannel()),
        static_cast<uint8>(param->GetChoice())).Transmit();
}


void Generator::SetOffset(Chan::E ch, float offset)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Offset(static_cast<uint8>(ch), ParamValue(offset).ToUINT64()).Transmit();
}


void Generator::SetAmplitude(Chan::E ch, float amplitude)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Amplitude(static_cast<uint8>(ch), ParamValue(amplitude).ToUINT64()).Transmit();
}


void Generator::SetFrequency(Chan::E ch, float frequency)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Frequency(static_cast<uint8>(ch), ParamValue(frequency).ToUINT64()).Transmit();
}


void Generator::SetParameter(ParameterValue *param)
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

    ParamValue value = param->GetValueNano();

    if (param->Is(ParameterValue::Offset))
    {
        value.Add(-5.0F);
    }

    Chan ch(param->GetForm()->GetWave()->GetChannel());

    Command com(commands[param->value]);

    Message::Set::Parameter(com, ch, value.ToUINT64()).Transmit();
}


void Generator::TuneChannel(Chan::E ch)
{
    EnableChannel(ch, CHANNEL_ENABLED(ch));

    FORM(ch)->TuneGenerator(ch);
}


void Generator::LoadSettings()
{
    Generator::TuneChannel(Chan::A);
    Generator::TuneChannel(Chan::B);
}
