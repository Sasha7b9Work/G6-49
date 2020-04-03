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

    uint start = HAL_TIM::TimeMS();

    while(HAL_TIM::TimeMS() - start < 10000)     // Ждём секунду
    {
        DInterface::Update();                   // Сообщения от панели о запуске обновления
        DLDrive::Update();
        if(Updater::NeedUpgrade())              // Если дождались
        {
            Updater::Upgrade();                 // Производим обновление
            break;
        }
    }

    Message::StartMainApplication().Transmit(); // Посылаем команду запуска основного приложения

    while(DInterface::GetOutbox().Size())       // И ждём, пока она отправится
    {
        DInterface::Update();
    }

    JumpToMainApplication();                    // И стартуем сам на основную прошивку
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
