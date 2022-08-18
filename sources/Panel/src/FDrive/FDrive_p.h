// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"


namespace FDrive
{
    // ��������� �������������
    void Init();

    // ��� ���������� - �������� ��� �����
    enum View
    {
        Dirs,
        Files
    };
    
    extern View view;

    // ��������� �����-���������
    void Draw();
    
    // ��������� ������� ������ "�����"
    void PressUp();
    
    // ��������� ������� ������ "����"
    void PressDown();
    
    // ��������� ������� ������ "�������"
    void PressChoose();

    char *CurrentDirectory();

    void SaveScreenToFlash();

    namespace Handler
    {
        bool Processing(SimpleMessage *msg);
    };
};
