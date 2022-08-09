// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator_p.h"
#include "Interface/Messages_p.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Settings/Settings.h"
#include "Display/Console.h"
#include "Signals/Signals.h"
#include "common/Command.h"
#include "structs.h"
#include "Display/Console.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Math.h"
#include <cmath>
#include <string.h>
#include <cstdlib>


namespace PGenerator
{
    // Загружает форму произвольного сигнала
    void LoadFormDDS(Form *form);

    // Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, пригодные для отправки в ПЛИС
    void TransformDataToCodeAndTransmit(const float data[DDS_NUM_POINTS], Form *form);
}


void PGenerator::EnableChannel(const Chan &ch, bool enable)
{
    set.enabled[ch] = enable;
    Message::EnableChannel((uint8)(ch.value), static_cast<uint8>(enable ? 1U : 0U)).Transmit();
}


void PGenerator::SingleStart()
{
    Message::SingleStart().Transmit();
}


void PGenerator::LoadStartMode(const Chan &ch, int signal, int mode)
{
    int firstByte = (uint8)ch + (signal << 1);

    Message::StartMode((uint8)firstByte, (uint8)mode).Transmit();
}


void PGenerator::LoadRegister(Register::E reg, uint64 value)
{
    Message::WriteRegister(static_cast<uint8>(reg), value).Transmit();
}


void PGenerator::SetDebugMode(bool enable)
{
    Message::DebugMode(static_cast<uint8>(enable ? 1 : 0)).Transmit();
}


void PGenerator::SetFormWave(const Wave *w)
{
    Chan ch = w->GetChannel();
    uint8 form = static_cast<uint8>(FORM(ch)->value);

    if(FORM(ch)->IsDDS())
    {
        LoadFormDDS(FORM(ch));
    }

    Message::Set::FormWave(static_cast<uint8>(ch), form).Transmit();
}


void PGenerator::SetFormWave(const Chan &ch, TypeForm::E form)
{
    // \todo Здесь, наверное, неправильная установка формы сигнала - что будет при установке произвольной формы?

    Message::Set::FormWave(static_cast<uint8>(ch), static_cast<uint8>(form)).Transmit();
}


