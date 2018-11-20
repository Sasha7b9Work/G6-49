#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Message.h"
#include "Transceiver.h"
#include "Hardware/CPU.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Send(uint8 *buffer, uint size)
{
    Message message;

    if (!message.CreateFromRawData(buffer, size))
    {
        /// Повторяем передачу, пока не передадим
        while (!Send(&message))
        {
        }
    }
    else
    {
        LOG_WRITE_FINALIZE("Нет память для сообщения");
    }
}
