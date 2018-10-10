#include "defines.h"
#include "Generator.h"
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

#include <math.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Chan ch, bool enable)
{
    SendToInterface(Buffer(CommandPanel::EnableChannel, ch, enable ? 1u : 0u));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadStartMode(Chan ch, int mode)
{
    SendToInterface(Buffer(CommandPanel::SetStartMode, ch, (uint8)mode));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Register reg, uint64 value)
{
    INIT_BIT_SET_64(bitSet, value);

    uint8 buffer[10] = {CommandPanel::WriteRegister, (uint8)reg,   bitSet.byte0, bitSet.byte1, bitSet.byte2, bitSet.byte3,
                                                                    bitSet.byte4, bitSet.byte5, bitSet.byte6, bitSet.byte7};
    SendToInterface(buffer, 10);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::EmptyCommand()
{
    uint8 buffer[10] = {CommandPanel::None};
    SendToInterface(buffer, 10);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    SendToInterface(Buffer(CommandPanel::ModeDebug, enable ? 1u : 0u));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    SendToInterface(Buffer(CommandPanel::RunReset));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
        SendToInterface(Buffer(CommandPanel::SetFormWave, ch, form));
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormDDS(Form *form)
{
    Chan ch = form->GetWave()->GetChannel();

    switch (form->value)
    {
        uint8 buffer[FPGA_NUM_POINTS * 2];
        float data[FPGA_NUM_POINTS];

        case Form::RampPlus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for(int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = -1.0f + step * i;
                }

                TransformDataToCode(data, buffer);

                LoadPointsToDDS(ch, buffer);
            }
            break;
        case Form::RampMinus:
            {
                float step = 2.0f / FPGA_NUM_POINTS;

                for (int i = 0; i < FPGA_NUM_POINTS; i++)
                {
                    data[i] = 1.0f - step * i;
                }

                TransformDataToCode(data, buffer);

                LoadPointsToDDS(ch, buffer);
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

                TransformDataToCode(data, buffer);

                LoadPointsToDDS(ch, buffer);
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TransformDataToCode(float d[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2])
{
    int max = 0x1fff;

    for (int i = 0; i < FPGA_NUM_POINTS; i++)
    {
        uint16 c = (uint16)(fabs(d[i]) * max);

        if (Sign(d[i]) == -1)
        {
            SetBit(c, 13);
        }

        code[i] = (uint8)c;
        code[i + FPGA_NUM_POINTS] = (uint8)(c >> 8);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadPointsToDDS(Chan ch, uint8 points[FPGA_NUM_POINTS * 2])
{
    SendToInterface(Buffer(CommandPanel::LoadFormDDS, ch));

    SendToInterface(points, FPGA_NUM_POINTS * 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Update()
{
    static uint timePrev = 0;

    if(TIME_MS - timePrev > 100)
    {
        ProcessDataFPGA();

        timePrev = TIME_MS;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterChoice *param)
{
    static const struct StructCommand
    {
        CommandPanel command;
        StructCommand(CommandPanel::E c) : command(c) {};
    }
    commands[ParameterChoice::Number] =
    {
        CommandPanel::SetPolarity,
        CommandPanel::SetStartMode,
        CommandPanel::SetManipulationMode,
        CommandPanel::SetManipulation
    };

    SendToInterface(Buffer((uint8)commands[param->value].command, (uint8)param->GetForm()->GetWave()->GetChannel(), (uint8)param->GetChoice()));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(ParameterValue *param)
{
    static const struct StructCommand
    {
        CommandPanel command;
        StructCommand(CommandPanel::E c) : command(c) {};
    }
    commands[ParameterValue::Number] =
    {
        CommandPanel::SetFrequency,
        CommandPanel::SetPeriod,
        CommandPanel::SetAmplitude,
        CommandPanel::SetOffset,
        CommandPanel::SetDuration,
        CommandPanel::SetDutyRatio,
        CommandPanel::SetPhase,
        CommandPanel::SetDelay,
        CommandPanel::SetDurationRise,
        CommandPanel::SetDurationFall,
        CommandPanel::SetDurationStady,
        CommandPanel::SetDutyFactor,
        CommandPanel::SetManipulationDuration,
        CommandPanel::SetManipulationPeriod,
        CommandPanel::SetPacketPeriod,
        CommandPanel::SetPacketNumber,
        CommandPanel::None
    };

    uint8 buffer[6] = {(uint8)commands[param->value].command, (uint8)param->GetForm()->GetWave()->GetChannel()};

    float value = param->GetValue();

    if(param->Is(ParameterValue::Offset))
    {
        value -= 5.0f;
    }

    memcpy(&buffer[2], &value, 4);
    SendToInterface(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SendToInterface(const Buffer &buffer)
{
    SendToInterface(buffer.Data(), (uint16)buffer.Length());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SendToInterface(const uint8 *buffer, uint16 size)
{
    CPU::SPI4_::Transmit((uint8 *)&size, 2);

    const uint8 *pointer = buffer;
    while(size > 0)
    {
        uint16 sizeChunk = (size > 1024u) ? 1024u : size; // –азмер куска дл€ передачи

        size -= sizeChunk;

        CPU::SPI4_::Transmit(pointer, sizeChunk);

        pointer += sizeChunk;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ProcessDataFPGA()
{
    uint8 command = CommandPanel::RequestData;

    SendToInterface(&command, 1);

    uint16 numBytes = 0;

    CPU::SPI4_::Receive((uint8 *)&numBytes, 2);

    if(numBytes)  // ѕрин€тое значение означает число байт, готовых дл€ передачи вспомогательным процессором
    {
        ReceiveAndRun(numBytes);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ReceiveAndRun(uint16 numBytes)
{
    uint8 *buffer = (uint8 *)malloc(numBytes);

    if (buffer)
    {
        CPU::SPI4_::Receive(buffer, numBytes);

        if (buffer[0] == CommandGenerator::FreqMeasure)
        {
            BitSet32 bs;
            for (int i = 0; i < 4; i++)
            {
                bs.byte[i] = buffer[i + 1];
            }
            FrequencyMeter::SetMeasure(bs.word);
        }
        else if (buffer[0] == CommandGenerator::Log)
        {
            char buf[LENGTH_SPI_BUFFER];
            for (int i = 0; i < LENGTH_SPI_BUFFER - 1; i++)
            {
                buf[i] = (char)buffer[i + 1];
            }
            buf[LENGTH_SPI_BUFFER - 1] = '\0';
            Console::AddString(buf);
        }
    }

    free(buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TuneChannel(Chan ch)
{
    EnableChannel(ch, CHANNEL_ENABLED(ch));

    FORM(ch)->TuneGenerator(ch);
}
