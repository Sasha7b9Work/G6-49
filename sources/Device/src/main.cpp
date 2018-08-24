#include "defines.h"
#include "Generator/Generator.h"
#include "Generator/AD5697.h"
#include "Interface/Interface.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter.h"
#include <stm32f4xx_hal.h>


extern SPI_HandleTypeDef hSPI1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    Timer::PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    Generator::Init();
    Interface::Init();
    CPU::SetReady();
  
    while (1)
    {
        FreqMeter::Update();
        Interface::Update();
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
