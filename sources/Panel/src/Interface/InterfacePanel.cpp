#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Utils/Buffer.h"
#include "InterfacePanel.h"
#include "Command.h"
#include "structs.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "FDrive/FDrivePanel.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Utils/Debug.h"
#include "InterfacePanel.h"
#include <stdlib.h>
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Send(const Buffer &buffer)
{
    Send(buffer.Data(), (uint16)buffer.Length());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{
    static uint timePrev = 0;
    
    if (TIME_MS - timePrev > 100)
    {
        ProcessDataFPGA();
    
        timePrev = TIME_MS;
    }
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessDataFPGA()
{
    /// \todo Процесс обмена прерывается иногда. Нужно проверять информацию на ошибки

    uint8 command = Command::RequestData;

    Send(&command, 1);

    uint numBytes = BytesForReceive();

    while (numBytes > 0)         // Принятое значение означает число байт, готовых для передачи вспомогательным процессором
    {
        ReceiveAndRun(numBytes);
        numBytes = BytesForReceive();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ReceiveAndRun(uint numBytes)
{
    uint8 *buffer = (uint8 *)malloc(numBytes);

    static int count = 0;

    if (buffer)
    {
        if(numBytes != 257)
        {
            LOG_WRITE("нужно принять %d байт", numBytes);
        }

        count += numBytes;

        if (!CPU::SPI4_::Receive(buffer, numBytes))
        {
            LOG_WRITE("Ошибка приёма - данные не приняты");
        }

        if (*buffer == Command::FreqMeasure)
        {
            BitSet32 bs;
            for (int i = 0; i < 4; i++)
            {
                bs.byte[i] = buffer[i + 1];
            }
            FrequencyMeter::SetMeasure(bs.word);
        }
        else if (*buffer == Command::Log)
        {
            /*
            LOG_WRITE("log");
            */
            char buf[LENGTH_SPI_BUFFER];
            for (int i = 0; i < LENGTH_SPI_BUFFER - 1; i++)
            {
                buf[i] = (char)buffer[i + 1];
            }
            buf[LENGTH_SPI_BUFFER - 1] = '\0';
            Console::AddString(buf);
        }
        else if (*buffer == Command::FDrive_Mount)
        {
            if(count)
            {
                //LOG_WRITE("принято %d после монтирования %d %d %d %d %d", count, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);
            }
            count = 0;
            if (buffer[1] == 0)
            {
                LOG_WRITE("unmout");
            }
            else
            {
                LOG_WRITE("mount");
            }

            FDrive::HandlerInterface(buffer);
        }
        else
        {
            LOG_WRITE("Неизвестная команда %d, %d байт", *buffer, numBytes);
        }
    }
    else
    {
        LOG_WRITE("Нет памяти");
    }

    free(buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Request(Data *request, Data *answer)
{
    Send(request->GetData(), request->GetSize());

    uint numBytes = BytesForReceive();  /// Узнаём количество байт для приёма

    answer->Release();

    if(numBytes)
    {
        if(answer->Init(numBytes))
        {
            CPU::SPI4_::Receive(answer->GetData(), answer->GetSize());
        }
        else
        {
            LOG_WRITE("Не хватает памяти для буфера");
        }
    }

    return numBytes != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::BytesForReceive()
{
    uint numBytes = 0;
    CPU::SPI4_::Receive(&numBytes, 2);
    return numBytes;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(const uint8 *buffer, uint size)
{
    if(*buffer != 0)
    {
        LOG_WRITE("Посылаю %d байт", size);
    }

    /*
        Процесс передачи состоит из двух частей.
        1. Сначала идут два байта - количество передаваемых данных
        2. Затем идут непосредственно данные
    */

    CPU::SPI4_::Transmit(&size, 2);

    const uint8 *pointer = buffer;
    while (size > 0)
    {
        uint sizeChunk = (size > 1024u) ? 1024u : size; // Размер куска для передачи

        size -= sizeChunk;

        CPU::SPI4_::Transmit(pointer, sizeChunk);

        pointer += sizeChunk;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size, uint8 command) : data(0)
{
    Init(size);

    if(size)
    {
        data[0] = command;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::~Data()
{
    Release();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::Init(uint _size)
{
    if(data)
    {
        delete data;
    }

    size = _size;

    if(size)
    {
        data = (uint8 *)malloc(size);

        return data != 0;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Data::Release()
{
    if(data)
    {
        delete data;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::IsEmpty() const
{
    return data == 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Interface::Data::GetData()
{
    return data;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::Data::GetSize() const
{
    return size;
}
