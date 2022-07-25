// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"


class String;



class File
{
public:
    File();
    ~File();
    
    // �������� ������ �� ���������� ����� � ������� numberFile (����� �������)
    void RequestFromPicture(int numberFile);
    
    // ���������� ����������
    void Draw(int x, int y);
    
    // ��������� ���� � ����������� ������
    void Close();

    static bool Handler(SimpleMessage *msg);
    
    // ��������� ������ ��� ������ �����
    static void SetDataToWave();
};


struct FileRequests
{
    // ������� ������ �� �����
    static void SendRequestForString(const String *name, int numString);
};
