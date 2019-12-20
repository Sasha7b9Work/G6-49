#include "defines.h"
#include "log.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/Generator_d.h"
#include "Generator/AD5697.h"
#include "Interface/Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/Modules/EEPROM.h"
#include "FreqMeter/FreqMeter.h"
#include <stm32f4xx_hal.h>


int main()
{
    CPU::Init();
    Timer::PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    VCP::Init();
    DGenerator::Init();
    FDrive::Init();
    EEPROM::Init();
  
    while (1)
    {
        FreqMeter::Update();
        FDrive::Update();
        Interface::Update();
    }
}
