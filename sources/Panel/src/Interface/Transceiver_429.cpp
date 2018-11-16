#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Transceiver::Send(Packet *packet)
{
    if (CPU::SPI4_::IsReady())
    {
        return false;                   // Сразу же выходим, если процессор находится в режиме ожидания. Это должно уменьшить сбои
    }

    CPU::SPI4_::WaitFreedom();          // Иначе ждём, пока прибор перейдёт из состояния "Занят" в состояние "Готов"

    CPU::SPI4_::Transmit(packet->Begin(), packet->Size());

    Packet recvPacket;

    CPU::SPI4_::Receive(recvPacket.Begin(), recvPacket.Size());

    return recvPacket.IsEquals(packet);
}
