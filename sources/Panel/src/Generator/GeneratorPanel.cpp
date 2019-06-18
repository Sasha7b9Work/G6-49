#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "GeneratorPanel.h"
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
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const int FPGA_NUM_POINTS = 8 * 1024;
/// Загружает форму произвольного сигнала
static void LoadFormDDS(Form *form);
/// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
static void TransformDataToCode(float data[FPGA_NUM_POINTS], Form *form);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Chan::E ch, bool enable)
{
    Message::EnableChannel((uint8)ch, (uint8)(enable ? 1U : 0U)).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadStartMode(Chan::E ch, int mode)
{
    Message::StartMode((uint8)ch, (uint8)mode).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Register::E reg, uint64 value)
{
    Message::WriteRegister((uint8)reg, value).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    Message::DebugMode((uint8)(enable ? 1 : 0)).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    Message::Reset().Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Wave *w)
{
    Chan::E ch = w->GetChannel();
    uint8 form = (uint8)FORM(ch)->value;

    if(FORM(ch)->IsDDS())
    {
        LoadFormDDS(FORM(ch));
    }

    Message::Set::FormWave((uint8)ch, form).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan::E ch, Form::E form)
{
    /// \todo Здесь, наверное, неправильная установка формы сигнала - что будет при установке произвольной формы?

    Message::Set::FormWave((uint8)ch, (uint8)form).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void LoadFormDDS(Form *form)
{
    float data[FPGA_NUM_POINTS];

    switch (form->value)
    {
        case Form::RampPlus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for(int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = -1.0f + step * i;
                }

                TransformDataToCode(data, form);
            }
            break;
        case Form::RampMinus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for (int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = 1.0f - step * i;
                }

                TransformDataToCode(data, form);
            }
            break;
        case Form::Triangle:
            {
                float step = 2.0f / (FPGA_NUM_POINTS / 2);

                for (int i = 0; i < FPGA_NUM_POINTS / 2; i++)
                {
                    data[i] = - 1.0f + step * i;
                }

                for(int i = FPGA_NUM_POINTS / 2; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = 1.0f - step * (i - FPGA_NUM_POINTS / 2);
                }

                TransformDataToCode(data, form);
            }
            break;
        case Form::Meander:
        case Form::Sine:
        case Form::Impulse:
        case Form::PacketImpuls:
        case Form::DDS:
        case Form::Number:
            break;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TransformDataToCode(float d[FPGA_NUM_POINTS], Form *form)
{
    Chan::E ch = form->GetWave()->GetChannel();

    SimpleMessage message(FPGA_NUM_POINTS * 2 + 2, Command::LoadFormDDS, (uint8)ch);

    uint8 code[FPGA_NUM_POINTS * 2];

    int max = 0x1fff;

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        uint16 c = (uint16)(std::fabs(d[i]) * max);

        if (Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i] = (uint8)c;
        code[i + FPGA_NUM_POINTS] = (uint8)(c >> 8);
    }

    message.PutData(code, FPGA_NUM_POINTS * 2);

    message.Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterChoice *param)
{
    DEF_STRUCT(StructCommand, Command::E) commands[ParameterChoice::Number] =
    {
        Command::SetPolarity,
        Command::SetStartMode,
        Command::SetManipulationMode,
        Command::SetManipulation
    };

    Message::Set::Parameter(commands[param->value].val,
        (uint8)param->GetForm()->GetWave()->GetChannel(),
        (uint8)param->GetChoice()).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Chan::E ch, float offset)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Offset((uint8)ch, ParamValue(offset).ToUINT64()).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetAmplitude(Chan::E ch, float amplitude)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Amplitude((uint8)ch, ParamValue(amplitude).ToUINT64()).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFrequency(Chan::E ch, float frequency)
{
    /// \todo Говнокод - запись параметра из двух мест
    Message::Set::Frequency((uint8)ch, ParamValue(frequency).ToUINT64()).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterValue *param)
{
    DEF_STRUCT(StructCommand, Command::E) commands[ParameterValue::Number] =
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
        value.Add(-5.0f);
    }

    Chan::E ch = param->GetForm()->GetWave()->GetChannel();

    Command::E com = commands[param->value].val;

    Message::Set::Parameter(com, (uint8)ch, value.ToUINT64()).Transmit();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TuneChannel(Chan::E ch)
{
    EnableChannel(ch, CHANNEL_ENABLED(ch));

    FORM(ch)->TuneGenerator(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadSettings()
{
    Generator::TuneChannel(Chan::A);
    Generator::TuneChannel(Chan::B);
}
