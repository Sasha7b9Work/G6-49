// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Hardware/CPU.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include <cstdlib>


uint  CPU::timeStartMeasFPS = 0;
int   CPU::numFrames = 0;
float CPU::fps = 0.0F;


void CPU::Init()
{
    HAL::Init();

    Keyboard::Init();

    Beeper::Init();
}


void CPU::Update()
{
    ++numFrames;
    if(_TIME_MS >= timeStartMeasFPS + 1000)  // ���� ������ ����� 1 ������� � ������ 1-��� �������
    {                                       // ������������ ���
        fps = (float)numFrames / (float)(_TIME_MS - timeStartMeasFPS) * 1e3F;
        numFrames = 0;
        timeStartMeasFPS = _TIME_MS;
    }
}


float CPU::GetFPS()
{
    return fps;
}
