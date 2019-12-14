#include "CPU.h"
#include "LTDC.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Log.h"
#include <cstdlib>


uint  CPU::timeStartMeasFPS = 0;
int   CPU::numFrames = 0;
float CPU::fps = 0.0F;


void CPU::Init()
{
    HAL_Init();

    HAL::Init();

    HAL_FSMC::Init();

    HAL_SPI4::Init();

    Keyboard::Init();

    HAL_CRC32::Init();

    Beeper::Init();
}


void CPU::Update()
{
    ++numFrames;
    if(TIME_MS >= timeStartMeasFPS + 1000)  // Если прошло более 1 секунды с начала 1-сек отрезка
    {                                       // рассчитываем ФПС
        fps = static_cast<float>(numFrames) / (TIME_MS - timeStartMeasFPS) * 1e3F;
        numFrames = 0;
        timeStartMeasFPS = TIME_MS;
    }
}


float CPU::GetFPS()
{
    return fps;
}


void HAL_MspInit()
{
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}
