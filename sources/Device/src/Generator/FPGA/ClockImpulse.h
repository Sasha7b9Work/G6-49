// 2022/08/30 11:42:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace FPGA
{
    // ������� ������� ��� ������������ �������� � ������� ���������. ��� ������������ �������� [0 ��� ... 40 ���]
    // ������� ������� 100 ���, ��� ������������ ����� - 1 ���
    struct ClockImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        static void Set(E v);
        static bool Is1MHz() { return value == _1MHz; }
        static bool Is100MHz() { return value == _100MHz; }
        static E Get() { return value; };
    private:
        static E value;
    };

    // ���� ��� ��������� ������������ �������� ����� �������� ������� ������� - ����������� ��� ��������� ��������:
    // ������ ��������, ������ ������, �������� ����� ��������.
    // �������� ������������ � ��������� �� ��, ��� ������� ������� ������ ���� �� clock, � ������ ����� clock
    void RecalculateImpulseRegistersTo(ClockImpulse::E clock);
}
