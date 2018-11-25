#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "GeneratorPanel.h"
#include "Interface/InterfacePanel.h"
#include "Log.h"
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
void Generator::EnableChannel(Chan ch, bool enable)
{
    Message message(3, Command::EnableChannel, ch, (uint8)(enable ? 1u : 0u));

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadStartMode(Chan ch, int mode)
{
    Message message(3, Command::SetStartMode, ch, (uint8)mode);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Register reg, uint64 value)
{
    Message message(10, Command::WriteRegister, (uint8)reg, value);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    Message message(2, Command::ModeDebug, (uint8)(enable ? 1u : 0u));

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    Message message(1, Command::RunReset);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Wave *w)
{
    Chan ch = w->GetChannel();
    uint8 form = (uint8)FORM(ch)->value;

    if(FORM(ch)->IsDDS())
    {
        SetFormDDS(FORM(ch));
    }
    else
    {
        Message message(3, Command::SetFormWave, ch, form);

        Interface::Send(&message);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, Form::E form)
{
    /// \todo Здесь, наверное, неправильная установка формы сигнала - что будет при установке произвольной формы?

    Message message(3, Command::SetFormWave, ch, (uint8)form);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormDDS(Form *form)
{
    Chan ch = form->GetWave()->GetChannel();

    float data[FPGA_NUM_POINTS];

    Message message(FPGA_NUM_POINTS * 2 + 2, Command::LoadFormDDS, ch);

    uint8 *points = message.Data(2);

    switch (form->value)
    {
        case Form::RampPlus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for(int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = -1.0f + step * i;
                }

                TransformDataToCode(data, points);

                Interface::Send(&message);
            }
            break;
        case Form::RampMinus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for (int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = 1.0f - step * i;
                }

                TransformDataToCode(data, points);

                Interface::Send(&message);
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

                TransformDataToCode(data, points);

                Interface::Send(&message);
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
void Generator::TransformDataToCode(float d[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2])
{
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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterChoice *param)
{
    static const struct StructCommand
    {
        Command command;
        StructCommand(Command::E c) : command(c) {};
    }
    commands[ParameterChoice::Number] =
    {
        Command::SetPolarity,
        Command::SetStartMode,
        Command::SetManipulationMode,
        Command::SetManipulation
    };

    Message message(3, (uint8)commands[param->value].command,
        (uint8)param->GetForm()->GetWave()->GetChannel(),
        (uint8)param->GetChoice());

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetOffset(Chan ch, float offset)
{
    Message message(6, (uint8)Command::SetOffset, (uint8)ch, offset);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetAmplitude(Chan ch, float amplitude)
{
    Message message(6, (uint8)Command::SetAmplitude, ch, amplitude);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterValue *param)
{
    static const struct StructCommand
    {
        Command command;
        StructCommand(Command::E c) : command(c) {};
    }
    commands[ParameterValue::Number] =
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
        Command::SetPacketPeriod,
        Command::SetPacketNumber,
        Command::RequestData
    };

    float value = param->GetValue();

    if(param->Is(ParameterValue::Offset))
    {
        value -= 5.0f;
    }

    Message message(6, (uint8)commands[param->value].command, (uint8)param->GetForm()->GetWave()->GetChannel(), value);

    Interface::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TuneChannel(Chan ch)
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
