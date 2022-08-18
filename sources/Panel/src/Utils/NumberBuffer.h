// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Keyboard/Controls.h"


/*
    ������������ ������� ��� ������ � ���������� �������, � ������� �������� �������� ��������.
    ��������:
    ����� char buffer[10] = {'1', '5', '3', '0', '\0'}
    ������������ ����� 1530.
    ������� ��������� ������������ ������� ������ � �������� �����
*/

namespace NumberBuffer
{
    // \brief ����� �������������� ���������. size - ������ ������ c ������ ������������ ����, position - ������� ������� �������, maxValue - 
    // ������������ ��������, ������� ����� ��������� � ������.
    void Set(char *buffer, int size, int position, int maxValue);

    // ��������� �������
    void ProcessKey(const Key::E key);

    // ��������� Backspace
    void PressBackspace();

    // ���������� ����� �������, � ������� ��������� ������
    int PositionCursor();
};
