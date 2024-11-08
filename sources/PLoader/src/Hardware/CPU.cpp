// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include <cstdlib>


namespace CPU
{
    // ����� ������ ������ ��� (����� ������ ���������� 1-���������� ������� �������)
    static uint timeStartMeasFPS = 0;

    // ������� ������� ��������� � ������ ���������� 1-���������� ������� �������
    static int numFrames = 0;

    static float fps = 0.0f;
}


void CPU::Init()
{
    HAL::Init();
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
