#pragma once
#pragma once
#include "Settings/SettingsTypes.h"


struct Parameter;


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

    Parameter *GetParameterForDraw(const Chan &, int i);
};
