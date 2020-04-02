#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"
#include <stm32f429xx.h>


/*

    Ход обновления следующий.

    1. После включения в течение секунды ожидается нажатие кнопки на панели. Если нажатие произошло, то запускается процесс обновления, нет

*/


#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000


typedef void(*pFunction)();


// Эта функция запускает процедуру обновления
static void Update();


static void JumpToMainApplication();


int main()
{
    CPU::Init();
    Timer::Init();
    Keyboard::Init();

    uint start = TIME_MS;

    while (TIME_MS - start < 1000)
    {
        if(!Keyboard::BufferIsEmpty())
        {
            Update();
            break;
        }
    }

    JumpToMainApplication();

    return 0;
}


static void Update()
{

}

static void JumpToMainApplication()
{
    __disable_irq();

    pFunction JumpToApplication;

    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));

    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);

    __enable_irq();

    JumpToApplication();
}
