// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct ModeButtonLeft
{
    enum E
    {
        EditPoints,     // ����� �������������� ��������� �����
        EditLines       // ����� �������������� ���������������� �������
    };

    static E Get();

    static void Set(E v);
};
