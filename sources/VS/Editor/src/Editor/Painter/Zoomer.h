// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*   ���������� ��������� � ��������� ���� ������
*/


class Zoomer
{
    friend struct Window;

public:

    // ��������� �������
    static void Increase(int mouseX = -1);

    // ��������� �������
    static void Decrease(int mouseX = -1);

    // ���������������� �� ������� ����������� �������
    static void SetOnRegion();

    static bool NoScaling();

    static int Scale();

    static float ScaleF();

    static void Draw();

    // ���������� x ��������� ������ ���������
    static int X();

    static int Right();

    // ���������� y ��������� ������ ���������
    static int Y();

    // ������ ������ ���������
    static int Width();

    // ������ ������ ���������
    static int Height();

    static int Bottom();

    // ������ ������ ��������� �����
    static int IndexFirsPoint();

    // ������ �����, ��������� �� ��������� ���������
    static int IndexLastPoint();

    // ���������� ��������� �����
    static int NumberDrawingPoints();

    // ���������� true, ���� ��������� ��� �����
    static bool UnderMouse(int mouseX, int mouseY);

    static bool MoveWindow(int mouseX);

    // ��������� �����
    static bool Grab(int mouseX, int mouseY);

    // �������� �� ����
    static bool UnGrab();

private:

    static int scale;
    static int indexFirstPoint;         // ������ ������ ��������� �����
    static int indexMiddlePoint;        // ������ ������� ��������� �����
    static int numberDrawingPoints;     // ���������� ��������� �����

    // ���������� �������� �������� �����
    static void CalculateIndexesPoints();
};
