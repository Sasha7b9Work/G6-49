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

#include <math.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Chan ch, bool enable)
{
    uint8 buffer[3] = {CommandPanel::EnableChannel, (uint8)ch, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadStartMode(Chan ch, int mode)
{
    uint8 buffer[3] = {CommandPanel::SetStartMode, (uint8)ch, (uint8)mode};
    SendToInterface(buffer, 3);
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
    uint8 buffer[2] = {CommandPanel::ModeDebug, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    uint8 command = CommandPanel::RunReset;
    SendToInterface(&command, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Wave *w)
{
    uint8 buffer[3] = {CommandPanel::SetFormWave, (uint8)w->GetChannel(), (uint8)FORM(w->GetChannel())->value};
    SendToInterface(buffer, 3);
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
        CommandPanel::SetModeManipulation,
        CommandPanel::SetManipulation
    };

    uint8 buffer[3] = {(uint8)commands[param->value].command, (uint8)param->GetForm()->GetWave()->GetChannel(), (uint8)param->GetChoice()};

    SendToInterface(buffer, 3);
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
#ifdef OPEN
void Generator::SendToInterface(uint8 *, uint16 size)
#else
void Generator::SendToInterface(uint8 *buffer, uint16 size)
#endif
{
    if(*buffer == 0)
    {
        LOG_WRITE("%s", CommandPanel(*buffer).Trace(buffer));
    }

    CPU::SPI4_::Transmit((uint8 *)&size, 2);

    uint8 *pointer = buffer;
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
