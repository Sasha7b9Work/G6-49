// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Memory
{
    static void Init();
    // �������� ������� ������ �������� size ���� � ��������� ��������� �� ����
    static void *Allocate(uint size);
    // ����������� ����� ���������� ������� ������
    static void Free(void *address);
};
