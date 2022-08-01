// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"


#define SCREEN_HEIGHT 240
#define SCREEN_WIDTH  320

struct Painter
{
    // ���������� � ������ ��������� ������� �����. ��������� ����� ������ color
    static void BeginScene(const Color &);
    
    // ���������� � ����� ��������� ������� �����. ��������� ���������� ������ �� �����
    static void EndScene();
    
    // ���������� ����� ������� ������
    static void SetPoint(int x, int y);
    
    // ���������� �������������� �����
    static void DrawHLine(int y, int x0, int x1, const Color & = Color::NUMBER);
    
    // ���������� ������������ �����
    static void DrawVLine(int x, int y0, int y1, const Color & = Color::NUMBER);
    
    // ���������� ������������ �����
    static void DrawLine(int x0, int y0, int x1, int y1, const Color & = Color::NUMBER);
    
    static void DrawRectangle(int x, int y, int width, int height, const Color & = Color::NUMBER);

    static void FillRegion(int x, int y, int width, int height, const Color & = Color::NUMBER);
};
