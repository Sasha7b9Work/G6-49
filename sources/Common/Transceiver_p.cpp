#include "defines.h"
#include "common/Transceiver.h"
#include "Display/Colors.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Debug.h"


void Transceiver::Transmit(SimpleMessage *message)
{
    uint timeout = (message->Size() > 1000U) ? 100U : 10U;

    bool result = false;
    
    uint failed = 0;

    while (!result)
    {
        HAL_SPI4::WaitFalling();                                                // Ожидаем перехода флага готовности прибора в состояние "свободен"

        for (int i = 0; i < 2; i++)
        {
            HAL_SPI4::Transmit(message->Size(), 10);                               // Передаём размер передаваемых данных

            HAL_SPI4::Transmit(message->TakeData(), message->Size(), timeout);         // Передаём непосредственно данные

            uint newSize = 0;
            HAL_SPI4::Receive(&newSize, 4, 10);                                    // Теперь принимаем размер данных, которые хочет передать нам устройство

            uint trashedBytes = HAL_SPI4::ReceiveAndCompare(message->TakeData(), message->Size());

            result = (trashedBytes == 0);
        }

        if (!result)
        {
            /// \todo Затычка на то, что при быстром нажатии кнопки ФОРМА происходит зависание. По хорошему надо бы разобраться в чем дело.
            failed++;
            if(failed > 5)
            {
                break;
            }
        }
    };
}


bool Transceiver::Receive(SimpleMessage *message)
{
    DEBUG_POINT_0;

    HAL_SPI4::WaitFalling();

    DEBUG_POINT_0;

    int size = 0;
    HAL_SPI4::Receive(&size, 4, 10);

    DEBUG_POINT_0;

    if (message->AllocateMemory(size))
    {
        DEBUG_POINT_0;
        HAL_SPI4::Receive(message->TakeData(), message->Size(), 50);

        DEBUG_POINT_0;

        return true;
    }

    DEBUG_POINT_0;

    return message->Size() != 0;
}
