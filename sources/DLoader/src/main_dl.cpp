#include "defines.h"
#include "common/Messages_pl.h"
#include "FDrive/FDrive_dl.h"
#include "Interface/Interface_dl.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Updater_dl.h"
#include <stm32f4xx_hal.h>


typedef void(*pFunction)();

static void StartWithoutUpgrade();

static void StartWithAutoUpgrade();

static void StartWithManualUpgrade();

static void JumpToMainApplication();


int main()
{
    CPU::Init();

    DLDrive::Init();

    pFuncVV funcs[] = { StartWithoutUpgrade, StartWithAutoUpgrade, StartWithManualUpgrade };

    funcs[MODE_START]();

    Message::StartMainApplication().Transmit(); // Посылаем команду запуска основного приложения

    while(DInterface::GetOutbox().Size())       // И ждём, пока она отправится
    {
        DInterface::Update();
    }

    JumpToMainApplication();                    // И стартуем сам на основную прошивку

    return 0;
}


static void StartWithoutUpgrade()
{

}


static void StartWithAutoUpgrade()
{
    uint start = HAL_TIM::TimeMS();

    while(HAL_TIM::TimeMS() - start < 5000)
    {
        DLDrive::Update();
        if(DLDrive::IsConnected())
        {
            Updater::UpgradeDevice();
            Updater::UpgradePanel();
            break;
        }
    }
}


static void StartWithManualUpgrade()
{
    uint start = HAL_TIM::TimeMS();

    while(HAL_TIM::TimeMS() - start < 1000)
    {
        DInterface::Update();               // Сообщения от панели о запуске обновления
        if(Updater::NeedUpgrade())          // Если дождались
        {
            StartWithoutUpgrade();
            break;
        }
    }
}


static void JumpToMainApplication()
{
    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = reinterpret_cast<pFunction>(*reinterpret_cast<__IO uint *>(Updater::MAIN_PROGRAM_START_ADDRESS + 4)); //-V566

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();
}
