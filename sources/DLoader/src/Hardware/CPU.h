// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CPU
{
    static void Init();
    // ���������� �� PA2 ������� ����, ��� ��������� �����
    static void SetBusy();
    // ���������� �� PA2 ������� ����, ��� ��������� ��������
    static void SetReady();

private:

    static void InitGPIOS();
};
