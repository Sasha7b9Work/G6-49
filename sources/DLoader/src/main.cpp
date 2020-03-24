#include "defines.h"
#include "FDrive/FDrive_d.h"
#include "Interface/Interface_d.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"


int main()
{
    CPU::Init();
    HAL_TIM::Delay(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    DDrive::Init();
    HAL_EEPROM::Init();
  
    while (1)
    {
    }
}
