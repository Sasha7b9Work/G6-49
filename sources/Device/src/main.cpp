#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/AD5697.h"
#include "Interface/InterfaceDevice.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/Modules/EEPROM.h"
#include "FreqMeter/FreqMeter.h"
#include <stm32f4xx_hal.h>
#endif


int main()
{
    CPU::Init();
    Timer::PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    VCP::Init();
    Generator::Init();
    FDrive::Init();
    EEPROM::Init();
  
    while (1)
    {
        FreqMeter::Update();
        FDrive::Update();
        Interface::Update();
    }
}
