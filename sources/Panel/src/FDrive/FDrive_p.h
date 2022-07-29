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

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static bool IsMount();
        static bool GetNumDirsAndFiles();
        static bool RequestFile();
        static bool RequestFileSize();
        static bool LoadDDSfromFile();
    };
};
