#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#include "Hardware/Modules/SPI.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Transceiver::Send(Packet *packet)
{
    if (SPI4_::IsReady())
    {
        return false;                                               // Сразу же выходим, если процессор находится в режиме ожидания. Это должно уменьшить сбои
    }

    SPI4_::WaitFreedom();                                      // Иначе ждём, пока прибор перейдёт из состояния "Занят" в состояние "Готов"

    if (!SPI4_::Transmit(packet->Begin(), packet->Size()))     // Засылаем пакет
    {
        return false;
    }

    Packet recvPacket;

    if (!SPI4_::Receive(recvPacket.Begin(), recvPacket.Size()))    // И принимаем ответ
    {
        return false;
    }

    return recvPacket.IsEquals(packet);                                 // Возвращаем false, если переданные и принятые данные не совпадают
}
