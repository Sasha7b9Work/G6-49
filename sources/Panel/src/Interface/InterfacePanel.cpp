#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Utils/Buffer.h"
#include "InterfacePanel.h"
#include "Command.h"
#include "structs.h"
#include "Display/Console.h"
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

    uint16 numBytes = 0;

    CPU::SPI4_::Receive(&numBytes, 2);

    while (numBytes > 0)         // Принятое значение означает число байт, готовых для передачи вспомогательным процессором
    {
        ReceiveAndRun(numBytes);
        CPU::SPI4_::Receive(&numBytes, 2);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Request(Data *request, Data *answer)
{
    Interface::Send(request->GetData(), request->GetSize());

    uint16 numBytes = 0;

    CPU::SPI4_::Receive(&numBytes, 2);  /// Узнаём количество байт для приёма

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
    else
    {
        LOG_WRITE("Нет данных для приёма");
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(const uint8 *buffer, uint size)
{
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
void Interface::ReceiveAndRun(uint16 numBytes)
{
    uint8 *buffer = (uint8 *)malloc(numBytes);

    if (buffer)
    {
        CPU::SPI4_::Receive(buffer, numBytes);

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
            char buf[LENGTH_SPI_BUFFER];
            for (int i = 0; i < LENGTH_SPI_BUFFER - 1; i++)
            {
                buf[i] = (char)buffer[i + 1];
            }
            buf[LENGTH_SPI_BUFFER - 1] = '\0';
            Console::AddString(buf);
        }
        else if(buffer[0] == Command::FDrive_Mount)
        {
            FDrive::HandlerInterface(buffer);
        }
    }

    free(buffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size) : data(0)
{
    Init(size);
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
