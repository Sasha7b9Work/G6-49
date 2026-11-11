// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Interface_d.h"
#include "FDrive/FDrive_d.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP_d.h"
#include "Hardware/HAL/HAL.h"
#include "FreqMeter/FreqMeter_d.h"
#include "Hardware/Timer.h"


int main()
{
    CPU::Init();
    HAL_TIM::Delay(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
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
