#pragma once
#pragma once
#include "Settings/SettingsTypes.h"

class Parameter;


class WaveGraphics
{
public:
    
    // ���������� ���������� � ���������� ������� �� ������
    static void Draw(const Chan &);
    
    // ������� x ������� ��������� �������
    static int X();
    
    // ������� y ������� ��������� �������
    static int Y(const Chan &);
    
    // ������ ������� ��������� �������
    static int Width();
    
    // ������ ������� ��������� �������
    static int Height();


    static Parameter *GetParameterForDraw(const Chan &, int i);

private:

    static void DrawParameters(const Chan &, int y0);

    // ���������� ��� �������
    static void DrawUGO(const Chan &, int x, int y);
};
