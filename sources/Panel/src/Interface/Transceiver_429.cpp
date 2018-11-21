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
bool Transceiver::Send(Message *message)
{
    LOG_WRITE_FINALIZE("Передаю %d байт. Команда %d", message->Size(), *message->Data());


    SPI4_::WaitFalling();                                               // Ожидаем перехода флага готовности прибора в состояние "свободен"

    bool result = false;

    for (int i = 1; i < 3; i++)
    {
        uint size = message->Size();

        if (!SPI4_::Transmit(&size, 4))                                 // Передаём размер передаваемых данных
        {
            break;
        }

        if (!SPI4_::Transmit(message->Data(), message->Size()))         // Передаём непосредственно данные
        {
            break;
        }

        uint newSize = 0;
        if (!SPI4_::Receive(&newSize, 4))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
        {
            break;
        }

        if (newSize == message->Size())
        {
            result = true;
        }

        if (SPI4_::ReceiveAndCompare(message->Data(), message->Size()))
        {
            result = true;
        }
    }

    static uint all = 0;
    static uint failed = 0;
    all++;


    if (!result)
    {
        failed++;
        LOG_WRITE_FINALIZE("%d из %d пакетов не передано. %f процентов потеряно", failed, all, (float)failed / all * 100.0f);
    }

    return result;
}
