#include "defines.h"

#ifdef STM32F429xx
#include <stm32f4xx.h>
#endif

#ifdef STM32F746xx
#include <stm32f7xx.h>
#endif

#include "stm32.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
__attribute((noreturn)) void STM32::ErrorHandler()
{
    while (true)
    {

    }
}


