#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FDrive/FDriveDevice.h"
#include "Generator/GeneratorDevice.h"
#include "Generator/AD5697.h"
#include "Interface/InterfaceDevice.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "FreqMeter/FreqMeter.h"
#include "Utils/Console.h"
#include <stm32f4xx_hal.h>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    CPU::Init();
    Timer::PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    Generator::Init();
    //FDrive::Init();
  
    while (1)
    {
        FreqMeter::Update();
        //FDrive::Update();
        Interface::Update();
    }
}
