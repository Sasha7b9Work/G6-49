// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH  320

namespace Painter
{
    // ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    void BeginScene(const Color &);

    // ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    void EndScene();

    // ���������� ����� ������� ������
    void SetPoint(int x, int y);

    // ���������� �������������� �����
    void DrawHLine(int y, int x0, int x1, const Color & = Color::NUMBER);

    // ���������� ������������ �����
    void DrawVLine(int x, int y0, int y1, const Color & = Color::NUMBER);

    // ���������� ������������ �����
    void DrawLine(int x0, int y0, int x1, int y1, const Color & = Color::NUMBER);

    void DrawRectangle(int x, int y, int width, int height, const Color & = Color::NUMBER);

    void FillRegion(int x, int y, int width, int height, const Color & = Color::NUMBER);
};
