#include "defines.h"
#include "Generator.h"
#include "Log.h"
#include "Menu/MenuItems.h"
#include "Hardware/CPU.h"
#include "Command.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Generator::EnableChannel(Channel ch, bool enable)
{
    uint8 buffer[3] = {ENABLE_CHANNEL, (uint8)ch, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::SetDebugMode(bool enable)
{
    uint8 buffer[2] = {MODE_DEBUG, (uint8)(enable ? 1 : 0)};
    SendToInterface(buffer, 2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::Reset(void)
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
void Generator::SendToInterface(uint8 *, int)
{
    /*
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
            ShiftToLeft(recvBuffer, LENGTH_SPI_BUFFER);
        }
        while (memcmp(buffer, recvBuffer, LENGTH_SPI_BUFFER) != 0);
        
        command = command;
        ch = ch;
    }
    */
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::TestSend(uint8 *data, int size)
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

        CPU::SPI4_::Transmit(buffer, LENGTH_SPI_BUFFER, 10);                               // Первая передача

        do
        {
            memset(recvBuffer, 0, LENGTH_SPI_BUFFER);                                       // Очищаем приёмный буфер
            CPU::SPI4_::TransmitReceive(buffer, recvBuffer, LENGTH_SPI_BUFFER, 5);
            ShiftToLeft(recvBuffer, LENGTH_SPI_BUFFER);
        } while (memcmp(buffer, recvBuffer, LENGTH_SPI_BUFFER) != 0);

        command = command;
        ch = ch;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::ShiftToLeft(uint8 *buffer, int length)
{
    for (int i = 0; i < length; i++)
    {
        buffer[i] = (uint8)(buffer[i] << 1);
        if (i < length - 1)
        {
            if (GetBit(buffer[i + 1], 7))
            {
                SetBit(buffer[i], 0);
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Generator::MasterSynchro(void)
{
    uint8 txByte = SPI_MASTER_SYNBYTE;
    uint8 rxByte = 0x00;
    
    do
    {
        CPU::SPI4_::TransmitReceive(&txByte, &rxByte, 1, HAL_MAX_DELAY);
    } while(rxByte != SPI_SLAVE_SYNBYTE);
}
