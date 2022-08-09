#pragma once
#pragma once
#include "Settings/SettingsTypes.h"


struct Param;


namespace WaveGraphics
{
    // ���������� ���������� � ���������� ������� �� ������
    void Draw(const Chan &);

    // ������� x ������� ��������� �������
    int X();

    // ������� y ������� ��������� �������
    int Y(const Chan &);

    // ������ ������� ��������� �������
    int Width();

    // ������ ������� ��������� �������
    int Height();

    Param *GetParameterForDraw(const Chan &, int i);
};
