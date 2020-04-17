#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Updater_pl.h"
#include "Interface/Handlers_pl.h"
#include <stm32f429xx.h>


static bool OnStartMainApplication(SimpleMessage *message);


static bool E(SimpleMessage *)
{
    return false;
}


bool PHandlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);
  
    uint8 com = msg->TakeUINT8();

    pFuncBpM func = E;

    switch(com)
    {
    case Command::StartApplication: func = OnStartMainApplication; break;

    case Command::PortionUpgradeDevice:
    case Command::AnswerUpgradePanel:
    case Command::RequestPortionUpgradePanel:
    case Command::AnswerPortionUpgradePanel:
        func = Updater::Handler;
        break;
    }
    
    if (com < Command::Count)
    {
        /// —юда сообщение передаЄтс€ уже без первого байта
        return func(msg);
    }

    return false;
}


static bool OnStartMainApplication(SimpleMessage *)
{
#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000

    typedef void(*pFunction)();

    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));

    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();

    return true;
}
