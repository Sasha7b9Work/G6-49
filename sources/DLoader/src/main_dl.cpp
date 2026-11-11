// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface/Interface_d.h"
#include "common/Interface/Messages_l.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Updater_dl.h"
#include <stm32f4xx_hal.h>


static void JumpToMainApplication();
void MainCycle();
void TestCycle();


int main()
{
    CPU::Init();

    while (true)
    {
        MainCycle();
    }
}


void MainCycle()
{
    Message::StartMainApplication().TransmitAndSend();      // ѕосылаем команду запуска основного приложени€

    JumpToMainApplication();                                // » стартуем сам на основную прошивку
}


void TestCycle()
{
    DInterface::Test();
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
