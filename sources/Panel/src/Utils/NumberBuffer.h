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

class NumberBuffer
{
public:

    // \brief ����� �������������� ���������. size - ������ ������ c ������ ������������ ����, position - ������� ������� �������, maxValue - 
    // ������������ ��������, ������� ����� ��������� � ������.
    static void Set(char *buffer, int size, int position, int maxValue);

    // ��������� �������
    static void ProcessKey(const Key::E key);

    // ��������� Backspace
    static void PressBackspace();

    // ���������� ����� �������, � ������� ��������� ������
    static int PositionCursor();

private:
    static char *buffer;
    static int size;
    static int position;
    // ������������ ��������, ������� ����� ���� ������������ � ������. 0 ��������, ��� ����������� �� �������� �������� ����
    static int max;

    // ���������� ���������� �������� � ������ (��� ����� ������������ ����).
    static int NumSymbols();

    // ��������� �������� �����
    static void ProcessRegulator(const Key::E key);

    // ��������� �������� � �������� �������. ���������� true, ���� �������� ���������
    static bool IncreaseDigit(int pos);

    // ��������� �������� � �������� �������. ���������� true, ���� �������� ���������
    static bool DecreaseDigit(int pos);

    // ���������� ������� ������� ��� ���������
    static int PositionSymbolForChange();

    // �������� �� ���������� ������ ������
    static bool AllowableSymbol(const Key::E key);

    // ���������� �������� ��������, �������������� � ������
    static uint ToUINT();

    // ���������� true, ���� � ����� ��� �������
    static bool All9();
};
