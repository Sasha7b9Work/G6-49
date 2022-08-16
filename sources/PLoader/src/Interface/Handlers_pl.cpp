// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Command.h"
#include "common/Messages.h"
#include "Updater_pl.h"
#include "Interface/Handlers_pl.h"
#include <stm32f429xx.h>



namespace PHandlers
{
    static bool E(SimpleMessage *)
    {
        return false;
    }
}


bool PHandlers::Processing(SimpleMessage *msg)
{
    typedef bool(*pFuncBpM)(SimpleMessage *);
  
    uint8 com = msg->TakeUINT8();

    pFuncBpM func = E;

    switch(com)
    {
    case Command::StartApplication: func = OnStartMainApplication; break;

    case Command::PortionUpgradeDevice:         // ���� ���������� DLoader ��������� � ��������� ������������� ���������� Device
    case Command::AnswerUpgradePanel:           // ���� ���������� DLoader �������� ������ �������� Panel � ������
    case Command::AnswerPortionUpgradePanel:    // ����� � "������" ��������
        func = Updater::Handler;
        break;
    }
    
    if (com < Command::Count)
    {
        // ���� ��������� ��������� ��� ��� ������� �����
        return func(msg);
    }

    return false;
}


bool PHandlers::OnStartMainApplication(SimpleMessage *)
{
    typedef void(*pFunction)();

    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(Updater::MAIN_PROGRAM_START_ADDRESS + 4));

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();

    return true;
}
