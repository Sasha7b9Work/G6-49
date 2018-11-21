#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Transceiver.h"
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
#include "Hardware/Modules/SPI.h"
#include "Utils/Array.h"
#include "Utils/Debug.h"
#include "InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Send(const Buffer &buffer)
{
    Send(buffer.Data(), (uint16)buffer.Length());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(RawData *message)
{
    Send(message->Data(), message->Size());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ProcessDataFPGA()
{
    /// \todo Процесс обмена прерывается иногда. Нужно проверять информацию на ошибки
    
    //uint8 command = Command::RequestData;
    
    //LOG_WRITE_FINALIZE("Делаем запрос %s", __FUNCTION__);
    
    uint8 command = Command::Test;

    Send(&command, 1);

    uint numBytes = BytesForReceive();
    
    //LOG_WRITE_FINALIZE("Нужно принять %d байт", numBytes);

    while (numBytes > 0)         // Принятое значение означает число байт, готовых для передачи вспомогательным процессором
    {
        ReceiveAndRun(numBytes);
        numBytes = BytesForReceive();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Request(Data *request, Data *answer)
{
    Send(request->GetData(), request->GetSize());

    uint numBytes = BytesForReceive();  /// Узнаём количество байт для приёма

    answer->Release();

    if(numBytes)
    {
        if(answer->Init(numBytes))
        {
            SPI4_::Receive(answer->GetData(), answer->GetSize(), 100);
        }
        else
        {
            LOG_WRITE("Не хватает памяти для буфера");
        }
    }

    return numBytes != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::BytesForReceive()
{
    uint numBytes = 0;
    if(!SPI4_::Receive(&numBytes, 2, 10))
    {
        LOG_ERROR("Не получил размер данных. Проверьте!");
    }
    return numBytes;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(uint8 *buffer, uint size)
{
    Message message;
    message.CreateFromRawData(buffer, size);

    Transceiver::Send(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{
    return;

    static uint time = 0;

    if (TIME_MS - time < 100)
    {
        return;
    }

    Message message;
    message.AllocateMemory(1);
    message.Put(Command::RequestData);


}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::ReceiveAndRun(uint numBytes)
{
    uint8 *buffer = (uint8 *)std::malloc(numBytes);

    static int count = 0;

    if (buffer)
    {
        LOG_WRITE("нужно принять %d байт", numBytes);

        count += numBytes;

        if(!SPI4_::Receive(buffer, numBytes, 100))
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
            LOG_WRITE("log");
            char buf[LENGTH_SPI_BUFFER];
            for (int i = 0; i < LENGTH_SPI_BUFFER - 1; i++)
            {
                buf[i] = (char)buffer[i + 1];
            }
            buf[LENGTH_SPI_BUFFER - 1] = '\0';
            Console::AddString(buf);
        }
        else if(*buffer == Command::FDrive_Mount)
        {
            count = 0;
            if(buffer[1] == 0)
            {
                LOG_WRITE("unmout");
            }
            else
            {
                LOG_WRITE("mount");
            }

            FDrive::HandlerInterface(buffer);
        }
        else if(*buffer == Command::Test)
        {
            LOG_WRITE("Получено в ответ на Test %d байт", numBytes);
        }
        else
        {
            LOG_WRITE("принято %d байт в ошибочной команде %d", numBytes, *buffer);
        }
    }
    else
    {
        LOG_WRITE("Нет памяти");
    }

    std::free(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::Data(uint size, uint8 command) : data(0)
{
    Init(size);

    if(size)
    {
        data[0] = command;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Interface::Data::~Data()
{
    Release();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::Init(uint _size)
{
    if(data)
    {
        delete data;
    }

    size = _size;

    if(size)
    {
        data = (uint8 *)std::malloc(size);

        return data != 0;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Data::Release()
{
    if(data)
    {
        delete data;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::Data::IsEmpty() const
{
    return data == 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Interface::Data::GetData()
{
    return data;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Interface::Data::GetSize() const
{
    return size;
}