void PGenerator::LoadFormDDS(Form *form)
{
    float data[DDS_NUM_POINTS];

    switch (form->value)
    {
        case TypeForm::RampPlus:
            {
                float step = 2.0F / static_cast<float>(DDS_NUM_POINTS);

                for(int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = -1.0F + step * static_cast<float>(i);
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::RampMinus:
            {
                float step = 2.0F / static_cast<float>(DDS_NUM_POINTS);

                for (int i = 0; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = 1.0F - step * static_cast<float>(i);
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::Triangle:
            {
                float step = 2.0F / static_cast<float>(DDS_NUM_POINTS / 2);

                for (int i = 0; i < DDS_NUM_POINTS / 2; i++)
                {
                    data[i] = - 1.0F + step * static_cast<float>(i);
                }

                for(int i = DDS_NUM_POINTS / 2; i < DDS_NUM_POINTS; i++)
                {
                    data[i] = 1.0F - step * static_cast<float>(i - DDS_NUM_POINTS / 2);
                }

                TransformDataToCodeAndTransmit(data, form);
            }
            break;
        case TypeForm::Meander:
        case TypeForm::Sine:
        case TypeForm::Impulse:
        case TypeForm::Packet:
        case TypeForm::Free:
        case TypeForm::Count:
            break;
    }
}


void PGenerator::TransformDataToCodeAndTransmit(const float d[DDS_NUM_POINTS], Form *form)
{
    uint16 buffer[DDS_NUM_POINTS];

    uint8 *code = reinterpret_cast<uint8 *>(buffer);

    float max = static_cast<float>(0x1fff);

    for (int i = 0; i < DDS_NUM_POINTS; i++)
    {
        float value = d[i];

        uint16 c = static_cast<uint16>(std::fabsf(value) * max);

        if (value > 0.0F)
        {
            _SET_BIT(c, 13);
        }

        code[i] = static_cast<uint8>(c);
        code[i + DDS_NUM_POINTS] = static_cast<uint8>(c >> 8);
    }

    Message::LoadFormDDS(static_cast<uint8>(form->GetWave()->GetChannel()), buffer).Transmit();
}


void PGenerator::SetParameterChoice(ParameterChoice *param)
{
    static const Command::E commands[ParameterChoiceType::Count] =
    {
        Command::SetPolarity,
        Command::SetStartMode,
        Command::SetManipulation
    };

    Message::Set::Param(commands[param->GetType()],
        static_cast<uint8>(param->GetForm()->GetWave()->GetChannel()),
        static_cast<uint8>(param->GetChoice())).Transmit();
}


void PGenerator::SetOffset(const Chan &ch, float offset)
{
    // \todo Говнокод - запись параметра из двух мест
    Message::Set::Offset(static_cast<uint8>(ch), Value(offset).ToUINT64()).Transmit();
}


void PGenerator::SetAmplitude(const Chan &ch, float amplitude)
{
    // \todo Говнокод - запись параметра из двух мест
    Message::Set::Amplitude(static_cast<uint8>(ch), Value(amplitude).ToUINT64()).Transmit();
}


void PGenerator::SetParameterDouble(DParam *param)
{
    static const Command::E commands[TypeDParam::Count] =
    {
        Command::SetFrequency,
        Command::SetPeriod,
        Command::SetAmplitude,
        Command::SetOffset,
        Command::SetDuration,
        Command::SetDutyRatio,
        Command::SetPhase,
        Command::SetDelay,
        Command::SetDurationRise,
        Command::SetDurationFall,
        Command::SetDurationStady,
        Command::SetDutyFactor,
        Command::SetManipulationDuration,
        Command::SetManipulationPeriod,
        Command::SetPacketPeriod
    };

    Value value = param->GetValue();

    Chan ch(param->GetForm()->GetWave()->GetChannel());

    Command com(commands[param->GetType()]);

    Message::Set::Param(com, ch, value.ToUINT64()).Transmit();
}


void PGenerator::SetParameterInteger(ParameterInteger *param)
{
    static const Command::E commands[TypeIParam::Count] =
    {
        Command::SetPacketNumber
    };

    Value value = param->GetValue();

    Chan ch(param->GetForm()->GetWave()->GetChannel());

    Command com(commands[param->GetType()]);

    Message::Set::Param(com, ch, value.ToUINT64()).Transmit();
}


void PGenerator::TuneChannel(const Chan &ch)
{
    EnableChannel(ch, ch.Enabled());

    FORM(ch)->TuneGenerator();
}


void PGenerator::LoadSettings()
{
    PGenerator::TuneChannel(ChA);
    PGenerator::TuneChannel(ChB);
}

void PGenerator::SetParameter(Param *parameter)
{
    if(parameter->IsDouble())
    {
        SetParameterDouble(static_cast<DParam *>(parameter));
    }
    else if(parameter->IsChoice())
    {
        SetParameterChoice(static_cast<ParameterChoice *>(parameter));
    }
    else if (parameter->IsInteger())
    {
        SetParameterInteger(static_cast<ParameterInteger *>(parameter));
    }
}


void PGenerator::LoadPictureDDSfromStorage(const Chan &ch)
{
    Message::Storage::RequestPictureDDS(ch).Transmit();
}


void PGenerator::Reset()
{
    setCal.Load();
    set.LoadDefault();
    set.LoadToDevice();

    Menu::Init();

    WAVE(ChA).Reset();
    WAVE(ChB).Reset();

    PageSignals::SetCurrentChanenl(ChB);
    PageSignals::SCPI_SetForm(TypeForm::Sine);

    PageSignals::SetCurrentChanenl(ChA);
    PageSignals::SCPI_SetForm(TypeForm::Sine);
}
