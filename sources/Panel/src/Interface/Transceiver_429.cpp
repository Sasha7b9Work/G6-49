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

    /*
    static uint time = 0;

    if (message->Size() > 100)
    {
        while (TIME_MS - time < 5000)
        {
        }

        time = TIME_MS;
    }
    */

    SPI4_::WaitFalling();                                               // Ожидаем перехода флага готовности прибора в состояние "свободен"

    bool result = false;

    for (int i = 1; i < 3; i++)
    {
        uint size = message->Size();

        if (!SPI4_::Transmit(&size, 4))                                 // Передаём размер передаваемых данных
        {
            LOG_WRITE_FINALIZE("Не могу передать размер");
        }

        if (!SPI4_::Transmit(message->Data(), message->Size()))         // Передаём непосредственно данные
        {
            LOG_WRITE_FINALIZE("Не могу передать данные");
        }

        uint newSize = 0;
        if (!SPI4_::Receive(&newSize, 4))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
        {
            LOG_WRITE_FINALIZE("Не могу принять размер");
        }

        if (newSize == message->Size())
        {
            LOG_WRITE_FINALIZE("Размеры совпадают");
            result = true;
        }
        else
        {
            result = false;
            LOG_WRITE_FINALIZE("Размеры не совпадают %d %d", message->Size(), newSize);
            Timer::PauseOnTime(5000);
        }

        if (SPI4_::ReceiveAndCompare(message->Data(), message->Size()))
        {
            LOG_WRITE_FINALIZE("Данные совпали");
            result = true;
        }
        else
        {
            result = false;
            LOG_WRITE_FINALIZE("Сравнение данных прошло неудачно");
            Timer::PauseOnTime(5000);
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
