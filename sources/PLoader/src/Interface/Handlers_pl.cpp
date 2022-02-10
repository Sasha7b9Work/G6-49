#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Updater_pl.h"
#include "Interface/Handlers_pl.h"
#include <stm32f429xx.h>


static bool E(SimpleMessage *)
{
    return false;
}


bool PHandlers::Processing(SimpleMessage *msg) //-V2506
{
    typedef bool(*pFuncBpM)(SimpleMessage *);
  
    uint8 com = msg->TakeUINT8();

    pFuncBpM func = E;

    switch(com)
    {
    case Command::StartApplication: func = OnStartMainApplication; break;

    case Command::PortionUpgradeDevice:         // Этим сообщением DLoader оповещает о процентах произведённого обновления Device
    case Command::AnswerUpgradePanel:           // Этим сообщением DLoader сообщает размер прошивки Panel в байтах
    case Command::AnswerPortionUpgradePanel:    // Ответ с "чанком" прошивки
        func = Updater::Handler;
        break;
    }
    
    if (com < Command::Count)
    {
        // Сюда сообщение передаётся уже без первого байта
        return func(msg);
    }

    return false;
}


bool PHandlers::OnStartMainApplication(SimpleMessage *)
{
    typedef void(*pFunction)();

    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(Updater::MAIN_PROGRAM_START_ADDRESS + 4)); //-V2571

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();

    return true;
}
