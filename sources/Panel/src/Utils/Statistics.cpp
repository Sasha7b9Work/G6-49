// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Statistics.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <string.h>


using namespace Primitives;


namespace Statistics
{
    // ����� ������ �����
    static uint timeStart = 0;

    // �����, ����������� �� ��������� ������ �����
    static int timeFrame = 0;

    // ����� ����� ����������� ����� ����� ���������
    static uint timeAccumFrames = 0;

    // ����� ��������� ���� ������ �� 1 �������
    static int timeAllFrames = 0;

    // ����� ������ ���������� 1-���������� �������, � ������� �������� ���������� ����� ����� ���������
    static uint timeStartFrames = 0;
}



void Statistics::BeginFrame()
{
    timeStart = _TIME_MS;
}


void Statistics::EndFrame()
{
    timeFrame = (int)(_TIME_MS - timeStart);

    timeAccumFrames += timeFrame;

    if (_TIME_MS >= timeStartFrames + 1000) //-V1051
    {
        timeAllFrames = (int)timeAccumFrames;
        timeStartFrames = _TIME_MS;
        timeAccumFrames = 0;
    }
}


void Statistics::Show()
{
    if (set.dbg.showStatistics)
    {
        Rectangle(60, 10).DrawFilled(0, Display::HEIGHT - 12, Color::BACK, Color::FILL);

        char buffer[100] = {0};
        std::strcpy(buffer, SU::Int2String((int)(CPU::GetFPS() + 0.5F), false, 3).c_str());
        std::strcat(buffer, "/");
        std::strcat(buffer, SU::Int2String(timeFrame, false, 3).c_str());
        std::strcat(buffer, "/");
        std::strcat(buffer, SU::Int2String(timeAllFrames, false, 3).c_str());
        String(buffer).Draw(2, Display::HEIGHT - 11, Color::FILL);
    }
}
