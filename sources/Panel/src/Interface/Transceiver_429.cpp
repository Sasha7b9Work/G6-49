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
    SPI4_::WaitFalling();                                               // Ожидаем перехода флага готовности прибора в состояние "свободен"

    Message recvMessage;                                                // Сюда будем принимать ответ

    for (int i = 0; i < 2; i++)
    {
        uint size = message->Size();
        if (!SPI4_::Transmit(&size, 4))                                 // Передаём размер передаваемых данных
        {
            return false;
        }

        if (!SPI4_::Transmit(message->Data(), message->Size()))         // Передаём непосредственно данные
        {
            return false;
        }

        uint newSize = 0;
        if (!SPI4_::Receive(&newSize, 4))                               // Теперь принимаем размер данных, которые хочет передать нам устройство
        {
            return false;
        }

        recvMessage.AllocateMemory(newSize);                            // Выделяем необходимое количество памяти для принимаемых данных
        if (!SPI4_::Receive(recvMessage.Data(), recvMessage.Size()))    // И принимаем ранее переданную информацию
        {
            return false;
        }
    }

    bool result = recvMessage.IsEquals(message);

    if (!result)
    {
        LOG_WRITE_FINALIZE("Принятый пакет не совпадает с переданным");
    }

    return result;
}
