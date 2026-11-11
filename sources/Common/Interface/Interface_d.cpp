// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface/Messages.h"
#include "common/Interface/Transceiver_d.h"
#include "common/Interface/Handlers_d.h"
#include "common/Interface/Interface_d.h"
#ifndef LOADER
#include "FDrive/FDrive_d.h"
#endif
#include "Generator/Generator_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"
#include "common/Utils/Queue.h"
#include "Utils/StringUtils.h"
#include "common/Interface/Command.h"
#include "structs.h"
#include <cstdlib>


namespace DInterface
{
    // ќчередь сообщений, ожидающих отправки
    static Queue outbox;

    // Ќенулевое значение означает, что его следует передать в панель как измеренное частотомером значение
    uint freqForSend = MAX_UINT;
}


Queue &DInterface::GetOutbox()
{
    return outbox;
}


void DInterface::ResetFreqForSend()
{
    freqForSend = MAX_UINT;
}


void DInterface::Update()
{
}


void DInterface::Test()
{
    uint8 in = 0;

    HAL_SPI1::TransmitReceive(0x81, &in);

    if (in != 0x7e)
    {
        in = in;
    }
}


bool DInterface::AddMessageForTransmit(SimpleMessage *message)
{
    SimpleMessage *clone = message->Clone();

    if (!outbox.Push(clone))
    {
        delete clone;
        // \tood «десь нужно сообщать об ошибке. —делать так, чтобы сообщени€ об ошибках
        // обладали привилегированным статусом - помещать их в очередь даже когда там
        // нет места (освободить)
        // LOG_ERROR("ќчередь переполнена");
        return false;
    }

    return true;
}


void SimpleMessage::Transmit()
{
    DInterface::AddMessageForTransmit(this);
}


void SimpleMessage::TransmitAndSend()
{
    Transmit();

    while(DInterface::GetOutbox().Size())
    {
        DInterface::Update();
    }
}
