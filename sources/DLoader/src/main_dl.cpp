#include "defines.h"
#include "FDrive/FDrive_d.h"
#include "Interface/Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"


#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000


typedef void(*pFunction)();


static void JumpToMainApplication();


int main()
{
    JumpToMainApplication();
    
    return 0;
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
