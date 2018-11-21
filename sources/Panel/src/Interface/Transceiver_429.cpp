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
    static uint time = 0;

    while (TIME_MS - time < 5000)
    {
    }

    time = TIME_MS;

    SPI4_::WaitFalling();                                               // Ожидаем перехода флага готовности прибора в состояние "свободен"

    Message recvMessage;                                                // Сюда будем принимать ответ

    for (int i = 1; i < 3; i++)
    {
        uint size = message->Size();

        LOG_WRITE_FINALIZE("Передаю размер %d-й раз", i);

        if (!SPI4_::Transmit(&size, 4))                                 // Передаём размер передаваемых данных
        {
            LOG_WRITE_FINALIZE("..... неудача");
            break;
        }

        LOG_WRITE_FINALIZE("Передан размер %d", size);

        LOG_WRITE_FINALIZE("Передаю данные %d-й раз", i);

        if (!SPI4_::Transmit(message->Data(), message->Size()))         // Передаём непосредственно данные
        {
            LOG_WRITE_FINALIZE("..... неудача");
            break;
        }

        LOG_WRITE_FINALIZE("Принимаю размер %d-й раз", i);

        uint newSize = 0;
        if (!SPI4_::Receive(&newSize, 4))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
        {
            LOG_WRITE_FINALIZE("..... неудача");
            break;
        }

        LOG_WRITE_FINALIZE("Принят размер %d", newSize);

        LOG_WRITE_FINALIZE("Принимаю данные %d-й раз", i);

        recvMessage.AllocateMemory(newSize);                            // Выделяем необходимое количество памяти для принимаемых данных
        if (!SPI4_::Receive(recvMessage.Data(), recvMessage.Size()))    // И принимаем ранее переданную информацию
        {
            LOG_WRITE_FINALIZE("..... неудача");
            break;
        }

        if (recvMessage.IsEquals(message))
        {
            LOG_WRITE_FINALIZE("Принятые в %d раз данные совпадают с переданными", i);
        }
        else
        {
            LOG_WRITE_FINALIZE("Принятые в %d раз данные не совпадают с переданными", i);
        }
    }

    bool result = recvMessage.IsEquals(message);

    if (!result)
    {
        LOG_WRITE_FINALIZE("Принятый пакет не совпадает с переданным");
    }
    else
    {
        LOG_WRITE_FINALIZE("Пакет передан успешно");
    }

    return result;
}
