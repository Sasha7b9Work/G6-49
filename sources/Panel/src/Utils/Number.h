// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

/*
    ����� ��� ������ � ������������� �����������
*/

namespace Number
{
    struct Step
    {
        enum E
        {
            Plus,
            Minus
        } value;
        Step(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    };

    // ������� �������, � ������� �������� ����� � ������� �����
    static const int SIZE_PART = 20;

    // ��������� �������. � ��� ������ ���� �������� ��������. �������� ����� ����������� ��������������� ��� ������� � ���� �������
    void Set(char integer[SIZE_PART], char fract[SIZE_PART], char sign);

    // ���������� ��������, �������������� ���������� �������
    float GetValue();

    // \brief �������� �������� � ������� position �� +1 ��� -1.
    // ��� ���� ������� -1 ������������ ������ ������� ����� �� ����� (��������� ����� ������), � ������� +1 - ������ ������� ������ �� �����
    // (������ ����� ��������)
    void ChangeDigit(int position, Step step);

    extern char *integer;
    
    extern char *fract;

    extern char sign;
};
