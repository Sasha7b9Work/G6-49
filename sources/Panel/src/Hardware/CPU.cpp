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
