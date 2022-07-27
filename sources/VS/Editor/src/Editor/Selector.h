// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*

    "����������" - ��, ��� ������� � ����������

*/


class Selector
{
public:
    
    static void BeginGrab(int mouseX);
    
    static void MoveBorder(int mouseX);
    
    static void EndGrab();
    
    static void DrawRegion();

    static void DrawCursors();

    // ���������� true, ���� ������ ��������� ��� �������� ����������� �������
    static bool CursorOverBorder(int mouseX);

    static bool IsEnabled();

    static void Enable();

    static void Disable();

private:

    static bool isEnabled;
};
