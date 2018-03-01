#include "Hardware.h"
#include "Generator/Generator.h"
#include "Interface/Interface.h"
#include "Hardware/Timer.h"
#include "Hardware/CPU.h"
#include <stm32f4xx_hal.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Hardware::Init(void)
{
    CPU::Init();
    CPU::WritePin(Pin_P3_OutA, false);
    CPU::WritePin(Pin_P4_OutB, false);
    timer.PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    generator.Init();
    interface.Init();
}
