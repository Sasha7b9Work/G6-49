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
    InitPins();
    WritePin(Pin_P3_OutA, false);
    WritePin(Pin_P4_OutB, false);
    timer.PauseOnTime(500);             // Задержка нужна для того, чтобы AD9952 успел пройти внутреннюю инициализацию
    generator.Init();
    interface.Init();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetBusy(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Hardware::SetReady(void)
{
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
}

#ifdef USE_FULL_ASSERT

//----------------------------------------------------------------------------------------------------------------------------------------------------
void assert_failed(uint8_t* file, uint32_t line)
{
}

#endif

