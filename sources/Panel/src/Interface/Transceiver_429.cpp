#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#include "Hardware/Modules/SPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Transceiver::Send(Message *message)
{
    uint timeout = (message->Size() > 1000U) ? 200U : 10U;

    SPI4_::WaitFalling();                                                   // Ожидаем перехода флага готовности прибора в состояние "свободен"

    bool result = false;

    for (int i = 1; i < 3; i++)
    {
        uint size = message->Size();

        if (!SPI4_::Transmit(&size, 4, 10))                                 // Передаём размер передаваемых данных
        {
//            LOG_WRITE_FINALIZE("Не могу передать размер");
        }

        if (!SPI4_::Transmit(message->Data(), message->Size(), timeout))    // Передаём непосредственно данные
        {
//            LOG_WRITE_FINALIZE("Не могу передать данные");
        }

        uint newSize = 0;
        if (!SPI4_::Receive(&newSize, 4, 10))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
        {
//            LOG_WRITE_FINALIZE("Не могу принять размер");
        }

        if (newSize == message->Size())
        {
            result = true;
        }
        else
        {
            result = false;
//            LOG_WRITE_FINALIZE("Размеры не совпадают %d %d", message->Size(), newSize);
        }

        if (SPI4_::ReceiveAndCompare(message->Data(), message->Size()))
        {
            result = true;
        }
        else
        {
            result = false;
//            LOG_WRITE_FINALIZE("Сравнение данных прошло неудачно");
        }
    }

    static uint all = 0;
    static uint failed = 0;
    all++;


    if (!result)
    {
        failed++;
//        LOG_WRITE_FINALIZE("%d из %d пакетов не передано. %f процентов потеряно", failed, all, (float)failed / all * 100.0f);
    }

    return result;
}
