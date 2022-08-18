// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    "����������" - ��, ��� ������� � ����������
*/


namespace Selector
{
    void BeginGrab(int mouseX);

    void MoveBorder(int mouseX);

    void EndGrab();

    void DrawRegion();

    void DrawCursors();

    // ���������� true, ���� ������ ��������� ��� �������� ����������� �������
    bool CursorOverBorder(int mouseX);

    bool IsEnabled();

    void Enable();

    void Disable();
};
