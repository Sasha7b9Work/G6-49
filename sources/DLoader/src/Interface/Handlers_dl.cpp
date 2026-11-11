// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "structs.h"
#include "common/Interface/Command.h"
#include "common/Interface/Handlers_d.h"
#include "common/Interface/Interface_d.h"
#include "common/Interface/Transceiver_d.h"
#include "Updater_dl.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"


namespace DHandlers
{
    static void E(SimpleMessage *)
    {
    }

    static void SendData(SimpleMessage *);
}



void DHandlers::Processing(SimpleMessage *msg)
{
    typedef void(*pFunc)(SimpleMessage *);

    uint8 com = msg->TakeUINT8();

    pFunc func = E;

    switch(com)
    {
    case Command::RequestData: func = SendData; break;

    case Command::RequestUpgrade:
    case Command::RequestPortionUpgradePanel:
        func = Updater::Handler;
        break;
    }

    func(msg);
}


void DHandlers::SendData(SimpleMessage *)
{
    CPU::SetBusy();

    if (DInterface::GetOutbox().Size() != 0)
    {
        HAL_TIM::Delay(2);

        CPU::SetReady();

        Transceiver::Transmit(DInterface::GetOutbox().Front());

        CPU::SetBusy();

        DInterface::GetOutbox().Pop();
    }
    else
    {
        Message::RequestData message;

        HAL_TIM::Delay(2);

        CPU::SetReady();

        Transceiver::Transmit(&message);

        CPU::SetBusy();
    }
}



