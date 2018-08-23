#include "defines.h"
#include "Generator.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Command.h"
#include "structs.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Generator::enabledCh[Chan::Number] = {true, true};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Chan ch, bool enable)
{
    enabledCh[ch] = enable;
    uint8 buffer[3] = {CommandPanel::ENABLE_CHANNEL, (uint8)ch, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Generator::ChannelEnabled(Chan ch)
{
    return enabledCh[ch];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Register reg, uint value)
{
    INIT_BIT_SET_32(set, value);

    uint8 buffer[6] = {CommandPanel::WRITE_REGISTER, (uint8)reg, set.byte0, set.byte1, set.byte2, set.byte3};
    SendToInterface(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    uint8 buffer[2] = {CommandPanel::MODE_DEBUG, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    uint8 command = CommandPanel::RUN_RESET;
    SendToInterface(&command, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Chan ch, WaveForm form)
{
    uint8 buffer[3] = {CommandPanel::SET_FORM_WAVE, (uint8)ch, form};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Update()
{
    static uint timePrev = 0;

    if(TIME_MS - timePrev > 100)
    {
        uint8 trans[LENGTH_SPI_BUFFER] = {0};

        SendToInterface(trans, LENGTH_SPI_BUFFER);

        timePrev = TIME_MS;
    }}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ExecuteCommand(uint8 *buffer, int)
{
    if(buffer[0] == CommandGenerator::COM_FREQ_MEASURE)
    {
        BitSet32 data;
        for(int i = 0; i < 4; i++)
        {
            data.byte[i] = buffer[i + 1];
        }
        FrequencyMeter::SetMeasure(data.word);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetParameter(Chan ch, WaveParameter param, float value)
{
    static const CommandPanel commands[WaveParameter::Number] =
    {
        CommandPanel::SET_FREQUENCY,
        CommandPanel::COMMAND_NONE,
        CommandPanel::SET_AMPLITUDE,
        CommandPanel::SET_OFFSET,
        CommandPanel::SET_DURATION,
        CommandPanel::SET_DUTYRATIO,
        CommandPanel::SET_PHASE,
        CommandPanel::SET_DELAY,
        CommandPanel::SET_DEPTH_MODULATION,
        CommandPanel::SET_POLARITY,
        CommandPanel::SET_DURATION_RISE,
        CommandPanel::SET_DURATION_FALL,
        CommandPanel::SET_DURATION_STADY,
        CommandPanel::SET_DUTY_FACTOR
    };

    uint8 buffer[6] = {(uint8)commands[param], (uint8)ch};
    memcpy(&buffer[2], &value, 4);
    SendToInterface(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SendToInterface(uint8 *data, int size)
{
    /*
        Алгоритм передачи.
        Передача совмещена с приёмом.
        В случае, если у генератора нет данных для передачи, он возвращает принятую информацию.
        В случае наличия информации для передачи он передаёт её.
    */
   
    static uint8 trans[LENGTH_SPI_BUFFER];          // Это массив для передаваемых данных
    static uint8 recv[LENGTH_SPI_BUFFER];           // Это массив для принимаемых данных

    memcpy(trans, data, (uint)size);

    CPU::SPI4_::TransmitReceive(trans, recv, LENGTH_SPI_BUFFER, 100);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ReadAndRunFromInterface()
{
    while(CPU::SPI4_::IsBusy())
    {
    }

    uint8 trans[LENGTH_SPI_BUFFER] = {0};
    uint8 recv[LENGTH_SPI_BUFFER];

    if(CPU::SPI4_::TransmitReceive(trans, recv, LENGTH_SPI_BUFFER, 100))
    {
        if(recv[0] == CommandGenerator::COM_FREQ_MEASURE)
        {
            BitSet32 data;
            for(int i = 0; i < 4; i++)
            {
                data.byte[i] = recv[1 + i];
            }
    
            FrequencyMeter::SetMeasure(data.word);
        }
    }
}
