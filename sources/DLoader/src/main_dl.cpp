// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface_d.h"
#include "common/Messages_l.h"
#include "FDrive/FDrive_dl.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Updater_dl.h"
#include <stm32f4xx_hal.h>


static void StartWithoutUpgrade();

static void StartWithAutoUpgrade();

static void StartWithManualUpgrade();

static void JumpToMainApplication();


#define START_WITHOUT_UPGRADE      0  /* «апуск без попытки обновлени€ */
#define START_WITH_AUTO_UPGRADE    1  /* «апуск с автоматическим обновлением - попытка обновитьс€ с флешки в течение некоторого времени */
#define START_WITH_MANUAL_UPGRADE  2  /* «апуск с обновлением по требованию - попытка обновлени€ происходит, если загрузчик получает запрос на обновление от панели */

static int MODE_START = START_WITHOUT_UPGRADE;

int main()
{
    CPU::Init();

    DLDrive::Init();

    pFuncVV funcs[] = { StartWithoutUpgrade, StartWithAutoUpgrade, StartWithManualUpgrade };

    funcs[MODE_START]();

    DLDrive::DeInit();

    Message::StartMainApplication().TransmitAndSend();      // ѕосылаем команду запуска основного приложени€

    JumpToMainApplication();                                // » стартуем сам на основную прошивку

    return 0;
}


static void StartWithoutUpgrade()
{

}


static void StartWithAutoUpgrade()
{
    uint start = HAL_TIM::TimeMS();

    bool upgraded = false;

    while((HAL_TIM::TimeMS() - start < 5000) && (!upgraded))
    {
        DLDrive::Update();
        if(DLDrive::IsConnected())
        {
            Updater::UpgradeDevice();
            Updater::UpgradePanel();
            upgraded = true;
        }
    }
}


static void StartWithManualUpgrade()
{
    uint start = HAL_TIM::TimeMS();

    while(HAL_TIM::TimeMS() - start < 200)
    {
        DInterface::Update();               // —ообщени€ от панели о запуске обновлени€
        if(Updater::NeedUpgrade())          // ≈сли дождались
        {
            StartWithAutoUpgrade();
            break;
        }
    }
}


static void JumpToMainApplication()
{
    __disable_irq();

    pFuncVV JumpToApplication;

    JumpToApplication = (pFuncVV)(*(__IO uint *)(Updater::MAIN_PROGRAM_START_ADDRESS + 4)); //-V566

    __set_MSP(*(__IO uint *)Updater::MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();
}
