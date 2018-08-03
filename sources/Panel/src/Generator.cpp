#include "defines.h"
#include "Generator.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Command.h"
#include "structs.h"
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

    if (TIME_MS - timePrev > 1000)
    {
        /// Пишем сервисную команду для запроса данных от основной платы
        uint8 command = WRITE_SERVICE_COMMAND;
        SendToInterface(&command, 1);

        /// Считываем количество байт
        uint size = 0;
        ReadFromInterface((uint8 *)&size, 4);

        /// Читаем байты
        if (size != 0)
        {
            uint8 *buffer = (uint8 *)malloc(size);

            ReadFromInterface(buffer, (int)size);

            ExecuteCommand(buffer, (int)size);

            free(buffer);
        }

        timePrev = TIME_MS;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ExecuteCommand(uint8 *buffer, int)
{
    if(buffer[0] == FREQ_MEASURE)
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
    static const CommandWrite commands[NumParameters] =
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
    while (CPU::SPI4_::IsBusy())
    {
    };
    
    if (size > LENGTH_SPI_BUFFER)
    {
        LOG_WRITE("слишком маленький буфер");
    }
    else
    {
        static uint8 buffer[LENGTH_SPI_BUFFER];         // Это массив для передаваемых данных
        static uint8 recvBuffer[LENGTH_SPI_BUFFER];     // Это массив для принимаемых данных

        memset(buffer, 0, LENGTH_SPI_BUFFER);
        memcpy(buffer, data, (uint)size);
        
        volatile CommandWrite command = (CommandWrite)buffer[0];
        volatile Channel ch = (Channel)buffer[1];

        CPU::SPI4_::Transmit(buffer, LENGTH_SPI_BUFFER, 10);                                // Первая передача

        int counter = 0;

        do
        {
            counter++;
            memset(recvBuffer, 0, LENGTH_SPI_BUFFER);                                       // Очищаем приёмный буфер
            CPU::SPI4_::TransmitReceive(buffer, recvBuffer, LENGTH_SPI_BUFFER, 5);
        }
        while (memcmp(buffer, recvBuffer, LENGTH_SPI_BUFFER) != 0);

        if(counter > 1)
        {
            counter++;
        }
        
        command = command;
        ch = ch;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ReadFromInterface(uint8 *buffer, int size)
{
    uint8 recv[LENGTH_SPI_BUFFER];
    CPU::SPI4_::Receive(recv, LENGTH_SPI_BUFFER, 5);
    memcpy(buffer, recv, (uint)size);

    /*
    // Даём запрос на чтение size байт
    static uint8 trans[5] = {READ_COMMAND};
    BitSet32 data;
    data.word = (uint)size;
    for(int i = 0; i < 4; i++)
    {
        trans[i + 1] = data.byte[i];
    }

    SendToInterface(trans, 5);

    static uint8 recv1[LENGTH_SPI_BUFFER];  // Буфер приёма1
    static uint8 recv2[LENGTH_SPI_BUFFER];  // Буфер приёма2

    do
    {
        memset(recv1, 0, LENGTH_SPI_BUFFER);
        memset(recv2, 0, LENGTH_SPI_BUFFER);
        CPU::SPI4_::Receive(recv1, LENGTH_SPI_BUFFER, 5);
        CPU::SPI4_::Receive(recv2, LENGTH_SPI_BUFFER, 5);

    } while (memcmp(recv1, recv2, LENGTH_SPI_BUFFER) != 0);

    memcpy(buffer, recv1, (uint)size);
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TestSend()
{
    uint8 data[3] = {ENABLE_CHANNEL, 0, 1};
    int size = 3;

    while (CPU::SPI4_::IsBusy())
    {
    };

    if (size > LENGTH_SPI_BUFFER)
    {
        LOG_WRITE("слишком маленький буфер");
    }
    else
    {
        static uint8 buffer[LENGTH_SPI_BUFFER];         // Это массив для передаваемых данных
        static uint8 recvBuffer[LENGTH_SPI_BUFFER];     // Это массив для принимаемых данных

        memset(buffer, 0, LENGTH_SPI_BUFFER);
        memcpy(buffer, data, (uint)size);

        volatile CommandWrite command = (CommandWrite)buffer[0];
        volatile Channel ch = (Channel)buffer[1];

        CPU::SPI4_::Transmit(buffer, LENGTH_SPI_BUFFER, 10);                               // Первая передача
    
        do
        {
            memset(recvBuffer, 0, LENGTH_SPI_BUFFER);                                       // Очищаем приёмный буфер
            CPU::SPI4_::TransmitReceive(buffer, recvBuffer, LENGTH_SPI_BUFFER, 5);
        } while (memcmp(buffer, recvBuffer, LENGTH_SPI_BUFFER) != 0);

        command = command;
        ch = ch;
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
