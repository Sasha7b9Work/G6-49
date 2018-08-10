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
void Generator::EnableChannel(Channel ch, bool enable)
{
    uint8 buffer[3] = {ENABLE_CHANNEL, (uint8)ch, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::LoadRegister(Name_Register reg, uint value)
{
    INIT_BIT_SET_32(set, value);

    uint8 buffer[6] = {WRITE_REGISTER, (uint8)reg, set.byte0, set.byte1, set.byte2, set.byte3};
    SendToInterface(buffer, 6);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    uint8 buffer[2] = {MODE_DEBUG, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset()
{
    uint8 command = RUN_RESET;
    SendToInterface(&command, 1);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetFormWave(Channel ch, WaveForm form)
{
    uint8 buffer[3] = {SET_FORM_WAVE, (uint8)ch, form.ToValue()};
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
    if(buffer[0] == COM_FREQ_MEASURE)
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
void Generator::SetParameter(Channel ch, Type_WaveParameter param, float value)
{
    static const CommandPanel commands[NumParameters] =
    {
        SET_FREQUENCY,
        COMMAND_NONE,
        SET_AMPLITUDE,
        SET_OFFSET,
        SET_DURATION,
        SET_DUTYRATIO,
        SET_PHASE,
        SET_DELAY
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

    while (CPU::SPI4_::IsBusy())                    // Ждём, пока генератор будет готов к приёму информации
    {
    };
    
    static uint8 trans[LENGTH_SPI_BUFFER];          // Это массив для передаваемых данных
    static uint8 recv[LENGTH_SPI_BUFFER];           // Это массив для принимаемых данных

    memcpy(trans, data, (uint)size);

    CPU::SPI4_::TransmitReceive(trans, recv, LENGTH_SPI_BUFFER, 5); // Передаём данные

    if(recv[0] != 0)                    // Если первый принятый байт не равен нулю, то у генератора есть данные для передачи
    {
        ReadAndRunFromInterface();      // Принимаем их
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ReadAndRunFromInterface()
{
    while(CPU::SPI4_::IsBusy())
    {
    }

    uint8 trans[LENGTH_SPI_BUFFER] = {0};
    uint8 recv[LENGTH_SPI_BUFFER];

    CPU::SPI4_::TransmitReceive(trans, recv, LENGTH_SPI_BUFFER, 5);

    if(recv[0] == COM_FREQ_MEASURE)
    {
        BitSet32 data;
        for(int i = 0; i < 4; i++)
        {
            data.byte[i] = recv[1 + i];
        }

        FrequencyMeter::SetMeasure(data.word);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::MasterSynchro()
{
    uint8 txByte = SPI_MASTER_SYNBYTE;
    uint8 rxByte = 0x00;
    
    do
    {
        CPU::SPI4_::TransmitReceive(&txByte, &rxByte, 1, HAL_MAX_DELAY);
    } while(rxByte != SPI_SLAVE_SYNBYTE);
}
