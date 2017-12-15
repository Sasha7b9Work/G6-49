#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/AD5697.h"
#include "Interface/Interface.h"
#include "Hardware/Hardware.h"
#include <stm32f4xx_hal.h>


extern SPI_HandleTypeDef hSPI1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(void)
{
    hardware.Init();
    
    hardware.SetReady();

    static uint prevTime = 0;
    
    while (1)
    {
        interface.ProcessingCommand();

/*        
        if(HAL_GetTick() - prevTime > 100)
        {
            generator.SetOffset(A, 1000.0f);
            generator.SetOffset(B, 3000.0f);
            prevTime = HAL_GetTick();
        }
        */
    }
}
