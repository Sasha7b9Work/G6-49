#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "GeneratorPanel.h"
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
#include <math.h>
#include <string.h>
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Принять и обработать данные от ПЛИС
static void ProcessDataFPGA();
/// Заслать в генератор данные
static void SendToInterface(const uint8 *buffer, uint16 size);

static void SendToInterface(const Buffer &buffer);
/// Принять numBytes байт от ПЛИС и выполнить их
static void ReceiveAndRun(uint16 numBytes);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Chan ch, bool enable)
{
    SendToInterface(Buffer(Command::EnableChannel, ch, enable ? 1u : 0u));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadStartMode(Chan ch, int mode)
{
    SendToInterface(Buffer(Command::SetStartMode, ch, (uint8)mode));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Register reg, uint64 value)
{
    INIT_BIT_SET_64(bitSet, value);

    uint8 buffer[10] = {Command::WriteRegister, (uint8)reg,   bitSet.byte0, bitSet.byte1, bitSet.byte2, bitSet.byte3,
                                                                    bitSet.byte4, bitSet.byte5, bitSet.byte6, bitSet.byte7};
    SendToInterface(buffer, 10);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    SendToInterface(Buffer(Command::ModeDebug, enable ? 1u : 0u));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    SendToInterface(Buffer(Command::RunReset));
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
        SendToInterface(Buffer(Command::SetFormWave, ch, form));
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
    SendToInterface(Buffer(Command::LoadFormDDS, ch));

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

    SendToInterface(Buffer((uint8)commands[param->value].command, (uint8)param->GetForm()->GetWave()->GetChannel(), (uint8)param->GetChoice()));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
static void SendToInterface(const Buffer &buffer)
{
    SendToInterface(buffer.Data(), (uint16)buffer.Length());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void SendToInterface(const uint8 *buffer, uint16 size)
{
    Command command(*buffer);
    if(Debug::ShowSends() && command.value != Command::RequestData)
    {
        //LOG_WRITE("передаю %s", command.Trace(buffer));
    }

    CPU::SPI4_::Transmit((uint8 *)&size, 2);

    const uint8 *pointer = buffer;
    while(size > 0)
    {
        uint16 sizeChunk = (size > 1024u) ? 1024u : size; // Размер куска для передачи

        size -= sizeChunk;

        CPU::SPI4_::Transmit(pointer, sizeChunk);

        pointer += sizeChunk;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessDataFPGA()
{
    uint8 command = Command::RequestData;

    SendToInterface(&command, 1);

    uint16 numBytes = 0;

    CPU::SPI4_::Receive((uint8 *)&numBytes, 2);

    if(numBytes)  // Принятое значение означает число байт, готовых для передачи вспомогательным процессором
    {
        ReceiveAndRun(numBytes);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void ReceiveAndRun(uint16 numBytes)
{
    LOG_WRITE("Требуется принять %d байт", numBytes);

    uint8 *buffer = (uint8 *)malloc(numBytes);

    if (buffer)
    {
        CPU::SPI4_::Receive(buffer, numBytes);

        if (buffer[0] == Command::FreqMeasure)
        {
            BitSet32 bs;
            for (int i = 0; i < 4; i++)
            {
                bs.byte[i] = buffer[i + 1];
            }
            FrequencyMeter::SetMeasure(bs.word);
        }
        else if (buffer[0] == Command::Log)
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
