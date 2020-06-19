#include "defines.h"
#include "structs.h"
#include "common/Command.h"
#include "common/Handlers_d.h"
#include "common/Interface_d.h"
#include "common/Transceiver.h"
#include "Updater_dl.h"
#include "FDrive/FDrive_dl.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"


static void E(SimpleMessage *);

static void SendData(SimpleMessage *);



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


static void SendData(SimpleMessage *)
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


static void E(SimpleMessage *)
{

}
