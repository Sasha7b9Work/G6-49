#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#include "Hardware/Modules/SPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Send(Message *message)
{
    uint timeout = (message->Size() > 1000U) ? 200U : 10U;

    bool result = false;

    while (!result)

    {
        SPI4_::WaitFalling();                                                   // Ожидаем перехода флага готовности прибора в состояние "свободен"

        for (int i = 1; i < 3; i++)
        {
            uint size = message->Size();

            if (!SPI4_::Transmit(&size, 4, 10))                                 // Передаём размер передаваемых данных
            {
            }

            if (!SPI4_::Transmit(message->Data(), message->Size(), timeout))    // Передаём непосредственно данные
            {
            }

            uint newSize = 0;
            if (!SPI4_::Receive(&newSize, 4, 10))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
            {
            }

            if (newSize == message->Size())
            {
                result = true;
            }
            else
            {
                result = false;
            }

            if (SPI4_::ReceiveAndCompare(message->Data(), message->Size()))
            {
                result = true;
            }
            else
            {
                result = false;
            }
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
