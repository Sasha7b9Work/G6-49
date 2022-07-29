// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/command.h"
#include "common/Messages.h"


class File;


namespace ListFiles
{
    void Init();

    // true ��������, ��� ��� �������� ������ �� ����������
    bool WaitAnswer();

    // ������� ������ �� ���������� ������
    void SendRequest();

    // ��������� ������� ������ "�����"
    void PressUp();

    // ��������� ������� ������ "����"
    void PressDown();

    // ���������� ���������� ������. ���� == -1, ������ �� ���������
    int NumberFiles();

    // ���������� ������������
    void Draw(int x, int y);

    // ���������� ����� �������� �����
    int NumberCurrentFile();

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    };
};
