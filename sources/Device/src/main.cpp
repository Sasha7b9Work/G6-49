#include "defines.h"
#include "log.h"
#include "FDrive/FDrive_d.h"
#include "Generator/Generator_d.h"
#include "Generator/AD5697.h"
#include "Interface/Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include "FreqMeter/FreqMeter_d.h"


int main()
{
    CPU::Init();
    Timer::PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    DVCP::Init();
    DGenerator::Init();
    DDrive::Init();
    HAL_EEPROM::Init();
  
    while (1)
    {
        DFreqMeter::Update();
        DDrive::Update();
        DInterface::Update();
    }
}
