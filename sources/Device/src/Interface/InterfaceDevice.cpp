#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Message.h"
#include "Transceiver.h"
#include "HandlersDevice.h"
#include "InterfaceDevice.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/FPGA.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#include "FreqMeter/FreqMeter.h"
#include "Settings/CalibrationSettings.h"
#include "Utils/Debug.h"
#include "Utils/Queue.h"
#include "Utils/StringUtils.h"
#include "Command.h"
#include "structs.h"
#include <cstdlib>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint  Interface::freqForSend = MAX_UINT;
Queue Interface::messages;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Interface::Update()
{
    CPU::SetReady();

    uint size = 0;

    if (SPI1_::Receive(&size, 4, 10))                                                           // Узнаём размер принимаемого сообщения
    {
        Message first;              // Сюда принимаем первое сообщение
        Message second;             // Сюда принимаем второе сообщение

        uint timeout = size > 100U ? 200U : 10U;

        if (first.AllocateMemory(size))
        {
            if (SPI1_::Receive(first.Data(), first.Size(), timeout))                            // Принимаем данные
            {
                if (SPI1_::Transmit(&size, 4, timeout))                                         // Передаём его размер
                {
                    if (SPI1_::Transmit(first.Data(), first.Size(), timeout))                   // И данные
                    {
                        if (SPI1_::Receive(&size, 4, 10))
                        {
                            if (second.AllocateMemory(size))                                    // Второй раз сообщение будем принимать в этот буфер
                            {
                                if (SPI1_::Receive(second.Data(), second.Size(), timeout))      // Что и делаем
                                {
                                    size = second.Size();

                                    if (SPI1_::Transmit(&size, 4, timeout))
                                    {
                                        if (SPI1_::Transmit(second.Data(), second.Size(), timeout))
                                        {
                                            if (second.IsEquals(&first))                        // Проверяем, совпали ли оба принятых сообщения
                                            {
                                                Handlers::Processing(&first);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    CPU::SetBusy();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Interface::AddMessageForTransmit(Message *message)
{
    if (!messages.Push(message))
    {
        delete message;
        /// \tood Здесь нужно сообщать об ошибке. Сделать так, чтобы сообщения об ошибках обладали привилегированным статусом - помещать их в очередь даже когда там нет места (освободить)
        // LOG_ERROR("Очередь переполнена");
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Message::Transmit()
{
    Interface::AddMessageForTransmit(this);
}
