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
    SPI4_::WaitFalling();                                           // Ожидаем перехода флага готовности прибора в состояние "свободен"

    if (!SPI4_::Transmit(packet->Begin(), packet->Size()))          // Засылаем пакет
    {
        return false;
    }

    Packet recvPacket;

    if (!SPI4_::Receive(recvPacket.Begin(), recvPacket.Size()))     // И принимаем ответ
    {
        return false;
    }

    return recvPacket.IsEquals(packet);                                 // Возвращаем false, если переданные и принятые данные не совпадают
}
