// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


class Hint
{
public:
    
    // ��������� ���������
    static void Draw();
    
    // ����� ����������. ���������� false, ���� ��������� �� ���������
    static bool ProcessControl(const Control control);

private:
    
    // \brief ������������ ��������� � �����������
    // start, end - ��������� � �������� ������ choice ��� ������
    // calculate - ���� true, �� ��������� �� ������������ - ������ ������ ��������
    static int DrawDetailedHint(const Choice *choice, int x, int y, int width, int start, int end, bool calculate = false);
    
    // ���������� ����� ������� � ���������
    static int NumPagesInHint(const Choice *choice, int x, int y, int width);
    
    // ������������ ������, ����������� ��� �������� ��������� �� ��������
    static void Calcualte(const Choice *choice, int x, int y, int width);
    
    // ������ ��� ������� � ��������������� ���������
    static void DrawPagesUGO(const int xRight, const int y);
    
    // ���� true - ����� �������� ��������� �� �����
    static bool show;
    
    // �������, ��� �������� ����� �������� ���������
    static const Item *item;
    
    // ����� ����������, ��� �������� ����� �������� ���������
    static Key::E key;
    
    // ���������� ������� � ���������
    static int numPages;
    
    // ������� �������������� ��������
    static int currentPage;
    
    // � ������ ������ ���������� ������ ��������
    static int firstItemOnSecondPage;
    
    // ������������� � true �������� ��������, ��� Item ��� ������ ��� �������� � ��������� �������� �� ��������
    static bool needCalculate;
};
