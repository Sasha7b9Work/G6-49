#include "defines.h"
#include "common/Messages_pl.h"
#include "FDrive/FDrive_dl.h"
#include "Interface/Interface_dl.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Updater_dl.h"
#include <stm32f4xx_hal.h>


typedef void(*pFunction)();


static void JumpToMainApplication();


int main()
{
    CPU::Init();

    DLDrive::Init();

    while(Updater::Process())
    {
        DLDrive::Update();
        DInterface::Update();
    }

    Message::StartMainApplication().Transmit();

    while(DInterface::GetOutbox().Size())
    {
        DInterface::Update();
    }

    JumpToMainApplication();
} //-V591


static void JumpToMainApplication()
{
    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(Updater::MAIN_PROGRAM_START_ADDRESS + 4)); //-V566

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();
}
