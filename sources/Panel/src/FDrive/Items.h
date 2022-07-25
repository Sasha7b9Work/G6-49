// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/command.h"
#include "common/Messages.h"



class File;

class ListFiles
{
public:

    static void Init();

    // true ��������, ��� ��� �������� ������ �� ����������
    static bool WaitAnswer()
    {
        return requestIsSend;
    }

    // ������� ������ �� ���������� ������
    static void SendRequest();

    // ��������� ������� ������ "�����"
    static void PressUp();

    // ��������� ������� ������ "����"
    static void PressDown();

    // ���������� ���������� ������. ���� == -1, ������ �� ���������
    static int NumberFiles();

    // ���������� ������������
    static void Draw(int x, int y);

    // ���������� ����� �������� �����
    static int NumberCurrentFile();

    class Handler
    {
    public:
        static bool Processing(SimpleMessage *msg);
    };

private:

    // ������ ������. ��������� �����
    static bool requestIsSend;
};
