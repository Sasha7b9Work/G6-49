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
#include "Handlers.h"
#include "InterfacePanel.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Send(const Buffer &buffer)
{
    Send(buffer.Data(), (uint16)buffer.Length());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(uint8 *buffer, uint size)
{
    Message message;
    message.CreateAllocate(buffer, size);

    Transceiver::Transmit(&message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Send(Message *message)
{
    Transceiver::Transmit(message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Interface::Update()
{
    static uint time = 0;

    if (TIME_MS - time < 100)
    {
        return;
    }

    time = TIME_MS;

    Message message(1, Command::RequestData);

    Transceiver::Transmit(&message);

    if (Transceiver::Receive(&message))
    {
        if (Handlers::Processing(&message))
        {
            time = 0;
            Update();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void Interface::ReceiveAndRun(uint numBytes)
{
        if (*buffer == Command::Log)
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
}
*/

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
