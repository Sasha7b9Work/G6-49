#include "defines.h"
#include "log.h"
#include "Message.h"
#include "Transceiver.h"
#include "HandlersDevice.h"
#include "InterfaceDevice.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/Generator_d.h"
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


namespace Interface
{
    /// Добавляет сообщение в очередь отправки. Если очередь заполнена, сообщение удаляется и функция возвращает false
    bool AddMessageForTransmit(SimpleMessage *message);
    /// Очередь сообщений, ожидающих отправки
    Queue outbox;
    /// Ненулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    uint freqForSend = MAX_UINT;
};



Queue &Interface::GetOutbox()
{
    return outbox;
}


void Interface::ResetFreqForSend()
{
    freqForSend = MAX_UINT;
}


void Interface::Update()
{
    CPU::SetReady();

    uint size = 0;

    if (SPI1_::Receive(&size, 4, 10))                                                           // Узнаём размер принимаемого сообщения
    {
        SimpleMessage first;              // Сюда принимаем первое сообщение
        SimpleMessage second;             // Сюда принимаем второе сообщение

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


bool Interface::AddMessageForTransmit(SimpleMessage *message)
{
    SimpleMessage *clone = message->Clone();

    if (!outbox.Push(clone))
    {
        delete clone;
        /// \tood Здесь нужно сообщать об ошибке. Сделать так, чтобы сообщения об ошибках обладали привилегированным статусом - помещать их в очередь даже когда там нет места (освободить)
        // LOG_ERROR("Очередь переполнена");
        return false;
    }

    return true;
}


void SimpleMessage::Transmit()
{
    Interface::AddMessageForTransmit(this);
}
