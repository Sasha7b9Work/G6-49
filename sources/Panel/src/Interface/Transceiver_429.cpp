#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/Modules/SPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Transmit(Message *message)
{
    uint timeout = (message->Size() > 1000U) ? 50U : 10U;

    bool result = false;

    while (!result)

    {
        SPI4_::WaitFalling();                                                   // Ожидаем перехода флага готовности прибора в состояние "свободен"

        for (int i = 0; i < 2; i++)
        {
            SPI4_::Transmit(message->Size(), 10);                               // Передаём размер передаваемых данных

            SPI4_::Transmit(message->Data(), message->Size(), timeout);         // Передаём непосредственно данные

            uint newSize = 0;
            SPI4_::Receive(&newSize, 4, 10);                                    // Теперь принимаем размер данных, которые хочет передать нам устройство

            result = (newSize == message->Size());

            result = SPI4_::ReceiveAndCompare(message->Data(), message->Size());

        }

        static uint all = 0;
        static uint failed = 0;
        all++;


        if (!result)
        {
            failed++;
            LOG_WRITE("%d/%d %.1f %% потерь", failed, all, (float)failed / all * 100.0f);
        }

    };
}

bool Transceiver::Receive(Message *message)
{
    SPI4_::WaitFalling();

    uint size = 0;
    SPI4_::Receive(&size, 4, 10);

    message->AllocateMemory(size);

    SPI4_::Receive(message->Data(), message->Size(), 50);

    return message->Size() != 0;
}

