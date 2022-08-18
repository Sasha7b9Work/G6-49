// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SimpleMessage;


namespace Updater
{
    static const int MAIN_PROGRAM_START_ADDRESS = 0x8020000;

    // true, ���� ����� ��������� ��������� ����������
    bool NeedUpgrade();

    // ������ ��������� ����������
    void UpgradeDevice();
    void UpgradePanel();

    // ���������� ����������� ������
    void Handler(SimpleMessage *);
};
