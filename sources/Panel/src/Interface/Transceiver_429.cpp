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
bool Transceiver::Send(Packet *packet)
{
    SPI4_::WaitFalling();                                               // Ожидаем перехода флага готовности прибора в состояние "свободен"

    Packet recvPacket;                                                  // Сюда будем принимать ответ

    for (int i = 0; i < 2; i++)                                         // Дважды повторим передачу информации, чтобы устройство имело возможность её сравнить и сделать вывод о её достоверности
    {
        if (!SPI4_::Transmit(packet->Begin(), packet->Size()))          // Засылаем пакет
        {
            LOG_WRITE_FINALIZE("Передать не удалось");
            return false;
        }

        if (!SPI4_::Receive(recvPacket.Begin(), recvPacket.Size()))     // И принимаем ответ
        {
            LOG_WRITE_FINALIZE("Подтверждения не дождались");
            return false;
        }
    }

    if (!recvPacket.IsEquals(packet))
    {
        LOG_WRITE_FINALIZE("Пакеты не совпадают");
    }

    return recvPacket.IsEquals(packet);                                 // Возвращаем false, если переданные и принятые данные не совпадают
}
